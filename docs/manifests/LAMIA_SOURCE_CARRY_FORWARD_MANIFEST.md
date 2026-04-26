# Lamia Source Carry-Forward Manifest

This manifest records what has been carried from the legacy Lamia tree into the rebuilt LamiaFabrica repository and how it should be rationalised.

## Core Language Lane

- `lamia_real_compiler.cpp`
- `lamia_compiler.cpp`
- `lamia_compiler_v3.cpp`
- `lamia_language_compiler.cpp`
- `lamia_native_compiler.cpp`
- `lamia_minimal_implementation.cpp`
- `lamia_enhanced_implementation.cpp`
- `lamia_complete_revolutionary_framework.cpp`
- `lamia_complete_revolutionary_framework.hpp`
- `lamia_language_specification.hpp`
- `lamia_minimal.hpp`

Destination direction: `src/compiler`, `src/runtime`, `include/lamia`, and C++26 module surfaces after build consolidation.

## Bridge And Ecosystem Lane

- `lamia_universal_bridge.cpp`
- `lamia_bridge_manufacturing.cpp`
- `lamia_bridge_simple.cpp`
- `lamia_bambu_integration.cpp`
- `lamia_manufacturing_final.cpp`
- `lamia_manufacturing_simplified.cpp`
- `manufacturing_bridges`
- `lib/3d_emotions`
- `mod_lamia`

Destination direction: `src/bridges`, `src/adapters/medusaserv`, `src/manufacturing`, and `src/emotion3d`.

## Developer Surface Lane

- `lamia_block_editor.hpp`
- `lamia_browser_validation.hpp`
- `lamia_extensible_architecture.hpp`
- `lamia_server_client_functions.hpp`
- `lamia_wysiwyg_editor.hpp`
- `Lamia-Docs`
- `Lamia-Plugins`
- `examples`
- `tests`

Destination direction: public SDK, examples, docs, and plugin registry after API naming is normalised.

## Generator And Release Lane

- `lamia_docs_generator.cpp`
- `lamia_github_generator.cpp`
- `lamia_libs_generator.cpp`
- `lamia_plugin_marketplace.cpp`
- `lamia_release_manager.cpp`
- `lamia_release_simple.cpp`
- `compile*.bat`
- `build_licensing_system.*`
- `test_and_deploy.bat`
- `scripts`

Destination direction: `tools`, `packaging`, and CMake presets. Generated outputs remain ignored.

## Medusa-Facing Lane

- `medusa_*.hpp`
- `medusa_*.cpp`
- `medusaserv_*.hpp`
- WebSocket compatibility shims after they have been replaced by maintained implementation files.

Destination direction: review into `src/adapters/medusaserv` or move back to MedusaServ if the file is server-owned rather than language-owned.

## Quarantine And Review Lane

- `LFSSL-main`: likely belongs in the LFSSL repository, not Lamia, unless a small public interface is required.
- `psiforce_*`: must be reviewed for closed-source boundary safety before public commit.
- Licensing portal files: may become examples, but private licence state is excluded by `.gitignore`.
- Root generated validation docs: preserve until the CMake build can produce fresh verification reports.

## Removed From New Repo Copy

Generated/local artefacts removed from `C:\McMaker Projects\Projects\Lamia` during rebuild:

- Top-level compiled Lamia executables without extensions.
- `lib/3d_emotions/lamia_3d_emotion_demo`.
- `mod_lamia` and `mod_lamia/web/site` compiled MedusaServ executables.
- `.so.backup` files.
- `connection_forensics.log`.
- `Montserrat.zip` duplicate font bundles.
- `desktop.ini` local Windows metadata.
