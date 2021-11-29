---
layout: post
title:  "Wireguard to VPN Subnet"
author: Gabriel Ewig
date:   2019-06-27
last_modified_at: 2020-03-31
---

### Summary

By connecting both a computer on the internal LAN and various clients to a centralized VPS with a static IP, we can use WireGuard to access a local network behind a permanent NAT/Firewall. WireGuard provides a fast and simple vpn protocol which works well for this situation. However, it should be noted that WireGuard is still under heavy development and has not undergone security audits. I would recommend reading more about WireGuard, particularly the "Conceptual Overview" here: <https://www.wireguard.com/>, before trying this example.

For this example I will refer to the following three clients. Although our VPS acts as a "server", each computer is considered a client by WireGuard.

*   A local client on the desired LAN (such as Pi)
    *   "lan"
    *   Wireguard IP: 10.10.2.2
    *   Local IP: 10.10.1.2
    *   Running Ubuntu for Pi 18.04 LTS
*   A VPS (or similar) accessible with a static IP
    *   "vps"
    *   Wireguard IP: 10.10.2.1
    *   Public IP: Accessible URL
    *   Running Ubuntu 18.04 LTS
*   Multiple clients for remote access
    *   "laptop"
    *   Wireguard IP: 10.10.2.3-255
    *   Local IP: Any DHCP Address
    *   Running Mac or Windows

For reference, the local network is on `10.10.1.0/24` while the WireGuard clients are on `10.10.2.0/24`. Obviously these can and should be adjusted as necessary.

### Installing WireGuard and Configuring the System

Information about the installation of WireGuard for specific systems is available at <https://www.wireguard.com/install/>. When used from the command line, a WireGuard interface can be created using either `wg` or `wg-quick`. `wg-quick` has a few features such as automatic routing, which will be utilized in this example. GUI clients have also worked fine in my testing.

The VPS and LAN client will need IP forwarding enabled. First check if it is enabled by running the following command a getting an output of 0 (disabled) or 1 (enabled):

    sysctl net.ipv4.ip_forward
    net.ipv4.ip_forward = 1

If disabled by default, it can sometimes be helpful to temporarily enable IP forwarding for testing with this command.

    sysctl -w net.ipv4.ip_forward=1

To enable IP forwarding permanently we can add the following line to `/etc/sysctl.conf` and restart the system.

    net.ipv4.ip_forward=1

Finally, we need to create a directory for our WireGuard configurations.

    mkdir /etc/wireguard
    cd /etc/wireguard

### Generating Keys

WireGuard uses a public/private key pair to encrypt packets sent along the interface. On each client needs to locally generate a key pair using `wg genkey` and `wg pubkey`. GUI clients will create these automatically, but in a terminal you can quickly save them to files with the following command.

    umask 077 # Prevent credential leak
    wg genkey | tee privatekey | wg pubkey > publickey

I recommend saving them in the save directory as your configuration files, `/etc/wireguard`. Now would also be a good time to secure this directory so that it is only accessible by root.

    sudo chown -R root:root /etc/wireguard/
    sudo chmod -R og-rwx /etc/wireguard/*

### Configuring WireGuard

`wg-quick` uses simple configuration files to create each WireGuard interface. Create a configuration in `/etc/wireguard` or through the GUI. Configuration files should be named whatever you wan to call the wireguard interface, generally `wg0.conf` or `wg1.conf`. Below are the three configurations used in this example.

##### LAN configuration
    [Interface]
    Address = 10.10.2.2
    PrivateKey = <lan private key>
    PostUp = iptables -A FORWARD -i %i -j ACCEPT; iptables -A FORWARD -o %i -j ACCEPT; iptables -t nat -A POSTROUTING -o <lan interface (eth0)> -j MASQUERADE
    PostDown = iptables -D FORWARD -i %i -j ACCEPT; iptables -D FORWARD -o %i -j ACCEPT; iptables -t nat -D POSTROUTING -o <lan interface (eth0)> -j MASQUERADE

    [Peer]
    PublicKey = <vps public key>
    Endpoint = <vps ip>:51820
    AllowedIPs = 10.10.2.1/24

    PersistentKeepalive = 25

##### VPS Configuration

    [Interface]
    Address = 10.10.2.1
    PrivateKey = <vps private key>
    ListenPort = 51820

    [Peer]
    PublicKey = <lan public key>
    AllowedIPs = 10.10.2.1/24, 10.10.1.1/24

    [Peer]
    PublicKey = <laptop public key>
    AllowedIPs = 10.10.2.3/32

##### Laptop Configuration

    [Interface]
    PrivateKey = <laptop private key>
    Address = 10.10.2.3

    [Peer]
    PublicKey = <vps public key>
    AllowedIPs = 10.10.0.0/16
    Endpoint = <vps ip>:51820

    PersistentKeepalive = 25

Although generally straightforward, a few of these entries deserve some explanation.

*   `PostUp` and `PostDown` on the LAN client create the necessary routing rules to allow traffic to pass between the local and vpn networks. There *are* other routing rules required, but `wg-quick` should take care of them.
*   The `Endpoint` and `PersistentKeepalive` entries on the Laptop and LAN clients is what allows them to communicate from behind a NAT. `Endpoint` initiates a direct connection to the VPS, while `PersistentKeepalive` keeps them in communication from behind any router. In this case it refreshes the link every 25 seconds.
*   `AllowedIPs` controls which devices or networks can communicate over an interface. WireGuard uses this to setup routing rules as well as which connections to allow or drop. If you expect a peer to send or receive packets from a client or network it should be included in its `AllowedIPs`.

### Testing the Network

To initialize an interface run simply run the following command, replacing `wg0` with your interface if necessary.

    wg-quick up wg0

After starting the interface on each device, check the status of each using the `wg` command. You should see an output similar to this on the VPS:

    interface: wg0
      public key: <vps public key>
      private key: (hidden)
      listening port: 51820

    peer: <lan public key>
      endpoint: <lan public ip>:<random port>
      allowed ips: 10.10.2.0/24, 10.10.1.0/24, 10.0.0.0/24
      latest handshake: 39 seconds ago
      transfer: 1.97 MiB received, 337.50 KiB sent

    peer: <laptop public key>
      endpoint: <laptop public ip>:<random port>
      allowed ips: 10.10.2.3/32
      latest handshake: 1 minute, 7 seconds ago
      transfer: 353.60 KiB received, 1.97 MiB sent

Finally, check to see if the laptop can access all clients as well as something on the LAN by sending a ping to `10.10.2.2` and `10.10.1.x`.

### Firewall Configuration

Generally it is a good idea to have a firewall setup on your computer. Given the wide variety of situations, I cannot recommend one certain setup, but can provide one option.

##### Ubuntu

If on Ubuntu, ``ufw`` is the easiest way to configure a basic firewall. Digital ocean has a [good guide to getting one setup and allowing SSH connections](https://www.digitalocean.com/community/tutorials/initial-server-setup-with-ubuntu-18-04).

I can't figure out how to use ufw while allowing wireguard to forward. Let me know if you can, please.

##### CentOS

Using FirewallD and CentOS, we can configure a basic firewall using `firewall-cmd`. Probably the easiest way to allow connections through WireGuard, while still maintaining some level of outside security is to set the `eth0` and `wg0` interfaces to different zones with different rules. By default `eth0` exists on the `public` zone, and you can change `wg0` to the `home` zone with this command.

    firewall-cmd --permanent --zone=home --change-interface=wg0

Anything configured in the home zone will then affect traffic passing through the VPN. This can be configured on a case-by-case basis, or you can enable all traffic with this command.

    firewall-cmd --permanent --zone=home --set-target=ACCEPT

While we're on the topic of security, remember to enable automatic updates on all the computers!

### Running WireGuard as a Service

If everything goes well, you can setup WireGuard as a service on the LAN and VPS clients so that is starts on boot with the command below. Make sure to permanently enable IP forwarding also.

    systemctl enable wg-quick@wg0.service

Go ahead and restart each computer, then verify that the connection still works.

### Setting up Pi-Hole

Pi-Hole is an easy way to block ads and create custom DNS and DHCP for the local netowrk. It is probably the easiest way to automatically use custom domain names with remote clients.

The [Pi-Hole website](https://pi-hole.net/) provides instructions for a basic installation. In addition, the following settings should be changed under the settings tab.

*   DHCP server enabled
*   Listen on all interfaces, permit all origins
*   Pi-Hole domain name: lan (allows devices to be accessed with hostname.lan)

### Diagnosing Connections

If you encounter problems making a connection try checking some of these common problems:

##### WireGuard interface doesn't start
*   If using a GUI, run from the command line to check for configuration errors.
*   Use `-v` to see additional information.

##### Cannot ping other clients or no handshake
*   Check that thee VPS public IP is visible.
*   Verify that each public/private key pair is correct in each configuration.
*   Check the `wg-quick` output for configuration errors.

##### Cannot access LAN computers
*   Ensure that the local computer is on and accepting ping by something on the local network.
*   Check that IP forwarding is enabled on *both* the VPS and the LAN (`sysctl net.ipv4.ip_forward`).
*   Make sure the laptop can ping the LAN client in the first place.
*   Check routing on all computers while `wg-quick` is active.

##### Can ping LAN but cannot connect and services or ports
*   Make sure that the VPS firewall is allowing connections through, and not blocking all ports.

##### Can access IPs but no domain name resolution
*   DNS resolution required additional DHCP configuration on your router or DHCP server.
*   Running Pi-Hole as a DHCP server on the LAN device seems to be the fastest way to enable this.
*   Add the Pi-Hole IP to the OS DNS table or in WireGuard's Interface configuration.
*   Allow DNS resolution for all devices in Pi-Hole.

##### Helpful Diagnostic Tools
*   `tcpdump` for checking what packets each computer is receiving.
*   `traceroute` to check how packets are being routed within the network.
*   `route` and `ip route` to show any configured static routes.
*   `nmap` to scan for open ports on a device, or devices on a given subnet.

### Additional Resources

Here are a couple of websites with additional information which might be helpful.

*   <https://www.wireguard.com/quickstart/>
*   <https://github.com/pirate/wireguard-docs>
*   <https://www.stavros.io/posts/how-to-configure-wireguard/>
*   <https://gist.github.com/insdavm/b1034635ab23b8839bf957aa406b5e39>
*   <https://securityespresso.org/tutorials/2019/03/22/vpn-server-using-wireguard-on-ubuntu/>
*   <http://www.ducea.com/2006/08/01/how-to-enable-ip-forwarding-in-linux/>
