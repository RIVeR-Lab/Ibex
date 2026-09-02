---
status: draft
owner:
last-verified:
---

<!--
HARDWARE PAGE TEMPLATE

Copy this file, rename it kebab-case, fill it in, delete these comments.

This page type is REASONING and REFERENCE. It owns what the component is, where it sits,
what feeds it, and the lookups needed to work with it. It does NOT own operational
procedures. Powering the vehicle on, running a collection, and bringing up a driver all
live in 02-operations/ — link to them.

Physical installation is the exception: mounting a component to the vehicle is a one-time
job done by whoever installs it, not a recurring operation, so it stays here.

Every section gets an answer. If a section does not apply, write "None" or "Not
applicable" — do not leave it blank. A blank section cannot be distinguished from an
unwritten one, which is what the `status` field in the frontmatter is for.

Do not invent values. Unknown part numbers, torque values, pinouts, and power draws get
TODO(verify) plus a note on what to measure or which document to read.
-->

# <Component name>

<!-- One sentence: what this is and which subsystem it serves. -->

## Overview

<!-- What the component does and why it is on the vehicle. What it contributes to the
system. If it serves more than one subsystem, say so and link to both. -->

## Physical location on vehicle

<!--
Where a person will physically find this. Be specific enough that someone who has never
opened the vehicle can locate it: "roof rack, forward crossmember, driver side" rather
than "on the roof."

Include what has to be removed to reach it, if anything. Link to a photo or CAD file in
docs/hardware/ if one exists.

If the component is distributed — a controller plus actuators at several points, or a box
plus remote sensors — use a table with one row per physical piece rather than trying to
write it as prose:

| Piece | Location | Access |
| --- | --- | --- |
| | | |
-->

## Power source / rail

<!--
Which battery, box, converter, or rail feeds this, and what has to be switched on before
it has power. Name the specific control if there is one.

This is the pairing that answers the question new people ask constantly: where is it, and
what turns it on. Keep it separate from the voltage and current figures in Hardware specs
— those describe what the component draws, this describes what supplies it.

Link to 04-subsystems/power/ for the rail design and to 02-operations/power-on.md for the
sequence. Do not restate either.
-->

## Hardware specs

<!--
Manufacturer, model, part number, serial number. Then the figures someone would otherwise
have to dig out of the datasheet.

Power draw belongs here: voltage, nominal current, nominal power, and startup or
worst-case draw if it differs materially.

Firmware or hardware revision belongs here too, with the date it was checked — this is
the field most likely to go stale silently.
-->

## Additional components

<!-- Interface boxes, breakout boards, brackets, adapters, cabling that is specific to
this component. 3D-printed parts get a link to the CAD file in docs/hardware/CAD Designs/
(percent-encode spaces as %20). -->

## Software

<!-- Link to the software pages that drive this. Do not describe the drivers here.

- Data structure / output format: what comes out of the device, in one or two lines
- Operational software: links to the relevant pages under */software/ -->

## Networking

<!--
Only for components on a network.

**Physical interface** — which port on which machine, and why that port. This is the part
that belongs here, because it is a fact about the physical build and the reasoning behind
it. Include how to re-verify it if cables get moved.

**Addressing** — do NOT put the address table here. 05-reference/network.md owns every
address on the vehicle so there is one place to look and one place to update. State the
hostname or how the device is addressed, then link to network.md for the table.

Anything unresolved (static addressing, MTU, jumbo frames) goes under Known issues and
gets cross-referenced from 99-appendix/open-questions.md.
-->

## Setup & calibration

<!--
**Physical installation** — how the component is mounted: fasteners, orientation, torque
if it matters. This is the one procedure that lives on a hardware page.

**Calibration** — what has to be calibrated, how often, and where the resulting values
are stored. If the result is an extrinsic transform, the values live in
05-reference/tf-frames.md and this section links to them rather than repeating them.

Driver installation and workspace setup are NOT here. Those belong on the software page
(Launch / invocation) and in 00-onboarding/workstation-setup.md.
-->

## Known issues & fixes

<!--
Symptom, cause, fix. Include the diagnostic command where there is one.

Unresolved issues stay here and get cross-referenced from
99-appendix/open-questions.md. Say plainly when something is unverified rather than
implying it is settled.

If an issue affects whether an operator can take manual control, stop the vehicle, or
trust an indicator, it must also be cross-referenced from 01-safety/estop-chain.md as a
known gap. The reasoning stays here; safety links in. A reader of the safety pages must
not be able to form a false belief about what a control guarantees.

Distinguish a fix that is INSTALLED from a fix that is PROPOSED. Write "proposed, not
implemented" explicitly where that is the case — an unbuilt solution written in the past
tense reads as a solved problem.
-->

## Datasheets

<!-- Vendor datasheets and manuals. Prefer the copy in docs/hardware/ over an external
URL, since the external one can move; include both where possible. Cite page numbers when
referencing a specific figure elsewhere on this page. -->

## Reorder

<!-- What to buy to replace this, and from where. Manufacturer part number, vendor, and
approximate lead time if known. Consumables and spares that pair with it. Consolidated
into 99-appendix/reorder.md. -->

## Related pages

<!--
Links out. Typical targets:
- The subsystem README this belongs to
- The software page(s) that drive it
- 05-reference/network.md, tf-frames.md
- 04-subsystems/power/ for the rail that feeds it
- 02-operations/power-on.md for the sequence that energizes it
-->
