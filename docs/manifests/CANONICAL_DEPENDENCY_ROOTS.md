# Canonical Dependency Roots

This manifest defines the single source-of-truth folders for LamiaFabrica work on
this machine. New development must resolve dependencies through these roots
instead of copying code from older archive, backup, or duplicated project folders.

Repository identity is also being consolidated under LamiaFabrica. Any current
remote under an older personal namespace is treated as existing wiring only until
the matching LamiaFabrica repository is initialized and validated.

## Active Roots

| Root Key | Local Path | Role |
| --- | --- | --- |
| `LAMIA_PUBLIC_ROOT` | `C:\McMaker Projects\Projects\Lamia` | Public Lamia language, runtime, compiler, SDK, adapters, examples, docs, and tests. |
| `LAMIA_PRIVATE_ROOT` | `C:\McMaker Projects\Projects\Lamia-Private` | Private Lamia product bridges, closed-source integration code, private manifests, and internal orchestration. |
| `PFQL_ROOT` | `C:\McMaker Projects\Projects\PFQL` | Public PsiForce Query Language grammar, parser, CLI, SDK, docs, and tooling root. |
| `SUPER_COLLIDER_ROOT` | `C:\McMaker Projects\Projects\Super-Collider` | LamiaFabrica-maintained SuperCollider fork and future C++26 uplift lane. |
| `PSIFORCEDB_PRIVATE_ROOT` | `C:\McMaker Projects\Projects\PsiForceDB_1-0-0` | Closed-source PsiForceDB engine, server, PFQL, and private database mechanics. |
| `MEDUSASERV_PUBLIC_ROOT` | `C:\McMaker Projects\Projects\MedusaServ Professional Web Server` | Open-source MedusaServ server/runtime surface and public web execution layer. |
| `BERTIEBOT_ROOT` | `C:\McMaker Projects\Projects\BertieBot` | Project governance, policy automation, build orchestration, and AI coordination surfaces. |
| `PURPLE_PAGES_ROOT` | `C:\McMaker Projects\Projects\Purple-Pages` | Developer-first policy, workflow, and knowledge surfaces. |
| `LFSSL_PUBLIC_ROOT` | `C:\McMaker Projects\Projects\LFSSL - Lamia Fabrica SSL` | Reusable SSL and security library root. |
| `FCCP_ROOT` | `C:\McMaker Projects\Projects\FCCP` | LamiaFabrica file and communication protocol root. |
| `GRAVITY_CDN_ROOT` | `C:\McMaker Projects\Projects\GravityCDN` | CDN/cache distribution root. |
| `LAMIACOMMS_ROOT` | `C:\McMaker Projects\Projects\LamiaComms Collaberation Hub` | Collaboration hub consumer and integration root. |

## Retired Or Intake-Only Roots

| Path | Status | Rule |
| --- | --- | --- |
| `C:\McMaker Projects\Projects\lamia_clean_final` | Legacy intake | Read only for source recovery, never as a new dependency root. |
| `C:\McMaker Projects\Projects\PsiForceDB_1.0.0` | Superseded lineage | Do not add new references; migrate useful deltas into `PSIFORCEDB_PRIVATE_ROOT` after review. |
| `C:\McMaker Projects\Projects\PsiForceDB_1-0-0-20260416` | Archive snapshot | Audit only; do not wire into active builds. |
| `C:\McMaker Projects\Projects\Archive - Pre Secret Development 1` | Archive snapshot | Audit only; do not wire into active builds. |
| Nested `include_external/liblamia_*` copies | Duplicate Lamia API material | Replace with public Lamia SDK includes from `LAMIA_PUBLIC_ROOT`. |
| Nested `deps/supercollider` copies | Duplicate fork material | Replace with `SUPER_COLLIDER_ROOT` or a pinned submodule that points at the LamiaFabrica fork. |

## Resolution Rules

- Consumers must include Lamia headers from `LAMIA_PUBLIC_ROOT` or an installed Lamia SDK produced from that root.
- PFQL language consumers must use `PFQL_ROOT` instead of project-local command/parser copies.
- New remotes should be initialized under LamiaFabrica unless a project is deliberately kept under a different owner for a documented reason.
- Private PsiForceDB integration must stay behind `LAMIA_PRIVATE_ROOT` or `PSIFORCEDB_PRIVATE_ROOT` boundaries.
- SuperCollider work must use the LamiaFabrica fork at `SUPER_COLLIDER_ROOT`; upstream remains attribution and merge-source only.
- Build outputs must live under local build directories and never become dependency roots.
- A dependency root change must update this manifest and `dependency_roots.json` in the same batch.
- When a duplicate copy is discovered, keep it as audit input until the source has been reconciled into the canonical root, then remove the reference path from active configuration.

## Current Follow-Up Targets

- Repoint Lamia consumers away from `include_external/liblamia_*` toward the public Lamia SDK.
- Repoint PsiForceDB SuperCollider references away from local duplicate dependency state and toward the LamiaFabrica fork.
- Keep public Lamia and private Lamia integration separated so open-source releases do not expose closed-source PsiForceDB mechanics.
