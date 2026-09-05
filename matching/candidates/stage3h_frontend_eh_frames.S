/*
 * Reconstructed GCC 3.2.2 EE unwind tables for the original frontend.
 *
 * The old EE compiler emits its DWARF CIE/FDE records into writable data.
 * Every row below describes unwind semantics already visible in the exact
 * public function bodies: stack-frame size, instruction advance, and saved
 * register/CFA offset.  Code, LSDA and personality addresses remain linker
 * symbols; this file contains no extracted target payload.
 */

    .macro STAGE3H_ADVANCE bytes
    .byte 0x04                    /* DW_CFA_advance_loc4 */
    .4byte \bytes
    .endm

    .macro STAGE3H_CFA bytes
    .byte 0x0e                    /* DW_CFA_def_cfa_offset */
    .uleb128 \bytes
    .endm

    .macro STAGE3H_SAVE reg, factor
    .byte 0x11                    /* DW_CFA_offset_extended_sf */
    .uleb128 \reg
    .sleb128 \factor             /* CIE data alignment factor is +4 */
    .endm

/* Frontend translation unit: zPL CIE and eleven throwing-call FDEs. */
    .section .data.stage3h.frontend_eh_1,"aw",@progbits
    .align 2
.Lframe_g1:
    .4byte .Lecie_g1-.Lscie_g1
.Lscie_g1:
    .4byte 0
    .byte 1
    .ascii "zPL\0"
    .uleb128 1
    .sleb128 4
    .byte 64
    .uleb128 6
    .byte 0                       /* absolute personality encoding */
    .4byte stage3h_gxx_personality
    .byte 0                       /* absolute LSDA encoding */
    .byte 0x0c                    /* DW_CFA_def_cfa */
    .uleb128 29
    .uleb128 0
    .align 2
.Lecie_g1:

    .4byte .Lfde_104e58_end-.Lfde_104e58_body
.Lfde_104e58_body:
    .4byte .Lfde_104e58_body-.Lframe_g1
    .4byte stage3h_pc_00104e58
    .4byte 0x24
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 8
    STAGE3H_CFA 16
    STAGE3H_ADVANCE 4
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_104e58_end:

    .4byte .Lfde_104e7c_end-.Lfde_104e7c_body
.Lfde_104e7c_body:
    .4byte .Lfde_104e7c_body-.Lframe_g1
    .4byte stage3h_pc_00104e7c
    .4byte 0x9c
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 112
    STAGE3H_ADVANCE 4
    STAGE3H_SAVE 17, -20
    STAGE3H_ADVANCE 16
    STAGE3H_SAVE 20, -8
    STAGE3H_SAVE 64, -4
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 19, -12
    STAGE3H_ADVANCE 12
    STAGE3H_SAVE 16, -24
    STAGE3H_SAVE 18, -16
    .align 2
.Lfde_104e7c_end:

    .4byte .Lfde_104f18_end-.Lfde_104f18_body
.Lfde_104f18_body:
    .4byte .Lfde_104f18_body-.Lframe_g1
    .4byte stage3h_pc_00104f18
    .4byte 0x798
    .uleb128 4
    .4byte stage3h_lsda_00426b71
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 96
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 19, -8
    STAGE3H_ADVANCE 16
    STAGE3H_SAVE 18, -12
    STAGE3H_SAVE 64, -4
    STAGE3H_SAVE 17, -16
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 16, -20
    .align 2
.Lfde_104f18_end:

    .4byte .Lfde_1056c0_end-.Lfde_1056c0_body
.Lfde_1056c0_body:
    .4byte .Lfde_1056c0_body-.Lframe_g1
    .4byte stage3h_pc_001056c0
    .4byte 0x20
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 16
    STAGE3H_ADVANCE 4
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_1056c0_end:

    .4byte .Lfde_105750_end-.Lfde_105750_body
.Lfde_105750_body:
    .4byte .Lfde_105750_body-.Lframe_g1
    .4byte stage3h_pc_00105750
    .4byte 0xac
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 32
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 16, -8
    STAGE3H_ADVANCE 40
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_105750_end:

    .4byte .Lfde_1057fc_end-.Lfde_1057fc_body
.Lfde_1057fc_body:
    .4byte .Lfde_1057fc_body-.Lframe_g1
    .4byte stage3h_pc_001057fc
    .4byte 0x9c
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 64
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 16, -16
    STAGE3H_ADVANCE 16
    STAGE3H_SAVE 17, -12
    STAGE3H_SAVE 18, -8
    STAGE3H_ADVANCE 44
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_1057fc_end:

    .4byte .Lfde_105898_end-.Lfde_105898_body
.Lfde_105898_body:
    .4byte .Lfde_105898_body-.Lframe_g1
    .4byte stage3h_pc_00105898
    .4byte 0x134
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 16
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_105898_end:

    .4byte .Lfde_105d30_end-.Lfde_105d30_body
.Lfde_105d30_body:
    .4byte .Lfde_105d30_body-.Lframe_g1
    .4byte stage3h_pc_00105d30
    .4byte 0x48
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 16
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_105d30_end:

    .4byte .Lfde_105d78_end-.Lfde_105d78_body
.Lfde_105d78_body:
    .4byte .Lfde_105d78_body-.Lframe_g1
    .4byte stage3h_pc_00105d78
    .4byte 0x8c
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 32
    STAGE3H_ADVANCE 12
    STAGE3H_SAVE 64, -4
    STAGE3H_SAVE 16, -8
    .align 2
.Lfde_105d78_end:

    .4byte .Lfde_105e04_end-.Lfde_105e04_body
.Lfde_105e04_body:
    .4byte .Lfde_105e04_body-.Lframe_g1
    .4byte stage3h_pc_00105e04
    .4byte 0x34
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 16
    STAGE3H_ADVANCE 4
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_105e04_end:

    .4byte .Lfde_105e48_end-.Lfde_105e48_body
.Lfde_105e48_body:
    .4byte .Lfde_105e48_body-.Lframe_g1
    .4byte stage3h_pc_00105e48
    .4byte 0x20c
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 8
    STAGE3H_CFA 6336
    STAGE3H_ADVANCE 12
    STAGE3H_SAVE 17, -16
    STAGE3H_SAVE 64, -4
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 16, -20
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 19, -8
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 18, -12
    .align 2
.Lfde_105e48_end:
    .4byte 0

/* Second frontend translation unit: two FDEs, with its own zPL CIE. */
    .section .data.stage3h.frontend_eh_2,"aw",@progbits
    .align 2
.Lframe_g2:
    .4byte .Lecie_g2-.Lscie_g2
.Lscie_g2:
    .4byte 0
    .byte 1
    .ascii "zPL\0"
    .uleb128 1
    .sleb128 4
    .byte 64
    .uleb128 6
    .byte 0
    .4byte stage3h_gxx_personality
    .byte 0
    .byte 0x0c
    .uleb128 29
    .uleb128 0
    .align 2
.Lecie_g2:

    .4byte .Lfde_1060dc_end-.Lfde_1060dc_body
.Lfde_1060dc_body:
    .4byte .Lfde_1060dc_body-.Lframe_g2
    .4byte stage3h_pc_001060dc
    .4byte 0x3e4
    .uleb128 4
    .4byte stage3h_lsda_00426b83
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 160
    STAGE3H_ADVANCE 4
    STAGE3H_SAVE 17, -12
    STAGE3H_ADVANCE 16
    STAGE3H_SAVE 18, -8
    STAGE3H_SAVE 64, -4
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 16, -16
    .align 2
.Lfde_1060dc_end:

    .4byte .Lfde_1064c0_end-.Lfde_1064c0_body
.Lfde_1064c0_body:
    .4byte .Lfde_1064c0_body-.Lframe_g2
    .4byte stage3h_pc_001064c0
    .4byte 0x344
    .uleb128 4
    .4byte 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 160
    STAGE3H_ADVANCE 24
    STAGE3H_SAVE 16, -20
    STAGE3H_SAVE 17, -16
    STAGE3H_SAVE 18, -12
    STAGE3H_SAVE 19, -8
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_1064c0_end:
    .4byte 0

/* Third translation unit has no LSDA encoding (zP CIE) and five FDEs. */
    .section .data.stage3h.frontend_eh_3,"aw",@progbits
    .align 2
.Lframe_g3:
    .4byte .Lecie_g3-.Lscie_g3
.Lscie_g3:
    .4byte 0
    .byte 1
    .ascii "zP\0"
    .uleb128 1
    .sleb128 4
    .byte 64
    .uleb128 5
    .byte 0
    .4byte stage3h_gxx_personality
    .byte 0x0c
    .uleb128 29
    .uleb128 0
.Lecie_g3:

    .4byte .Lfde_106824_end-.Lfde_106824_body
.Lfde_106824_body:
    .4byte .Lfde_106824_body-.Lframe_g3
    .4byte stage3h_pc_00106824
    .4byte 0x78
    .uleb128 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 64
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 17, -8
    STAGE3H_ADVANCE 12
    STAGE3H_SAVE 16, -12
    STAGE3H_ADVANCE 12
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_106824_end:

    .4byte .Lfde_10689c_end-.Lfde_10689c_body
.Lfde_10689c_body:
    .4byte .Lfde_10689c_body-.Lframe_g3
    .4byte stage3h_pc_0010689c
    .4byte 0x330
    .uleb128 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 144
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 64, -4
    STAGE3H_SAVE 16, -8
    .align 2
.Lfde_10689c_end:

    .4byte .Lfde_106bcc_end-.Lfde_106bcc_body
.Lfde_106bcc_body:
    .4byte .Lfde_106bcc_body-.Lframe_g3
    .4byte stage3h_pc_00106bcc
    .4byte 0x3c
    .uleb128 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 16
    STAGE3H_ADVANCE 4
    STAGE3H_SAVE 64, -4
    .align 2
.Lfde_106bcc_end:

    .4byte .Lfde_106ca0_end-.Lfde_106ca0_body
.Lfde_106ca0_body:
    .4byte .Lfde_106ca0_body-.Lframe_g3
    .4byte stage3h_pc_00106ca0
    .4byte 0x6b8
    .uleb128 0
    STAGE3H_ADVANCE 4
    STAGE3H_CFA 368
    STAGE3H_ADVANCE 52
    STAGE3H_SAVE 52, -4
    STAGE3H_SAVE 16, -44
    STAGE3H_SAVE 17, -40
    STAGE3H_SAVE 18, -36
    STAGE3H_SAVE 19, -32
    STAGE3H_SAVE 20, -28
    STAGE3H_SAVE 21, -24
    STAGE3H_SAVE 22, -20
    STAGE3H_SAVE 23, -16
    STAGE3H_SAVE 30, -12
    STAGE3H_SAVE 64, -8
    .align 2
.Lfde_106ca0_end:

    .4byte .Lfde_107358_end-.Lfde_107358_body
.Lfde_107358_body:
    .4byte .Lfde_107358_body-.Lframe_g3
    .4byte stage3h_pc_00107358
    .4byte 0x220
    .uleb128 0
    STAGE3H_ADVANCE 8
    STAGE3H_CFA 80
    STAGE3H_ADVANCE 20
    STAGE3H_SAVE 18, -12
    STAGE3H_SAVE 17, -16
    STAGE3H_SAVE 19, -8
    STAGE3H_SAVE 64, -4
    STAGE3H_ADVANCE 8
    STAGE3H_SAVE 16, -20
    .align 2
.Lfde_107358_end:
    .4byte 0

    .section .note.GNU-stack,"",@progbits
