# Lamia Ecosystem Reference Map

Generated from the local project tree on 2026-04-26. This is a consolidation map, not a deletion list.

## Primary Lamia Repositories

- `C:\McMaker Projects\Projects\Lamia`: rebuilt LamiaFabrica repository, remote `https://github.com/LamiaFabrica/Lamia.git`.
- `C:\McMaker Projects\Projects\Lamia-Private`: private LamiaFabrica repository for closed-source Lamia integration and product bridges.
- `C:\McMaker Projects\Projects\PFQL`: public LamiaFabrica repository for the PsiForce Query Language surface.
- `C:\McMaker Projects\Projects\lamia_clean_final`: legacy source inventory, remote `https://github.com/The-Medusa-Initiative/Lamia.git`.
- `C:\McMaker Projects\Projects\LamiaFabrica`: organisation/policy workspace for LamiaFabrica operations.
- `C:\McMaker Projects\Projects\LamiaComms Collaberation Hub`: collaboration surface that should consume Lamia through stable libraries instead of copied source.

Canonical dependency roots are now defined in `CANONICAL_DEPENDENCY_ROOTS.md` and `dependency_roots.json`. This reference map explains the project lanes; the root manifest is the authority for active path resolution.

## High-Signal Consumers

- `PsiForceDB_1-0-0`: consumes Lamia through database/client/server integration paths. PsiForceDB-specific logic stays private and is not moved into public Lamia.
- `MedusaServ Professional Web Server`: should consume public Lamia adapters for SSR+/PSR+, web rendering, forms, notifications, and server-side execution.
- `BertieBot`: currently contains Lamia intelligence, MCP, policy, and governance code. Reusable Lamia language components should move into Lamia; BertieBot-specific orchestration should stay in BertieBot.
- `Purple-Pages`: contains policy, governance, repo organisation, and developer workflow material. Lamia policy docs may be mirrored there, but implementation source belongs in Lamia.
- `LFSSL - Lamia Fabrica SSL`: security library work should remain a separate reusable library, with Lamia consuming it through defined interfaces.
- `Super-Collider`: external fork maintenance is separate from Lamia, but LamiaFabrica dependency policy applies.

## Observed Cleanup Hotspots

- `BertieBot\include_external\liblamia_*.hpp`: candidate for replacement with Lamia SDK include paths.
- `BertieBot\v0.4.1\lamia\*`: likely valuable C++26/module consolidation work to inspect before any deletion.
- `BertieBot\lamia_clean_final\*`: duplicated older Lamia browser/server module work; review and fold into Lamia if still current.
- `Purple-Pages\Purple-Pages.com\BertieBot\*`: mirrored BertieBot material; treat as archive or documentation unless it contains newer source.
- `PsiForceDB_1-0-0\src\lamia\*` and Lamia-related headers: should become clean consumers of Lamia APIs, not a second Lamia implementation.
- `PsiForceDB_1-0-0\include\psiforcedb\pfql`, `src\pfql`, and `src\server\PFQL_Command_System*`: should become consumers of the canonical PFQL public SDK where the code is language-owned rather than private engine-owned.
- `PsiForceDB_1-0-0\deps\supercollider`: should be replaced or repointed to the LamiaFabrica fork root instead of drifting as an independent duplicate.
- `MedusaServ Professional Web Server` Lamia references: should resolve through public Lamia/MedusaServ adapter boundaries.

## Consolidation Order

1. Stabilise the new Lamia repo with source-only rules, primer docs, and carry-forward manifests.
2. Pull any newer Lamia compiler/runtime/module work from `BertieBot\v0.4.1\lamia` into the Lamia repo after inspection.
3. Convert copied `include_external\liblamia_*` users to include Lamia SDK headers.
4. Convert language-owned PFQL command/parser code to consume `C:\McMaker Projects\Projects\PFQL`.
5. Repoint SuperCollider consumers to `C:\McMaker Projects\Projects\Super-Collider`.
6. Keep PsiForceDB private mechanics in PsiForceDB and expose only the minimum stable Lamia interface needed by the database.
7. Move shared security through LFSSL interfaces rather than copying LFSSL source into Lamia.
8. Replace scattered binary drops with CMake-built artefacts from committed source.

## Scan Notes

The full raw `rg` scan is intentionally not committed because the match list includes archives, mirrored projects, local MCP settings, and inaccessible WSL-style symlinks. This map records the actionable lanes rather than the noise.
