# Shared Library Boundaries

LamiaFabrica projects share capability through loadable libraries, not by copying private source into every project. Lamia owns the public runtime loader and public language-facing adapters. Proprietary PsiForceDB mechanics remain behind private DLL/SO boundaries and are loaded through declared roots.

## Boundary Rules

- A project loads a module from a registered root, not from arbitrary current-working-directory state.
- A public Lamia module can be shared across Lamia, MedusaServ, BertieBot, Purple Pages, and PsiForceDB consumers.
- A private PsiForceDB module can be consumed through a stable ABI without moving closed-source implementation into Lamia.
- Generated libraries are build outputs and are not committed to the Lamia source repository.
- Every module must have a logical name, a root name, and a relative path so BertieBot and future policy tooling can reason about ownership.

## Runtime Implementation

The public implementation starts in:

- `include/lamia/runtime/shared_library.hpp`
- `src/runtime/shared_library.cpp`

The loader supports Windows DLLs and Linux/macOS shared libraries, enforces registered roots, blocks relative path escape, provides logical library mapping, resolves symbols, and unloads libraries through RAII.

## Singular Location Pattern

Use this structure when promoting scattered Lamia modules into one maintained location:

- `LAMIA_PUBLIC_ROOT`: public Lamia runtime/compiler/adapter libraries.
- `MEDUSASERV_PUBLIC_ROOT`: open-source MedusaServ server modules.
- `LFSSL_PUBLIC_ROOT`: reusable security libraries.
- `PSIFORCEDB_PRIVATE_ROOT`: closed-source database modules, never copied into Lamia.
- `LOCAL_BUILD_ROOT`: generated developer build artefacts ignored by git.

Each consumer should point at these roots rather than carrying duplicated source or stale compiled binaries.
