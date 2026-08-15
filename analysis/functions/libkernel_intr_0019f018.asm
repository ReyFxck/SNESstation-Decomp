# Focused committed target extract: DIntr/EIntr.
  19f018: 00 60 03 40  \tmfc0\t$3, $12, 0x0
  19f01c: 01 00 02 3c  \tlui\t$2, 0x1
  19f020: 24 18 62 00  \tand\t$3, $3, $2
  19f024: 2d 20 00 00  \tmove\t$4, $zero
  19f028: 0a 00 60 10  \tbeqz\t$3, 0x19f054
  19f02c: 2b 28 03 00  \tsltu\t$5, $zero, $3
  19f030: 39 00 00 42  \tdi
  19f034: 0f 04 00 00  \tsync 0x10
  19f038: 00 60 03 40  \tmfc0\t$3, $12, 0x0
  19f03c: 01 00 02 3c  \tlui\t$2, 0x1
  19f040: 24 18 62 00  \tand\t$3, $3, $2
  19f044: 00 00 00 00  \tnop
  19f048: f9 ff 60 14  \tbnez\t$3, 0x19f030
  19f04c: 00 00 00 00  \tnop
  19f050: 2d 20 a0 00  \tmove\t$4, $5
  19f054: 08 00 e0 03  \tjr\t$ra
  19f058: 2d 10 80 00  \tmove\t$2, $4
  19f05c: 00 00 00 00  \tnop
  19f060: 00 60 02 40  \tmfc0\t$2, $12, 0x0
  19f064: 01 00 03 3c  \tlui\t$3, 0x1
  19f068: 24 10 43 00  \tand\t$2, $2, $3
  19f06c: 38 00 00 42  \tei
  19f070: 08 00 e0 03  \tjr\t$ra
  19f074: 2b 10 02 00  \tsltu\t$2, $zero, $2
