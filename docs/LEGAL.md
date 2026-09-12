# Legal and provenance notes

This repository intentionally ships no original SNES Station executable, no
unpacked executable image, no extracted embedded IRX payloads, no SNES ROMs,
and no PlayStation 2 BIOS material.

The single documentation exception is
`assets/snes-station-logo.png`: a 382x70 PNG rendering of the homebrew's own
title asset, used in the README to identify the subject of the preservation
project. It is not linked into a build and is not treated as recovered source.
Its SHA-256 is
`79d2dda621545c43b2ace805baa73fd9de6e974b42d0d32c3cf80620554c3ef3`.
Copyright and trademark rights remain with their respective owners.

The target binary is required only as a user-supplied local reference and is excluded by `.gitignore`.

SNES Station identifies its emulation core as Snes9x 1.41. Portions reconstructed from that lineage may therefore remain subject to the historical Snes9x license and copyrights. PS2-specific code and modifications have separate original authorship. This repository does not claim ownership over those original works and does not grant rights it does not possess.

The purpose of the project is preservation, interoperability, documentation, and reverse-engineering research.

Contributors should avoid adding third-party binaries or copyrighted assets
that are not necessary source-level reconstruction or identification material.
The logo exception above does not permit committing the background, font,
music, IRX modules, Memory Card icon or other extracted target resources.
