---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults

layout: default
title: Portfolio
---

![Lock and Dam Along the Erie Canal](/assets/wallpaper-erie-canal.jpg)
{: style="width: 100%; margin: 0 auto;"}

Gabriel is an undergraduate student at Cornell University studying mechanical engineering with a focus on environmental sustainability.
He is interested in applying engineering solutions to environmental issues in fields such as renewable energy and transportation. This webpage serves as a portfolio with some of Gabriel's research and coursework. The [posts](/posts) section also includes documentation about various projects.
{: style="text-align: left; margin-top: 5%;"}

---
{: style="text-align: left; margin: 8%; margin-left: 40%; margin-right: 40%;"}

# Professional Experience

## Wave-Powered Aquaculture

[![Map of Fishing Conflicts](/assets/conflicts-map.png)](/assets/conflicts-map.png)
{: style="width: 60%; float: left; margin: 2%;"}

My research with the [SEA Lab](https://sea.mae.cornell.edu/) has focused on developing maps and models to assess the potential of wave-powered aquaculture along the northeast coast of the US. Offshore aquaculture powered by wave energy converters (WECs) has several benefits including decreased local environmental impact and fossil fuel use. By estimating the carrying capacity of different locations and looking at potential conflicts between fisheries and offshore infrastructure, we have been able to identify promising sites along with yield and costs estimates for them. I work alongside a PhD student and a post-doc in the lab, in addition to several industry partners and groups who we consult for information about their logistics and to ensure that our research is relevant to their work.

My portion of the project has focused on processing GIS datasets of environmental conditions and integrating them into a Python model that optimizes for carrying capacity and cost. I have also used some techniques from marine spatial planning to analyze other factors such as conflicts between offshore wind development and commercial fishing. This map shows some of those conflicts and highlights one of the optimal locations we have found for a wave-powered aquaculture farm.

## Mini-WEC

This miniature wave energy converter is another project at the [SEA Lab](https://sea.mae.cornell.edu/) where a PhD student and I built a small but functional WEC for use as a teaching tool in a K-12 classroom. The WEC works in a small aquarium, uses a rack and pinion as a power take-off mechanism, and uses an Arduino running a feedback control loop to control power generation. The device can generate some power, but further testing is needed to compare it to our modeled predictions. We also brought the WEC to a local third grade classroom and used it as a part of a lesson on renewable energy. After learning about WECs more generally, the children were able to make waves in the tank and see how much energy they could generate.

{% include image-gallery.html folder="assets/mini-wec" %}

## Cyclikal

[![Rack of Keithley Power Supplies](/assets/keithleys.jpg)](/assets/keithleys.jpg)
{: style="width: 40%; float: right; margin: 2%;"}

I worked as a developer and lab technician at [Cyclikal](https://cyclikal.com/), a company providing high precision calorimetry testing for lithium-ion batteries.

My work focused on developing the [Cyckei](https://docs.cyclikal.com/projects/cyckei/en/stable/) software package which automates voltage and current steps, and data handling while cycling li-ion cells. The software and accompanying tools are open source and [available on GitHub](https://github.com/cyclikal/cyckei).

# Course Projects

## Mechanical Synthesis

My course on mechanical synthesis focused on the design and manufacturing of mechanical components. I worked on two final projects as well as several smaller pieces and CAD models throughout the course including a Reuleaux machine and a milk frother to practice more complex designs.

For the original design project, a partner and I interviewed a nurse about the challenges he experienced in a hospital setting. We identified the transportation of patients as a key issue and created a prototype hospital bed to address this. The bed uses a removable stretcher, attachments for medical equipment, and a scissor lift mechanism to improve patient transportation. We prototyped the device by laser-cutting the bed and scissor lift mechanism and using a rack and pinion to control the bed's lift.

The second project involved designing a water pump with a small group. We chose to create a peristaltic pump which works without having fluid contact the pump mechanism. Our design is adjustable for different tubing sizes and uses a combination of machined and laser-cut components. Much of the process was collaborative, but I focused on many of the initial sketches and fabricating the pump rollers on a lathe.

{% include image-gallery.html folder="assets/design" %}

## Mechatronics & Fluids

In mechatronics I learned about circuitry and microcontroller use in engineering projects. The course culminated in two final projects: a group project that included building a small autonomous robot for a competition, and an individual project where I prototyped a bike light that changes behavior based on speed. Both used Arduino microcontrollers, a variety of sensors, and laser-cut and 3D printed components. More documentation for the bike light is included in [this post]({% post_url 2021-12-03-speed-controlled-bike-light %}).

Also pictured is the hard disk from a "fluid mechanical dissection" which I completed with a small team to understand the fluid flow in a hard drive air bearing. This air bearing uses viscous flow, as described by the Navier-Stokes equation, to keep the head elevated at a precise distance above the plate.

{% include image-gallery.html folder="assets/mech-fluids" %}

# Independent Design

I have explored fabrication of small components with 3D printing and manual machining techniques. Some of these projects are bicycle related and stem from issues I have run into while working as a professional bicycle mechanic, while others are just to practice designing and fabricating various objects. Pictured are a selection of items I have made including machined aluminum top caps for bike cockpits, a front light and GoPro compatible mount, and some parts that help with common bike repairs such as hydraulic brake caliper service and bleeding.

{% include image-gallery.html folder="assets/hobby" %}

<style>
  h1 {
    margin-top: 8%;
  }
</style>

<script type="text/javascript" src="/js/lightbox.js"></script>
<link rel="stylesheet" href="/css/lightbox.css">
