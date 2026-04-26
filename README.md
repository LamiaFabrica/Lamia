# Lamia

Lamia is the LamiaFabrica language and runtime bridge: a C++26-first system for moving between native code, MedusaServ-facing web surfaces, SSR+/PSR+ rendering, manufacturing workflows, 3D emotion primitives, and future PsiForceDB entity processors without treating HTML or JSON blobs as the final architecture.

This repository is being rebuilt as the clean LamiaFabrica source of truth. It carries source, language examples, public integration adapters, tests, docs, and manifest rules. It does not carry compiled binaries, local editor state, generated release bundles, private credentials, or closed-source PsiForceDB internals.

## Repository Role

- `src` and top-level `lamia_*.cpp` files contain the current compiler, bridge, generation, benchmarking, and framework work that is being consolidated into a stricter layout.
- `include`, `Lamia-Libs/include`, and `Lamia-Libs/src` carry the public Lamia library headers and source that need to become the stable SDK surface.
- `manufacturing_bridges`, `lib/3d_emotions`, and `mod_lamia` hold the current bridge layers that connect Lamia to manufacturing, emotion primitives, and MedusaServ-style execution.
- `Lamia-Docs`, `examples`, `tests`, and `docs` are retained as the public development and verification surface.
- PsiForceDB-specific secrets, private licensing material, and generated runtime state remain outside this public Lamia source repository.

## Build Direction

The target toolchain is C++26 with GCC 15.2.0 under WSL, using CMake as the consolidation target. Existing Makefiles and scripts are retained until their logic is folded into the CMake build without losing capability.

Compiled outputs belong in local build directories or release pipelines, not in git. If a binary is needed for distribution, it should be produced by the build, packaged by release automation, and traceable to committed source.

## Design Direction

Lamia exists to serve developer-first and engineer-first flows while keeping the LamiaFabrica ecosystem coherent:

- Construct Primers define how language, meaning, machine lingua, semantics, pragmatics, phonetics, symbology, writing, and experience are classified.
- Entity Processors define the public processing families, including table, vector, graph, holographic, security, simulation, and linguistic forms.
- MedusaServ-facing components remain public where appropriate.
- PsiForceDB-facing behaviour remains separated so the closed-source database can consume Lamia without exposing private security architecture.

## Current Consolidation Documents

- `docs/policies/LAMIA_REPO_CONSOLIDATION_POLICY.md`
- `docs/manifests/CANONICAL_DEPENDENCY_ROOTS.md`
- `docs/manifests/dependency_roots.json`
- `docs/manifests/LAMIA_ECOSYSTEM_REFERENCE_MAP.md`
- `docs/manifests/LAMIA_SOURCE_CARRY_FORWARD_MANIFEST.md`
- `docs/primers/CONSTRUCT_PRIMERS.md`
- `docs/primers/ENTITY_PROCESSORS.md`

## Origin

Lamia is maintained under LamiaFabrica by D Hargreaves / Roylepython as part of the wider LamiaFabrica, MedusaServ, BertieBot, Purple Pages, and PsiForceDB ecosystem.

Repository: `https://github.com/LamiaFabrica/Lamia`
