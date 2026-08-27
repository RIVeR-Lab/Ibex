---
status: draft
owner: jtukpah
last-verified: 2026-08-27
---

# Authoring guide

Read this before adding or editing a page in `docs/manual/`. It exists so that the next
person does not invent a fourth page type or a third template.

Templates in this directory:

- [software-page.md](software-page.md) — any ROS package, driver, or vendor application
- [hardware-page.md](hardware-page.md) — any physical component on the vehicle

## One-home rule

Every fact appears exactly once, on the page that owns it. Duplication is the failure mode
this manual is built to avoid: two copies of a power-on sequence, a wiring change updates
one of them, and now a safety procedure in the repository is wrong.

If you find yourself copying a paragraph from another page, stop and link to it instead.
Cross-references cost nothing.

## The three page types

Every page is one of these. Not two.

**Procedure** — owns steps. Imperative, numbered, no explanation. The reader is doing this
right now, possibly with one hand on the vehicle.

> Press the main power button on the Kairos box. The red LED illuminates.

**Reasoning** — owns *why*. Design rationale, tradeoffs, known issues, what a control
actually cuts. The reader is trying to understand the system before they change it.

> The relay bypass uses an NTC thermistor because inrush current on the converter tripped
> the relay during cold starts.

**Reference** — owns lookups. Tables you grep, not prose you read. The reader knows what
they want and needs the value.

> | Topic | Type | Publisher | Rate |

### Placement test

Ask: is this something someone **does**, something someone needs to **understand**, or
something someone **looks up**?

If the answer is genuinely two of those, you have two paragraphs that got merged. Split
them and put each where it belongs. Both get better.

Worked example. The Kairos P4S4 source note contained both `Setup & calibration`
(mounting the rack, connecting the brake — installation work a researcher does once, if
ever) and `Known issues & fixes` (the system is not reliably back-drivable — something
they must understand *before* touching the vehicle). Different readers, different urgency,
different pages.

## Link directionality

Links run one way: **procedures link out to subsystems for "why"; subsystem pages never
restate procedures.**

A procedure page may say "see [estop-chain.md](../01-safety/estop-chain.md) for what this
cuts." A subsystem page may not contain a numbered power-on sequence. If a subsystem page
needs to reference an operation, name it and link to it.

Sections 00–03 are task-ordered and mostly procedure. Sections 04–05 are
component-ordered and mostly reasoning and reference.

## Frontmatter

Every page. No exceptions.

```yaml
---
status: draft | reviewed | stable
owner: name
last-verified: YYYY-MM-DD
---
```

- `status` — `draft` means written but unchecked; `reviewed` means someone other than the
  author read it against the system; `stable` means it is trusted and changes rarely.
  This field exists because an empty heading is ambiguous: it cannot distinguish "nothing
  to say" from "not written yet."
- `owner` — a person, not a team. The person to ask.
- `last-verified` — the date someone last confirmed the page matches the physical system.
  This matters most on safety and operations pages. A power-on sequence with a
  `last-verified` date from before the last rewiring should not be trusted.

Update `last-verified` when you check a page, not when you edit prose in it.

## Conventions

**Filenames** are kebab-case: `kairos-p4s4.md`, not `Kairos P4S4.md`.

**Links are relative markdown links.** Never wikilinks. `[[Foo]]`, `[[Foo|Bar]]`, and
`![[image.png]]` render as literal text on GitHub, as do Obsidian callouts and Dataview
blocks. In Obsidian: Settings → Files & Links → disable "Use [[Wikilinks]]" and set "New
link format" to "Relative path to file." Obsidian keeps its graph view either way.

Percent-encode spaces in paths (`%20`) when linking into `docs/` assets, or the link
silently fails on GitHub.

**Vendor documentation is authoritative.** Link into the PDFs in `../../hardware/`; do not
restate them. Where a page summarizes a vendor manual, the summary is a convenience and
the PDF wins. Cite the page number.

**Transcribed vendor reference becomes tables and numbered steps**, not nested bullets.
Deeply nested lists are unreadable on GitHub past about two levels. If the source is a
six-level bullet tree, it is reference material and wants a table.

**Images** go in `../assets/images/` and are referenced relatively. Give them descriptive
kebab-case names.

**Unverified facts get marked, not guessed.** If you do not know a topic name, parameter
default, pinout, or torque value, write `TODO(verify)` with a note on what to measure or
which document to read. An invented value is worse than a gap, because a gap is visible.

## Forks and submodules

`kiss-icp`, `ouster-ros`, and `shared_link_bridge` are forks vendored as submodules.

Integration documentation — our configuration, our quirks, which branch we track, what we
changed and why — lives in the parent repository, on the relevant subsystem page. Only
genuinely upstreamable material goes inside a submodule, because anything written there
either diverges from upstream or has to be carried forward on every rebase.

Every software page for a fork fills in the **Fork status** field: upstream URL, what we
changed, and whether it is upstreamable.

## Never write these

- **Credentials.** No usernames paired with passwords, no API keys, no tokens. Point to
  the lab password manager.
- **Physical security details.** No door or cubicle codes, no key storage locations.
  `01-safety/` may state that key control exists and is governed by the SOP.

If you encounter either in source material you are migrating, stop and report it before
continuing. Deleting the line does not fix it — git history is permanent, so the
credential must be rotated.
