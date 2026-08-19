# HUNT500+ V29 — uppercase .CPP language fix + bounded cache harvest

- Base checkpoint: **471/1041**
- Final checkpoint: **473/1041 (45.44%)**
- New strict matches: **2**
- Strategy: reuse V28's failed retry matrix, force uppercase `.CPP` files through the GCC 3.2.2 C++ frontend with `-x c++`, reuse the verified zlib/include environment, then run strict closure over new objects plus a bounded V23-V29 cache pool.
- The V21/V27 14k-object global closure is intentionally not repeated.
- No target machine bytes are inserted into sources or candidates.
- Promotion policy: exact next-boundary size + relocation-normalized equality + no unknown relocations; ambiguous fingerprints remain evidence-only.
- Evidence: `analysis/matching/hunt500plus-v29-validated-2.tsv`
