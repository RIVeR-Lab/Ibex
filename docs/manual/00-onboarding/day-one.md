---
status: draft
owner: TODO(verify)
last-verified: TODO(verify)
---

# Day one

This page is a reading order, not a reference. Each item is a link and a reason to read it
now. The explanations live on the linked pages — follow them rather than expecting this
page to summarize.

The goal for today is understanding, not operation. You will not drive Ibex today.

## Start this before anything else

Kick off [workstation-setup.md](workstation-setup.md), sections 1 through 4. The clone and
`colcon build` take a long time and mostly involve waiting, so start them now and read the
rest of this list while they run.

Stop at section 5. The vendor SDKs take hours and are only needed for the hyperspectral
cameras and point spectrometers — do them later, and only if your work touches that
hardware.

## The day, in order

**1. [Manual front door](../README.md)**
Five minutes. How the manual is laid out and where to look for what. Understanding the
structure saves you from reading the wrong page later.

**2. [Glossary](glossary.md)**
Skim it. Every page that follows says P4S4, OCU, SharedLink, HSI, and `hyper_drive`
without stopping to explain them.

**3. [Safety overview](../01-safety/README.md) and [the SOP](../01-safety/sop.md)**
Not optional and not skimmable. The SOP is the agreed standard governing vehicle use in
the EXP high bay, and it defines what you are permitted to do and when.

**4. [The e-stop chain](../01-safety/estop-chain.md)**
What each stop control actually cuts, and where the gaps are. Read this before you are
near the vehicle while it is powered. The gaps are the part people get wrong — in
particular, engaging the drive-by-wire system does not guarantee you can take manual
control of the steering, throttle, brake, or transmission.

**5. [System block diagram](../04-subsystems/README.md)**
Sensors through compute to actuators, with network segments and power rails. This is the
mental model the rest of the manual hangs on. Spend real time here.

**6. Check on your build**
Return to [workstation-setup.md](workstation-setup.md) section 4 and confirm the build
completed. If it failed, the troubleshooting table at the bottom of that page covers the
common causes.

**7. [Power on](../02-operations/power-on.md) and [power off](../02-operations/power-off.md)**
Read both sequences, then find someone to execute them while you watch. Reading a power-on
sequence and seeing one are different things — the order matters, and several steps have
no visible feedback if you get them wrong.

**8. [Ownership](ownership.md)**
Who owns which subsystem. Read it before you need it, so that your first question goes to
the right person.

## What you may not do yet

Ibex requires a supervised period before you operate it independently. This is a safety
requirement, not a formality — see [01-safety/README.md](../01-safety/README.md) for the
rule and what releases it.

Vehicle access is governed by the SOP, including key control. If you need access, ask the
person listed in [ownership.md](ownership.md) rather than looking for a workaround.

> TODO(verify): the supervision requirement needs to be stated concretely in
> `01-safety/README.md` — who is qualified to supervise, what a new person may do while
> supervised, and what specifically releases the gate. This page links to that rule and
> should not restate it.

## After day one

No fixed timeline. You start as an operator and move toward development work as you get
comfortable, and how long that takes depends on you and on what you are working on. Talk
to your subsystem owner about pacing.

Read these next, in roughly this order:

- [01-safety/checklists.md](../01-safety/checklists.md) — pre-run and post-run. You will
  use these every session.
- [02-operations/running-the-system.md](../02-operations/running-the-system.md) — bringing
  up the software stack.
- [02-operations/data-collection.md](../02-operations/data-collection.md) — how a
  collection is run and how it is recorded.
- The subsystem you will be working on, under
  [04-subsystems/](../04-subsystems/) — its README first, then the hardware and software
  pages beneath it.
- [05-reference/](../05-reference/) — the ROS graph, frame tree, and network map. Skim once
  so you know what is there, then return to them as lookups.
- [03-base-vehicle/](../03-base-vehicle/) — the Wolverine underneath. Relevant when
  something mechanical is wrong rather than something autonomous.
- [99-appendix/open-questions.md](../99-appendix/open-questions.md) — what we do not know
  yet. Worth reading early; you may be the one who resolves an item.

Field work adds [02-operations/transport.md](../02-operations/transport.md) and
[02-operations/field-sites.md](../02-operations/field-sites.md) when it becomes relevant.

## If a page is wrong

Documentation drifts from hardware. Check the `last-verified` date in a page's
frontmatter before trusting it, and fix what you find wrong — a correction from someone
following the steps for the first time is worth more than one from someone who already
knows the answer.
