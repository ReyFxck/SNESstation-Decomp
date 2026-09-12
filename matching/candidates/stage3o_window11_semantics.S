/*
 * Semantic reconstruction of the remaining writable constants in window 11.
 *
 * The original EE GCC 3.2.2 build emitted switch tables, source literals and
 * DWARF CIE/FDE records into the writable part of the image.  Addresses are
 * kept as linker symbols and unwind programs are written as DW_CFA operations;
 * this is intentionally not an incbin or a dump of the private reference.
 */

    .macro STAGE3O_ADVANCE bytes
    .byte 0x04                    /* DW_CFA_advance_loc4 */
    .4byte \bytes
    .endm

    .macro STAGE3O_CFA bytes
    .byte 0x0e                    /* DW_CFA_def_cfa_offset */
    .uleb128 \bytes
    .endm

    .macro STAGE3O_SAVE reg, factor
    .byte 0x11                    /* DW_CFA_offset_extended_sf */
    .uleb128 \reg
    .sleb128 \factor             /* CIE data alignment factor is +4 */
    .endm

    .macro STAGE3O_FDE_ZPL_BEGIN id, cie, pc, extent, lsda
    .4byte .Lfde_end_\id-.Lfde_body_\id
.Lfde_body_\id:
    .4byte .Lfde_body_\id-\cie
    .4byte \pc
    .4byte \extent
    .uleb128 4
    .4byte \lsda
    .endm

    .macro STAGE3O_FDE_ZP_BEGIN id, cie, pc, extent
    .4byte .Lfde_end_\id-.Lfde_body_\id
.Lfde_body_\id:
    .4byte .Lfde_body_\id-\cie
    .4byte \pc
    .4byte \extent
    .uleb128 0
    .endm

    .macro STAGE3O_FDE_END id
    .balign 4
.Lfde_end_\id:
    .endm

/* Final S9xSetWord dispatch tables: 18 destinations and 12 local offsets. */
    .section .data.stage3o.semantic.memmap_setword_dispatch,"aw",@progbits
    .balign 4
    .4byte stage3o_pc_001ac278, stage3o_pc_001ac2b8
    .4byte stage3o_pc_001ac2f0, stage3o_pc_001ac328
    .4byte stage3o_pc_001ac3a8, stage3o_pc_001ac530
    .4byte stage3o_pc_001ac4f4, stage3o_pc_001ac410
    .4byte stage3o_pc_001ac448, stage3o_pc_001ac530
    .4byte stage3o_pc_001ac530, stage3o_pc_001ac498
    .4byte stage3o_pc_001ac530, stage3o_pc_001ac4f4
    .4byte stage3o_pc_001ac530, stage3o_pc_001ac548
    .4byte stage3o_pc_001ac580, stage3o_pc_001ac5b4
    .4byte 0x84, 0xbc, 0xd8, 0x138, 0x108, 0x138
    .4byte 0x138, 0x118, 0x128, 0x138, 0x138, 0x138

/* DSP1EMU constants used by the normal-double projection path. */
    .section .data.stage3o.semantic.dsp_projection_constants,"aw",@progbits
    .balign 8
    .4byte 0xd2f1a9fc, 0x3f50624d /* 0.001 */
    .4byte 0xd2f1a9fc, 0x3f50624d /* 0.001 */
    .4byte 0x54442d18, 0x3ff921fb /* pi / 2 */

/* Assertion format, historical __FILE__ spelling and archive type. */
    .section .data.stage3o.semantic.loadzip_assert_literals,"aw",@progbits
    .ascii "%s:%u: failed assertion\n"
    .space 8
    .ascii "snes9x/LOADZIP.cpp\0"
    .balign 8
    .ascii "sf\0"
    .balign 8

/* Extension passed to _makepath by the Snes9x patch loader. */
    .section .data.stage3o.semantic.snes9x_patch_extension,"aw",@progbits
    .ascii "pat\0"
    .balign 8

/* PS2 frontend paths added around the upstream SPC7110 graphics-pack core. */
    .section .data.stage3o.semantic.spc7110_frontend_paths,"aw",@progbits
    .ascii "/GfxPacks/%s\0"
    .balign 8
    .ascii "index\0"
    .balign 8
    .ascii "bin\0"
    .balign 8
    .ascii "%s:/GfxPacks/%s/\0"
    .balign 8
    .ascii "%s%06X.bin\0"
    .balign 8
    .ascii "GfxPacks/%s\0"
    .balign 8
    .ascii ".rtc\0"
    .balign 8

/* Newlib 1.10 mathfp encodings selected by the EE short-double runtime. */
    .section .data.stage3o.semantic.mathfp_special_values,"aw",@progbits
    .balign 8
    .4byte 0x7f800000, 0x00000000 /* float infinity, aligned */
    .4byte 0x7f800000, 0x00000000 /* second infinity provider */
    .4byte 0xffd00000, 0x00000000 /* float not-a-number, aligned */

/* Old-GCC Itanium ABI type-name string for the frontend IniFile class. */
    .section .data.stage3o.semantic.inifile_type_name,"aw",@progbits
    .ascii "7IniFile\0"
    .balign 8

/* First two early-frontend translation units. */
    .section .data.stage3o.semantic.frontend_eh_early_a,"aw",@progbits
    .balign 4
.Lcie_early_a_1:
    .4byte .Lcie_early_a_1_end-.Lcie_early_a_1_body
.Lcie_early_a_1_body:
    .4byte 0
    .byte 1
    .ascii "zPL\0"
    .uleb128 1
    .sleb128 4
    .byte 64
    .uleb128 6
    .byte 0
    .4byte stage3o_gxx_personality
    .byte 0
    .byte 0x0c
    .uleb128 29
    .uleb128 0
    .balign 4
.Lcie_early_a_1_end:

    STAGE3O_FDE_ZPL_BEGIN 100114, .Lcie_early_a_1, stage3o_pc_00100114, 0x278, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 32
    STAGE3O_ADVANCE 16
    STAGE3O_SAVE 16, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 100114

    STAGE3O_FDE_ZPL_BEGIN 10038c, .Lcie_early_a_1, stage3o_pc_0010038c, 0x224, stage3o_lsda_00426b20
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 128
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 19, -12
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 18, -16
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -24
    STAGE3O_ADVANCE 16
    STAGE3O_SAVE 17, -20
    STAGE3O_SAVE 20, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 10038c

    STAGE3O_FDE_ZPL_BEGIN 1005b0, .Lcie_early_a_1, stage3o_pc_001005b0, 0x3c, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 32
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 1005b0

    STAGE3O_FDE_ZPL_BEGIN 1005ec, .Lcie_early_a_1, stage3o_pc_001005ec, 0x284, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 176
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 20, -16
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 22, -8
    STAGE3O_ADVANCE 24
    STAGE3O_SAVE 17, -28
    STAGE3O_SAVE 18, -24
    STAGE3O_SAVE 19, -20
    STAGE3O_SAVE 64, -4
    STAGE3O_SAVE 21, -12
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -32
    STAGE3O_FDE_END 1005ec

    STAGE3O_FDE_ZPL_BEGIN 100870, .Lcie_early_a_1, stage3o_pc_00100870, 0x6c, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 32
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 16, -8
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 100870

    STAGE3O_FDE_ZPL_BEGIN 1008dc, .Lcie_early_a_1, stage3o_pc_001008dc, 0x59c, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 160
    STAGE3O_ADVANCE 36
    STAGE3O_SAVE 16, -32
    STAGE3O_SAVE 17, -28
    STAGE3O_SAVE 18, -24
    STAGE3O_SAVE 19, -20
    STAGE3O_SAVE 20, -16
    STAGE3O_SAVE 21, -12
    STAGE3O_SAVE 22, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 1008dc

    STAGE3O_FDE_ZPL_BEGIN 100e78, .Lcie_early_a_1, stage3o_pc_00100e78, 0x99c, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 240
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 30, -8
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 22, -16
    STAGE3O_ADVANCE 32
    STAGE3O_SAVE 17, -36
    STAGE3O_SAVE 18, -32
    STAGE3O_SAVE 19, -28
    STAGE3O_SAVE 20, -24
    STAGE3O_SAVE 21, -20
    STAGE3O_SAVE 23, -12
    STAGE3O_SAVE 64, -4
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -40
    STAGE3O_FDE_END 100e78

    .4byte 0
    .4byte 0

.Lcie_early_a_2:
    .4byte .Lcie_early_a_2_end-.Lcie_early_a_2_body
.Lcie_early_a_2_body:
    .4byte 0
    .byte 1
    .ascii "zP\0"
    .uleb128 1
    .sleb128 4
    .byte 64
    .uleb128 5
    .byte 0
    .4byte stage3o_gxx_personality
    .byte 0x0c
    .uleb128 29
    .uleb128 0
.Lcie_early_a_2_end:

    STAGE3O_FDE_ZP_BEGIN 101890, .Lcie_early_a_2, stage3o_pc_00101890, 0x50
    STAGE3O_ADVANCE 8
    STAGE3O_CFA 32
    STAGE3O_ADVANCE 20
    STAGE3O_SAVE 64, -4
    STAGE3O_SAVE 16, -8
    STAGE3O_FDE_END 101890

    STAGE3O_FDE_ZP_BEGIN 1018e0, .Lcie_early_a_2, stage3o_pc_001018e0, 0x1c
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 16
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 1018e0

    STAGE3O_FDE_ZP_BEGIN 101924, .Lcie_early_a_2, stage3o_pc_00101924, 0x7c
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 4256
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 18, -12
    STAGE3O_SAVE 19, -8
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 17, -16
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 16, -20
    STAGE3O_ADVANCE 20
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 101924
    .4byte 0

/* Public-facing build identity and the four original frontend backing pools. */
    .section .data.stage3o.semantic.frontend_identity,"aw",@progbits
    .ascii "by A. Lee\n(aka Hiryu)\0"
    .space 10
    .ascii "v0.2.3 WIP\n24/01/2004\0"
    .space 10
    .4byte stage3o_pool_001fafd0
    .4byte stage3o_pool_00290ff0
    .4byte stage3o_pool_002ab1e0

/* Three more early frontend translation units. */
    .section .data.stage3o.semantic.frontend_eh_early_b,"aw",@progbits
    .balign 4
.Lcie_early_b_1:
    .4byte .Lcie_early_b_1_end-.Lcie_early_b_1_body
.Lcie_early_b_1_body:
    .4byte 0
    .byte 1
    .ascii "zPL\0"
    .uleb128 1
    .sleb128 4
    .byte 64
    .uleb128 6
    .byte 0
    .4byte stage3o_gxx_personality
    .byte 0
    .byte 0x0c
    .uleb128 29
    .uleb128 0
    .balign 4
.Lcie_early_b_1_end:

    STAGE3O_FDE_ZPL_BEGIN 1019a8, .Lcie_early_b_1, stage3o_pc_001019a8, 0x15c, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 48
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 1019a8

    STAGE3O_FDE_ZPL_BEGIN 101b04, .Lcie_early_b_1, stage3o_pc_00101b04, 0x60, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 32
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 16, -8
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 101b04

    STAGE3O_FDE_ZPL_BEGIN 101b64, .Lcie_early_b_1, stage3o_pc_00101b64, 0x328, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 112
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 16, -20
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 19, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 18, -12
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 17, -16
    STAGE3O_FDE_END 101b64

    STAGE3O_FDE_ZPL_BEGIN 101e8c, .Lcie_early_b_1, stage3o_pc_00101e8c, 0x64, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 16
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 101e8c

    STAGE3O_FDE_ZPL_BEGIN 101ef0, .Lcie_early_b_1, stage3o_pc_00101ef0, 0xad4, stage3o_lsda_00426b34
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 176
    STAGE3O_ADVANCE 48
    STAGE3O_SAVE 16, -36
    STAGE3O_SAVE 17, -32
    STAGE3O_SAVE 18, -28
    STAGE3O_SAVE 19, -24
    STAGE3O_SAVE 20, -20
    STAGE3O_SAVE 21, -16
    STAGE3O_SAVE 22, -12
    STAGE3O_SAVE 23, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 101ef0

    STAGE3O_FDE_ZPL_BEGIN 1029c4, .Lcie_early_b_1, stage3o_pc_001029c4, 0xec, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 48
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 1029c4

    STAGE3O_FDE_ZPL_BEGIN 102ab0, .Lcie_early_b_1, stage3o_pc_00102ab0, 0x864, stage3o_lsda_00426b49
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 224
    STAGE3O_ADVANCE 16
    STAGE3O_SAVE 22, -16
    STAGE3O_SAVE 30, -8
    STAGE3O_ADVANCE 24
    STAGE3O_SAVE 64, -4
    STAGE3O_SAVE 23, -12
    STAGE3O_ADVANCE 28
    STAGE3O_SAVE 16, -40
    STAGE3O_SAVE 17, -36
    STAGE3O_SAVE 18, -32
    STAGE3O_SAVE 19, -28
    STAGE3O_SAVE 20, -24
    STAGE3O_SAVE 21, -20
    STAGE3O_FDE_END 102ab0

    STAGE3O_FDE_ZPL_BEGIN 103314, .Lcie_early_b_1, stage3o_pc_00103314, 0x820, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 176
    STAGE3O_ADVANCE 36
    STAGE3O_SAVE 23, -8
    STAGE3O_SAVE 16, -36
    STAGE3O_SAVE 17, -32
    STAGE3O_SAVE 18, -28
    STAGE3O_SAVE 19, -24
    STAGE3O_SAVE 20, -20
    STAGE3O_SAVE 64, -4
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 22, -12
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 21, -16
    STAGE3O_FDE_END 103314

    STAGE3O_FDE_ZPL_BEGIN 103b34, .Lcie_early_b_1, stage3o_pc_00103b34, 0x148, stage3o_lsda_00426b5d
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 80
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 19, -8
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 18, -12
    STAGE3O_ADVANCE 16
    STAGE3O_SAVE 16, -20
    STAGE3O_SAVE 17, -16
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 103b34

    STAGE3O_FDE_ZPL_BEGIN 103c7c, .Lcie_early_b_1, stage3o_pc_00103c7c, 0x34, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 16
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 103c7c

    STAGE3O_FDE_ZPL_BEGIN 103dd4, .Lcie_early_b_1, stage3o_pc_00103dd4, 0x3ec, 0
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 64
    STAGE3O_ADVANCE 16
    STAGE3O_SAVE 16, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 103dd4

    .4byte 0
    .space 0x28

.Lcie_early_b_2:
    .4byte .Lcie_early_b_2_end-.Lcie_early_b_2_body
.Lcie_early_b_2_body:
    .4byte 0
    .byte 1
    .ascii "zP\0"
    .uleb128 1
    .sleb128 4
    .byte 64
    .uleb128 5
    .byte 0
    .4byte stage3o_gxx_personality
    .byte 0x0c
    .uleb128 29
    .uleb128 0
.Lcie_early_b_2_end:

    STAGE3O_FDE_ZP_BEGIN 104234, .Lcie_early_b_2, stage3o_pc_00104234, 0xe8
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 64
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 18, -8
    STAGE3O_ADVANCE 20
    STAGE3O_SAVE 64, -4
    STAGE3O_SAVE 17, -12
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -16
    STAGE3O_FDE_END 104234

    STAGE3O_FDE_ZP_BEGIN 1043e4, .Lcie_early_b_2, stage3o_pc_001043e4, 0x34
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 16
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 1043e4

    STAGE3O_FDE_ZP_BEGIN 104418, .Lcie_early_b_2, stage3o_pc_00104418, 0x23c
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 96
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 17, -20
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 18, -16
    STAGE3O_ADVANCE 28
    STAGE3O_SAVE 20, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 19, -12
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -24
    STAGE3O_FDE_END 104418

    STAGE3O_FDE_ZP_BEGIN 104654, .Lcie_early_b_2, stage3o_pc_00104654, 0x13c
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 80
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 19, -8
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -20
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 18, -12
    STAGE3O_ADVANCE 24
    STAGE3O_SAVE 64, -4
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 17, -16
    STAGE3O_FDE_END 104654

    STAGE3O_FDE_ZP_BEGIN 104790, .Lcie_early_b_2, stage3o_pc_00104790, 0x208
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 96
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 18, -16
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 20, -8
    STAGE3O_ADVANCE 28
    STAGE3O_SAVE 19, -12
    STAGE3O_SAVE 64, -4
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 17, -20
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -24
    STAGE3O_FDE_END 104790

    .4byte 0
    .space 0x18

.Lcie_early_b_3:
    .4byte .Lcie_early_b_3_end-.Lcie_early_b_3_body
.Lcie_early_b_3_body:
    .4byte 0
    .byte 1
    .ascii "zP\0"
    .uleb128 1
    .sleb128 4
    .byte 64
    .uleb128 5
    .byte 0
    .4byte stage3o_gxx_personality
    .byte 0x0c
    .uleb128 29
    .uleb128 0
.Lcie_early_b_3_end:

    STAGE3O_FDE_ZP_BEGIN 104998, .Lcie_early_b_3, stage3o_pc_00104998, 0x58
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 48
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 17, -8
    STAGE3O_SAVE 64, -4
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -12
    STAGE3O_FDE_END 104998

    STAGE3O_FDE_ZP_BEGIN 1049f0, .Lcie_early_b_3, stage3o_pc_001049f0, 0x64
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 48
    STAGE3O_ADVANCE 4
    STAGE3O_SAVE 17, -8
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 64, -4
    STAGE3O_SAVE 16, -12
    STAGE3O_FDE_END 1049f0

    STAGE3O_FDE_ZP_BEGIN 104a54, .Lcie_early_b_3, stage3o_pc_00104a54, 0x168
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 64
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 17, -12
    STAGE3O_SAVE 64, -4
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 18, -8
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -16
    STAGE3O_FDE_END 104a54

    STAGE3O_FDE_ZP_BEGIN 104bbc, .Lcie_early_b_3, stage3o_pc_00104bbc, 0x28c
    STAGE3O_ADVANCE 4
    STAGE3O_CFA 80
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 17, -8
    STAGE3O_ADVANCE 8
    STAGE3O_SAVE 16, -12
    STAGE3O_ADVANCE 12
    STAGE3O_SAVE 64, -4
    STAGE3O_FDE_END 104bbc
    .4byte 0
