# Historical EE compiler matrix

| Candidate | Date | Repository role | Confidence |
|---|---|---|---|
| `archive/ee-gcc3.2-030210-beta2.tar.gz` | 2003-02-10 | older control/fingerprint | historical candidate |
| `archive/ee-gcc3.2-030926.tar.gz` | 2003-09-26 | strongest pre-target compiler fingerprint candidate | candidate, bytes must decide |
| GCC 3.2.2 + vendored PS2DEV patch | patch labels itself BETA 3, 2004-02-14 | reproducible source build used by current stage-one bootstrap | proven useful, globally newer than target |

Target date: January 24, 2004.

The archived decomp.me compiler bundles are preserved as historical compiler
artifacts. They may be host-platform binaries and are not assumed to execute
natively on ARM64 DroidSpaces. Their primary purpose in this repository is
provenance/fingerprint preservation; execution compatibility is a separate
question.

No compiler is considered the exact global SNES Station compiler solely from
its date. Function bytes are the gate.
