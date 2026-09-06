## Build

- `gcc main.c -o app -lglut -lGLU -lGL && ./app` — only documented build command, for `main.c`, run inside the dev container terminal (README.md:41).
- No test or lint command found (no manifest, no Makefile, no CI config at REPO_ROOT).

## Rules

- No manifest/build-system file exists at REPO_ROOT — build is a direct `gcc` invocation, not a package-manager script.
- Other root `.c` files (desert_scene.c, platformer_game.c, platformer_player.c, platformer_scene.c, player.c) have no documented build command in README.md — TODO: verify before assuming the `main.c` gcc line applies to them.

## Read first

1. main.c — the only documented entry point; full render/animation loop.
2. README.md — dev container setup and the only known build/run command.
3. player.h — small header, likely the shared interface for the platformer_* programs (not yet read/verified).

Architecture: see ARCHITECTURE.md — read before structural changes
