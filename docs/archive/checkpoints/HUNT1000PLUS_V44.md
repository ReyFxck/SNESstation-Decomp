# HUNT1000+ V44 — `InitGraphField` syscall wrapper

V44 promoted `InitGraphField` at `0x00199160`, moving matching from **724/1041
(69.55%)** to **725/1041 (69.64%)**.

The EE build now expresses the historical five-instruction syscall wrapper with
explicit R5900 register constraints, while the host-syntax branch retains the
portable helper model. The 20 candidate bytes match exactly under `-O2`; the
four bytes before the next sparse manifest entry are verified target zeros.

Evidence:
[`analysis/matching/hunt1000plus-v44-validated-1.tsv`](../analysis/matching/hunt1000plus-v44-validated-1.tsv).
