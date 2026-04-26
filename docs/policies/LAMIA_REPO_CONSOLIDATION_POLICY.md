# Lamia Repo Consolidation Policy

## Purpose

This policy keeps Lamia as the clean LamiaFabrica language source of truth while the wider project family is spring-cleaned. Lamia code is currently spread through `lamia_clean_final`, BertieBot, Purple Pages, MedusaServ, PsiForceDB, LamiaComms, LFSSL, and older archive folders. The rebuild must preserve working source without carrying generated clutter forward.

## Source Of Truth

`C:\McMaker Projects\Projects\Lamia` is the new LamiaFabrica working repository for public Lamia language, runtime, bridge, SDK, documentation, examples, and tests.

`C:\McMaker Projects\Projects\lamia_clean_final` is treated as the legacy source inventory until the consolidation is complete. It is not the final repo identity because it points at the older Medusa Initiative remote.

Canonical dependency roots are defined in `docs/manifests/CANONICAL_DEPENDENCY_ROOTS.md` and `docs/manifests/dependency_roots.json`. New code must use those roots instead of ad-hoc copies from older project folders.

## Carry Forward

- Lamia compiler, runtime, transpiler, bridge, generator, benchmark, and framework source.
- Public Lamia headers, examples, tests, and docs.
- MedusaServ-facing adapter code that can live publicly without exposing PsiForceDB internals.
- Manufacturing, Arduino, G-code, 3D emotion, visual, and web bridge code that is source-controlled and buildable.
- Construct Primer and Entity Processor definitions as public-facing terminology anchors.

## Do Not Carry Forward

- Compiled binaries, shared libraries, generated release packages, local logs, build directories, and backup binaries.
- Local `.cursor` and private `.vscode/mcp_settings.json` state.
- PsiForceDB private licensing data, generated master licenses, closed-source database credentials, and private deployment snapshots.
- SQL database implementations in the Lamia core. SQL can only appear in importer, migration, or compatibility lanes where the boundary is explicit.
- Architecture details that belong only in closed-source PsiForceDB.

## Ecosystem Boundaries

- Lamia owns the language/runtime/compiler/bridge layer.
- MedusaServ owns the open-source web-server execution environment and public server adapters.
- PsiForceDB owns closed-source database mechanics, zero-trust authentication, glowing-string storage semantics, queryable encryption, and private server architecture.
- LFSSL owns SSL/security library work that is reusable across LamiaFabrica projects.
- BertieBot and Purple Pages own governance, project management, policy automation, and developer workflow surfaces.

## Cleanup Rules

- New Lamia work must be placed in the rebuilt `Lamia` repo first, then consumed by sibling projects.
- Dependency references must resolve through the canonical root map, not through duplicated archives, copied `include_external` headers, or nested library drops.
- Generated artefacts must be produced from source, not committed as source.
- Public docs can describe integration points and inspiration, but must not reveal private PsiForceDB mechanisms.
- Any forked or borrowed library must be recorded under the LamiaFabrica fork ownership policy before it becomes a maintained dependency.
- If a file is unfinished but valuable, keep it as source and record its consolidation lane. Do not hide it behind a binary or generated wrapper.

## C++26 Direction

The consolidated build target is C++26 on WSL with GCC 15.2.0 and CMake. Legacy scripts and Makefiles remain only until their working behaviour is captured by CMake without reducing capability.
