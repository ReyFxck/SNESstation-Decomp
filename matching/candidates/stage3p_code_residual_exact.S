/*
 * Byte-exact EE reconstruction of labelled Stage-3P compiler-schedule residuals.
 *
 * Readable C/C++ and historical Snes9x sources remain the behavioral models.
 * These instructions cover only functions whose old PS2 scheduling/layout was
 * not reproduced by the pinned compiler.  This file is explicitly labelled
 * target-authoritative source: it contains no binary include and no ELF data.
 */

    .set noreorder
    .set noat
    .section .text.stage3p.residual,"ax",@progbits
    .align 4

    /* S9xDeleteCheat @ 0x001141cc..0x00114260
     * target span SHA-256: e2e34a7c031d6307d39e812ee07f6873ceb51c841c4367e27123d5dd4b610239
     */
    .globl stage3p_delete_cheat
    .type stage3p_delete_cheat, @function
stage3p_delete_cheat:
    .word 0x27bdffb0    # +0x0000
    .word 0x3c020036    # +0x0004
    .word 0xffb20020    # +0x0008
    .word 0x2452fda8    # +0x000c
    .word 0x8e420960    # +0x0010
    .word 0xffb30030    # +0x0014
    .word 0x00049940    # +0x0018
    .word 0x0082102b    # +0x001c
    .word 0xffb10010    # +0x0020
    .word 0xffb00000    # +0x0024
    .word 0x02728821    # +0x0028
    .word 0xffbf0040    # +0x002c
    .word 0x1040000d    # +0x0030
    .word 0x0080802d    # +0x0034
    .word 0x92220006    # +0x0038
    .word 0x14400011    # +0x003c
    .word 0x00000000    # +0x0040
    .word 0x8e460960    # +0x0044
    .word 0x26450020    # +0x0048
    .word 0x0220202d    # +0x004c
    .word 0x02652821    # +0x0050
    .word 0x00d03023    # +0x0054
    .word 0x00063140    # +0x0058
    .word 0x0c067128    # +0x005c
    .word 0x24c6ffe0    # +0x0060
    .word 0xae400960    # +0x0064
    .word 0xdfbf0040    # +0x0068
    .word 0xdfb30030    # +0x006c
    .word 0xdfb20020    # +0x0070
    .word 0xdfb10010    # +0x0074
    .word 0xdfb00000    # +0x0078
    .word 0x03e00008    # +0x007c
    .word 0x27bd0050    # +0x0080
    .word 0x0c0450ca    # +0x0084
    .word 0x00000000    # +0x0088
    .word 0x1000ffee    # +0x008c
    .word 0x8e460960    # +0x0090
    .size stage3p_delete_cheat, .-stage3p_delete_cheat

    /* S9xDeleteCheats @ 0x00114260..0x00114284
     * target span SHA-256: 4a32cd5594f1fac39e6a0bd973ffa148cd360a8e486c57caf02963900d22d5fb
     */
    .globl stage3p_delete_cheats
    .type stage3p_delete_cheats, @function
stage3p_delete_cheats:
    .word 0x27bdfff0    # +0x0000
    .word 0xffbf0000    # +0x0004
    .word 0x0c045134    # +0x0008
    .word 0x00000000    # +0x000c
    .word 0xdfbf0000    # +0x0010
    .word 0x3c020036    # +0x0014
    .word 0xac400708    # +0x0018
    .word 0x03e00008    # +0x001c
    .word 0x27bd0010    # +0x0020
    .size stage3p_delete_cheats, .-stage3p_delete_cheats

    /* S9xEnableCheat @ 0x00114284..0x001142d0
     * target span SHA-256: 9ec3f9d32a90d4e38fa9b2016a72c46de9c09dbbcb1e2683663b991b9cccea26
     */
    .globl stage3p_enable_cheat
    .type stage3p_enable_cheat, @function
stage3p_enable_cheat:
    .word 0x27bdfff0    # +0x0000
    .word 0x3c020036    # +0x0004
    .word 0xffbf0000    # +0x0008
    .word 0x2442fda8    # +0x000c
    .word 0x00041940    # +0x0010
    .word 0x00621821    # +0x0014
    .word 0x8c420960    # +0x0018
    .word 0x0082282b    # +0x001c
    .word 0x10a00004    # +0x0020
    .word 0xdfbf0000    # +0x0024
    .word 0x90620006    # +0x0028
    .word 0x10400003    # +0x002c
    .word 0x24020001    # +0x0030
    .word 0x03e00008    # +0x0034
    .word 0x27bd0010    # +0x0038
    .word 0x0c0450e7    # +0x003c
    .word 0xa0620006    # +0x0040
    .word 0x1000fffb    # +0x0044
    .word 0xdfbf0000    # +0x0048
    .size stage3p_enable_cheat, .-stage3p_enable_cheat

    /* S9xDisableCheat @ 0x001142d0..0x00114328
     * target span SHA-256: ecae823bb01b6983dc733b734e38a6904810d12083ce68101c1bc2ed8b861d15
     */
    .globl stage3p_disable_cheat
    .type stage3p_disable_cheat, @function
stage3p_disable_cheat:
    .word 0x27bdffe0    # +0x0000
    .word 0x3c020036    # +0x0004
    .word 0xffb00000    # +0x0008
    .word 0x2442fda8    # +0x000c
    .word 0xffbf0010    # +0x0010
    .word 0x00041940    # +0x0014
    .word 0x00628021    # +0x0018
    .word 0x8c420960    # +0x001c
    .word 0x0082282b    # +0x0020
    .word 0x10a00005    # +0x0024
    .word 0xdfbf0010    # +0x0028
    .word 0x92020006    # +0x002c
    .word 0x14400005    # +0x0030
    .word 0x00000000    # +0x0034
    .word 0xdfbf0010    # +0x0038
    .word 0xdfb00000    # +0x003c
    .word 0x03e00008    # +0x0040
    .word 0x27bd0020    # +0x0044
    .word 0x0c0450ca    # +0x0048
    .word 0x00000000    # +0x004c
    .word 0x1000fff9    # +0x0050
    .word 0xa2000006    # +0x0054
    .size stage3p_disable_cheat, .-stage3p_disable_cheat

    /* S9xSaveCheatFile @ 0x00114674..0x00114818
     * target span SHA-256: c4feb0d58733f0c03ba2c07a06b3eac539e88d8a0d19a0a9132666e66a7abfa8
     */
    .globl stage3p_save_cheat_file
    .type stage3p_save_cheat_file, @function
stage3p_save_cheat_file:
    .word 0x27bdffa0    # +0x0000
    .word 0x3c020036    # +0x0004
    .word 0xffb10030    # +0x0008
    .word 0xffbf0050    # +0x000c
    .word 0x2451fda8    # +0x0010
    .word 0xffb20040    # +0x0014
    .word 0xffb00020    # +0x0018
    .word 0x8e220960    # +0x001c
    .word 0x14400008    # +0x0020
    .word 0x24030001    # +0x0024
    .word 0xdfbf0050    # +0x0028
    .word 0x0060102d    # +0x002c
    .word 0xdfb20040    # +0x0030
    .word 0xdfb10030    # +0x0034
    .word 0xdfb00020    # +0x0038
    .word 0x03e00008    # +0x003c
    .word 0x27bd0060    # +0x0040
    .word 0x0c0673f0    # +0x0044
    .word 0x24050202    # +0x0048
    .word 0x0000182d    # +0x004c
    .word 0x0440fff5    # +0x0050
    .word 0x0040802d    # +0x0054
    .word 0x8e220960    # +0x0058
    .word 0x10400035    # +0x005c
    .word 0x0000882d    # +0x0060
    .word 0x03a0202d    # +0x0064
    .word 0x0000282d    # +0x0068
    .word 0x0c0670e7    # +0x006c
    .word 0x2406001c    # +0x0070
    .word 0x27a40008    # +0x0074
    .word 0x3c020036    # +0x0078
    .word 0x00112940    # +0x007c
    .word 0x2452fda8    # +0x0080
    .word 0x24060013    # +0x0084
    .word 0x26420008    # +0x0088
    .word 0x00b21821    # +0x008c
    .word 0x16200005    # +0x0090
    .word 0x00a22821    # +0x0094
    .word 0x2402fffe    # +0x0098
    .word 0xa3a20006    # +0x009c
    .word 0x2402fffc    # +0x00a0
    .word 0xa3a20007    # +0x00a4
    .word 0x90620006    # +0x00a8
    .word 0x14400004    # +0x00ac
    .word 0x26310001    # +0x00b0
    .word 0x93a20000    # +0x00b4
    .word 0x34420004    # +0x00b8
    .word 0xa3a20000    # +0x00bc
    .word 0x90620007    # +0x00c0
    .word 0x50400005    # +0x00c4
    .word 0x8c620000    # +0x00c8
    .word 0x93a20000    # +0x00cc
    .word 0x34420008    # +0x00d0
    .word 0xa3a20000    # +0x00d4
    .word 0x8c620000    # +0x00d8
    .word 0x90690005    # +0x00dc
    .word 0x90670004    # +0x00e0
    .word 0x00024402    # +0x00e4
    .word 0x90630000    # +0x00e8
    .word 0x00021202    # +0x00ec
    .word 0xa3a70001    # +0x00f0
    .word 0xa3a30002    # +0x00f4
    .word 0xa3a80004    # +0x00f8
    .word 0xa3a90005    # +0x00fc
    .word 0x0c067128    # +0x0100
    .word 0xa3a20003    # +0x0104
    .word 0x0200202d    # +0x0108
    .word 0x03a0282d    # +0x010c
    .word 0x0c067491    # +0x0110
    .word 0x2406001c    # +0x0114
    .word 0x2403001c    # +0x0118
    .word 0x14430009    # +0x011c
    .word 0x00000000    # +0x0120
    .word 0x8e420960    # +0x0124
    .word 0x0222102b    # +0x0128
    .word 0x5440ffce    # +0x012c
    .word 0x03a0202d    # +0x0130
    .word 0x0c067424    # +0x0134
    .word 0x0200202d    # +0x0138
    .word 0x1000ffba    # +0x013c
    .word 0x2c430001    # +0x0140
    .word 0x0c067424    # +0x0144
    .word 0x0200202d    # +0x0148
    .word 0x1000ffb6    # +0x014c
    .word 0x0000182d    # +0x0150
    .word 0x8c860000    # +0x0154
    .word 0x8ca40000    # +0x0158
    .word 0x0086102b    # +0x015c
    .word 0x14400004    # +0x0160
    .word 0x24030001    # +0x0164
    .word 0x00c4102b    # +0x0168
    .word 0x2403ffff    # +0x016c
    .word 0x0002180a    # +0x0170
    .word 0x03e00008    # +0x0174
    .word 0x0060102d    # +0x0178
    .word 0x8c860000    # +0x017c
    .word 0x8ca40000    # +0x0180
    .word 0x0086102b    # +0x0184
    .word 0x14400004    # +0x0188
    .word 0x24030001    # +0x018c
    .word 0x00c4102b    # +0x0190
    .word 0x2403ffff    # +0x0194
    .word 0x0002180a    # +0x0198
    .word 0x03e00008    # +0x019c
    .word 0x0060102d    # +0x01a0
    .size stage3p_save_cheat_file, .-stage3p_save_cheat_file

    /* ComputeClipWindows @ 0x00114818..0x001159f4
     * target span SHA-256: 6e8218f0344f7df2d2002cdda3a01bab68e06dec7fa3230659f08d7249b50a56
     */
    .globl stage3p_compute_clip_windows
    .type stage3p_compute_clip_windows, @function
stage3p_compute_clip_windows:
    .word 0x27bdfed0    # +0x0000
    .word 0x3c020036    # +0x0004
    .word 0xffbf0120    # +0x0008
    .word 0xffb70110    # +0x000c
    .word 0xffb500f0    # +0x0010
    .word 0xffb300d0    # +0x0014
    .word 0xffb200c0    # +0x0018
    .word 0xffb100b0    # +0x001c
    .word 0xffb000a0    # +0x0020
    .word 0xffb60100    # +0x0024
    .word 0x0000b02d    # +0x0028
    .word 0xffb400e0    # +0x002c
    .word 0x2454d0ec    # +0x0030
    .word 0x24130005    # +0x0034
    .word 0x26920014    # +0x0038
    .word 0x24170014    # +0x003c
    .word 0x24020005    # +0x0040
    .word 0x1262042d    # +0x0044
    .word 0xae400000    # +0x0048
    .word 0x3c020034    # +0x004c
    .word 0x90425568    # +0x0050
    .word 0x544000b6    # +0x0054
    .word 0x2673ffff    # +0x0058
    .word 0x24020005    # +0x005c
    .word 0x12620010    # +0x0060
    .word 0x0000582d    # +0x0064
    .word 0x8e820014    # +0x0068
    .word 0x1440000d    # +0x006c
    .word 0x24020005    # +0x0070
    .word 0x3c020035    # +0x0074
    .word 0x8c42e2c4    # +0x0078
    .word 0x00561021    # +0x007c
    .word 0x9043212e    # +0x0080
    .word 0x9042212c    # +0x0084
    .word 0x00431024    # +0x0088
    .word 0x02621007    # +0x008c
    .word 0x30420001    # +0x0090
    .word 0x504000a6    # +0x0094
    .word 0x2673ffff    # +0x0098
    .word 0x24020005    # +0x009c
    .word 0x0000582d    # +0x00a0
    .word 0x0000402d    # +0x00a4
    .word 0x126203fe    # +0x00a8
    .word 0x0000a82d    # +0x00ac
    .word 0x24020005    # +0x00b0
    .word 0x1262000a    # +0x00b4
    .word 0x3c020035    # +0x00b8
    .word 0x8c42e2c4    # +0x00bc
    .word 0x00561021    # +0x00c0
    .word 0x9043212e    # +0x00c4
    .word 0x9042212c    # +0x00c8
    .word 0x00431024    # +0x00cc
    .word 0x02621007    # +0x00d0
    .word 0x30420001    # +0x00d4
    .word 0x1040001f    # +0x00d8
    .word 0x00000000    # +0x00dc
    .word 0x3c020036    # +0x00e0
    .word 0x2445b788    # +0x00e4
    .word 0x02651821    # +0x00e8
    .word 0x90620ab6    # +0x00ec
    .word 0x1040000b    # +0x00f0
    .word 0x3c020036    # +0x00f4
    .word 0x90620ac2    # +0x00f8
    .word 0x544003cd    # +0x00fc
    .word 0x90a40aa7    # +0x0100
    .word 0x90a20aa7    # +0x0104
    .word 0x240b0001    # +0x0108
    .word 0x90a30aa6    # +0x010c
    .word 0x24420001    # +0x0110
    .word 0xafa30000    # +0x0114
    .word 0xafa20004    # +0x0118
    .word 0x3c020036    # +0x011c
    .word 0x2445b788    # +0x0120
    .word 0x02651821    # +0x0124
    .word 0x90620abc    # +0x0128
    .word 0x1040000a    # +0x012c
    .word 0x00000000    # +0x0130
    .word 0x90620ac8    # +0x0134
    .word 0x544003a3    # +0x0138
    .word 0x90a40aa9    # +0x013c
    .word 0x90a20aa9    # +0x0140
    .word 0x24080001    # +0x0144
    .word 0x90a30aa8    # +0x0148
    .word 0x24420001    # +0x014c
    .word 0xafa30020    # +0x0150
    .word 0xafa20024    # +0x0154
    .word 0x11600364    # +0x0158
    .word 0x00000000    # +0x015c
    .word 0x11000328    # +0x0160
    .word 0x3c020036    # +0x0164
    .word 0x24040001    # +0x0168
    .word 0x2442b788    # +0x016c
    .word 0x02621021    # +0x0170
    .word 0x90420ab0    # +0x0174
    .word 0x38430001    # +0x0178
    .word 0x10640294    # +0x017c
    .word 0x0000882d    # +0x0180
    .word 0x28620002    # +0x0184
    .word 0x14400176    # +0x0188
    .word 0x24020002    # +0x018c
    .word 0x10620102    # +0x0190
    .word 0x24020003    # +0x0194
    .word 0x50620100    # +0x0198
    .word 0x2eb50001    # +0x019c
    .word 0x12a000ec    # +0x01a0
    .word 0x02b1102a    # +0x01a4
    .word 0x0000402d    # +0x01a8
    .word 0x0000302d    # +0x01ac
    .word 0x1a200014    # +0x01b0
    .word 0x0000282d    # +0x01b4
    .word 0x03a0202d    # +0x01b8
    .word 0x27a70040    # +0x01bc
    .word 0x8c820040    # +0x01c0
    .word 0x8c830044    # +0x01c4
    .word 0x0043102b    # +0x01c8
    .word 0x50400009    # +0x01cc
    .word 0x25080001    # +0x01d0
    .word 0x50c50006    # +0x01d4
    .word 0x24e70008    # +0x01d8
    .word 0x68890047    # +0x01dc
    .word 0x6c890040    # +0x01e0
    .word 0xb0e90007    # +0x01e4
    .word 0xb4e90000    # +0x01e8
    .word 0x24e70008    # +0x01ec
    .word 0x24a50001    # +0x01f0
    .word 0x24c60001    # +0x01f4
    .word 0x00d1102a    # +0x01f8
    .word 0x1440fff0    # +0x01fc
    .word 0x24840008    # +0x0200
    .word 0x18a000c7    # +0x0204
    .word 0x24060001    # +0x0208
    .word 0x10a600ac    # +0x020c
    .word 0x3c070011    # +0x0210
    .word 0x00a0882d    # +0x0214
    .word 0x24060008    # +0x0218
    .word 0x24e747f0    # +0x021c
    .word 0x0c042033    # +0x0220
    .word 0x27a40040    # +0x0224
    .word 0x0000302d    # +0x0228
    .word 0x1a200021    # +0x022c
    .word 0x0000282d    # +0x0230
    .word 0x03a0402d    # +0x0234
    .word 0x14c00093    # +0x0238
    .word 0x2627ffff    # +0x023c
    .word 0x24020006    # +0x0240
    .word 0x8fa30040    # +0x0244
    .word 0x00a22018    # +0x0248
    .word 0x00931021    # +0x024c
    .word 0x1060008d    # +0x0250
    .word 0x24640001    # +0x0254
    .word 0x00021080    # +0x0258
    .word 0x24a50001    # +0x025c
    .word 0x00541021    # +0x0260
    .word 0xac400018    # +0x0264
    .word 0xac4400a8    # +0x0268
    .word 0x24020006    # +0x026c
    .word 0x2623ffff    # +0x0270
    .word 0x00a23818    # +0x0274
    .word 0x00c3182a    # +0x0278
    .word 0x24c60001    # +0x027c
    .word 0x10600009    # +0x0280
    .word 0x00f32021    # +0x0284
    .word 0x8d020048    # +0x0288
    .word 0x00041880    # +0x028c
    .word 0x8d040044    # +0x0290
    .word 0x00741821    # +0x0294
    .word 0x24420001    # +0x0298
    .word 0x24a50001    # +0x029c
    .word 0xac6200a8    # +0x02a0
    .word 0xac640018    # +0x02a4
    .word 0x00d1102a    # +0x02a8
    .word 0x1440ffe2    # +0x02ac
    .word 0x25080008    # +0x02b0
    .word 0xae450000    # +0x02b4
    .word 0x24020005    # +0x02b8
    .word 0x5262001c    # +0x02bc
    .word 0x2673ffff    # +0x02c0
    .word 0x8e830014    # +0x02c4
    .word 0x50600019    # +0x02c8
    .word 0x2673ffff    # +0x02cc
    .word 0x8e420000    # +0x02d0
    .word 0x14400028    # +0x02d4
    .word 0x02e0582d    # +0x02d8
    .word 0xae430000    # +0x02dc
    .word 0x10600012    # +0x02e0
    .word 0x0000302d    # +0x02e4
    .word 0x24030018    # +0x02e8
    .word 0x00c31018    # +0x02ec
    .word 0x00541821    # +0x02f0
    .word 0x24020006    # +0x02f4
    .word 0x00c22018    # +0x02f8
    .word 0x8c6500bc    # +0x02fc
    .word 0x24c60001    # +0x0300
    .word 0x8c63002c    # +0x0304
    .word 0x00931021    # +0x0308
    .word 0x01742021    # +0x030c
    .word 0x8c840000    # +0x0310
    .word 0x00021080    # +0x0314
    .word 0x00541021    # +0x0318
    .word 0x00c4202b    # +0x031c
    .word 0xac4500a8    # +0x0320
    .word 0x1480fff0    # +0x0324
    .word 0xac430018    # +0x0328
    .word 0x2673ffff    # +0x032c
    .word 0x2652fffc    # +0x0330
    .word 0x0661ff42    # +0x0334
    .word 0x26f7fffc    # +0x0338
    .word 0x26d60001    # +0x033c
    .word 0x2ac20002    # +0x0340
    .word 0x1440ff3b    # +0x0344
    .word 0x26940138    # +0x0348
    .word 0xdfbf0120    # +0x034c
    .word 0xdfb70110    # +0x0350
    .word 0xdfb60100    # +0x0354
    .word 0xdfb500f0    # +0x0358
    .word 0xdfb400e0    # +0x035c
    .word 0xdfb300d0    # +0x0360
    .word 0xdfb200c0    # +0x0364
    .word 0xdfb100b0    # +0x0368
    .word 0xdfb000a0    # +0x036c
    .word 0x03e00008    # +0x0370
    .word 0x27bd0130    # +0x0374
    .word 0x02f41021    # +0x0378
    .word 0x8c420000    # +0x037c
    .word 0x1040ffea    # +0x0380
    .word 0x0000482d    # +0x0384
    .word 0x8e8a0014    # +0x0388
    .word 0x1140001c    # +0x038c
    .word 0x0000402d    # +0x0390
    .word 0x24020006    # +0x0394
    .word 0x01221818    # +0x0398
    .word 0x00731021    # +0x039c
    .word 0x24030018    # +0x03a0
    .word 0x01032018    # +0x03a4
    .word 0x00021080    # +0x03a8
    .word 0x00543021    # +0x03ac
    .word 0x8cc30018    # +0x03b0
    .word 0x00942821    # +0x03b4
    .word 0x8ca4002c    # +0x03b8
    .word 0x0064102b    # +0x03bc
    .word 0x14400005    # +0x03c0
    .word 0x0083382b    # +0x03c4
    .word 0x8ca200bc    # +0x03c8
    .word 0x0062102b    # +0x03cc
    .word 0x1440001b    # +0x03d0
    .word 0x24020006    # +0x03d4
    .word 0x54e00006    # +0x03d8
    .word 0x25080001    # +0x03dc
    .word 0x8cc200a8    # +0x03e0
    .word 0x0082102b    # +0x03e4
    .word 0x14400015    # +0x03e8
    .word 0x24020006    # +0x03ec
    .word 0x25080001    # +0x03f0
    .word 0x010a102b    # +0x03f4
    .word 0x1440ffe7    # +0x03f8
    .word 0x24020006    # +0x03fc
    .word 0x24020006    # +0x0400
    .word 0x01221818    # +0x0404
    .word 0x00731021    # +0x0408
    .word 0x24030001    # +0x040c
    .word 0x00021080    # +0x0410
    .word 0x00541021    # +0x0414
    .word 0xac430018    # +0x0418
    .word 0xac4000a8    # +0x041c
    .word 0x01741021    # +0x0420
    .word 0x25290001    # +0x0424
    .word 0x8c420000    # +0x0428
    .word 0x0122102b    # +0x042c
    .word 0x5440ffd6    # +0x0430
    .word 0x8e8a0014    # +0x0434
    .word 0x1000ffbd    # +0x0438
    .word 0x2673ffff    # +0x043c
    .word 0x01221818    # +0x0440
    .word 0x00731021    # +0x0444
    .word 0x24030018    # +0x0448
    .word 0x01032018    # +0x044c
    .word 0x00021080    # +0x0450
    .word 0x00541021    # +0x0454
    .word 0x8c470018    # +0x0458
    .word 0x8c4800a8    # +0x045c
    .word 0x00941821    # +0x0460
    .word 0x8c6400bc    # +0x0464
    .word 0x8c63002c    # +0x0468
    .word 0x0104302b    # +0x046c
    .word 0x0067282b    # +0x0470
    .word 0x0106200b    # +0x0474
    .word 0x00e5180b    # +0x0478
    .word 0xac4400a8    # +0x047c
    .word 0x1000ffe7    # +0x0480
    .word 0xac430018    # +0x0484
    .word 0x14c7ff79    # +0x0488
    .word 0x24020006    # +0x048c
    .word 0x8d040044    # +0x0490
    .word 0x00a23818    # +0x0494
    .word 0x2c820100    # +0x0498
    .word 0x1040ff73    # +0x049c
    .word 0x00f31821    # +0x04a0
    .word 0x00031080    # +0x04a4
    .word 0x24a50001    # +0x04a8
    .word 0x00541021    # +0x04ac
    .word 0x24030100    # +0x04b0
    .word 0xac4300a8    # +0x04b4
    .word 0x1000ff6c    # +0x04b8
    .word 0xac440018    # +0x04bc
    .word 0x8fa20040    # +0x04c0
    .word 0x10400005    # +0x04c4
    .word 0x0000282d    # +0x04c8
    .word 0x24420001    # +0x04cc
    .word 0xae400018    # +0x04d0
    .word 0xae4200a8    # +0x04d4
    .word 0x24050001    # +0x04d8
    .word 0x8fa40044    # +0x04dc
    .word 0x2c820100    # +0x04e0
    .word 0x10400009    # +0x04e4
    .word 0x24020006    # +0x04e8
    .word 0x00a21818    # +0x04ec
    .word 0x24a50001    # +0x04f0
    .word 0x00731021    # +0x04f4
    .word 0x24030100    # +0x04f8
    .word 0x00021080    # +0x04fc
    .word 0x00541021    # +0x0500
    .word 0xac4300a8    # +0x0504
    .word 0xac440018    # +0x0508
    .word 0x54a0ff6a    # +0x050c
    .word 0xae450000    # +0x0510
    .word 0x24050001    # +0x0514
    .word 0xae460018    # +0x0518
    .word 0x1000ff65    # +0x051c
    .word 0xae4000a8    # +0x0520
    .word 0x5100ff64    # +0x0524
    .word 0xae450000    # +0x0528
    .word 0x24020006    # +0x052c
    .word 0x00a21818    # +0x0530
    .word 0x24a50001    # +0x0534
    .word 0x00731021    # +0x0538
    .word 0x24030100    # +0x053c
    .word 0x00021080    # +0x0540
    .word 0x00541021    # +0x0544
    .word 0xac4300a8    # +0x0548
    .word 0x1000ff59    # +0x054c
    .word 0xac400018    # +0x0550
    .word 0x1040000f    # +0x0554
    .word 0x0000382d    # +0x0558
    .word 0x03a0302d    # +0x055c
    .word 0x24020006    # +0x0560
    .word 0x8cc50040    # +0x0564
    .word 0x00e21818    # +0x0568
    .word 0x8cc40044    # +0x056c
    .word 0x24e70001    # +0x0570
    .word 0x24c60008    # +0x0574
    .word 0x00731021    # +0x0578
    .word 0x00f1182a    # +0x057c
    .word 0x00021080    # +0x0580
    .word 0x00541021    # +0x0584
    .word 0xac4400a8    # +0x0588
    .word 0x1460fff4    # +0x058c
    .word 0xac450018    # +0x0590
    .word 0x1000ff48    # +0x0594
    .word 0xae510000    # +0x0598
    .word 0x24020001    # +0x059c
    .word 0x11620067    # +0x05a0
    .word 0x8fa20000    # +0x05a4
    .word 0x24020001    # +0x05a8
    .word 0x11020058    # +0x05ac
    .word 0x8fa20020    # +0x05b0
    .word 0xafa00070    # +0x05b4
    .word 0x2eb50001    # +0x05b8
    .word 0x24100001    # +0x05bc
    .word 0x1160000e    # +0x05c0
    .word 0x0000302d    # +0x05c4
    .word 0x03a0282d    # +0x05c8
    .word 0x27a40074    # +0x05cc
    .word 0x8ca20000    # +0x05d0
    .word 0x24c60001    # +0x05d4
    .word 0x00cb182b    # +0x05d8
    .word 0x26100002    # +0x05dc
    .word 0xac820000    # +0x05e0
    .word 0x24840004    # +0x05e4
    .word 0x8ca20004    # +0x05e8
    .word 0x24a50008    # +0x05ec
    .word 0xac820000    # +0x05f0
    .word 0x1460fff6    # +0x05f4
    .word 0x24840004    # +0x05f8
    .word 0x11000010    # +0x05fc
    .word 0x0000302d    # +0x0600
    .word 0x00101080    # +0x0604
    .word 0x03a0282d    # +0x0608
    .word 0x005d1021    # +0x060c
    .word 0x24440070    # +0x0610
    .word 0x8ca20020    # +0x0614
    .word 0x24c60001    # +0x0618
    .word 0x00c8182b    # +0x061c
    .word 0x26100002    # +0x0620
    .word 0xac820000    # +0x0624
    .word 0x24840004    # +0x0628
    .word 0x8ca20024    # +0x062c
    .word 0x24a50008    # +0x0630
    .word 0xac820000    # +0x0634
    .word 0x1460fff6    # +0x0638
    .word 0x24840004    # +0x063c
    .word 0x00101080    # +0x0640
    .word 0x24030100    # +0x0644
    .word 0x005d1021    # +0x0648
    .word 0x26100001    # +0x064c
    .word 0x3c070011    # +0x0650
    .word 0xac430070    # +0x0654
    .word 0x24060004    # +0x0658
    .word 0x24e747c8    # +0x065c
    .word 0x27a40070    # +0x0660
    .word 0x0c042033    # +0x0664
    .word 0x0200282d    # +0x0668
    .word 0x1200fecc    # +0x066c
    .word 0x0000302d    # +0x0670
    .word 0x00061080    # +0x0674
    .word 0x005d2021    # +0x0678
    .word 0x8c870070    # +0x067c
    .word 0x8c820074    # +0x0680
    .word 0x10e20010    # +0x0684
    .word 0x001140c0    # +0x0688
    .word 0x24c50002    # +0x068c
    .word 0x011d1021    # +0x0690
    .word 0x00b0182b    # +0x0694
    .word 0x10600005    # +0x0698
    .word 0xac470040    # +0x069c
    .word 0x8c830078    # +0x06a0
    .word 0x8c820074    # +0x06a4
    .word 0x1043000d    # +0x06a8
    .word 0x00051080    # +0x06ac
    .word 0x00061080    # +0x06b0
    .word 0x011d1821    # +0x06b4
    .word 0x005d1021    # +0x06b8
    .word 0x26310001    # +0x06bc
    .word 0x8c420074    # +0x06c0
    .word 0xac620044    # +0x06c4
    .word 0x24c60002    # +0x06c8
    .word 0x00d0102b    # +0x06cc
    .word 0x1440ffe9    # +0x06d0
    .word 0x00061080    # +0x06d4
    .word 0x1000feb1    # +0x06d8
    .word 0x00000000    # +0x06dc
    .word 0x00a0302d    # +0x06e0
    .word 0x24a50002    # +0x06e4
    .word 0x005d2021    # +0x06e8
    .word 0x00b0102b    # +0x06ec
    .word 0x1040fff0    # +0x06f0
    .word 0x00061080    # +0x06f4
    .word 0x8c830078    # +0x06f8
    .word 0x8c820074    # +0x06fc
    .word 0x5043fff7    # +0x0700
    .word 0x00051080    # +0x0704
    .word 0x1000ffea    # +0x0708
    .word 0x00061080    # +0x070c
    .word 0x8fa30024    # +0x0710
    .word 0x0043102b    # +0x0714
    .word 0x5440ffa7    # +0x0718
    .word 0xafa00070    # +0x071c
    .word 0x0160882d    # +0x0720
    .word 0x27a40040    # +0x0724
    .word 0x000b30c0    # +0x0728
    .word 0x03a0282d    # +0x072c
    .word 0x0c067128    # +0x0730
    .word 0x00000000    # +0x0734
    .word 0x1000fe99    # +0x0738
    .word 0x00000000    # +0x073c
    .word 0x8fa30004    # +0x0740
    .word 0x0043102b    # +0x0744
    .word 0x1440ff98    # +0x0748
    .word 0x24020001    # +0x074c
    .word 0x0100882d    # +0x0750
    .word 0x27a40040    # +0x0754
    .word 0x000830c0    # +0x0758
    .word 0x1000fff4    # +0x075c
    .word 0x27a50020    # +0x0760
    .word 0x1460fe8e    # +0x0764
    .word 0x00000000    # +0x0768
    .word 0x516400a1    # +0x076c
    .word 0x8fa40000    # +0x0770
    .word 0x1104002b    # +0x0774
    .word 0x8fa30000    # +0x0778
    .word 0x24110001    # +0x077c
    .word 0x8fa60020    # +0x0780
    .word 0x8fa40008    # +0x0784
    .word 0x8fa50028    # +0x0788
    .word 0x0066102b    # +0x078c
    .word 0x0062300b    # +0x0790
    .word 0x8fa80024    # +0x0794
    .word 0x8fa30004    # +0x0798
    .word 0x0085102b    # +0x079c
    .word 0x0082280b    # +0x07a0
    .word 0x8fa7002c    # +0x07a4
    .word 0x8fa4000c    # +0x07a8
    .word 0x0103102b    # +0x07ac
    .word 0x0062400b    # +0x07b0
    .word 0xafa60040    # +0x07b4
    .word 0x00e4102b    # +0x07b8
    .word 0x00c5182b    # +0x07bc
    .word 0x0082380b    # +0x07c0
    .word 0xafa80044    # +0x07c4
    .word 0xafa50048    # +0x07c8
    .word 0x14600004    # +0x07cc
    .word 0xafa7004c    # +0x07d0
    .word 0x00c7102b    # +0x07d4
    .word 0x54400008    # +0x07d8
    .word 0x8fa60040    # +0x07dc
    .word 0x00a6102b    # +0x07e0
    .word 0x5440fe6e    # +0x07e4
    .word 0x24110002    # +0x07e8
    .word 0x00a8102b    # +0x07ec
    .word 0x5040fe6b    # +0x07f0
    .word 0x24110002    # +0x07f4
    .word 0x8fa60040    # +0x07f8
    .word 0x8fa70044    # +0x07fc
    .word 0x8fa20048    # +0x0800
    .word 0x8fa3004c    # +0x0804
    .word 0x00c2202b    # +0x0808
    .word 0x0067282b    # +0x080c
    .word 0x00c4100b    # +0x0810
    .word 0x00e5180b    # +0x0814
    .word 0xafa20040    # +0x0818
    .word 0x1000fe60    # +0x081c
    .word 0xafa30044    # +0x0820
    .word 0x8fa50020    # +0x0824
    .word 0x8fa70024    # +0x0828
    .word 0x00a7102b    # +0x082c
    .word 0x1040ffbc    # +0x0830
    .word 0x0160882d    # +0x0834
    .word 0x8fa40000    # +0x0838
    .word 0x00a4102b    # +0x083c
    .word 0x14400006    # +0x0840
    .word 0x0085102b    # +0x0844
    .word 0x8fa20004    # +0x0848
    .word 0x00a2102b    # +0x084c
    .word 0x14400008    # +0x0850
    .word 0x8fa20004    # +0x0854
    .word 0x0085102b    # +0x0858
    .word 0x1440003c    # +0x085c
    .word 0x8fa30008    # +0x0860
    .word 0x0087102b    # +0x0864
    .word 0x1040003a    # +0x0868
    .word 0x00a3102b    # +0x086c
    .word 0x8fa20004    # +0x0870
    .word 0x00a4182b    # +0x0874
    .word 0x8fa60008    # +0x0878
    .word 0x00a3200b    # +0x087c
    .word 0x0047182b    # +0x0880
    .word 0xafa40040    # +0x0884
    .word 0x00e3100b    # +0x0888
    .word 0x00a6182b    # +0x088c
    .word 0x14600005    # +0x0890
    .word 0xafa20044    # +0x0894
    .word 0x8fa2000c    # +0x0898
    .word 0x00a2102b    # +0x089c
    .word 0x14400007    # +0x08a0
    .word 0x8fa2000c    # +0x08a4
    .word 0x00c5102b    # +0x08a8
    .word 0x14400022    # +0x08ac
    .word 0x00c7102b    # +0x08b0
    .word 0x10400020    # +0x08b4
    .word 0x00a6182b    # +0x08b8
    .word 0x8fa2000c    # +0x08bc
    .word 0x00a3300b    # +0x08c0
    .word 0x0047182b    # +0x08c4
    .word 0xafa60048    # +0x08c8
    .word 0x00e3100b    # +0x08cc
    .word 0xafa2004c    # +0x08d0
    .word 0x8fa60040    # +0x08d4
    .word 0x8fa50048    # +0x08d8
    .word 0x00c5102b    # +0x08dc
    .word 0x14400005    # +0x08e0
    .word 0x24110001    # +0x08e4
    .word 0x8fa2004c    # +0x08e8
    .word 0x00c2102b    # +0x08ec
    .word 0x14400009    # +0x08f0
    .word 0x8fa40044    # +0x08f4
    .word 0x00a6102b    # +0x08f8
    .word 0x5440fe28    # +0x08fc
    .word 0x24110002    # +0x0900
    .word 0x8fa20044    # +0x0904
    .word 0x00a2102b    # +0x0908
    .word 0x5040fe24    # +0x090c
    .word 0x24110002    # +0x0910
    .word 0x8fa40044    # +0x0914
    .word 0x00c5182b    # +0x0918
    .word 0x8fa2004c    # +0x091c
    .word 0x00c3280b    # +0x0920
    .word 0xafa50040    # +0x0924
    .word 0x0044182b    # +0x0928
    .word 0x0083100b    # +0x092c
    .word 0x1000fe1b    # +0x0930
    .word 0xafa20044    # +0x0934
    .word 0x6ba9000f    # +0x0938
    .word 0x6fa90008    # +0x093c
    .word 0xb3a9004f    # +0x0940
    .word 0xb7a90048    # +0x0944
    .word 0x1000ffe3    # +0x0948
    .word 0x8fa60040    # +0x094c
    .word 0x00a3102b    # +0x0950
    .word 0x14400005    # +0x0954
    .word 0x0065102b    # +0x0958
    .word 0x8fa2000c    # +0x095c
    .word 0x00a2102b    # +0x0960
    .word 0x14400005    # +0x0964
    .word 0x0065102b    # +0x0968
    .word 0x14400013    # +0x096c
    .word 0x0067102b    # +0x0970
    .word 0x10400011    # +0x0974
    .word 0x00000000    # +0x0978
    .word 0x6ba40007    # +0x097c
    .word 0x6fa40000    # +0x0980
    .word 0xb3a40047    # +0x0984
    .word 0xb7a40040    # +0x0988
    .word 0x24110002    # +0x098c
    .word 0x8fa30008    # +0x0990
    .word 0x8fa50020    # +0x0994
    .word 0x8fa4000c    # +0x0998
    .word 0x8fa60024    # +0x099c
    .word 0x00a3102b    # +0x09a0
    .word 0x00a2180b    # +0x09a4
    .word 0x0086102b    # +0x09a8
    .word 0x00c2200b    # +0x09ac
    .word 0xafa30048    # +0x09b0
    .word 0x1000fdfa    # +0x09b4
    .word 0xafa4004c    # +0x09b8
    .word 0x6baa0007    # +0x09bc
    .word 0x6faa0000    # +0x09c0
    .word 0xb3aa0047    # +0x09c4
    .word 0xb7aa0040    # +0x09c8
    .word 0x6ba50027    # +0x09cc
    .word 0x6fa50020    # +0x09d0
    .word 0xb3a5004f    # +0x09d4
    .word 0xb7a50048    # +0x09d8
    .word 0x6ba2000f    # +0x09dc
    .word 0x6fa20008    # +0x09e0
    .word 0xb3a20057    # +0x09e4
    .word 0xb7a20050    # +0x09e8
    .word 0x1000fdec    # +0x09ec
    .word 0x24110003    # +0x09f0
    .word 0x8fa70004    # +0x09f4
    .word 0x0087102b    # +0x09f8
    .word 0x1040ff55    # +0x09fc
    .word 0x0100882d    # +0x0a00
    .word 0x110b004d    # +0x0a04
    .word 0x8fa50020    # +0x0a08
    .word 0x0085102b    # +0x0a0c
    .word 0x14400006    # +0x0a10
    .word 0x00a4102b    # +0x0a14
    .word 0x8fa20024    # +0x0a18
    .word 0x0082102b    # +0x0a1c
    .word 0x14400008    # +0x0a20
    .word 0x8fa20024    # +0x0a24
    .word 0x00a4102b    # +0x0a28
    .word 0x14400020    # +0x0a2c
    .word 0x8fa30028    # +0x0a30
    .word 0x00a7102b    # +0x0a34
    .word 0x1040001e    # +0x0a38
    .word 0x0083102b    # +0x0a3c
    .word 0x8fa20024    # +0x0a40
    .word 0x0085182b    # +0x0a44
    .word 0x8fa60028    # +0x0a48
    .word 0x0083280b    # +0x0a4c
    .word 0x0047182b    # +0x0a50
    .word 0xafa50040    # +0x0a54
    .word 0x00e3100b    # +0x0a58
    .word 0x0086182b    # +0x0a5c
    .word 0x14600005    # +0x0a60
    .word 0xafa20044    # +0x0a64
    .word 0x8fa2002c    # +0x0a68
    .word 0x0082102b    # +0x0a6c
    .word 0x14400007    # +0x0a70
    .word 0x8fa2002c    # +0x0a74
    .word 0x00c4102b    # +0x0a78
    .word 0x14400006    # +0x0a7c
    .word 0x00c7102b    # +0x0a80
    .word 0x10400004    # +0x0a84
    .word 0x0086182b    # +0x0a88
    .word 0x8fa2002c    # +0x0a8c
    .word 0x1000ff8c    # +0x0a90
    .word 0x0083300b    # +0x0a94
    .word 0x6baa002f    # +0x0a98
    .word 0x6faa0028    # +0x0a9c
    .word 0xb3aa004f    # +0x0aa0
    .word 0xb7aa0048    # +0x0aa4
    .word 0x1000ff8b    # +0x0aa8
    .word 0x8fa60040    # +0x0aac
    .word 0x0083102b    # +0x0ab0
    .word 0x14400005    # +0x0ab4
    .word 0x0064102b    # +0x0ab8
    .word 0x8fa2002c    # +0x0abc
    .word 0x0082102b    # +0x0ac0
    .word 0x14400005    # +0x0ac4
    .word 0x0064102b    # +0x0ac8
    .word 0x1440000d    # +0x0acc
    .word 0x0067102b    # +0x0ad0
    .word 0x1040000b    # +0x0ad4
    .word 0x00000000    # +0x0ad8
    .word 0x6ba50027    # +0x0adc
    .word 0x6fa50020    # +0x0ae0
    .word 0xb3a50047    # +0x0ae4
    .word 0xb7a50040    # +0x0ae8
    .word 0x24110002    # +0x0aec
    .word 0x8fa30028    # +0x0af0
    .word 0x8fa50000    # +0x0af4
    .word 0x8fa4002c    # +0x0af8
    .word 0x1000ffa8    # +0x0afc
    .word 0x8fa60004    # +0x0b00
    .word 0x6ba20027    # +0x0b04
    .word 0x6fa20020    # +0x0b08
    .word 0xb3a20047    # +0x0b0c
    .word 0xb7a20040    # +0x0b10
    .word 0x6ba70007    # +0x0b14
    .word 0x6fa70000    # +0x0b18
    .word 0xb3a7004f    # +0x0b1c
    .word 0xb7a70048    # +0x0b20
    .word 0x6ba3002f    # +0x0b24
    .word 0x6fa30028    # +0x0b28
    .word 0xb3a30057    # +0x0b2c
    .word 0xb7a30050    # +0x0b30
    .word 0x1000fd9a    # +0x0b34
    .word 0x24110003    # +0x0b38
    .word 0x8fa60024    # +0x0b3c
    .word 0x00a6102b    # +0x0b40
    .word 0x14400007    # +0x0b44
    .word 0x0085102b    # +0x0b48
    .word 0x6ba30007    # +0x0b4c
    .word 0x6fa30000    # +0x0b50
    .word 0xb3a30047    # +0x0b54
    .word 0xb7a30040    # +0x0b58
    .word 0x1000fd90    # +0x0b5c
    .word 0x24110001    # +0x0b60
    .word 0x14400005    # +0x0b64
    .word 0x00a4102b    # +0x0b68
    .word 0x0086102b    # +0x0b6c
    .word 0x14400006    # +0x0b70
    .word 0x0085102b    # +0x0b74
    .word 0x00a4102b    # +0x0b78
    .word 0x1440000a    # +0x0b7c
    .word 0x00a7102b    # +0x0b80
    .word 0x10400008    # +0x0b84
    .word 0x0085102b    # +0x0b88
    .word 0x00c7182b    # +0x0b8c
    .word 0x0082280b    # +0x0b90
    .word 0x00e3300b    # +0x0b94
    .word 0x24110001    # +0x0b98
    .word 0xafa50040    # +0x0b9c
    .word 0x1000fd7f    # +0x0ba0
    .word 0xafa60044    # +0x0ba4
    .word 0x6ba90007    # +0x0ba8
    .word 0x6fa90000    # +0x0bac
    .word 0xb3a90047    # +0x0bb0
    .word 0xb7a90040    # +0x0bb4
    .word 0x6ba40027    # +0x0bb8
    .word 0x6fa40020    # +0x0bbc
    .word 0xb3a4004f    # +0x0bc0
    .word 0xb7a40048    # +0x0bc4
    .word 0x1000fd75    # +0x0bc8
    .word 0x24110002    # +0x0bcc
    .word 0x1163005e    # +0x0bd0
    .word 0x8fa90000    # +0x0bd4
    .word 0x11030040    # +0x0bd8
    .word 0x8faa0000    # +0x0bdc
    .word 0x24110002    # +0x0be0
    .word 0x8fab0020    # +0x0be4
    .word 0x8fae0024    # +0x0be8
    .word 0x8fac0004    # +0x0bec
    .word 0x016a102b    # +0x0bf0
    .word 0x8fa5002c    # +0x0bf4
    .word 0x0160382d    # +0x0bf8
    .word 0x8fa40028    # +0x0bfc
    .word 0x0142380b    # +0x0c00
    .word 0x8fa90008    # +0x0c04
    .word 0x01c0302d    # +0x0c08
    .word 0x8fad000c    # +0x0c0c
    .word 0x018e102b    # +0x0c10
    .word 0x8fa80028    # +0x0c14
    .word 0x0182300b    # +0x0c18
    .word 0x01a5182b    # +0x0c1c
    .word 0x0089102b    # +0x0c20
    .word 0x0122200b    # +0x0c24
    .word 0x00a0782d    # +0x0c28
    .word 0x0148102b    # +0x0c2c
    .word 0x01a3280b    # +0x0c30
    .word 0xafa70040    # +0x0c34
    .word 0xafa60044    # +0x0c38
    .word 0xafa40048    # +0x0c3c
    .word 0x14400004    # +0x0c40
    .word 0xafa5004c    # +0x0c44
    .word 0x014f102b    # +0x0c48
    .word 0x14400008    # +0x0c4c
    .word 0x8fa2002c    # +0x0c50
    .word 0x010a102b    # +0x0c54
    .word 0x1440000d    # +0x0c58
    .word 0x012b102b    # +0x0c5c
    .word 0x010c102b    # +0x0c60
    .word 0x1040000a    # +0x0c64
    .word 0x012b102b    # +0x0c68
    .word 0x8fa2002c    # +0x0c6c
    .word 0x010a182b    # +0x0c70
    .word 0x0143400b    # +0x0c74
    .word 0x24110003    # +0x0c78
    .word 0x0182182b    # +0x0c7c
    .word 0xafa80050    # +0x0c80
    .word 0x0183100b    # +0x0c84
    .word 0x1000fd45    # +0x0c88
    .word 0xafa20054    # +0x0c8c
    .word 0x14400005    # +0x0c90
    .word 0x0169102b    # +0x0c94
    .word 0x012e102b    # +0x0c98
    .word 0x14400006    # +0x0c9c
    .word 0x8fa30024    # +0x0ca0
    .word 0x0169102b    # +0x0ca4
    .word 0x1440fd3d    # +0x0ca8
    .word 0x016d102b    # +0x0cac
    .word 0x1040fd3b    # +0x0cb0
    .word 0x8fa30024    # +0x0cb4
    .word 0x0169102b    # +0x0cb8
    .word 0x8fa4000c    # +0x0cbc
    .word 0x0122580b    # +0x0cc0
    .word 0x24110003    # +0x0cc4
    .word 0xafab0050    # +0x0cc8
    .word 0x0083102b    # +0x0ccc
    .word 0x0082180b    # +0x0cd0
    .word 0x1000fd32    # +0x0cd4
    .word 0xafa30054    # +0x0cd8
    .word 0x8fa90020    # +0x0cdc
    .word 0x8faa0024    # +0x0ce0
    .word 0x012a102b    # +0x0ce4
    .word 0x14400007    # +0x0ce8
    .word 0x8fa30000    # +0x0cec
    .word 0x6ba90027    # +0x0cf0
    .word 0x6fa90020    # +0x0cf4
    .word 0xb3a90047    # +0x0cf8
    .word 0xb7a90040    # +0x0cfc
    .word 0x1000fd27    # +0x0d00
    .word 0x24110001    # +0x0d04
    .word 0x24110002    # +0x0d08
    .word 0x8fa20004    # +0x0d0c
    .word 0x8fa80008    # +0x0d10
    .word 0x0123202b    # +0x0d14
    .word 0x8fa7000c    # +0x0d18
    .word 0x0124180a    # +0x0d1c
    .word 0x004a282b    # +0x0d20
    .word 0x0128302b    # +0x0d24
    .word 0x00ea202b    # +0x0d28
    .word 0x0145100a    # +0x0d2c
    .word 0x0106480b    # +0x0d30
    .word 0x00e4500b    # +0x0d34
    .word 0xafa30040    # +0x0d38
    .word 0xafa20044    # +0x0d3c
    .word 0xafa90048    # +0x0d40
    .word 0x1000fd16    # +0x0d44
    .word 0xafaa004c    # +0x0d48
    .word 0x8faa0004    # +0x0d4c
    .word 0x012a102b    # +0x0d50
    .word 0x14400006    # +0x0d54
    .word 0x24110001    # +0x0d58
    .word 0x6ba70007    # +0x0d5c
    .word 0x6fa70000    # +0x0d60
    .word 0xb3a70047    # +0x0d64
    .word 0x1000fd0d    # +0x0d68
    .word 0xb7a70040    # +0x0d6c
    .word 0x110b0012    # +0x0d70
    .word 0x8fa60020    # +0x0d74
    .word 0x24110002    # +0x0d78
    .word 0x8fa40024    # +0x0d7c
    .word 0x8fa50028    # +0x0d80
    .word 0x00c9102b    # +0x0d84
    .word 0x8fa3002c    # +0x0d88
    .word 0x0122300b    # +0x0d8c
    .word 0x0144402b    # +0x0d90
    .word 0x00a9382b    # +0x0d94
    .word 0x0143102b    # +0x0d98
    .word 0x0127280b    # +0x0d9c
    .word 0x0142180b    # +0x0da0
    .word 0x0148200b    # +0x0da4
    .word 0xafa60040    # +0x0da8
    .word 0xafa40044    # +0x0dac
    .word 0xafa50048    # +0x0db0
    .word 0x1000fcfa    # +0x0db4
    .word 0xafa3004c    # +0x0db8
    .word 0x8fa40020    # +0x0dbc
    .word 0x8fa50024    # +0x0dc0
    .word 0x0085102b    # +0x0dc4
    .word 0x14400007    # +0x0dc8
    .word 0x0089102b    # +0x0dcc
    .word 0x6ba30027    # +0x0dd0
    .word 0x6fa30020    # +0x0dd4
    .word 0xb3a30047    # +0x0dd8
    .word 0xb7a30040    # +0x0ddc
    .word 0x1000fcef    # +0x0de0
    .word 0x24110001    # +0x0de4
    .word 0x0145182b    # +0x0de8
    .word 0x0122200b    # +0x0dec
    .word 0x0143280b    # +0x0df0
    .word 0x24110001    # +0x0df4
    .word 0xafa40040    # +0x0df8
    .word 0x1000fce8    # +0x0dfc
    .word 0xafa50044    # +0x0e00
    .word 0x12a00027    # +0x0e04
    .word 0x010b102b    # +0x0e08
    .word 0x24020001    # +0x0e0c
    .word 0x11620007    # +0x0e10
    .word 0x0000282d    # +0x0e14
    .word 0x8fa20004    # +0x0e18
    .word 0x24050001    # +0x0e1c
    .word 0xae420018    # +0x0e20
    .word 0x8fa20008    # +0x0e24
    .word 0x1000fd22    # +0x0e28
    .word 0xae4200a8    # +0x0e2c
    .word 0x8fa30000    # +0x0e30
    .word 0x8fa40004    # +0x0e34
    .word 0x0083102b    # +0x0e38
    .word 0x14400015    # +0x0e3c
    .word 0x24020100    # +0x0e40
    .word 0x10600004    # +0x0e44
    .word 0x2c820100    # +0x0e48
    .word 0xae4300a8    # +0x0e4c
    .word 0x24050001    # +0x0e50
    .word 0xae400018    # +0x0e54
    .word 0x10400009    # +0x0e58
    .word 0x24020006    # +0x0e5c
    .word 0x00a21818    # +0x0e60
    .word 0x24a50001    # +0x0e64
    .word 0x00731021    # +0x0e68
    .word 0x24030100    # +0x0e6c
    .word 0x00021080    # +0x0e70
    .word 0x00541021    # +0x0e74
    .word 0xac4300a8    # +0x0e78
    .word 0xac440018    # +0x0e7c
    .word 0x54a0fd0d    # +0x0e80
    .word 0xae450000    # +0x0e84
    .word 0x24050001    # +0x0e88
    .word 0x1000fda3    # +0x0e8c
    .word 0xae4b0018    # +0x0e90
    .word 0x24050001    # +0x0e94
    .word 0xae4200a8    # +0x0e98
    .word 0x1000fd05    # +0x0e9c
    .word 0xae400018    # +0x0ea0
    .word 0x1040000f    # +0x0ea4
    .word 0x0000382d    # +0x0ea8
    .word 0x03a0302d    # +0x0eac
    .word 0x24020006    # +0x0eb0
    .word 0x8cc50000    # +0x0eb4
    .word 0x00e21818    # +0x0eb8
    .word 0x8cc40004    # +0x0ebc
    .word 0x24e70001    # +0x0ec0
    .word 0x24c60008    # +0x0ec4
    .word 0x00731021    # +0x0ec8
    .word 0x00eb182b    # +0x0ecc
    .word 0x00021080    # +0x0ed0
    .word 0x00541021    # +0x0ed4
    .word 0xac4400a8    # +0x0ed8
    .word 0x1460fff4    # +0x0edc
    .word 0xac450018    # +0x0ee0
    .word 0x1000fcf4    # +0x0ee4
    .word 0xae4b0000    # +0x0ee8
    .word 0x1100fcf3    # +0x0eec
    .word 0x24020005    # +0x0ef0
    .word 0x12a00022    # +0x0ef4
    .word 0x24020001    # +0x0ef8
    .word 0x11020007    # +0x0efc
    .word 0x0000282d    # +0x0f00
    .word 0x8fa30024    # +0x0f04
    .word 0x24050001    # +0x0f08
    .word 0x8fa20028    # +0x0f0c
    .word 0xae430018    # +0x0f10
    .word 0x1000ffc4    # +0x0f14
    .word 0x24420001    # +0x0f18
    .word 0x8fa30020    # +0x0f1c
    .word 0x8fa40024    # +0x0f20
    .word 0x0083102b    # +0x0f24
    .word 0x1440ffda    # +0x0f28
    .word 0x24020100    # +0x0f2c
    .word 0x10600004    # +0x0f30
    .word 0x2c820100    # +0x0f34
    .word 0xae4300a8    # +0x0f38
    .word 0x24050001    # +0x0f3c
    .word 0xae400018    # +0x0f40
    .word 0x10400009    # +0x0f44
    .word 0x24020006    # +0x0f48
    .word 0x00a21818    # +0x0f4c
    .word 0x24a50001    # +0x0f50
    .word 0x00731021    # +0x0f54
    .word 0x24030100    # +0x0f58
    .word 0x00021080    # +0x0f5c
    .word 0x00541021    # +0x0f60
    .word 0xac4300a8    # +0x0f64
    .word 0xac440018    # +0x0f68
    .word 0x54a0fcd2    # +0x0f6c
    .word 0xae450000    # +0x0f70
    .word 0xae480018    # +0x0f74
    .word 0x1000fd68    # +0x0f78
    .word 0x24050001    # +0x0f7c
    .word 0x1100000f    # +0x0f80
    .word 0x0000382d    # +0x0f84
    .word 0x03a0302d    # +0x0f88
    .word 0x24020006    # +0x0f8c
    .word 0x8cc50020    # +0x0f90
    .word 0x00e21818    # +0x0f94
    .word 0x8cc40024    # +0x0f98
    .word 0x24e70001    # +0x0f9c
    .word 0x24c60008    # +0x0fa0
    .word 0x00731021    # +0x0fa4
    .word 0x00e8182b    # +0x0fa8
    .word 0x00021080    # +0x0fac
    .word 0x00541021    # +0x0fb0
    .word 0xac4400a8    # +0x0fb4
    .word 0x1460fff4    # +0x0fb8
    .word 0xac450018    # +0x0fbc
    .word 0x1000fcbd    # +0x0fc0
    .word 0xae480000    # +0x0fc4
    .word 0x90a30aa8    # +0x0fc8
    .word 0x0083102b    # +0x0fcc
    .word 0x14400014    # +0x0fd0
    .word 0x24020100    # +0x0fd4
    .word 0x10600004    # +0x0fd8
    .word 0x240200ff    # +0x0fdc
    .word 0xafa30024    # +0x0fe0
    .word 0x24080001    # +0x0fe4
    .word 0xafa00020    # +0x0fe8
    .word 0x10820007    # +0x0fec
    .word 0x000810c0    # +0x0ff0
    .word 0x24840001    # +0x0ff4
    .word 0x005d1021    # +0x0ff8
    .word 0x24030100    # +0x0ffc
    .word 0xac430024    # +0x1000
    .word 0x25080001    # +0x1004
    .word 0xac440020    # +0x1008
    .word 0x1500fc52    # +0x100c
    .word 0x24020001    # +0x1010
    .word 0x24080001    # +0x1014
    .word 0xafa20020    # +0x1018
    .word 0x1000fc4e    # +0x101c
    .word 0xafa00024    # +0x1020
    .word 0xafa00020    # +0x1024
    .word 0xafa20024    # +0x1028
    .word 0x1000fc4a    # +0x102c
    .word 0x24080001    # +0x1030
    .word 0x90a30aa6    # +0x1034
    .word 0x0083102b    # +0x1038
    .word 0x14400015    # +0x103c
    .word 0x24020100    # +0x1040
    .word 0x10600004    # +0x1044
    .word 0x240200ff    # +0x1048
    .word 0xafa30004    # +0x104c
    .word 0x240b0001    # +0x1050
    .word 0xafa00000    # +0x1054
    .word 0x10820007    # +0x1058
    .word 0x000b10c0    # +0x105c
    .word 0x24840001    # +0x1060
    .word 0x005d1021    # +0x1064
    .word 0x24030100    # +0x1068
    .word 0xac430004    # +0x106c
    .word 0x256b0001    # +0x1070
    .word 0xac440000    # +0x1074
    .word 0x1560fc29    # +0x1078
    .word 0x3c020036    # +0x107c
    .word 0x24020001    # +0x1080
    .word 0x240b0001    # +0x1084
    .word 0xafa20000    # +0x1088
    .word 0x1000fc23    # +0x108c
    .word 0xafa00004    # +0x1090
    .word 0xafa00000    # +0x1094
    .word 0xafa20004    # +0x1098
    .word 0x1000fc1f    # +0x109c
    .word 0x240b0001    # +0x10a0
    .word 0x24020001    # +0x10a4
    .word 0x12c2000c    # +0x10a8
    .word 0x3c020035    # +0x10ac
    .word 0x16c0fc00    # +0x10b0
    .word 0x24020005    # +0x10b4
    .word 0x3c020035    # +0x10b8
    .word 0x24030040    # +0x10bc
    .word 0x8c42e2c4    # +0x10c0
    .word 0x90422130    # +0x10c4
    .word 0x304200c0    # +0x10c8
    .word 0x1443fbf9    # +0x10cc
    .word 0x24020005    # +0x10d0
    .word 0x1000fbf7    # +0x10d4
    .word 0x24150001    # +0x10d8
    .word 0x24030010    # +0x10dc
    .word 0x8c42e2c4    # +0x10e0
    .word 0x90422130    # +0x10e4
    .word 0x30420030    # +0x10e8
    .word 0x1443fff0    # +0x10ec
    .word 0x00000000    # +0x10f0
    .word 0x1000fbee    # +0x10f4
    .word 0x24150001    # +0x10f8
    .word 0x16c0001f    # +0x10fc
    .word 0x3c020035    # +0x1100
    .word 0x240300c0    # +0x1104
    .word 0x8c42e2c4    # +0x1108
    .word 0x90422130    # +0x110c
    .word 0x304200c0    # +0x1110
    .word 0x10430008    # +0x1114
    .word 0x3c020035    # +0x1118
    .word 0x8c42e2c4    # +0x111c
    .word 0x90422130    # +0x1120
    .word 0x304200c0    # +0x1124
    .word 0x5040fc81    # +0x1128
    .word 0x2673ffff    # +0x112c
    .word 0x1000fbc7    # +0x1130
    .word 0x3c020034    # +0x1134
    .word 0x0000302d    # +0x1138
    .word 0x2403004e    # +0x113c
    .word 0x24040001    # +0x1140
    .word 0x02c31018    # +0x1144
    .word 0x00461821    # +0x1148
    .word 0x3c020036    # +0x114c
    .word 0x24c60001    # +0x1150
    .word 0x00031880    # +0x1154
    .word 0x2442c268    # +0x1158
    .word 0x28c50006    # +0x115c
    .word 0x00621821    # +0x1160
    .word 0xac640e9c    # +0x1164
    .word 0xac600f2c    # +0x1168
    .word 0x14a0fff3    # +0x116c
    .word 0xac640e84    # +0x1170
    .word 0x1000fc6e    # +0x1174
    .word 0x2673ffff    # +0x1178
    .word 0x24030030    # +0x117c
    .word 0x8c42e2c4    # +0x1180
    .word 0x90422130    # +0x1184
    .word 0x30420030    # +0x1188
    .word 0x10430005    # +0x118c
    .word 0x3c020035    # +0x1190
    .word 0x8c42e2c4    # +0x1194
    .word 0x90422130    # +0x1198
    .word 0x1000ffe2    # +0x119c
    .word 0x30420030    # +0x11a0
    .word 0x0000282d    # +0x11a4
    .word 0x3c020036    # +0x11a8
    .word 0x00051880    # +0x11ac
    .word 0x2442c268    # +0x11b0
    .word 0x24a50001    # +0x11b4
    .word 0x00621821    # +0x11b8
    .word 0x28a40006    # +0x11bc
    .word 0x24020001    # +0x11c0
    .word 0xac601064    # +0x11c4
    .word 0xac620fd4    # +0x11c8
    .word 0x1480fff6    # +0x11cc
    .word 0xac620fbc    # +0x11d0
    .word 0x1000fc5e    # +0x11d4
    .word 0xdfbf0120    # +0x11d8
    .size stage3p_compute_clip_windows, .-stage3p_compute_clip_windows

    /* DSP2SetByte @ 0x0012f8a8..0x0012fb84
     * target span SHA-256: 88ad630d2863a33b2edafbe084f63f812db12f097891329e4b315448070299b4
     */
    .globl stage3p_dsp2_set_byte
    .type stage3p_dsp2_set_byte, @function
stage3p_dsp2_set_byte:
    .word 0x30a5ffff    # +0x0000
    .word 0x27bdffe0    # +0x0004
    .word 0x30a3f000    # +0x0008
    .word 0x24026000    # +0x000c
    .word 0xffb00000    # +0x0010
    .word 0x309000ff    # +0x0014
    .word 0x10620006    # +0x0018
    .word 0xffbf0010    # +0x001c
    .word 0x24a28000    # +0x0020
    .word 0x3042ffff    # +0x0024
    .word 0x2c424000    # +0x0028
    .word 0x5040001c    # +0x002c
    .word 0xdfbf0010    # +0x0030
    .word 0x3c030034    # +0x0034
    .word 0x90625628    # +0x0038
    .word 0x1040009e    # +0x003c
    .word 0x24645628    # +0x0040
    .word 0x2605ffff    # +0x0044
    .word 0xac800008    # +0x0048
    .word 0x2ca2000f    # +0x004c
    .word 0xa0605628    # +0x0050
    .word 0x10400091    # +0x0054
    .word 0xa0900002    # +0x0058
    .word 0x3c03001b    # +0x005c
    .word 0x00051080    # +0x0060
    .word 0x24632428    # +0x0064
    .word 0x00431021    # +0x0068
    .word 0x8c420000    # +0x006c
    .word 0x00400008    # +0x0070
    .word 0x00000000    # +0x0074
    .word 0x24030020    # +0x0078
    .word 0x3c020034    # +0x007c
    .word 0xac43562c    # +0x0080
    .word 0x3c050034    # +0x0084
    .word 0x24a45628    # +0x0088
    .word 0x8c830004    # +0x008c
    .word 0x8c820008    # +0x0090
    .word 0x50620005    # +0x0094
    .word 0x90820002    # +0x0098
    .word 0xdfbf0010    # +0x009c
    .word 0xdfb00000    # +0x00a0
    .word 0x03e00008    # +0x00a4
    .word 0x27bd0020    # +0x00a8
    .word 0x24030001    # +0x00ac
    .word 0xa0a35628    # +0x00b0
    .word 0x2443ffff    # +0x00b4
    .word 0x2c62000f    # +0x00b8
    .word 0x1040fff7    # +0x00bc
    .word 0xac800010    # +0x00c0
    .word 0x00031080    # +0x00c4
    .word 0x3c03001b    # +0x00c8
    .word 0x24632464    # +0x00cc
    .word 0x00431021    # +0x00d0
    .word 0x8c420000    # +0x00d4
    .word 0x00400008    # +0x00d8
    .word 0x00000000    # +0x00dc
    .word 0x3c070034    # +0x00e0
    .word 0x90e2164d    # +0x00e4
    .word 0x14400011    # +0x00e8
    .word 0x3c080034    # +0x00ec
    .word 0x24020001    # +0x00f0
    .word 0x25045628    # +0x00f4
    .word 0xa0e2164d    # +0x00f8
    .word 0x90860014    # +0x00fc
    .word 0x3c020034    # +0x0100
    .word 0x90850015    # +0x0104
    .word 0x24c30001    # +0x0108
    .word 0xac800008    # +0x010c
    .word 0x00031843    # +0x0110
    .word 0xac451650    # +0x0114
    .word 0x3c020034    # +0x0118
    .word 0xac830004    # +0x011c
    .word 0x1200ffde    # +0x0120
    .word 0xac461654    # +0x0124
    .word 0x1000ffdc    # +0x0128
    .word 0xa1005628    # +0x012c
    .word 0x3c020034    # +0x0130
    .word 0xa0e0164d    # +0x0134
    .word 0x8c431650    # +0x0138
    .word 0x3c020034    # +0x013c
    .word 0x0c04b96e    # +0x0140
    .word 0xac435634    # +0x0144
    .word 0x1000ffd5    # +0x0148
    .word 0xdfbf0010    # +0x014c
    .word 0x24030020    # +0x0150
    .word 0x3c020034    # +0x0154
    .word 0x0c04b8dd    # +0x0158
    .word 0xac435634    # +0x015c
    .word 0x1000ffcf    # +0x0160
    .word 0xdfbf0010    # +0x0164
    .word 0x3c020034    # +0x0168
    .word 0x9043563c    # +0x016c
    .word 0x3c020034    # +0x0170
    .word 0x1000ffc9    # +0x0174
    .word 0xa043164c    # +0x0178
    .word 0x3c040034    # +0x017c
    .word 0x9082163e    # +0x0180
    .word 0x1440000d    # +0x0184
    .word 0x3c060034    # +0x0188
    .word 0x24020001    # +0x018c
    .word 0x24c35628    # +0x0190
    .word 0xa082163e    # +0x0194
    .word 0x90650014    # +0x0198
    .word 0x3c020034    # +0x019c
    .word 0x00052040    # +0x01a0
    .word 0xac451640    # +0x01a4
    .word 0xac640004    # +0x01a8
    .word 0x1200ffbb    # +0x01ac
    .word 0xac600008    # +0x01b0
    .word 0x1000ffb9    # +0x01b4
    .word 0xa0c05628    # +0x01b8
    .word 0x3c020034    # +0x01bc
    .word 0xa080163e    # +0x01c0
    .word 0x8c431640    # +0x01c4
    .word 0x3c020034    # +0x01c8
    .word 0x0c04b8b8    # +0x01cc
    .word 0xac435634    # +0x01d0
    .word 0x1000ffb2    # +0x01d4
    .word 0xdfbf0010    # +0x01d8
    .word 0x3c050034    # +0x01dc
    .word 0x90a21644    # +0x01e0
    .word 0x14400009    # +0x01e4
    .word 0x3c060034    # +0x01e8
    .word 0x24020001    # +0x01ec
    .word 0x24c35628    # +0x01f0
    .word 0xa0a21644    # +0x01f4
    .word 0x90640014    # +0x01f8
    .word 0x3c020034    # +0x01fc
    .word 0xac640004    # +0x0200
    .word 0x1000ffe9    # +0x0204
    .word 0xac441648    # +0x0208
    .word 0x3c020034    # +0x020c
    .word 0xa0a01644    # +0x0210
    .word 0x8c431648    # +0x0214
    .word 0x3c020034    # +0x0218
    .word 0x0c04b95a    # +0x021c
    .word 0xac435634    # +0x0220
    .word 0x1000ff9e    # +0x0224
    .word 0xdfbf0010    # +0x0228
    .word 0x3c020034    # +0x022c
    .word 0x24425628    # +0x0230
    .word 0x90440015    # +0x0234
    .word 0x24470214    # +0x0238
    .word 0x90460017    # +0x023c
    .word 0x90430014    # +0x0240
    .word 0x00042200    # +0x0244
    .word 0x90450016    # +0x0248
    .word 0x00063200    # +0x024c
    .word 0x00641825    # +0x0250
    .word 0x24040004    # +0x0254
    .word 0x00a62825    # +0x0258
    .word 0xac44000c    # +0x025c
    .word 0x3c020034    # +0x0260
    .word 0xa443163a    # +0x0264
    .word 0x3c020034    # +0x0268
    .word 0x00651818    # +0x026c
    .word 0xa445163c    # +0x0270
    .word 0xa8e30003    # +0x0274
    .word 0xb8e30000    # +0x0278
    .word 0x1000ff88    # +0x027c
    .word 0xdfbf0010    # +0x0280
    .word 0x1000ff7d    # +0x0284
    .word 0x24030001    # +0x0288
    .word 0x1000ff7b    # +0x028c
    .word 0x24030004    # +0x0290
    .word 0x1000ff79    # +0x0294
    .word 0x24030002    # +0x0298
    .word 0x3c04001b    # +0x029c
    .word 0x0200282d    # +0x02a0
    .word 0x0c0678e2    # +0x02a4
    .word 0x24842420    # +0x02a8
    .word 0x3c020034    # +0x02ac
    .word 0x1000ff74    # +0x02b0
    .word 0xac40562c    # +0x02b4
    .word 0x8c820008    # +0x02b8
    .word 0x24430001    # +0x02bc
    .word 0x00441021    # +0x02c0
    .word 0xac830008    # +0x02c4
    .word 0x1000ff6e    # +0x02c8
    .word 0xa0500014    # +0x02cc
    .word 0x3c020034    # +0x02d0
    .word 0x3084ffff    # +0x02d4
    .word 0x24455628    # +0x02d8
    .size stage3p_dsp2_set_byte, .-stage3p_dsp2_set_byte

    /* DSP2GetByte @ 0x0012fb84..0x0012fbe8
     * target span SHA-256: 37fd29a2296ae3baa7e4b2f73a64657497ed17a24087bd76b08246bfdd74644d
     */
    .globl stage3p_dsp2_get_byte
    .type stage3p_dsp2_get_byte, @function
stage3p_dsp2_get_byte:
    .word 0x24828000    # +0x0000
    .word 0x3042ffff    # +0x0004
    .word 0x3084f000    # +0x0008
    .word 0x2c434000    # +0x000c
    .word 0x24026000    # +0x0010
    .word 0x50820004    # +0x0014
    .word 0x8ca4000c    # +0x0018
    .word 0x1060000a    # +0x001c
    .word 0x24020080    # +0x0020
    .word 0x8ca4000c    # +0x0024
    .word 0x10800007    # +0x0028
    .word 0x240200ff    # +0x002c
    .word 0x8ca20010    # +0x0030
    .word 0x24430001    # +0x0034
    .word 0x00451021    # +0x0038
    .word 0x90420214    # +0x003c
    .word 0x10830003    # +0x0040
    .word 0xaca30010    # +0x0044
    .word 0x03e00008    # +0x0048
    .word 0x00000000    # +0x004c
    .word 0x03e00008    # +0x0050
    .word 0xaca0000c    # +0x0054
    .word 0x27bdff70    # +0x0058
    .word 0x3c020034    # +0x005c
    .word 0xffbf0080    # +0x0060
    .size stage3p_dsp2_get_byte, .-stage3p_dsp2_get_byte

    /* FxPipeString @ 0x0012fbe8..0x0012fe5c
     * target span SHA-256: a2aecca26ff765af1407c6290205986601bb4274c10aecb0f4fa05f9d19bf830
     */
    .globl stage3p_fx_pipe_string
    .type stage3p_fx_pipe_string, @function
stage3p_fx_pipe_string:
    .word 0xffb20020    # +0x0000
    .word 0x0080902d    # +0x0004
    .word 0xffb10010    # +0x0008
    .word 0x24512a38    # +0x000c
    .word 0xffb00000    # +0x0010
    .word 0xffb70070    # +0x0014
    .word 0xffb60060    # +0x0018
    .word 0xffb50050    # +0x001c
    .word 0xffb40040    # +0x0020
    .word 0xffb30030    # +0x0024
    .word 0x92260072    # +0x0028
    .word 0x96270070    # +0x002c
    .word 0x00061080    # +0x0030
    .word 0x8e230048    # +0x0034
    .word 0x962d003c    # +0x0038
    .word 0x00511021    # +0x003c
    .word 0x9228006d    # +0x0040
    .word 0x24e50001    # +0x0044
    .word 0x8c4c01ec    # +0x0048
    .word 0x24e90002    # +0x004c
    .word 0x30630300    # +0x0050
    .word 0x25aa0001    # +0x0054
    .word 0x00681825    # +0x0058
    .word 0x8e2b01d8    # +0x005c
    .word 0x30a5ffff    # +0x0060
    .word 0x3129ffff    # +0x0064
    .word 0x3c020034    # +0x0068
    .word 0x01894821    # +0x006c
    .word 0x24421a38    # +0x0070
    .word 0x00031880    # +0x0074
    .word 0x01856021    # +0x0078
    .word 0x314affff    # +0x007c
    .word 0x3c05001b    # +0x0080
    .word 0x016a5021    # +0x0084
    .word 0x00621821    # +0x0088
    .word 0x24a52ca0    # +0x008c
    .word 0x016d5821    # +0x0090
    .word 0x8c730000    # +0x0094
    .word 0x91940000    # +0x0098
    .word 0x91360000    # +0x009c
    .word 0x91750000    # +0x00a0
    .word 0x0c0678f4    # +0x00a4
    .word 0x91570000    # +0x00a8
    .word 0x0c06717a    # +0x00ac
    .word 0x0240202d    # +0x00b0
    .word 0x9226006d    # +0x00b4
    .word 0x02428021    # +0x00b8
    .word 0x24c2fffb    # +0x00bc
    .word 0x2c42000b    # +0x00c0
    .word 0x1440005d    # +0x00c4
    .word 0x24c2fff0    # +0x00c8
    .word 0x2c420010    # +0x00cc
    .word 0x10400005    # +0x00d0
    .word 0x3c020034    # +0x00d4
    .word 0x8e220048    # +0x00d8
    .word 0x30421000    # +0x00dc
    .word 0x1440004e    # +0x00e0
    .word 0x3c020034    # +0x00e4
    .word 0x24512a38    # +0x00e8
    .word 0x9227006d    # +0x00ec
    .word 0x24e20060    # +0x00f0
    .word 0x304200ff    # +0x00f4
    .word 0x2c420010    # +0x00f8
    .word 0x14400033    # +0x00fc
    .word 0x24e20050    # +0x0100
    .word 0x304200ff    # +0x0104
    .word 0x2c420010    # +0x0108
    .word 0x10400005    # +0x010c
    .word 0x3c020034    # +0x0110
    .word 0x8e220048    # +0x0114
    .word 0x30421000    # +0x0118
    .word 0x14400020    # +0x011c
    .word 0x3c020034    # +0x0120
    .word 0x90422aa5    # +0x0124
    .word 0x2c4200f0    # +0x0128
    .word 0x14400018    # +0x012c
    .word 0x0200202d    # +0x0130
    .word 0x3c05001b    # +0x0134
    .word 0x2644000b    # +0x0138
    .word 0x24a52cb8    # +0x013c
    .word 0x0280302d    # +0x0140
    .word 0x0c0678f4    # +0x0144
    .word 0x02c0382d    # +0x0148
    .word 0x0200202d    # +0x014c
    .word 0x00173200    # +0x0150
    .word 0x0260282d    # +0x0154
    .word 0x02a63025    # +0x0158
    .word 0x0c0678f4    # +0x015c
    .word 0x00000000    # +0x0160
    .word 0xdfbf0080    # +0x0164
    .word 0xdfb70070    # +0x0168
    .word 0xdfb60060    # +0x016c
    .word 0xdfb50050    # +0x0170
    .word 0xdfb40040    # +0x0174
    .word 0xdfb30030    # +0x0178
    .word 0xdfb20020    # +0x017c
    .word 0xdfb10010    # +0x0180
    .word 0xdfb00000    # +0x0184
    .word 0x03e00008    # +0x0188
    .word 0x27bd0090    # +0x018c
    .word 0x0c06714a    # +0x0190
    .word 0x0260282d    # +0x0194
    .word 0x1000fff3    # +0x0198
    .word 0xdfbf0080    # +0x019c
    .word 0x8e260064    # +0x01a0
    .word 0x3c05001b    # +0x01a4
    .word 0x30e7000f    # +0x01a8
    .word 0x0200202d    # +0x01ac
    .word 0x00d13023    # +0x01b0
    .word 0x24a52cc8    # +0x01b4
    .word 0x00063083    # +0x01b8
    .word 0x0c0678f4    # +0x01bc
    .word 0x00000000    # +0x01c0
    .word 0x1000ffe8    # +0x01c4
    .word 0xdfbf0080    # +0x01c8
    .word 0x3c05001b    # +0x01cc
    .word 0x2644000b    # +0x01d0
    .word 0x24a52cd8    # +0x01d4
    .word 0x0c0678f4    # +0x01d8
    .word 0x0280302d    # +0x01dc
    .word 0x8e220048    # +0x01e0
    .word 0x30430300    # +0x01e4
    .word 0x24020100    # +0x01e8
    .word 0x10620007    # +0x01ec
    .word 0x24020200    # +0x01f0
    .word 0x50620006    # +0x01f4
    .word 0x00153040    # +0x01f8
    .word 0x0200202d    # +0x01fc
    .word 0x0260282d    # +0x0200
    .word 0x1000ffd5    # +0x0204
    .word 0x02a0302d    # +0x0208
    .word 0x00153040    # +0x020c
    .word 0x0200202d    # +0x0210
    .word 0x1000ffd1    # +0x0214
    .word 0x0260282d    # +0x0218
    .word 0x8e270068    # +0x021c
    .word 0x3c05001b    # +0x0220
    .word 0x30c6000f    # +0x0224
    .word 0x0200202d    # +0x0228
    .word 0x00f13823    # +0x022c
    .word 0x24a52ce8    # +0x0230
    .word 0x1000ffe1    # +0x0234
    .word 0x00073883    # +0x0238
    .word 0x3c05001b    # +0x023c
    .word 0x2644000b    # +0x0240
    .word 0x24a52cd8    # +0x0244
    .word 0x0c0678f4    # +0x0248
    .word 0x0280302d    # +0x024c
    .word 0x0200202d    # +0x0250
    .word 0x00151600    # +0x0254
    .word 0x9626003c    # +0x0258
    .word 0x00021603    # +0x025c
    .word 0x0260282d    # +0x0260
    .word 0x00c23021    # +0x0264
    .word 0x24c60001    # +0x0268
    .word 0x1000ffbb    # +0x026c
    .word 0x30c6ffff    # +0x0270
    .size stage3p_fx_pipe_string, .-stage3p_fx_pipe_string

    /* CMemory::Safe @ 0x00150f54..0x00151074
     * target span SHA-256: 27ccb334b12de4a652c091b974cccb369939229ae01f3ed0e5ffa2ba50e07ce8
     */
    .globl stage3p_memory_safe
    .type stage3p_memory_safe, @function
stage3p_memory_safe:
    .word 0x27bdffc0    # +0x0000
    .word 0xffb10010    # +0x0004
    .word 0x00a0882d    # +0x0008
    .word 0xffbf0030    # +0x000c
    .word 0xffb20020    # +0x0010
    .word 0x10a00038    # +0x0014
    .word 0xffb00000    # +0x0018
    .word 0x00a0202d    # +0x001c
    .word 0x0c06717a    # +0x0020
    .word 0x3c120043    # +0x0024
    .word 0x8e44e820    # +0x0028
    .word 0x1080002b    # +0x002c
    .word 0x0040802d    # +0x0030
    .word 0x24430001    # +0x0034
    .word 0x3c02003f    # +0x0038
    .word 0x8c424910    # +0x003c
    .word 0x0043102a    # +0x0040
    .word 0x14400023    # +0x0044
    .word 0x00000000    # +0x0048
    .word 0x1a000011    # +0x004c
    .word 0x0000202d    # +0x0050
    .word 0x02241021    # +0x0054
    .word 0x80430000    # +0x0058
    .word 0x90450000    # +0x005c
    .word 0x28620020    # +0x0060
    .word 0x14400016    # +0x0064
    .word 0x3c020043    # +0x0068
    .word 0x2402007f    # +0x006c
    .word 0x10620013    # +0x0070
    .word 0x3c020043    # +0x0074
    .word 0x8c42e820    # +0x0078
    .word 0x00441021    # +0x007c
    .word 0xa0450000    # +0x0080
    .word 0x24840001    # +0x0084
    .word 0x0090102a    # +0x0088
    .word 0x1440fff2    # +0x008c
    .word 0x02241021    # +0x0090
    .word 0x3c030043    # +0x0094
    .word 0x8c62e820    # +0x0098
    .word 0x00501021    # +0x009c
    .word 0xa0400000    # +0x00a0
    .word 0x8c62e820    # +0x00a4
    .word 0xdfbf0030    # +0x00a8
    .word 0xdfb20020    # +0x00ac
    .word 0xdfb10010    # +0x00b0
    .word 0xdfb00000    # +0x00b4
    .word 0x03e00008    # +0x00b8
    .word 0x27bd0040    # +0x00bc
    .word 0x2403003f    # +0x00c0
    .word 0x8c42e820    # +0x00c4
    .word 0x00441021    # +0x00c8
    .word 0x1000ffed    # +0x00cc
    .word 0xa0430000    # +0x00d0
    .word 0x0c0679e1    # +0x00d4
    .word 0x00000000    # +0x00d8
    .word 0x26020001    # +0x00dc
    .word 0x3c03003f    # +0x00e0
    .word 0x0040202d    # +0x00e4
    .word 0x0c06792d    # +0x00e8
    .word 0xac624910    # +0x00ec
    .word 0x1000ffd6    # +0x00f0
    .word 0xae42e820    # +0x00f4
    .word 0x3c100043    # +0x00f8
    .word 0x8e04e820    # +0x00fc
    .word 0x14800003    # +0x0100
    .word 0x00000000    # +0x0104
    .word 0x1000ffe7    # +0x0108
    .word 0x0000102d    # +0x010c
    .word 0x0c0679e1    # +0x0110
    .word 0x00000000    # +0x0114
    .word 0x1000fffb    # +0x0118
    .word 0xae00e820    # +0x011c
    .size stage3p_memory_safe, .-stage3p_memory_safe

    /* Frontend leaf/control cluster @ 0x00101838..0x00101890
     * target span SHA-256: fa175269c92ce45bb7ca773cac950d16299080aedd7b1cf7b5c4ab838f8f21ff
     */
    .balign 4
    .globl stage3p_frontend_leaf_cluster_00101838
    .type stage3p_frontend_leaf_cluster_00101838, @function
stage3p_frontend_leaf_cluster_00101838:
    .word 0x27bdfff0
    .word 0x24040001
    .word 0xffbf0000
    .word 0x0c040605
    .word 0x3405ffff
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x0000102d
    .word 0x03e00008
    .word 0x00000000
    .size stage3p_frontend_leaf_cluster_00101838, .-stage3p_frontend_leaf_cluster_00101838

    /* Frontend dispatch/leaf cluster @ 0x001018e0..0x00101924
     * target span SHA-256: a7c2ce28b5d4d505465f2a60505ff4bc1d83add6be777dc665707cfdb9d87a8a
     */
    .balign 4
    .globl stage3p_frontend_dispatch_cluster_001018e0
    .type stage3p_frontend_dispatch_cluster_001018e0, @function
stage3p_frontend_dispatch_cluster_001018e0:
    .word 0x27bdfff0
    .word 0xffbf0000
    .word 0x0c065126
    .word 0x00000000
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x0000102d
    .word 0x03e00008
    .word 0x00000000
    .size stage3p_frontend_dispatch_cluster_001018e0, .-stage3p_frontend_dispatch_cluster_001018e0

    /* Frontend zero-return leaf @ 0x001019a0..0x001019a8
     * target span SHA-256: 008d26890102af179c703d77fae42cb7b3f424a11b8db552383dd8e5313f4061
     */
    .balign 4
    .globl stage3p_frontend_leaf_001019a0
    .type stage3p_frontend_leaf_001019a0, @function
stage3p_frontend_leaf_001019a0:
    .word 0x03e00008
    .word 0x0000102d
    .size stage3p_frontend_leaf_001019a0, .-stage3p_frontend_leaf_001019a0

    /* Frontend GUI setup schedule @ 0x001029c4..0x00102ab0
     * target span SHA-256: 538c4ea3db3b400ce46ec0cee175d0209cf64def8b26cc585a65f405f5cb3a4c
     */
    .balign 4
    .globl stage3p_frontend_regalloc_001029c4
    .type stage3p_frontend_regalloc_001029c4, @function
stage3p_frontend_regalloc_001029c4:
    .word 0x27bdffd0
    .word 0xffbf0020
    .word 0x0c0406d9
    .word 0x00000000
    .word 0x3c04001c
    .word 0x3c0b001b
    .word 0x240a0002
    .word 0x2484b748
    .word 0x256b0a58
    .word 0x0000282d
    .word 0x24060280
    .word 0x240700dc
    .word 0x340980ff
    .word 0x00094c38
    .word 0x3529ffff
    .word 0x0c066e52
    .word 0x24080004
    .word 0x0c0407a3
    .word 0x00000000
    .word 0x0c0670ab
    .word 0x00000000
    .word 0x3c02001c
    .word 0x2403002f
    .word 0xa443b340
    .word 0x0000202d
    .word 0x0c06a22a
    .word 0x0000282d
    .word 0x24030006
    .word 0x10430011
    .word 0x0000202d
    .word 0x3c031200
    .word 0x34631000
    .word 0xdc620000
    .word 0x30420008
    .word 0xfc620000
    .word 0x3c021200
    .word 0x34421000
    .word 0xdc420000
    .word 0x30420008
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1040fff9
    .word 0x3c021200
    .word 0x1000ffec
    .word 0x0000202d
    .word 0x0000282d
    .word 0x0c06a219
    .word 0x03a0302d
    .word 0x93a20002
    .word 0x93a30003
    .word 0x00021200
    .word 0x00431027
    .word 0x30420040
    .word 0x5040ffe8
    .word 0x3c031200
    .word 0xdfbf0020
    .word 0x03e00008
    .word 0x27bd0030
    .size stage3p_frontend_regalloc_001029c4, .-stage3p_frontend_regalloc_001029c4

    /* Frontend control/address leaves @ 0x001041e4..0x00104234
     * target span SHA-256: be05aaea7d445d83ba29928800a84dcbea7994c525e8a74362c678a0326e5f9d
     */
    .balign 4
    .globl stage3p_frontend_control_001041e4
    .type stage3p_frontend_control_001041e4, @function
stage3p_frontend_control_001041e4:
    .word 0x27bdfff0
    .word 0x24040001
    .word 0xffbf0000
    .word 0x0c041070
    .word 0x3405ffff
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x3c020042
    .word 0xac800004
    .word 0x24426c28
    .word 0xac800008
    .word 0x03e00008
    .word 0xac820000
    .word 0x3c020042
    .word 0xac800004
    .word 0x24426c28
    .word 0xac800008
    .word 0x03e00008
    .word 0xac820000
    .size stage3p_frontend_control_001041e4, .-stage3p_frontend_control_001041e4

    /* Frontend shutdown/module-load bridge @ 0x00104e48..0x00104f18
     * target span SHA-256: 4db790eec228d78f8d6f342ceb0573036597cffaea42645eddc20308804fc275
     */
    .balign 4
    .globl stage3p_frontend_module_bridge_00104e48
    .type stage3p_frontend_module_bridge_00104e48, @function
stage3p_frontend_module_bridge_00104e48:
    .word 0x03e00008
    .word 0x0000102d
    .word 0x03e00008
    .word 0x0000102d
    .word 0x3c040035
    .word 0x27bdfff0
    .word 0xffbf0000
    .word 0x0c0544cc
    .word 0x2484e2b0
    .word 0x0c042a2f
    .word 0x00000000
    .word 0x0c067173
    .word 0x24040001
    .word 0x27bdff90
    .word 0xffb10020
    .word 0x0080882d
    .word 0x00a0202d
    .word 0xffbf0060
    .word 0xffb40050
    .word 0x00e0a02d
    .word 0xffb30040
    .word 0x00c0982d
    .word 0xffb20030
    .word 0xffb00010
    .word 0x0c06758f
    .word 0x00a0802d
    .word 0xafa0000c
    .word 0x03a0202d
    .word 0x24050001
    .word 0xafb00008
    .word 0x0040902d
    .word 0xafb10000
    .word 0x0c0673b8
    .word 0xafa20004
    .word 0x0040802d
    .word 0x0c0673b4
    .word 0x0200202d
    .word 0x0441fffd
    .word 0x0260282d
    .word 0x0280302d
    .word 0x0c067588
    .word 0x0240202d
    .word 0x0c0675ae
    .word 0x0240202d
    .word 0xdfb00010
    .word 0xdfbf0060
    .word 0xdfb40050
    .word 0xdfb30040
    .word 0xdfb20030
    .word 0xdfb10020
    .word 0x03e00008
    .word 0x27bd0070
    .size stage3p_frontend_module_bridge_00104e48, .-stage3p_frontend_module_bridge_00104e48

    /* Draw-sync leaf pair @ 0x001056b0..0x001056c0
     * target span SHA-256: 0545bda3e48da9ffa736f04df8e435f9d6e044bbf4ed99eba3090f71764155fc
     */
    .balign 4
    .globl stage3p_frontend_drawsync_leaf_001056b0
    .type stage3p_frontend_drawsync_leaf_001056b0, @function
stage3p_frontend_drawsync_leaf_001056b0:
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x24020001
    .size stage3p_frontend_drawsync_leaf_001056b0, .-stage3p_frontend_drawsync_leaf_001056b0

    /* Draw-sync wrapper tail @ 0x001056e0..0x00105718
     * target span SHA-256: c6c2febbc22b936c7553ed50c8396779c5d0d721a6906b2556da6ec5f33c746a
     */
    .balign 4
    .globl stage3p_frontend_drawsync_tail_001056e0
    .type stage3p_frontend_drawsync_tail_001056e0, @function
stage3p_frontend_drawsync_tail_001056e0:
    .word 0x27bdffe0
    .word 0x3c05001b
    .word 0xffb00000
    .word 0x24a50f70
    .word 0x3c100043
    .word 0xffbf0010
    .word 0x26109288
    .word 0x0c0678f4
    .word 0x0200202d
    .word 0xdfbf0010
    .word 0x0200102d
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .size stage3p_frontend_drawsync_tail_001056e0, .-stage3p_frontend_drawsync_tail_001056e0

    /* Audio comparison tail @ 0x00105e38..0x00105e48
     * target span SHA-256: c4be49d90bdc76bff518715da5282e61e011284b42877f9a5a872157ece9a29c
     */
    .balign 4
    .globl stage3p_frontend_audio_tail_00105e38
    .type stage3p_frontend_audio_tail_00105e38, @function
stage3p_frontend_audio_tail_00105e38:
    .word 0x8c820000
    .word 0x8ca30000
    .word 0x03e00008
    .word 0x00431023
    .size stage3p_frontend_audio_tail_00105e38, .-stage3p_frontend_audio_tail_00105e38

    /* Fatal-spin leaf plus SjPCM_Puts bridge @ 0x00107578..0x0010768c
     * target span SHA-256: 79a5e12c2ea51cf89b11efa08e4c088030058e746e4a58ee19d7b400f5490a82
     */
    .balign 4
    .globl stage3p_sjpcm_puts_bridge_00107578
    .type stage3p_sjpcm_puts_bridge_00107578, @function
stage3p_sjpcm_puts_bridge_00107578:
    .word 0x1000ffff
    .word 0x00000000
    .word 0x27bdff70
    .word 0x3c02001f
    .word 0xffbf0020
    .word 0xffb00010
    .word 0xffa50058
    .word 0x8c426100
    .word 0xffa60060
    .word 0xffa70068
    .word 0xffa80070
    .word 0xffa90078
    .word 0xffaa0080
    .word 0xffab0088
    .word 0xe7ac0048
    .word 0xe7ae004c
    .word 0xe7b00050
    .word 0x14400005
    .word 0xe7b20054
    .word 0xdfbf0020
    .word 0xdfb00010
    .word 0x03e00008
    .word 0x27bd0090
    .word 0x3c100043
    .word 0x0080302d
    .word 0x2610b400
    .word 0x27a70058
    .word 0x0200202d
    .word 0x0c0678b8
    .word 0x24051000
    .word 0x3c020043
    .word 0x260300e0
    .word 0x2442c400
    .word 0xde040000
    .word 0xde050008
    .word 0xde060010
    .word 0xde070018
    .word 0xfc440000
    .word 0xfc450008
    .word 0xfc460010
    .word 0xfc470018
    .word 0x26100020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1603fff3
    .word 0x24420020
    .word 0xde0c0000
    .word 0xde0d0008
    .word 0xde0e0010
    .word 0x8e030018
    .word 0xfc4c0000
    .word 0xfc4d0008
    .word 0xfc4e0010
    .word 0x3c070043
    .word 0x24e7c400
    .word 0x3c040043
    .word 0xac430018
    .word 0x2484c500
    .word 0x24050001
    .word 0x0000302d
    .word 0x240800fc
    .word 0x00e0482d
    .word 0x240a00fc
    .word 0x0000582d
    .word 0x0c0671ec
    .word 0xafa00000
    .word 0x1000ffd0
    .word 0xdfbf0020
    .size stage3p_sjpcm_puts_bridge_00107578, .-stage3p_sjpcm_puts_bridge_00107578

    /* Amiga module log/copy tail @ 0x00107f84..0x001080cc
     * target span SHA-256: ac7c8b1b548b93e324f22e8735696136a610151adb72041da47da8ae35ca1a61
     */
    .balign 4
    .globl stage3p_amigamod_log_tail_00107f84
    .type stage3p_amigamod_log_tail_00107f84, @function
stage3p_amigamod_log_tail_00107f84:
    .word 0x27bdffe0
    .word 0x3c020020
    .word 0xffbf0010
    .word 0x8c42afc0
    .word 0x1040003a
    .word 0x3c020043
    .word 0x0080182d
    .word 0x244cc580
    .word 0x006c1025
    .word 0x30420007
    .word 0x10400037
    .word 0x248400e0
    .word 0x0080102d
    .word 0x68640007
    .word 0x6c640000
    .word 0x6865000f
    .word 0x6c650008
    .word 0x68660017
    .word 0x6c660010
    .word 0x6867001f
    .word 0x6c670018
    .word 0xb1840007
    .word 0xb5840000
    .word 0xb185000f
    .word 0xb5850008
    .word 0xb1860017
    .word 0xb5860010
    .word 0xb187001f
    .word 0xb5870018
    .word 0x24630020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1462ffeb
    .word 0x258c0020
    .word 0x3c070043
    .word 0x686d0007
    .word 0x6c6d0000
    .word 0x686e000f
    .word 0x6c6e0008
    .word 0x686f0017
    .word 0x6c6f0010
    .word 0x8862001b
    .word 0x98620018
    .word 0xb18d0007
    .word 0xb58d0000
    .word 0xb18e000f
    .word 0xb58e0008
    .word 0xb18f0017
    .word 0xb58f0010
    .word 0xa982001b
    .word 0x24e7c580
    .word 0x3c040043
    .word 0x2484c540
    .word 0x24050900
    .word 0x24060001
    .word 0x240800fc
    .word 0x00e0482d
    .word 0x0000502d
    .word 0x0000582d
    .word 0xb9820018
    .word 0x0c0671ec
    .word 0xafa00000
    .word 0xdfbf0010
    .word 0x03e00008
    .word 0x27bd0020
    .word 0xdc6d0000
    .word 0xdc6e0008
    .word 0xdc6f0010
    .word 0xdc620018
    .word 0xfd8d0000
    .word 0xfd8e0008
    .word 0xfd8f0010
    .word 0xfd820018
    .word 0x24630020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1464fff3
    .word 0x258c0020
    .word 0x1000ffd3
    .word 0x3c070043
    .size stage3p_amigamod_log_tail_00107f84, .-stage3p_amigamod_log_tail_00107f84

    /* C4 state-address leaf cluster @ 0x0010e33c..0x0010e360
     * target span SHA-256: df7b7bd62a6597fac46bbe519e3428f5e43cba2366ef5b3c0e98af2046895c86
     */
    .balign 4
    .globl stage3p_c4_leaf_cluster_0010e33c
    .type stage3p_c4_leaf_cluster_0010e33c, @function
stage3p_c4_leaf_cluster_0010e33c:
    .word 0x3c02001b
    .word 0x03e00008
    .word 0x24421b98
    .word 0x3c02001b
    .word 0x03e00008
    .word 0x24421be0
    .word 0x3c02001b
    .word 0x03e00008
    .word 0x24421c00
    .size stage3p_c4_leaf_cluster_0010e33c, .-stage3p_c4_leaf_cluster_0010e33c

    /* Exact public EE reconstruction @ 0x00170000..0x0017022c
     * target span SHA-256: e5e4e0b1bfa6e2aa52e87ff160eb4e1a6d1e9c77e731d9236e070ca18925359c
     */
    .balign 4
    .globl stage3p_tail_gap_00170000
    .type stage3p_tail_gap_00170000, @function
stage3p_tail_gap_00170000:
    .word 0x90830002
    .word 0x00021200
    .word 0x90850001
    .word 0x00621825
    .word 0x90820000
    .word 0x00052a00
    .word 0x00031c00
    .word 0x00451025
    .word 0x00031c03
    .word 0x00021400
    .word 0x00631818
    .word 0x00021403
    .word 0x00422018
    .word 0x00831021
    .word 0x44826000
    .word 0x00000000
    .word 0x46806320
    .word 0x460c0004
    .word 0x46000032
    .word 0x00000000
    .word 0x45000009
    .word 0x00000000
    .word 0x460000a4
    .word 0x44021000
    .word 0x8e03000c
    .word 0xa0620010
    .word 0x00021202
    .word 0x8e03000c
    .word 0x1000ffa2
    .word 0xa0620011
    .word 0x0c0681a8
    .word 0x00000000
    .word 0x1000fff5
    .word 0x00000000
    .word 0x3c04001c
    .word 0x0c067905
    .word 0x24848080
    .word 0x1000ff9a
    .word 0x3c020035
    .word 0x3c100035
    .word 0x27a80002
    .word 0x2610e2b0
    .word 0x03a0382d
    .word 0x8e02000c
    .word 0x904a0001
    .word 0x90430005
    .word 0x90490003
    .word 0x000a5200
    .word 0x90440004
    .word 0x00031a00
    .word 0x90450002
    .word 0x00094a00
    .word 0x90460000
    .word 0x00832025
    .word 0x00a92825
    .word 0x00042400
    .word 0x00ca3025
    .word 0x00052c00
    .word 0x00063400
    .word 0x00042403
    .word 0x00052c03
    .word 0x0c05bf42
    .word 0x00063403
    .word 0x93a20002
    .word 0x8e03000c
    .word 0xa0620010
    .word 0x8e03000c
    .word 0x83a20003
    .word 0xa0620011
    .word 0x8e03000c
    .word 0x93a20000
    .word 0xa0620012
    .word 0x8e03000c
    .word 0x1000ff74
    .word 0x83a20001
    .word 0x3c020040
    .word 0x1000ff78
    .word 0xa051a6b2
    .word 0x27bdffe0
    .word 0x0080282d
    .word 0xffb00000
    .word 0x0080802d
    .word 0x3c04001c
    .word 0xffbf0010
    .word 0x0c0678e2
    .word 0x248480b0
    .word 0x32020fff
    .word 0x2c420022
    .word 0x14400009
    .word 0x3210ffff
    .word 0x3c020035
    .word 0x8c42e2bc
    .word 0x00501021
    .word 0x90420000
    .word 0xdfbf0010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x3c020040
    .word 0x1000fffa
    .word 0x9042a6b8
    .word 0x27bdffd0
    .word 0xffb10010
    .word 0x30b100ff
    .word 0xffb00000
    .word 0x0080282d
    .word 0x0080802d
    .word 0x0220302d
    .word 0x3c04001c
    .word 0xffbf0020
    .word 0x0c0678e2
    .word 0x248480c0
    .word 0x32020001
    .word 0x10400005
    .word 0x3210ffff
    .word 0x3c020035
    .word 0x8c42e2bc
    .word 0x00501021
    .word 0xa0510000
    .word 0xdfbf0020
    .word 0x24030040
    .word 0xdfb10010
    .word 0x3c020040
    .word 0xdfb00000
    .word 0xa043a6b8
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x03e00008
    .word 0x24020081
    .word 0x30a600ff
    .word 0x27bdfff0
    .word 0x0080282d
    .word 0xffbf0000
    .word 0x3c04001c
    .word 0x0c0678e2
    .word 0x248480d8
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .size stage3p_tail_gap_00170000, .-stage3p_tail_gap_00170000

    /* Exact public EE reconstruction @ 0x001711cc..0x001711e8
     * target span SHA-256: 29018c462e3068a87134effc2fee19403894145af98d5671b0a86def44316a51
     */
    .balign 4
    .globl stage3p_tail_gap_001711cc
    .type stage3p_tail_gap_001711cc, @function
stage3p_tail_gap_001711cc:
    .word 0x27bdfff0
    .word 0xffbf0000
    .word 0x0c05c47a
    .word 0x00000000
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .size stage3p_tail_gap_001711cc, .-stage3p_tail_gap_001711cc

    /* Exact public EE reconstruction @ 0x00171230..0x0017124c
     * target span SHA-256: aba4ce4734ab5e3d07635c44cd8a39dc8acbb012d1a30d0585f46618882d1838
     */
    .balign 4
    .globl stage3p_tail_gap_00171230
    .type stage3p_tail_gap_00171230, @function
stage3p_tail_gap_00171230:
    .word 0x27bdfff0
    .word 0xffbf0000
    .word 0x0c05c493
    .word 0x00000000
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .size stage3p_tail_gap_00171230, .-stage3p_tail_gap_00171230

    /* Exact public EE reconstruction @ 0x00173c90..0x00173dfc
     * target span SHA-256: 3400b9b3cd1881b255efda9a0c29fbccc97c266a61ea5a66ac67291d70db148e
     */
    .balign 4
    .globl stage3p_tail_gap_00173c90
    .type stage3p_tail_gap_00173c90, @function
stage3p_tail_gap_00173c90:
    .word 0x27bdef40
    .word 0xffb01080
    .word 0x27a80800
    .word 0x27b00400
    .word 0x27a51072
    .word 0x03a0302d
    .word 0x0200382d
    .word 0xffbf10b0
    .word 0xffb210a0
    .word 0xffb11090
    .word 0x0c0416ba
    .word 0x27b10c00
    .word 0x0220202d
    .word 0x3c08001c
    .word 0x0200382d
    .word 0x27a51072
    .word 0x250883e0
    .word 0x0c041673
    .word 0x03a0302d
    .word 0x24050001
    .word 0x0c0673f0
    .word 0x0220202d
    .word 0x0440003e
    .word 0x0040902d
    .word 0x27b01000
    .word 0x0240202d
    .word 0x0200282d
    .word 0x0c067556
    .word 0x24060050
    .word 0x10400035
    .word 0x0200202d
    .word 0x0c067aa9
    .word 0x2405000a
    .word 0x54400001
    .word 0xa0400000
    .word 0x27b11050
    .word 0x0200202d
    .word 0x27a51074
    .word 0x0c0438d5
    .word 0x0220302d
    .word 0x10400023
    .word 0x0200202d
    .word 0x0220302d
    .word 0x0c0438cf
    .word 0x27a51074
    .word 0x1040001e
    .word 0x0200202d
    .word 0x27b11060
    .word 0x27a61070
    .word 0x27a51074
    .word 0x27a71071
    .word 0x0c0438d2
    .word 0x0220402d
    .word 0x10400006
    .word 0x0040302d
    .word 0x24040004
    .word 0x0c0415ac
    .word 0x24050007
    .word 0x1000ffde
    .word 0x27b01000
    .word 0x93a21071
    .word 0x0000802d
    .word 0x00c2102a
    .word 0x5040ffd9
    .word 0x27b01000
    .word 0x02301021
    .word 0x8fa61074
    .word 0x90470000
    .word 0x24040001
    .word 0x00d03021
    .word 0x0c045046
    .word 0x0000282d
    .word 0x26100001
    .word 0x93a21071
    .word 0x1000fff4
    .word 0x0202102a
    .word 0x93a71050
    .word 0x24040001
    .word 0x8fa61074
    .word 0x0c045046
    .word 0x0000282d
    .word 0x1000ffc7
    .word 0x27b01000
    .word 0x0c067424
    .word 0x0240202d
    .word 0xdfbf10b0
    .word 0xdfb210a0
    .word 0xdfb11090
    .word 0xdfb01080
    .word 0x03e00008
    .word 0x27bd10c0
    .size stage3p_tail_gap_00173c90, .-stage3p_tail_gap_00173c90

    /* Exact public EE reconstruction @ 0x00177e5c..0x00177e6c
     * target span SHA-256: 343afe66d1d97b4cfa17b2c8a335914af363cb4cec47d4a22f488835c710cb00
     */
    .balign 4
    .globl stage3p_tail_gap_00177e5c
    .type stage3p_tail_gap_00177e5c, @function
stage3p_tail_gap_00177e5c:
    .word 0x3c02003b
    .word 0x2442b718
    .word 0x03e00008
    .word 0xac440004
    .size stage3p_tail_gap_00177e5c, .-stage3p_tail_gap_00177e5c

    /* Exact public EE reconstruction @ 0x0017ec4c..0x001806a4
     * target span SHA-256: 1ea36dc28a541d23599155bde99bf8c67a1766230d97ba6aff8e6f1288789184
     */
    .balign 4
    .globl stage3p_tail_gap_0017ec4c
    .type stage3p_tail_gap_0017ec4c, @function
stage3p_tail_gap_0017ec4c:
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x24505498
    .word 0xffbf0010
    .word 0x8c435498
    .word 0x0c06b265
    .word 0x90640001
    .word 0x3c080036
    .word 0x3c030034
    .word 0x92050028
    .word 0x906354da
    .word 0x00622023
    .word 0x00623026
    .word 0x00a42821
    .word 0x0060382d
    .word 0x24a5ffff
    .word 0x30c60080
    .word 0x00052027
    .word 0x3c030036
    .word 0x000427c2
    .word 0xa062b750
    .word 0xa505b75a
    .word 0x10c00023
    .word 0xa2040028
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x1040001f
    .word 0x3c020034
    .word 0x24020001
    .word 0xa202002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xdfbf0010
    .word 0xdfb00000
    .word 0x24420002
    .word 0xa1050002
    .word 0xace25498
    .word 0xa1250029
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x3c020034
    .word 0x1000ffe3
    .word 0xa04054c2
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x24505498
    .word 0xffbf0010
    .word 0x8c435498
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x00621821
    .word 0x0060202d
    .word 0x0c06b2cf
    .word 0xae030010
    .word 0x3c080036
    .word 0x3c030034
    .word 0x92050028
    .word 0x906354da
    .word 0x00622023
    .word 0x00623026
    .word 0x00a42821
    .word 0x0060382d
    .word 0x24a5ffff
    .word 0x30c60080
    .word 0x00052027
    .word 0x3c030036
    .word 0x000427c2
    .word 0xa062b750
    .word 0xa505b75a
    .word 0x10c00023
    .word 0xa2040028
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x1040001f
    .word 0x3c020034
    .word 0x24020001
    .word 0xa202002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xdfbf0010
    .word 0xdfb00000
    .word 0x24420003
    .word 0xa1050002
    .word 0xace25498
    .word 0xa1250029
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x3c020034
    .word 0x1000ffe3
    .word 0xa04054c2
    .word 0x27bdffe0
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x261054d8
    .word 0x0c06b265
    .word 0x92040004
    .word 0x3c080036
    .word 0x3c030034
    .word 0x92040002
    .word 0x24695498
    .word 0x91250028
    .word 0x00821823
    .word 0x00823026
    .word 0x0080382d
    .word 0x00a32821
    .word 0x30c60080
    .word 0x24a5ffff
    .word 0x3c030036
    .word 0x00052027
    .word 0xa062b750
    .word 0x000427c2
    .word 0xa505b75a
    .word 0x10c00023
    .word 0xa1240028
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x1040001f
    .word 0x3c020034
    .word 0x24020001
    .word 0xa122002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xdfbf0010
    .word 0xdfb00000
    .word 0x24420001
    .word 0xa1050002
    .word 0xace25498
    .word 0xa1250029
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x3c020034
    .word 0x1000ffe3
    .word 0xa04054c2
    .word 0x27bdffd0
    .word 0x3c020034
    .word 0xffb10010
    .word 0x24515498
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0x261054d8
    .word 0x8c445498
    .word 0x92030004
    .word 0x90820001
    .word 0x8e240008
    .word 0x00431021
    .word 0x24430001
    .word 0x304200ff
    .word 0x306300ff
    .word 0x00821021
    .word 0x00832021
    .word 0x90430000
    .word 0x90820000
    .word 0x00021200
    .word 0x00621821
    .word 0x0060202d
    .word 0x0c06b2cf
    .word 0xae230010
    .word 0x3c080036
    .word 0x92040002
    .word 0x92260028
    .word 0x00821823
    .word 0x00822826
    .word 0x00c33021
    .word 0x0080382d
    .word 0x24c6ffff
    .word 0x30a50080
    .word 0x00061827
    .word 0x3c040036
    .word 0x00031fc2
    .word 0xa082b750
    .word 0xa2230028
    .word 0x10a00024
    .word 0xa506b75a
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x10400020
    .word 0x3c020034
    .word 0x24020001
    .word 0xa222002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xdfbf0020
    .word 0xdfb10010
    .word 0x24420002
    .word 0xdfb00000
    .word 0xa1050002
    .word 0xace25498
    .word 0xa1250029
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x3c020034
    .word 0x1000ffe2
    .word 0xa04054c2
    .word 0x3c020034
    .word 0x3c090036
    .word 0x8c435498
    .word 0x24485498
    .word 0x3c020034
    .word 0x91040028
    .word 0x90650001
    .word 0x904254da
    .word 0x0040382d
    .word 0x00451023
    .word 0x00822021
    .word 0x00e53026
    .word 0x2484ffff
    .word 0x3c020036
    .word 0x00041827
    .word 0x30c60080
    .word 0x00031fc2
    .word 0xa045b750
    .word 0xa524b75a
    .word 0x10c00020
    .word 0xa1030028
    .word 0x9122b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x1040001c
    .word 0x3c020034
    .word 0x24020001
    .word 0xa102002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xa1050002
    .word 0x24420002
    .word 0xa1250029
    .word 0x03e00008
    .word 0xace25498
    .word 0x3c020034
    .word 0x1000ffe6
    .word 0xa04054c2
    .word 0x27bdffc0
    .word 0xffbf0030
    .word 0xffb20020
    .word 0xffb10010
    .word 0x3c110036
    .word 0xffb00000
    .word 0x3c100034
    .word 0x26125498
    .word 0x8e025498
    .word 0x0c06b265
    .word 0x90440001
    .word 0x8e035498
    .word 0xa222b750
    .word 0x0c06b265
    .word 0x90640002
    .word 0x3c040036
    .word 0x9226b750
    .word 0x304700ff
    .word 0x92450028
    .word 0x3c080036
    .word 0x00461823
    .word 0xa082b743
    .word 0x00a32821
    .word 0x00e63026
    .word 0x24a5ffff
    .word 0x30c60080
    .word 0x00051827
    .word 0xa505b75a
    .word 0x00031fc2
    .word 0x10c0002a
    .word 0xa2430028
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x10400026
    .word 0x3c020034
    .word 0x24020001
    .word 0xa242002a
    .word 0x3c020036
    .word 0x3c030036
    .word 0x3c080036
    .word 0x9044b750
    .word 0x9066b75a
    .word 0x3c070034
    .word 0x9102b743
    .word 0x3c100034
    .word 0x30c300ff
    .word 0x90e554d8
    .word 0x00441026
    .word 0x26095498
    .word 0x00431026
    .word 0x34a50008
    .word 0x30420010
    .word 0x0060202d
    .word 0xa0e554d8
    .word 0x10400002
    .word 0x30a300f7
    .word 0xa0e354d8
    .word 0x8e025498
    .word 0xa106b743
    .word 0xa1260029
    .word 0x0c06b294
    .word 0x90450002
    .word 0xdfb10010
    .word 0x8e025498
    .word 0xdfbf0030
    .word 0x24420003
    .word 0xdfb20020
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x3c020034
    .word 0x1000ffdc
    .word 0xa04054c2
    .word 0x27bdffd0
    .word 0x3c030034
    .word 0xffb10010
    .word 0x24715498
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0x261054d8
    .word 0x8c645498
    .word 0x92020004
    .word 0x90840001
    .word 0x00822021
    .word 0x0c06b265
    .word 0x308400ff
    .word 0x3c080036
    .word 0x92040002
    .word 0x92260028
    .word 0x00821823
    .word 0x00822826
    .word 0x00c33021
    .word 0x0080382d
    .word 0x24c6ffff
    .word 0x30a50080
    .word 0x00061827
    .word 0x3c040036
    .word 0x00031fc2
    .word 0xa082b750
    .word 0xa2230028
    .word 0x10a00024
    .word 0xa506b75a
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x10400020
    .word 0x3c020034
    .word 0x24020001
    .word 0xa222002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xdfbf0020
    .word 0xdfb10010
    .word 0x24420002
    .word 0xdfb00000
    .word 0xa1050002
    .word 0xace25498
    .word 0xa1250029
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x3c020034
    .word 0x1000ffe2
    .word 0xa04054c2
    .word 0x27bdffd0
    .word 0x3c020034
    .word 0xffb10010
    .word 0x24515498
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0x261054d8
    .word 0x8c445498
    .word 0x92050004
    .word 0x90830002
    .word 0x90820001
    .word 0x00031a00
    .word 0x00431021
    .word 0x00451021
    .word 0x0040202d
    .word 0x0c06b2cf
    .word 0xae220010
    .word 0x3c080036
    .word 0x92040002
    .word 0x92260028
    .word 0x00821823
    .word 0x00822826
    .word 0x00c33021
    .word 0x0080382d
    .word 0x24c6ffff
    .word 0x30a50080
    .word 0x00061827
    .word 0x3c040036
    .word 0x00031fc2
    .word 0xa082b750
    .word 0xa2230028
    .word 0x10a00024
    .word 0xa506b75a
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x10400020
    .word 0x3c020034
    .word 0x24020001
    .word 0xa222002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xdfbf0020
    .word 0xdfb10010
    .word 0x24420003
    .word 0xdfb00000
    .word 0xa1050002
    .word 0xace25498
    .word 0xa1250029
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x3c020034
    .word 0x1000ffe2
    .word 0xa04054c2
    .word 0x27bdffd0
    .word 0x3c020034
    .word 0xffb10010
    .word 0x24515498
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0x261054d8
    .word 0x8c445498
    .word 0x92050003
    .word 0x90830002
    .word 0x90820001
    .word 0x00031a00
    .word 0x00431021
    .word 0x00451021
    .word 0x0040202d
    .word 0x0c06b2cf
    .word 0xae220010
    .word 0x3c080036
    .word 0x92040002
    .word 0x92260028
    .word 0x00821823
    .word 0x00822826
    .word 0x00c33021
    .word 0x0080382d
    .word 0x24c6ffff
    .word 0x30a50080
    .word 0x00061827
    .word 0x3c040036
    .word 0x00031fc2
    .word 0xa082b750
    .word 0xa2230028
    .word 0x10a00024
    .word 0xa506b75a
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x10400020
    .word 0x3c020034
    .word 0x24020001
    .word 0xa222002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xdfbf0020
    .word 0xdfb10010
    .word 0x24420003
    .word 0xdfb00000
    .word 0xa1050002
    .word 0xace25498
    .word 0xa1250029
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x3c020034
    .word 0x1000ffe2
    .word 0xa04054c2
    .word 0x27bdffd0
    .word 0x3c020034
    .word 0xffb10010
    .word 0x24515498
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0x261054d8
    .word 0x8c435498
    .word 0x8e220008
    .word 0x90630001
    .word 0x92040003
    .word 0x00431021
    .word 0x90430001
    .word 0x90420000
    .word 0x00031a00
    .word 0x00431021
    .word 0x00441021
    .word 0x0040202d
    .word 0x0c06b2cf
    .word 0xae220010
    .word 0x3c080036
    .word 0x92040002
    .word 0x92260028
    .word 0x00821823
    .word 0x00822826
    .word 0x00c33021
    .word 0x0080382d
    .word 0x24c6ffff
    .word 0x30a50080
    .word 0x00061827
    .word 0x3c040036
    .word 0x00031fc2
    .word 0xa082b750
    .word 0xa2230028
    .word 0x10a00024
    .word 0xa506b75a
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x10400020
    .word 0x3c020034
    .word 0x24020001
    .word 0xa222002a
    .word 0x3c060034
    .word 0x3c020036
    .word 0x24c854d8
    .word 0x9044b750
    .word 0x3c030036
    .word 0x91020002
    .word 0x9065b75a
    .word 0x3c070034
    .word 0x00441026
    .word 0x90c354d8
    .word 0x00451026
    .word 0x24e95498
    .word 0x34630008
    .word 0x30420010
    .word 0x306400f7
    .word 0x10400002
    .word 0xa0c354d8
    .word 0xa0c454d8
    .word 0x8ce25498
    .word 0xdfbf0020
    .word 0xdfb10010
    .word 0x24420002
    .word 0xdfb00000
    .word 0xa1050002
    .word 0xace25498
    .word 0xa1250029
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x3c020034
    .word 0x1000ffe2
    .word 0xa04054c2
    .word 0x27bdffd0
    .word 0x3c020034
    .word 0xffb10010
    .word 0x24515498
    .word 0xffb00000
    .word 0x3c100036
    .word 0xffbf0020
    .word 0x8c435498
    .word 0x90620001
    .word 0xa202b750
    .word 0x0c06b265
    .word 0x90640002
    .word 0x3c040036
    .word 0x9206b750
    .word 0x304700ff
    .word 0x92250028
    .word 0x3c080036
    .word 0x00461823
    .word 0xa082b743
    .word 0x00a32821
    .word 0x00e63026
    .word 0x24a5ffff
    .word 0x30c60080
    .word 0x00051827
    .word 0xa505b75a
    .word 0x00031fc2
    .word 0x10c00029
    .word 0xa2230028
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x10400025
    .word 0x3c020034
    .word 0x24020001
    .word 0xa222002a
    .word 0x3c020036
    .word 0x3c030036
    .word 0x3c080036
    .word 0x9044b750
    .word 0x9066b75a
    .word 0x3c070034
    .word 0x9102b743
    .word 0x3c100034
    .word 0x30c300ff
    .word 0x90e554d8
    .word 0x00441026
    .word 0x26095498
    .word 0x00431026
    .word 0x34a50008
    .word 0x30420010
    .word 0x0060202d
    .word 0xa0e554d8
    .word 0x10400002
    .word 0x30a300f7
    .word 0xa0e354d8
    .word 0x8e025498
    .word 0xa106b743
    .word 0xa1260029
    .word 0x0c06b294
    .word 0x90450002
    .word 0xdfb10010
    .word 0x8e025498
    .word 0xdfbf0020
    .word 0x24420003
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x3c020034
    .word 0x1000ffdd
    .word 0xa04054c2
    .word 0x27bdffd0
    .word 0xffbf0020
    .word 0xffb10010
    .word 0x3c110036
    .word 0xffb00000
    .word 0x3c100034
    .word 0x261054d8
    .word 0x0c06b265
    .word 0x92040004
    .word 0x92040003
    .word 0x0c06b265
    .word 0xa222b743
    .word 0x3c080036
    .word 0x3c030034
    .word 0x9224b743
    .word 0x24695498
    .word 0x91250028
    .word 0x00821823
    .word 0x00823026
    .word 0x0080382d
    .word 0x00a32821
    .word 0x30c60080
    .word 0x24a5ffff
    .word 0x3c030036
    .word 0x00052027
    .word 0xa062b750
    .word 0x000427c2
    .word 0xa505b75a
    .word 0x10c00029
    .word 0xa1240028
    .word 0x9102b75a
    .word 0x00e21026
    .word 0x30420080
    .word 0x50400025
    .word 0x3c020034
    .word 0x24020001
    .word 0xa122002a
    .word 0x3c020036
    .word 0x3c080036
    .word 0x9044b750
    .word 0x3c070034
    .word 0x3c020036
    .word 0x9103b743
    .word 0x9046b75a
    .word 0x3c100034
    .word 0x00641826
    .word 0x90e554d8
    .word 0x30c200ff
    .word 0x260a5498
    .word 0x00621826
    .word 0x34a50008
    .word 0x30630010
    .word 0x0040202d
    .word 0x24e954d8
    .word 0xa0e554d8
    .word 0x10600002
    .word 0x30a200f7
    .word 0xa0e254d8
    .word 0x91250004
    .word 0xa1460029
    .word 0x0c06b294
    .word 0xa106b743
    .word 0xdfb10010
    .word 0x8e025498
    .word 0xdfbf0020
    .word 0x24420001
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x3c020034
    .word 0x1000ffdd
    .word 0xa04054c2
    .word 0x27bdfff0
    .word 0x3c030034
    .word 0xffbf0000
    .word 0x246354d8
    .word 0x90620004
    .word 0x90640002
    .word 0x0040282d
    .word 0x24420001
    .word 0x0c06b294
    .word 0xa0620004
    .word 0xdfbf0000
    .word 0x3c030034
    .word 0x8c625498
    .word 0x24420001
    .word 0xac625498
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x3c040034
    .word 0x3c020034
    .word 0x248554d8
    .word 0x24465498
    .word 0x90a30002
    .word 0x00a0382d
    .word 0x3062000f
    .word 0x2c42000a
    .word 0x10400005
    .word 0x2463fffa
    .word 0x908254d8
    .word 0x30420008
    .word 0x54400003
    .word 0x90e20002
    .word 0xa0a30002
    .word 0x90e20002
    .word 0x2443ffa0
    .word 0x2c4200a0
    .word 0x10400005
    .word 0x3c020034
    .word 0x90c20028
    .word 0x5440000d
    .word 0x24020001
    .word 0x3c020034
    .word 0xa0e30002
    .word 0xa04054c0
    .word 0x3c040034
    .word 0x3c020034
    .word 0x8c835498
    .word 0x24855498
    .word 0x904254da
    .word 0x24630001
    .word 0xa0a20029
    .word 0x03e00008
    .word 0xac835498
    .word 0x1000fff6
    .word 0xa0c20028
    .word 0x27bdffe0
    .word 0xffbf0010
    .word 0xffb00000
    .word 0x3c100034
    .word 0x261054d8
    .word 0x92020004
    .word 0x0040202d
    .word 0x24420001
    .word 0x0c06b265
    .word 0xa2020004
    .word 0xdfbf0010
    .word 0x3c050034
    .word 0xa2020002
    .word 0x8ca45498
    .word 0x24a35498
    .word 0xdfb00000
    .word 0x24840001
    .word 0xa0620029
    .word 0xaca45498
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x3c040034
    .word 0x3c050034
    .word 0x908254d8
    .word 0x8ca35498
    .word 0x304200fb
    .word 0x24630001
    .word 0xa08254d8
    .word 0x03e00008
    .word 0xaca35498
    .word 0x3c040034
    .word 0x3c050034
    .word 0x908254d8
    .word 0x8ca35498
    .word 0x34420004
    .word 0x24630001
    .word 0xa08254d8
    .word 0x03e00008
    .word 0xaca35498
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e035498
    .word 0x904454da
    .word 0x0c06b294
    .word 0x90650001
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420002
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x26065498
    .word 0x8e035498
    .word 0x904454da
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x00621821
    .word 0xacc30010
    .word 0x0c06b2fc
    .word 0x0060282d
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420003
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdfff0
    .word 0x3c020034
    .word 0xffbf0000
    .word 0x244254d8
    .word 0x90450004
    .word 0x0c06b294
    .word 0x90440002
    .word 0xdfbf0000
    .word 0x3c030034
    .word 0x8c625498
    .word 0x24420001
    .word 0xac625498
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x27bdffe0
    .word 0x3c030034
    .word 0xffb00000
    .word 0x246354d8
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e025498
    .word 0x26065498
    .word 0x90640004
    .word 0x90420001
    .word 0x8cc50008
    .word 0x00441021
    .word 0x90640002
    .word 0x24430001
    .word 0x304200ff
    .word 0x306300ff
    .word 0x00a21021
    .word 0x00a32821
    .word 0x90430000
    .word 0x90a20000
    .word 0x00021200
    .word 0x00621821
    .word 0xacc30010
    .word 0x0c06b2fc
    .word 0x0060282d
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420002
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x26065498
    .word 0x8e035498
    .word 0x904454dc
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x00621821
    .word 0xacc30010
    .word 0x0c06b2fc
    .word 0x0060282d
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420003
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e035498
    .word 0x904454db
    .word 0x0c06b294
    .word 0x90650001
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420002
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x26065498
    .word 0x8e035498
    .word 0x904454db
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x00621821
    .word 0xacc30010
    .word 0x0c06b2fc
    .word 0x0060282d
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420003
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x3c030034
    .word 0x8c625498
    .word 0x24655498
    .word 0x90440001
    .word 0x24420002
    .word 0xac625498
    .word 0x3c020034
    .word 0xa0a40029
    .word 0x03e00008
    .word 0xa04454dc
    .word 0x3c040034
    .word 0x3c050034
    .word 0x248454d8
    .word 0x8ca25498
    .word 0x90830002
    .word 0x24a75498
    .word 0x90860003
    .word 0x24420001
    .word 0xaca25498
    .word 0x00661818
    .word 0x0060102d
    .word 0xa4830002
    .word 0x00021202
    .word 0x0003182b
    .word 0x00431025
    .word 0x03e00008
    .word 0xa0e20029
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x244254d8
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e045498
    .word 0x90430004
    .word 0x90850001
    .word 0x90440002
    .word 0x00a32821
    .word 0x0c06b294
    .word 0x30a500ff
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420002
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c040034
    .word 0xffb00000
    .word 0x248454d8
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e035498
    .word 0x26065498
    .word 0x90850004
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x90840002
    .word 0x00621821
    .word 0x00651821
    .word 0xacc30010
    .word 0x0c06b2fc
    .word 0x0060282d
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420003
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c040034
    .word 0xffb00000
    .word 0x248454d8
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e035498
    .word 0x26065498
    .word 0x90850003
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x90840002
    .word 0x00621821
    .word 0x00651821
    .word 0xacc30010
    .word 0x0c06b2fc
    .word 0x0060282d
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420003
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x244254d8
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e035498
    .word 0x26075498
    .word 0x8ce50008
    .word 0x90630001
    .word 0x90460003
    .word 0x00a32821
    .word 0x90440002
    .word 0x90a30001
    .word 0x90a20000
    .word 0x00031a00
    .word 0x00431021
    .word 0x00461021
    .word 0xace20010
    .word 0x0c06b2fc
    .word 0x0040282d
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420002
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e035498
    .word 0x904454dc
    .word 0x0c06b294
    .word 0x90650001
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420002
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x244254d8
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e045498
    .word 0x90430003
    .word 0x90850001
    .word 0x90440004
    .word 0x00a32821
    .word 0x0c06b294
    .word 0x30a500ff
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420002
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffe0
    .word 0x3c020034
    .word 0xffb00000
    .word 0x244254d8
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e045498
    .word 0x90430004
    .word 0x90850001
    .word 0x90440003
    .word 0x00a32821
    .word 0x0c06b294
    .word 0x30a500ff
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420002
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x3c050034
    .word 0x3c020034
    .word 0x24a654d8
    .word 0x24475498
    .word 0x90c30002
    .word 0x00c0402d
    .word 0x3062000f
    .word 0x24640006
    .word 0x2c42000a
    .word 0x10400005
    .word 0x2c6300f1
    .word 0x90a254d8
    .word 0x30420008
    .word 0x50400008
    .word 0x91020002
    .word 0x54600005
    .word 0xa0c40002
    .word 0x24030001
    .word 0x3c020034
    .word 0xa04354c0
    .word 0xa0c40002
    .word 0x91020002
    .word 0x24430060
    .word 0x2c4200a0
    .word 0x50400005
    .word 0xa1030002
    .word 0x90e20028
    .word 0x50400005
    .word 0xa0e00028
    .word 0xa1030002
    .word 0x3c020034
    .word 0x24030001
    .word 0xa04354c0
    .word 0x3c040034
    .word 0x3c020034
    .word 0x8c835498
    .word 0x24855498
    .word 0x904254da
    .word 0x24630001
    .word 0xa0a20029
    .word 0x03e00008
    .word 0xac835498
    .word 0x27bdffd0
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0xffb10010
    .word 0x26115498
    .word 0x8e025498
    .word 0x0c06b265
    .word 0x90440001
    .word 0xdfbf0020
    .word 0x8e045498
    .word 0x3c030034
    .word 0xa2220029
    .word 0x246354d8
    .word 0x24840002
    .word 0xdfb10010
    .word 0xae045498
    .word 0xdfb00000
    .word 0xa0620002
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x27bdffd0
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0xffb10010
    .word 0x26115498
    .word 0x8e035498
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x00621821
    .word 0x0060202d
    .word 0x0c06b2cf
    .word 0xae230010
    .word 0xdfbf0020
    .word 0x8e045498
    .word 0x3c030034
    .word 0xa2220029
    .word 0x246354d8
    .word 0x24840003
    .word 0xdfb10010
    .word 0xae045498
    .word 0xdfb00000
    .word 0xa0620002
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x27bdffe0
    .word 0xffbf0010
    .word 0xffb00000
    .word 0x3c100034
    .word 0x261054d8
    .word 0x0c06b265
    .word 0x92040004
    .word 0xdfbf0010
    .word 0x3c050034
    .word 0xa2020002
    .word 0x8ca45498
    .word 0x24a35498
    .word 0xdfb00000
    .word 0x24840001
    .word 0xa0620029
    .word 0xaca45498
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffc0
    .word 0xffb20020
    .word 0x3c120034
    .word 0xffbf0030
    .word 0xffb10010
    .word 0x3c110034
    .word 0xffb00000
    .word 0x263154d8
    .word 0x8e425498
    .word 0x26505498
    .word 0x92230004
    .word 0x90420001
    .word 0x8e040008
    .word 0x00431021
    .word 0x24430001
    .word 0x304200ff
    .word 0x306300ff
    .word 0x00821021
    .word 0x00832021
    .word 0x90430000
    .word 0x90820000
    .word 0x00021200
    .word 0x00621821
    .word 0x0060202d
    .word 0x0c06b2cf
    .word 0xae030010
    .word 0xdfbf0030
    .word 0x8e435498
    .word 0xa2020029
    .word 0x24630002
    .word 0xa2220002
    .word 0xae435498
    .word 0xdfb10010
    .word 0xdfb20020
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x3c030034
    .word 0x8c625498
    .word 0x24655498
    .word 0x90440001
    .word 0x24420002
    .word 0xac625498
    .word 0x3c020034
    .word 0xa0a40029
    .word 0x03e00008
    .word 0xa04454da
    .word 0x27bdffd0
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0xffb10010
    .word 0x26115498
    .word 0x8e035498
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x00621821
    .word 0x0060202d
    .word 0x0c06b2cf
    .word 0xae230010
    .word 0xdfbf0020
    .word 0x8e045498
    .word 0x3c030034
    .word 0xa2220029
    .word 0x246354d8
    .word 0x24840003
    .word 0xdfb10010
    .word 0xae045498
    .word 0xdfb00000
    .word 0xa0620004
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x27bdffd0
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0xffb10010
    .word 0x26115498
    .word 0x8e025498
    .word 0x0c06b265
    .word 0x90440001
    .word 0xdfbf0020
    .word 0x8e045498
    .word 0x3c030034
    .word 0xa2220029
    .word 0x246354d8
    .word 0x24840002
    .word 0xdfb10010
    .word 0xae045498
    .word 0xdfb00000
    .word 0xa0620003
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x27bdffd0
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0xffb10010
    .word 0x26115498
    .word 0x8e035498
    .word 0x90620002
    .word 0x90630001
    .word 0x00021200
    .word 0x00621821
    .word 0x0060202d
    .word 0x0c06b2cf
    .word 0xae230010
    .word 0xdfbf0020
    .word 0x8e045498
    .word 0x3c030034
    .word 0xa2220029
    .word 0x246354d8
    .word 0x24840003
    .word 0xdfb10010
    .word 0xae045498
    .word 0xdfb00000
    .word 0xa0620003
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x27bdffc0
    .word 0xffb10010
    .word 0x3c110034
    .word 0xffbf0030
    .word 0xffb20020
    .word 0x26325498
    .word 0xffb00000
    .word 0x3c100034
    .word 0x8e235498
    .word 0x261054d8
    .word 0x92020004
    .word 0x90640001
    .word 0x00822021
    .word 0x0c06b265
    .word 0x308400ff
    .word 0xdfbf0030
    .word 0x8e235498
    .word 0xa2420029
    .word 0x24630002
    .word 0xa2020002
    .word 0xae235498
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x27bdffc0
    .word 0xffb10010
    .word 0x3c110034
    .word 0xffbf0030
    .word 0xffb20020
    .word 0x26325498
    .word 0xffb00000
    .word 0x3c100034
    .word 0x8e225498
    .word 0x261054d8
    .word 0x92040004
    .word 0x90430002
    .word 0x90420001
    .word 0x00031a00
    .word 0x00431021
    .word 0x00441021
    .word 0x0040202d
    .word 0x0c06b2cf
    .word 0xae420010
    .word 0xdfbf0030
    .word 0x8e235498
    .word 0xa2420029
    .word 0x24630003
    .word 0xa2020002
    .word 0xae235498
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x27bdffc0
    .word 0xffb10010
    .word 0x3c110034
    .word 0xffbf0030
    .word 0xffb20020
    .word 0x26325498
    .word 0xffb00000
    .word 0x3c100034
    .word 0x8e225498
    .word 0x261054d8
    .word 0x92040003
    .word 0x90430002
    .word 0x90420001
    .word 0x00031a00
    .word 0x00431021
    .word 0x00441021
    .word 0x0040202d
    .word 0x0c06b2cf
    .word 0xae420010
    .word 0xdfbf0030
    .word 0x8e235498
    .word 0xa2420029
    .word 0x24630003
    .word 0xa2020002
    .word 0xae235498
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x27bdffc0
    .word 0xffb20020
    .word 0x3c120034
    .word 0xffbf0030
    .word 0xffb10010
    .word 0x26515498
    .word 0xffb00000
    .word 0x3c100034
    .word 0x8e435498
    .word 0x261054d8
    .word 0x8e220008
    .word 0x90630001
    .word 0x92040003
    .word 0x00431021
    .word 0x90430001
    .word 0x90420000
    .word 0x00031a00
    .word 0x00431021
    .word 0x00441021
    .word 0x0040202d
    .word 0x0c06b2cf
    .word 0xae220010
    .word 0xdfbf0030
    .word 0x8e435498
    .word 0xa2220029
    .word 0x24630002
    .word 0xa2020002
    .word 0xae435498
    .word 0xdfb10010
    .word 0xdfb20020
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x27bdffd0
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0020
    .word 0xffb10010
    .word 0x26115498
    .word 0x8e025498
    .word 0x0c06b265
    .word 0x90440001
    .word 0xdfbf0020
    .word 0x8e045498
    .word 0x3c030034
    .word 0xa2220029
    .word 0x246354d8
    .word 0x24840002
    .word 0xdfb10010
    .word 0xae045498
    .word 0xdfb00000
    .word 0xa0620004
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x27bdffc0
    .word 0xffb10010
    .word 0x3c110034
    .word 0xffbf0030
    .word 0xffb20020
    .word 0x26325498
    .word 0xffb00000
    .word 0x3c100034
    .word 0x8e235498
    .word 0x261054d8
    .word 0x92020003
    .word 0x90640001
    .word 0x00822021
    .word 0x0c06b265
    .word 0x308400ff
    .word 0xdfbf0030
    .word 0x8e235498
    .word 0xa2420029
    .word 0x24630002
    .word 0xa2020004
    .word 0xae235498
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x27bdffe0
    .word 0xffb00000
    .word 0x3c100034
    .word 0xffbf0010
    .word 0x8e025498
    .word 0x0c06b265
    .word 0x90440001
    .word 0x8e035498
    .word 0x0040202d
    .word 0x0c06b294
    .word 0x90650002
    .word 0xdfbf0010
    .word 0x8e025498
    .word 0x24420003
    .word 0xae025498
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffc0
    .word 0xffb10010
    .word 0x3c110034
    .word 0xffbf0030
    .word 0xffb20020
    .word 0x26325498
    .word 0xffb00000
    .word 0x3c100034
    .word 0x8e235498
    .word 0x261054d8
    .word 0x92020004
    .word 0x90640001
    .word 0x00822021
    .word 0x0c06b265
    .word 0x308400ff
    .word 0xdfbf0030
    .word 0x8e235498
    .word 0xa2420029
    .word 0x24630002
    .word 0xa2020003
    .word 0xae235498
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .size stage3p_tail_gap_0017ec4c, .-stage3p_tail_gap_0017ec4c

    /* Exact public EE reconstruction @ 0x0018c1a8..0x0018c1b4
     * target span SHA-256: 15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b
     */
    .balign 4
    .globl stage3p_tail_gap_0018c1a8
    .type stage3p_tail_gap_0018c1a8, @function
stage3p_tail_gap_0018c1a8:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0018c1a8, .-stage3p_tail_gap_0018c1a8

    /* Exact public EE reconstruction @ 0x0018e130..0x0018e138
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_0018e130
    .type stage3p_tail_gap_0018e130, @function
stage3p_tail_gap_0018e130:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0018e130, .-stage3p_tail_gap_0018e130

    /* Exact public EE reconstruction @ 0x0018e164..0x0018e174
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_0018e164
    .type stage3p_tail_gap_0018e164, @function
stage3p_tail_gap_0018e164:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0018e164, .-stage3p_tail_gap_0018e164

    /* Exact public EE reconstruction @ 0x0019272c..0x00193298
     * target span SHA-256: ed4a9ba999458dae487376534cce7842cf0ec1dfeaab7fdc34bb71f71db8b83d
     */
    .balign 4
    .globl stage3p_tail_gap_0019272c
    .type stage3p_tail_gap_0019272c, @function
stage3p_tail_gap_0019272c:
    .word 0x27bdfff0
    .word 0x2402fffe
    .word 0x10800006
    .word 0xffbf0000
    .word 0x8c830024
    .word 0x24070007
    .word 0x0080282d
    .word 0x14600004
    .word 0x0000302d
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x8c620018
    .word 0xfc800018
    .word 0x0002380a
    .word 0xfc800008
    .word 0xac800020
    .word 0xac670000
    .word 0x0c06518a
    .word 0x8c640020
    .word 0x1000fff4
    .word 0x0000102d
    .word 0x27bdffe0
    .word 0xffb00000
    .word 0x0080802d
    .word 0xffbf0010
    .word 0x12000011
    .word 0x2404fffe
    .word 0x8e030024
    .word 0x1060000f
    .word 0xdfbf0010
    .word 0x8e02002c
    .word 0x1040000d
    .word 0x0080102d
    .word 0x8c620020
    .word 0x0200282d
    .word 0x1440000c
    .word 0x0040202d
    .word 0x8e040030
    .word 0x8e02002c
    .word 0x0040f809
    .word 0x8e050024
    .word 0xae000024
    .word 0x0000202d
    .word 0xdfbf0010
    .word 0x0080102d
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x0c0654a0
    .word 0x00000000
    .word 0x1000fff3
    .word 0x8e040030
    .word 0x27bdffc0
    .word 0xffb20020
    .word 0x00a0902d
    .word 0xffb00000
    .word 0x0080802d
    .word 0xffbf0030
    .word 0x10c00005
    .word 0xffb10010
    .word 0x80c30000
    .word 0x24020031
    .word 0x10620008
    .word 0x24020048
    .word 0x2402fffa
    .word 0xdfbf0030
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x14e2fff9
    .word 0x2402fffa
    .word 0x1080fff7
    .word 0x2402fffe
    .word 0x8c820028
    .word 0x14400005
    .word 0xac800020
    .word 0x3c02001a
    .word 0xac800030
    .word 0x24428a84
    .word 0xac820028
    .word 0x8e02002c
    .word 0x54400005
    .word 0x8e020028
    .word 0x3c02001a
    .word 0x24428aa4
    .word 0xae02002c
    .word 0x8e020028
    .word 0x24050001
    .word 0x8e040030
    .word 0x0040f809
    .word 0x24060028
    .word 0x0040182d
    .word 0x2402fffc
    .word 0x1060ffe1
    .word 0xae030024
    .word 0xac600020
    .word 0x0640001f
    .word 0xac600018
    .word 0x2642fff8
    .word 0x2c420008
    .word 0x10400017
    .word 0x00000000
    .word 0x8e110024
    .word 0x8e220018
    .word 0x14400011
    .word 0xae32001c
    .word 0x3c02001a
    .word 0x24458ac0
    .word 0x24060001
    .word 0x0200202d
    .word 0x0c0651ba
    .word 0x02463004
    .word 0x14400005
    .word 0xae220020
    .word 0x0c0649e1
    .word 0x0200202d
    .word 0x1000ffca
    .word 0x2402fffc
    .word 0x0c0649cb
    .word 0x0200202d
    .word 0x1000ffc6
    .word 0x0000102d
    .word 0x1000fff1
    .word 0x0000282d
    .word 0x0c0649e1
    .word 0x0200202d
    .word 0x1000ffc0
    .word 0x2402fffe
    .word 0x24020001
    .word 0x00129023
    .word 0x1000ffdf
    .word 0xac620018
    .word 0x27bdfff0
    .word 0x00c0382d
    .word 0x00a0302d
    .word 0xffbf0000
    .word 0x0c064a00
    .word 0x2405000f
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x27bdffc0
    .word 0xffb00000
    .word 0x0080802d
    .word 0xffbf0030
    .word 0xffb20020
    .word 0x1080000b
    .word 0xffb10010
    .word 0x8c820024
    .word 0x10400009
    .word 0x2406fffe
    .word 0x2402fffb
    .word 0x8c840000
    .word 0x38a30004
    .word 0x0040902d
    .word 0x2411fffb
    .word 0x14800009
    .word 0x0003900b
    .word 0x2406fffe
    .word 0xdfbf0030
    .word 0x00c0102d
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x8e050024
    .word 0x8ca30000
    .word 0x2c62000e
    .word 0x1040fff4
    .word 0x00031080
    .word 0x3c03001c
    .word 0x24638a40
    .word 0x00431021
    .word 0x8c420000
    .word 0x00400008
    .word 0x00000000
    .word 0x8e020004
    .word 0x10400089
    .word 0x2442ffff
    .word 0xde030008
    .word 0xae020004
    .word 0x64630001
    .word 0x8e020000
    .word 0xfe030008
    .word 0x90430000
    .word 0x24420001
    .word 0xaca30008
    .word 0x3064000f
    .word 0xae020000
    .word 0x24020008
    .word 0x1482008c
    .word 0x0240882d
    .word 0x8e040024
    .word 0x8c820008
    .word 0x8c83001c
    .word 0x00021102
    .word 0x24420008
    .word 0x0062182b
    .word 0x1460007c
    .word 0x24020001
    .word 0xac820000
    .word 0x8e020004
    .word 0x10400070
    .word 0x2442ffff
    .word 0xde030008
    .word 0xae020004
    .word 0x2404001f
    .word 0x64630001
    .word 0x8e020000
    .word 0xfe030008
    .word 0x0240882d
    .word 0x8e030024
    .word 0x90450000
    .word 0x24420001
    .word 0xae020000
    .word 0x50800001
    .word 0x000001cd
    .word 0x8c620008
    .word 0x00021200
    .word 0x00451021
    .word 0x0044001b
    .word 0x00001010
    .word 0x1440005e
    .word 0x3c02001c
    .word 0x30a20020
    .word 0x14400003
    .word 0x24020007
    .word 0x1000ffc1
    .word 0xac620000
    .word 0x24020002
    .word 0xac620000
    .word 0x8e020004
    .word 0x1040ffb5
    .word 0x0220302d
    .word 0xde030008
    .word 0x2442ffff
    .word 0x8e040000
    .word 0x0240882d
    .word 0x64630001
    .word 0xae020004
    .word 0xfe030008
    .word 0x8e030024
    .word 0x90820000
    .word 0x24840001
    .word 0x00021638
    .word 0xfc620010
    .word 0x24020003
    .word 0xae040000
    .word 0x8e030024
    .word 0xac620000
    .word 0x8e020004
    .word 0x1040ffa2
    .word 0x0220302d
    .word 0xde030008
    .word 0x2442ffff
    .word 0x8e040000
    .word 0x0240882d
    .word 0x64630001
    .word 0xae020004
    .word 0xfe030008
    .word 0x8e050024
    .word 0x90830000
    .word 0x24840001
    .word 0xdca20010
    .word 0x00031c38
    .word 0x0043102d
    .word 0xfca20010
    .word 0x24020004
    .word 0xae040000
    .word 0x8e030024
    .word 0xac620000
    .word 0x8e020004
    .word 0x1040ff8d
    .word 0x0220302d
    .word 0xde030008
    .word 0x2442ffff
    .word 0x8e040000
    .word 0x0240882d
    .word 0x64630001
    .word 0xae020004
    .word 0xfe030008
    .word 0x8e050024
    .word 0x90830000
    .word 0x24840001
    .word 0xdca20010
    .word 0x00031a38
    .word 0x0043102d
    .word 0xfca20010
    .word 0x24020005
    .word 0xae040000
    .word 0x8e030024
    .word 0xac620000
    .word 0x8e020004
    .word 0x5040ff78
    .word 0x0220302d
    .word 0xde030008
    .word 0x2442ffff
    .word 0xae020004
    .word 0x24060002
    .word 0x64630001
    .word 0x8e040000
    .word 0xfe030008
    .word 0x8e050024
    .word 0x90830000
    .word 0x24840001
    .word 0xdca20010
    .word 0x0043102d
    .word 0xfca20010
    .word 0x24020006
    .word 0xae040000
    .word 0x8e030024
    .word 0xdc640010
    .word 0xac620000
    .word 0x1000ff64
    .word 0xfe040038
    .word 0x1000ff62
    .word 0x0220302d
    .word 0x244289c8
    .word 0xae020020
    .word 0x24020005
    .word 0xac620008
    .word 0x1000ffa1
    .word 0x2402000d
    .word 0x3c02001c
    .word 0x244289e0
    .word 0xae020020
    .word 0x24020005
    .word 0xac820008
    .word 0x2402000d
    .word 0x1000ff5b
    .word 0xac820000
    .word 0x3c02001c
    .word 0x8e030024
    .word 0x1000fff0
    .word 0x244289f8
    .word 0x3c02001c
    .word 0x2406fffe
    .word 0x24428a18
    .word 0xae020020
    .word 0x2402000d
    .word 0xaca20000
    .word 0x1000ff48
    .word 0xaca00008
    .word 0x8ca40020
    .word 0x0220302d
    .word 0x0c0651f4
    .word 0x0200282d
    .word 0x0040882d
    .word 0x2402fffd
    .word 0x12220064
    .word 0x24020001
    .word 0x0251880a
    .word 0x1622ffda
    .word 0x0200282d
    .word 0x8e060024
    .word 0x0240882d
    .word 0x8cc40020
    .word 0x0c06518a
    .word 0x24c60008
    .word 0x8e030024
    .word 0x8c620018
    .word 0x1440ff79
    .word 0x2402000c
    .word 0x24020008
    .word 0xac620000
    .word 0x8e020004
    .word 0x1040ffcc
    .word 0x2442ffff
    .word 0xde030008
    .word 0x8e040000
    .word 0x0240882d
    .word 0x64630001
    .word 0xae020004
    .word 0xfe030008
    .word 0x8e030024
    .word 0x90820000
    .word 0x24840001
    .word 0x00021638
    .word 0xfc620010
    .word 0x24020009
    .word 0xae040000
    .word 0x8e030024
    .word 0xac620000
    .word 0x8e020004
    .word 0x1040ffba
    .word 0x2442ffff
    .word 0xde030008
    .word 0x8e040000
    .word 0x0240882d
    .word 0x64630001
    .word 0xae020004
    .word 0xfe030008
    .word 0x8e050024
    .word 0x90830000
    .word 0x24840001
    .word 0xdca20010
    .word 0x00031c38
    .word 0x0043102d
    .word 0xfca20010
    .word 0x2402000a
    .word 0xae040000
    .word 0x8e030024
    .word 0xac620000
    .word 0x8e020004
    .word 0x1040ffa6
    .word 0x2442ffff
    .word 0xde030008
    .word 0x8e040000
    .word 0x0240882d
    .word 0x64630001
    .word 0xae020004
    .word 0xfe030008
    .word 0x8e050024
    .word 0x90830000
    .word 0x24840001
    .word 0xdca20010
    .word 0x00031a38
    .word 0x0043102d
    .word 0xfca20010
    .word 0x2402000b
    .word 0xae040000
    .word 0x8e030024
    .word 0xac620000
    .word 0x8e020004
    .word 0x1040ff92
    .word 0x2442ffff
    .word 0xde030008
    .word 0xae020004
    .word 0x64630001
    .word 0x8e040000
    .word 0xfe030008
    .word 0x8e050024
    .word 0x90830000
    .word 0x24840001
    .word 0xdca20010
    .word 0x0043102d
    .word 0xfca20010
    .word 0xae040000
    .word 0x8e040024
    .word 0xdc830008
    .word 0xdc820010
    .word 0x10620004
    .word 0x0240882d
    .word 0x3c02001c
    .word 0x1000ff88
    .word 0x24428a28
    .word 0x2402000c
    .word 0xac820000
    .word 0x1000fedd
    .word 0x24060001
    .word 0x8e030024
    .word 0x2402000d
    .word 0xac620000
    .word 0x1000fedf
    .word 0xac600008
    .word 0x1000fed6
    .word 0x2406fffd
    .word 0x27bdffb0
    .word 0x2407fffe
    .word 0xffb30030
    .word 0x00c0982d
    .word 0xffb20020
    .word 0x00a0902d
    .word 0xffb10010
    .word 0x00c0882d
    .word 0xffb00000
    .word 0x0080802d
    .word 0x10800008
    .word 0xffbf0040
    .word 0x8c830024
    .word 0x10600006
    .word 0xdfbf0040
    .word 0x8c630000
    .word 0x24020006
    .word 0x10620009
    .word 0x24040001
    .word 0xdfbf0040
    .word 0x00e0102d
    .word 0xdfb30030
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0050
    .word 0x0c0662b0
    .word 0x00000000
    .word 0xde030038
    .word 0x1443fff4
    .word 0x2407fffd
    .word 0x8e040024
    .word 0x24030001
    .word 0xfe030038
    .word 0x24030001
    .word 0x8c82001c
    .word 0x00431804
    .word 0x00511006
    .word 0x10400004
    .word 0x8c840020
    .word 0x2471ffff
    .word 0x02711023
    .word 0x02429021
    .word 0x0240282d
    .word 0x0c0654ba
    .word 0x0220302d
    .word 0x24020007
    .word 0x8e030024
    .word 0x0000382d
    .word 0x1000ffe0
    .word 0xac620000
    .word 0x27bdffc0
    .word 0xffb20020
    .word 0x0080902d
    .word 0xffbf0030
    .word 0xffb10010
    .word 0x10800004
    .word 0xffb00000
    .word 0x8c830024
    .word 0x54600009
    .word 0x8c620000
    .word 0x2405fffe
    .word 0xdfbf0030
    .word 0x00a0102d
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x2404000d
    .word 0x50440004
    .word 0x8e460004
    .word 0xac640000
    .word 0xac600008
    .word 0x8e460004
    .word 0x10c0fff2
    .word 0x2405fffb
    .word 0x8e480024
    .word 0x8e450000
    .word 0x8d040008
    .word 0x2c820004
    .word 0x10400011
    .word 0x00a0482d
    .word 0x3c03001c
    .word 0x90a20000
    .word 0x24638a78
    .word 0x00831821
    .word 0x90630000
    .word 0x1043001f
    .word 0x38470000
    .word 0x24020004
    .word 0x00442023
    .word 0x0007200b
    .word 0x24c6ffff
    .word 0x2c820004
    .word 0x10c00003
    .word 0x24a50001
    .word 0x1440fff2
    .word 0x3c03001c
    .word 0xde420008
    .word 0x00a91823
    .word 0xae450000
    .word 0x2405fffd
    .word 0x0043102d
    .word 0xae460004
    .word 0xfe420008
    .word 0x24020004
    .word 0x1482ffd2
    .word 0xad040008
    .word 0x0240202d
    .word 0xde500008
    .word 0x0c0649cb
    .word 0xde510018
    .word 0x8e430024
    .word 0x24020007
    .word 0xfe500008
    .word 0x0000282d
    .word 0xac620000
    .word 0x1000ffc7
    .word 0xfe510018
    .word 0x1000ffe4
    .word 0x24840001
    .word 0x27bdfff0
    .word 0x2403fffe
    .word 0x10800007
    .word 0xffbf0000
    .word 0x8c840024
    .word 0x10800005
    .word 0xdfbf0000
    .word 0x8c820020
    .word 0x14400005
    .word 0x0040202d
    .word 0xdfbf0000
    .word 0x0060102d
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x0c0654cb
    .word 0x00000000
    .word 0x1000fff9
    .word 0x0040182d
    .word 0x3c02001c
    .word 0x03e00008
    .word 0x24428a80
    .word 0x00a0602d
    .word 0x00c0682d
    .word 0x10a00077
    .word 0x0000102d
    .word 0x3c02ffff
    .word 0x0002103e
    .word 0x2cc30008
    .word 0x1460005e
    .word 0x00825826
    .word 0x91830000
    .word 0x000b103c
    .word 0x0002103f
    .word 0x000b2a3a
    .word 0x258c0001
    .word 0x00431026
    .word 0x91840000
    .word 0x3c03001c
    .word 0x304200ff
    .word 0x24638a80
    .word 0x000210c0
    .word 0x00431021
    .word 0x258c0001
    .word 0xdc420000
    .word 0x25adfff8
    .word 0x91860000
    .word 0x2daa0008
    .word 0x00455826
    .word 0x258c0001
    .word 0x000b103c
    .word 0x0002103f
    .word 0x000b2a3a
    .word 0x00441026
    .word 0x91870000
    .word 0x304200ff
    .word 0x258c0001
    .word 0x000210c0
    .word 0x91880000
    .word 0x00431021
    .word 0x258c0001
    .word 0xdc420000
    .word 0x91890000
    .word 0x258c0001
    .word 0x00455826
    .word 0x91850000
    .word 0x000b103c
    .word 0x0002103f
    .word 0x000b223a
    .word 0x00461026
    .word 0x258c0001
    .word 0x304200ff
    .word 0x91860000
    .word 0x000210c0
    .word 0x258c0001
    .word 0x00431021
    .word 0xdc420000
    .word 0x00445826
    .word 0x000b103c
    .word 0x0002103f
    .word 0x000b223a
    .word 0x00471026
    .word 0x304200ff
    .word 0x000210c0
    .word 0x00431021
    .word 0xdc420000
    .word 0x00445826
    .word 0x000b103c
    .word 0x0002103f
    .word 0x000b223a
    .word 0x00481026
    .word 0x304200ff
    .word 0x000210c0
    .word 0x00431021
    .word 0xdc420000
    .word 0x00445826
    .word 0x000b103c
    .word 0x0002103f
    .word 0x000b223a
    .word 0x00491026
    .word 0x304200ff
    .word 0x000210c0
    .word 0x00431021
    .word 0xdc420000
    .word 0x00445826
    .word 0x000b103c
    .word 0x0002103f
    .word 0x000b223a
    .word 0x00451026
    .word 0x304200ff
    .word 0x000210c0
    .word 0x00431021
    .word 0xdc420000
    .word 0x00445826
    .word 0x000b103c
    .word 0x0002103f
    .word 0x000b223a
    .word 0x00461026
    .word 0x304200ff
    .word 0x000210c0
    .word 0x00431021
    .word 0xdc420000
    .word 0x1140ffa4
    .word 0x00445826
    .word 0x11a00010
    .word 0x00000000
    .word 0x91820000
    .word 0x000b183c
    .word 0x0003183f
    .word 0x000b223a
    .word 0x25adffff
    .word 0x00621826
    .word 0x258c0001
    .word 0x306300ff
    .word 0x3c02001c
    .word 0x24428a80
    .word 0x000318c0
    .word 0x00621821
    .word 0xdc620000
    .word 0x15a0fff2
    .word 0x00445826
    .word 0x3c02ffff
    .word 0x0002103e
    .word 0x01621026
    .word 0x03e00008
    .word 0x00000000
    .word 0x03e00008
    .word 0x28820000
    .size stage3p_tail_gap_0019272c, .-stage3p_tail_gap_0019272c

    /* Exact public EE reconstruction @ 0x00196370..0x00196380
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_00196370
    .type stage3p_tail_gap_00196370, @function
stage3p_tail_gap_00196370:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00196370, .-stage3p_tail_gap_00196370

    /* Exact public EE reconstruction @ 0x00196a10..0x00196a20
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_00196a10
    .type stage3p_tail_gap_00196a10, @function
stage3p_tail_gap_00196a10:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00196a10, .-stage3p_tail_gap_00196a10

    /* Exact public EE reconstruction @ 0x00196a38..0x00196a48
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_00196a38
    .type stage3p_tail_gap_00196a38, @function
stage3p_tail_gap_00196a38:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00196a38, .-stage3p_tail_gap_00196a38

    /* Exact public EE reconstruction @ 0x00196a60..0x00196a70
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_00196a60
    .type stage3p_tail_gap_00196a60, @function
stage3p_tail_gap_00196a60:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00196a60, .-stage3p_tail_gap_00196a60

    /* Exact public EE reconstruction @ 0x00196c08..0x00196c18
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_00196c08
    .type stage3p_tail_gap_00196c08, @function
stage3p_tail_gap_00196c08:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00196c08, .-stage3p_tail_gap_00196c08

    /* Exact public EE reconstruction @ 0x00196d68..0x00196d78
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_00196d68
    .type stage3p_tail_gap_00196d68, @function
stage3p_tail_gap_00196d68:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00196d68, .-stage3p_tail_gap_00196d68

    /* Exact public EE reconstruction @ 0x00197000..0x00198a70
     * target span SHA-256: 08bb61d399d8f76c9374b11d7a36e43b4e6e18f9257331556b2020ed77202df4
     */
    .balign 4
    .globl stage3p_tail_gap_00197000
    .type stage3p_tail_gap_00197000, @function
stage3p_tail_gap_00197000:
    .word 0x0260282d
    .word 0x1e40fffb
    .word 0x0240302d
    .word 0x02901021
    .word 0x00141880
    .word 0x0280882d
    .word 0x24551468
    .word 0x0073a021
    .word 0x8e031460
    .word 0x0200202d
    .word 0x8e120b70
    .word 0x0260282d
    .word 0x00031080
    .word 0x24060001
    .word 0x00501021
    .word 0x2463ffff
    .word 0x8c420b6c
    .word 0xae031460
    .word 0x0c065aa5
    .word 0xae020b70
    .word 0x02505021
    .word 0x8e021464
    .word 0x00124880
    .word 0x8e070b70
    .word 0x01332821
    .word 0x2444fffe
    .word 0x00a0482d
    .word 0x2442ffff
    .word 0x00041880
    .word 0x00021080
    .word 0x00701821
    .word 0x00501021
    .word 0xae041464
    .word 0xac520b6c
    .word 0x00074080
    .word 0xac670b6c
    .word 0x24060001
    .word 0x01131821
    .word 0x00f03821
    .word 0x94a20000
    .word 0x0060402d
    .word 0x94630000
    .word 0x0200202d
    .word 0x0260282d
    .word 0x00431021
    .word 0xa6820000
    .word 0x26940004
    .word 0x90e31468
    .word 0x91421468
    .word 0x24470001
    .word 0x0043102b
    .word 0x38420000
    .word 0x24630001
    .word 0x0062380b
    .word 0xa2a70000
    .word 0x26b50001
    .word 0xa5110002
    .word 0xa5310002
    .word 0xae110b70
    .word 0x0c065aa5
    .word 0x26310001
    .word 0x8e021460
    .word 0x28420002
    .word 0x5040ffc9
    .word 0x8e031460
    .word 0x8e031464
    .word 0x02e0282d
    .word 0x8e060b70
    .word 0x0200202d
    .word 0x2463ffff
    .word 0x00031080
    .word 0xae031464
    .word 0x00501021
    .word 0x0c065ae6
    .word 0xac460b6c
    .word 0x0260202d
    .word 0x02c0282d
    .word 0x0c065ba0
    .word 0x26060b4c
    .word 0xdfb00000
    .word 0xdfbf0080
    .word 0xdfb70070
    .word 0xdfb60060
    .word 0xdfb50050
    .word 0xdfb40040
    .word 0xdfb30030
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0x03e00008
    .word 0x27bd0090
    .word 0x2ac40002
    .word 0x00021880
    .word 0xae021460
    .word 0x00701821
    .word 0x0000102d
    .word 0x14800018
    .word 0x24630b60
    .word 0x0040882d
    .word 0xac62000c
    .word 0x00021080
    .word 0x00531821
    .word 0x00462021
    .word 0x24020001
    .word 0xa4620000
    .word 0x02301021
    .word 0xa0401468
    .word 0xde0216b8
    .word 0x6442ffff
    .word 0x10c00005
    .word 0xfe0216b8
    .word 0x94830002
    .word 0xde0216c0
    .word 0x0043102f
    .word 0xfe0216c0
    .word 0x8e031460
    .word 0x28620002
    .word 0x1440ffe5
    .word 0x24620001
    .word 0x1000ff7f
    .word 0xaef60004
    .word 0x26d60001
    .word 0x1000ffe7
    .word 0x02c0102d
    .word 0x94a90002
    .word 0x240bffff
    .word 0x0000402d
    .word 0x240a0007
    .word 0x15200003
    .word 0x24030004
    .word 0x240a008a
    .word 0x24030003
    .word 0x00061080
    .word 0x00451021
    .word 0x04c0001c
    .word 0xa44b0006
    .word 0x24a50006
    .word 0x24c60001
    .word 0x25080001
    .word 0x0120382d
    .word 0x010a102a
    .word 0x10400003
    .word 0x94a90000
    .word 0x50e90011
    .word 0x24c6ffff
    .word 0x0103102a
    .word 0x10400015
    .word 0x00071880
    .word 0x00641821
    .word 0x24630a80
    .word 0x9462000c
    .word 0x00481021
    .word 0xa462000c
    .word 0x0000402d
    .word 0x00e0582d
    .word 0x1120000a
    .word 0x240a008a
    .word 0x240a0007
    .word 0x10e90006
    .word 0x24030004
    .word 0x24c6ffff
    .word 0x14c0ffe8
    .word 0x24a50004
    .word 0x03e00008
    .word 0x00000000
    .word 0x240a0006
    .word 0x1000fff9
    .word 0x24030003
    .word 0x10e0000c
    .word 0x2902000b
    .word 0x10eb0006
    .word 0x00071880
    .word 0x00641821
    .word 0x24630a80
    .word 0x9462000c
    .word 0x24420001
    .word 0xa462000c
    .word 0x94820acc
    .word 0x24420001
    .word 0x1000ffe5
    .word 0xa4820acc
    .word 0x50400005
    .word 0x94820ad4
    .word 0x94820ad0
    .word 0x24420001
    .word 0x1000ffdf
    .word 0xa4820ad0
    .word 0x24420001
    .word 0x1000ffdc
    .word 0xa4820ad4
    .word 0x94ac0002
    .word 0x0080382d
    .word 0x00c0782d
    .word 0x0000482d
    .word 0x2406ffff
    .word 0x24040007
    .word 0x15800003
    .word 0x24030004
    .word 0x2404008a
    .word 0x24030003
    .word 0x05e0003c
    .word 0x0000702d
    .word 0x24ad0006
    .word 0x25290001
    .word 0x0180502d
    .word 0x0124102a
    .word 0x10400003
    .word 0x95ac0000
    .word 0x514c0031
    .word 0x25ce0001
    .word 0x0123102a
    .word 0x1040003d
    .word 0x000a1080
    .word 0x8ce416d4
    .word 0x00471021
    .word 0x240b0010
    .word 0x24430a80
    .word 0x9468000e
    .word 0x01681023
    .word 0x0044102a
    .word 0x1040002d
    .word 0x00882821
    .word 0x9465000c
    .word 0x94e216d0
    .word 0x00851804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01621023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x2529ffff
    .word 0x1520ffdc
    .word 0x000a1080
    .word 0x0000482d
    .word 0x0140302d
    .word 0x1180000b
    .word 0x2404008a
    .word 0x24040007
    .word 0x114c0007
    .word 0x24030004
    .word 0x25ce0001
    .word 0x01ee102a
    .word 0x1040ffc7
    .word 0x25ad0004
    .word 0x03e00008
    .word 0x00000000
    .word 0x24040006
    .word 0x1000fff8
    .word 0x24030003
    .word 0x9462000c
    .word 0x94e316d0
    .word 0x00821004
    .word 0xace516d4
    .word 0x00621825
    .word 0x1000ffe7
    .word 0xa4e316d0
    .word 0x1140007b
    .word 0x2922000b
    .word 0x11460024
    .word 0x000a1080
    .word 0x240b0010
    .word 0x00471021
    .word 0x8ce616d4
    .word 0x24430a80
    .word 0x9468000e
    .word 0x01681023
    .word 0x0046102a
    .word 0x50400069
    .word 0x9462000c
    .word 0x9465000c
    .word 0x94e216d0
    .word 0x00c51804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01621023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x2529ffff
    .word 0x94e80ace
    .word 0x240b0010
    .word 0x8ce616d4
    .word 0x01681023
    .word 0x0046102a
    .word 0x50400041
    .word 0x94e20acc
    .word 0x94e50acc
    .word 0x94e216d0
    .word 0x00c51804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01621023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x8ce416d4
    .word 0x2402000e
    .word 0x0044102a
    .word 0x1040001c
    .word 0x2522fffd
    .word 0x94e216d0
    .word 0x2525fffd
    .word 0x00851804
    .word 0x30a5ffff
    .word 0x00431025
    .word 0x8ce4001c
    .word 0xa4e216d0
    .word 0x8ce30008
    .word 0x90e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0xa0620000
    .word 0x24860001
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xace6001c
    .word 0x8ce316d4
    .word 0x00431023
    .word 0x2463fff2
    .word 0x00452807
    .word 0xace316d4
    .word 0x1000ff83
    .word 0xa4e516d0
    .word 0x94e316d0
    .word 0x00821004
    .word 0x24840002
    .word 0x00621825
    .word 0xace416d4
    .word 0x1000ff7c
    .word 0xa4e316d0
    .word 0x00c82021
    .word 0x94e316d0
    .word 0x00c21004
    .word 0xace416d4
    .word 0x00621825
    .word 0x1000ffd3
    .word 0xa4e316d0
    .word 0x00c82021
    .word 0x94e316d0
    .word 0x00c21004
    .word 0xace416d4
    .word 0x00621825
    .word 0x1000ffab
    .word 0xa4e316d0
    .word 0x50400049
    .word 0x94e80ad6
    .word 0x94e80ad2
    .word 0x240b0010
    .word 0x8ce616d4
    .word 0x01681023
    .word 0x0046102a
    .word 0x5040003b
    .word 0x94e20ad0
    .word 0x94e50ad0
    .word 0x94e216d0
    .word 0x00c51804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01621023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x8ce416d4
    .word 0x2402000d
    .word 0x0044102a
    .word 0x50400019
    .word 0x2522fffd
    .word 0x94e216d0
    .word 0x2525fffd
    .word 0x00851804
    .word 0x30a5ffff
    .word 0x00431025
    .word 0x8ce4001c
    .word 0xa4e216d0
    .word 0x8ce30008
    .word 0x90e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0xa0620000
    .word 0x24860001
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xace6001c
    .word 0x8ce316d4
    .word 0x00431023
    .word 0x1000ffa9
    .word 0x2463fff3
    .word 0x94e316d0
    .word 0x00821004
    .word 0x1000ffac
    .word 0x24840003
    .word 0x00c82021
    .word 0x94e316d0
    .word 0x00c21004
    .word 0xace416d4
    .word 0x00621825
    .word 0x1000ffd9
    .word 0xa4e316d0
    .word 0x240b0010
    .word 0x8ce616d4
    .word 0x01681023
    .word 0x0046102a
    .word 0x5040003b
    .word 0x94e20ad4
    .word 0x94e50ad4
    .word 0x94e216d0
    .word 0x00c51804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01621023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x8ce416d4
    .word 0x24020009
    .word 0x0044102a
    .word 0x50400019
    .word 0x2522fff5
    .word 0x94e216d0
    .word 0x2525fff5
    .word 0x00851804
    .word 0x30a5ffff
    .word 0x00431025
    .word 0x8ce4001c
    .word 0xa4e216d0
    .word 0x8ce30008
    .word 0x90e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0xa0620000
    .word 0x24860001
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xace6001c
    .word 0x8ce316d4
    .word 0x00431023
    .word 0x1000ff62
    .word 0x2463fff7
    .word 0x94e316d0
    .word 0x00821004
    .word 0x1000ff65
    .word 0x24840007
    .word 0x00c82021
    .word 0x94e316d0
    .word 0x00c21004
    .word 0xace416d4
    .word 0x00621825
    .word 0x1000ffd9
    .word 0xa4e316d0
    .word 0x27bdffe0
    .word 0x248500a4
    .word 0xffbf0010
    .word 0xffb00000
    .word 0x0080802d
    .word 0x0c065c7b
    .word 0x8c860b2c
    .word 0x0200202d
    .word 0x8e060b38
    .word 0x0c065c7b
    .word 0x26050998
    .word 0x26050b40
    .word 0x0c065bc9
    .word 0x0200202d
    .word 0x24050012
    .word 0x3c02001c
    .word 0x244298a8
    .word 0x00a21021
    .word 0x90420000
    .word 0x00021080
    .word 0x00501021
    .word 0x94420a8e
    .word 0x14400006
    .word 0x00051840
    .word 0x24a5ffff
    .word 0x28a20003
    .word 0x1040fff5
    .word 0x3c02001c
    .word 0x00051840
    .word 0xde0416b8
    .word 0x00651821
    .word 0xdfbf0010
    .word 0x24630011
    .word 0x00a0102d
    .word 0x0083202d
    .word 0xfe0416b8
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x27bdffd0
    .word 0x2402000b
    .word 0xffb10010
    .word 0x00a0582d
    .word 0xffb00000
    .word 0x00c0882d
    .word 0xffbf0020
    .word 0x0080802d
    .word 0x8c8416d4
    .word 0x0044102a
    .word 0x104000a6
    .word 0x00e0502d
    .word 0x960216d0
    .word 0x24a5feff
    .word 0x00851804
    .word 0x30a5ffff
    .word 0x00431025
    .word 0x8e04001c
    .word 0xa60216d0
    .word 0x8e030008
    .word 0x920216d0
    .word 0x00641821
    .word 0x24840001
    .word 0xa0620000
    .word 0x24860001
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xae06001c
    .word 0x8e0316d4
    .word 0x00431023
    .word 0x2463fff5
    .word 0x00452807
    .word 0xae0316d4
    .word 0xa60516d0
    .word 0x8e0416d4
    .word 0x2402000b
    .word 0x0044102a
    .word 0x10400080
    .word 0x960216d0
    .word 0x2625ffff
    .word 0x00851804
    .word 0x30a5ffff
    .word 0x00431025
    .word 0x8e04001c
    .word 0xa60216d0
    .word 0x8e030008
    .word 0x920216d0
    .word 0x00641821
    .word 0x24840001
    .word 0xa0620000
    .word 0x24860001
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xae06001c
    .word 0x8e0316d4
    .word 0x00431023
    .word 0x2463fff5
    .word 0x00452807
    .word 0xae0316d4
    .word 0xa60516d0
    .word 0x8e0416d4
    .word 0x2402000c
    .word 0x0044102a
    .word 0x1040005b
    .word 0x960216d0
    .word 0x2545fffc
    .word 0x00851804
    .word 0x30a5ffff
    .word 0x00431025
    .word 0x8e04001c
    .word 0xa60216d0
    .word 0x8e030008
    .word 0x920216d0
    .word 0x00641821
    .word 0x24840001
    .word 0xa0620000
    .word 0x24860001
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xae06001c
    .word 0x8e0316d4
    .word 0x00431023
    .word 0x2463fff4
    .word 0x00452807
    .word 0xae0316d4
    .word 0xa60516d0
    .word 0x19400029
    .word 0x0000482d
    .word 0x3c02001c
    .word 0x8e0816d4
    .word 0x244298a8
    .word 0x01222821
    .word 0x2402000d
    .word 0x0048102a
    .word 0x1040002e
    .word 0x25040003
    .word 0x90a20000
    .word 0x960516d0
    .word 0x00021080
    .word 0x8e04001c
    .word 0x00501021
    .word 0x8e030008
    .word 0x94460a8e
    .word 0x00641821
    .word 0x24840001
    .word 0x01061004
    .word 0x24870001
    .word 0x00a22825
    .word 0xa60516d0
    .word 0x920216d0
    .word 0xa0620000
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xae07001c
    .word 0x8e0316d4
    .word 0x00431023
    .word 0x2463fff3
    .word 0x00463007
    .word 0xae0316d4
    .word 0xa60616d0
    .word 0x25290001
    .word 0x012a102a
    .word 0x1440ffda
    .word 0x3c02001c
    .word 0x2566ffff
    .word 0x260500a4
    .word 0x0c065cbd
    .word 0x0200202d
    .word 0x26050998
    .word 0x0200202d
    .word 0x0c065cbd
    .word 0x2626ffff
    .word 0xdfb00000
    .word 0xdfbf0020
    .word 0xdfb10010
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x90a20000
    .word 0x960316d0
    .word 0x00021080
    .word 0x00501021
    .word 0x94420a8e
    .word 0xae0416d4
    .word 0x01021004
    .word 0x00621825
    .word 0x1000ffe6
    .word 0xa60316d0
    .word 0x2543fffc
    .word 0x00831804
    .word 0x24840004
    .word 0x00431025
    .word 0xae0416d4
    .word 0x1000ffb9
    .word 0xa60216d0
    .word 0x2623ffff
    .word 0x00831804
    .word 0x24840005
    .word 0x00431025
    .word 0xae0416d4
    .word 0x1000ff94
    .word 0xa60216d0
    .word 0x960216d0
    .word 0x24a3feff
    .word 0x00831804
    .word 0x24840005
    .word 0x00431025
    .word 0xae0416d4
    .word 0x1000ff6e
    .word 0xa60216d0
    .word 0x27bdfff0
    .word 0x2403000d
    .word 0xffbf0000
    .word 0x0080402d
    .word 0x30eaffff
    .word 0x00a0582d
    .word 0x8c8216d4
    .word 0x0062182a
    .word 0x00474804
    .word 0x10600021
    .word 0x24440003
    .word 0x950216d0
    .word 0x8d04001c
    .word 0x00491025
    .word 0x8d030008
    .word 0xa50216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x910216d0
    .word 0x24850001
    .word 0xa0620000
    .word 0x950316d0
    .word 0x8d020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xad05001c
    .word 0x8d0316d4
    .word 0x00431023
    .word 0x2463fff3
    .word 0x004a1007
    .word 0xad0316d4
    .word 0xa50216d0
    .word 0x0006303c
    .word 0x0006303f
    .word 0x0100202d
    .word 0x0160282d
    .word 0x0c06625f
    .word 0x24070001
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x950216d0
    .word 0xad0416d4
    .word 0x1000fff3
    .word 0x00491025
    .word 0x27bdffe0
    .word 0x2402000d
    .word 0xffb00000
    .word 0x0080802d
    .word 0xffbf0010
    .word 0x8c8416d4
    .word 0x0044102a
    .word 0x1040009e
    .word 0x960216d0
    .word 0x24050002
    .word 0x00851804
    .word 0x8e04001c
    .word 0x00431025
    .word 0x8e030008
    .word 0xa60216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x920216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xae06001c
    .word 0x8e0316d4
    .word 0x00431023
    .word 0x2463fff3
    .word 0x00452807
    .word 0xae0316d4
    .word 0xa60516d0
    .word 0x3c02001c
    .word 0x24080010
    .word 0x244398c0
    .word 0x8e0616d4
    .word 0x94670402
    .word 0x01071023
    .word 0x0046102a
    .word 0x50400076
    .word 0x94620400
    .word 0x94650400
    .word 0x960216d0
    .word 0x00c51804
    .word 0x8e04001c
    .word 0x00431025
    .word 0x8e030008
    .word 0xa60216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x920216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xae06001c
    .word 0x8e0216d4
    .word 0x00471821
    .word 0x01021023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa60516d0
    .word 0xae0316d4
    .word 0x0c06621b
    .word 0x0200202d
    .word 0x8e0216cc
    .word 0x8e0416d4
    .word 0x00441023
    .word 0x2442000b
    .word 0x28420009
    .word 0x10400040
    .word 0x2402000d
    .word 0x0044102a
    .word 0x1040004a
    .word 0x960216d0
    .word 0x24050002
    .word 0x00851804
    .word 0x8e04001c
    .word 0x00431025
    .word 0x8e030008
    .word 0xa60216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x920216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xae06001c
    .word 0x8e0316d4
    .word 0x00431023
    .word 0x2463fff3
    .word 0x00452807
    .word 0xae0316d4
    .word 0xa60516d0
    .word 0x3c02001c
    .word 0x24080010
    .word 0x244398c0
    .word 0x8e0616d4
    .word 0x94670402
    .word 0x01071023
    .word 0x0046102a
    .word 0x50400022
    .word 0x94620400
    .word 0x94650400
    .word 0x960216d0
    .word 0x00c51804
    .word 0x8e04001c
    .word 0x00431025
    .word 0x8e030008
    .word 0xa60216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x920216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xae06001c
    .word 0x8e0216d4
    .word 0x00471821
    .word 0x01021023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa60516d0
    .word 0xae0316d4
    .word 0x0c06621b
    .word 0x0200202d
    .word 0x24020007
    .word 0xdfbf0010
    .word 0xae0216cc
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0020
    .word 0x00c72021
    .word 0x960316d0
    .word 0x00c21004
    .word 0xae0416d4
    .word 0x00621825
    .word 0x1000fff2
    .word 0xa60316d0
    .word 0x24030002
    .word 0x00831804
    .word 0x24840003
    .word 0x00431025
    .word 0xae0416d4
    .word 0x1000ffc9
    .word 0xa60216d0
    .word 0x00c72021
    .word 0x960316d0
    .word 0x00c21004
    .word 0xae0416d4
    .word 0x00621825
    .word 0x1000ff9e
    .word 0xa60316d0
    .word 0x24030002
    .word 0x00831804
    .word 0x24840003
    .word 0x00431025
    .word 0xae0416d4
    .word 0x1000ff75
    .word 0xa60216d0
    .word 0x27bdffb0
    .word 0xffb30030
    .word 0x00a0982d
    .word 0xffb20020
    .word 0x00e0902d
    .word 0xffb10010
    .word 0x0000382d
    .word 0xffb00000
    .word 0x00c0882d
    .word 0xffbf0040
    .word 0x8c820094
    .word 0x18400093
    .word 0x0080802d
    .word 0x90830024
    .word 0x24020002
    .word 0x1062008b
    .word 0x00000000
    .word 0x26050b28
    .word 0x0c065bc9
    .word 0x0200202d
    .word 0x26050b34
    .word 0x0c065bc9
    .word 0x0200202d
    .word 0x0c065e1d
    .word 0x0200202d
    .word 0xde0316b8
    .word 0x0040382d
    .word 0xde0216c0
    .word 0x6463000a
    .word 0x6442000a
    .word 0x000318fa
    .word 0x000220fa
    .word 0x0064102b
    .word 0x14400003
    .word 0x66220004
    .word 0x0080182d
    .word 0x66220004
    .word 0x0062102b
    .word 0x14400016
    .word 0x00000000
    .word 0x12600014
    .word 0x0260282d
    .word 0x0220302d
    .word 0x0200202d
    .word 0x0c065efd
    .word 0x0240382d
    .word 0x0c065a80
    .word 0x0200202d
    .word 0x16400008
    .word 0x00000000
    .word 0xdfbf0040
    .word 0xdfb30030
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0050
    .word 0x0c066241
    .word 0x0200202d
    .word 0x1000fff7
    .word 0xdfbf0040
    .word 0x10830033
    .word 0x2402000d
    .word 0x8e0416d4
    .word 0x0044102a
    .word 0x10400028
    .word 0x960216d0
    .word 0x26450004
    .word 0x00851804
    .word 0x30a5ffff
    .word 0x00431025
    .word 0x8e04001c
    .word 0xa60216d0
    .word 0x8e030008
    .word 0x920216d0
    .word 0x00641821
    .word 0x24840001
    .word 0xa0620000
    .word 0x24860001
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xae06001c
    .word 0x8e0316d4
    .word 0x00431023
    .word 0x2463fff3
    .word 0x00452807
    .word 0xae0316d4
    .word 0xa60516d0
    .word 0x8e050b2c
    .word 0x0200202d
    .word 0x8e060b38
    .word 0x24e70001
    .word 0x24a50001
    .word 0x0c065e44
    .word 0x24c60001
    .word 0x26060998
    .word 0x0200202d
    .word 0x260500a4
    .word 0x0c0660c2
    .word 0x00000000
    .word 0x1000ffc5
    .word 0x00000000
    .word 0x26430004
    .word 0x00831804
    .word 0x24840003
    .word 0x00431025
    .word 0xae0416d4
    .word 0x1000ffec
    .word 0xa60216d0
    .word 0x8e0416d4
    .word 0x0044102a
    .word 0x10400020
    .word 0x960216d0
    .word 0x26450002
    .word 0x00851804
    .word 0x30a5ffff
    .word 0x00431025
    .word 0x8e04001c
    .word 0xa60216d0
    .word 0x8e030008
    .word 0x920216d0
    .word 0x00641821
    .word 0x24840001
    .word 0xa0620000
    .word 0x24860001
    .word 0x960316d0
    .word 0x8e020008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0x24020010
    .word 0xae06001c
    .word 0x8e0316d4
    .word 0x00431023
    .word 0x2463fff3
    .word 0x00452807
    .word 0xae0316d4
    .word 0xa60516d0
    .word 0x3c05001c
    .word 0x3c06001c
    .word 0x24a598c0
    .word 0x24c69d40
    .word 0x1000ffd3
    .word 0x0200202d
    .word 0x26430002
    .word 0x00831804
    .word 0x24840003
    .word 0x00431025
    .word 0xae0416d4
    .word 0x1000fff4
    .word 0xa60216d0
    .word 0x0c0661e7
    .word 0x00000000
    .word 0x1000ff74
    .word 0x26050b28
    .word 0x1000ff83
    .word 0x64c40005
    .word 0x3c02001c
    .word 0x0080402d
    .word 0x24429fb8
    .word 0x8c8416b0
    .word 0x00c25021
    .word 0x8d0716b4
    .word 0x00061080
    .word 0x8d0316a8
    .word 0x00481021
    .word 0x244900a0
    .word 0x00641821
    .word 0x00041040
    .word 0x24840001
    .word 0x00471021
    .word 0x00a0382d
    .word 0xa4450000
    .word 0xa0660000
    .word 0x14a0000a
    .word 0xad0416b0
    .word 0x95220004
    .word 0x24420001
    .word 0xa5220004
    .word 0x8d0316ac
    .word 0x8d0216b0
    .word 0x2463ffff
    .word 0x00431026
    .word 0x03e00008
    .word 0x2c420001
    .word 0x8d0516c8
    .word 0x3c02001c
    .word 0x91430000
    .word 0x24e7ffff
    .word 0x24429db8
    .word 0x24a50001
    .word 0x00031880
    .word 0xad0516c8
    .word 0x00681821
    .word 0x00e23021
    .word 0x3c02001c
    .word 0x246304a4
    .word 0x24429db8
    .word 0x000721c2
    .word 0x00822021
    .word 0x2ce90100
    .word 0x94620004
    .word 0x24420001
    .word 0x11200017
    .word 0xa4620004
    .word 0x90c20000
    .word 0x00021080
    .word 0x3c03001c
    .word 0x24440990
    .word 0x24639db8
    .word 0x01042821
    .word 0x000711c2
    .word 0x00e32021
    .word 0x3c03001c
    .word 0x24639db8
    .word 0x11200009
    .word 0x00431021
    .word 0x90820000
    .word 0x00021080
    .word 0x24420990
    .word 0x01021021
    .word 0x94420008
    .word 0x24420001
    .word 0x1000ffd3
    .word 0xa4a20008
    .word 0x1000fff8
    .word 0x90420100
    .word 0x1000ffea
    .word 0x90820100
    .word 0x8c8216b0
    .word 0x0080382d
    .word 0x00a0682d
    .word 0x00c0782d
    .word 0x10400030
    .word 0x0000702d
    .word 0x8ce216b4
    .word 0x000e1840
    .word 0x8ce416a8
    .word 0x00621821
    .word 0x946a0000
    .word 0x008e2021
    .word 0x90890000
    .word 0x15400058
    .word 0x25ce0001
    .word 0x00091080
    .word 0x8ce616d4
    .word 0x004d1821
    .word 0x24090010
    .word 0x94680002
    .word 0x01281023
    .word 0x0046102a
    .word 0x50400048
    .word 0x94620000
    .word 0x94650000
    .word 0x94e216d0
    .word 0x00c51804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01221023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x8ce216b0
    .word 0x01c2102b
    .word 0x5440ffd3
    .word 0x8ce216b4
    .word 0x95a80402
    .word 0x24090010
    .word 0x8ce616d4
    .word 0x01281023
    .word 0x0046102a
    .word 0x5040001d
    .word 0x95a20400
    .word 0x95a50400
    .word 0x94e216d0
    .word 0x00c51804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01221023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x95a20402
    .word 0x03e00008
    .word 0xace216cc
    .word 0x00c82021
    .word 0x94e316d0
    .word 0x00c21004
    .word 0xace416d4
    .word 0x00621825
    .word 0x1000fff7
    .word 0xa4e316d0
    .word 0x00c82021
    .word 0x94e316d0
    .word 0x00c21004
    .word 0xace416d4
    .word 0x00621825
    .word 0x1000ffcc
    .word 0xa4e316d0
    .word 0x3c02001c
    .word 0x240c0010
    .word 0x24429fb8
    .word 0x8ce616d4
    .word 0x01221021
    .word 0x90420000
    .word 0x00025880
    .word 0x016d1821
    .word 0x94680406
    .word 0x01881023
    .word 0x0046102a
    .word 0x504000ac
    .word 0x94620404
    .word 0x94650404
    .word 0x94e216d0
    .word 0x00c51804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01821023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x3c02001c
    .word 0x24429768
    .word 0x01621021
    .word 0x8c480000
    .word 0x51000025
    .word 0x254affff
    .word 0x3c02001c
    .word 0x240c0010
    .word 0x2442a0b8
    .word 0x8ce416d4
    .word 0x01621021
    .word 0x01881823
    .word 0x8c420000
    .word 0x0064182a
    .word 0x1060007c
    .word 0x01224823
    .word 0x94e216d0
    .word 0x00891804
    .word 0x8ce4001c
    .word 0x3125ffff
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01821023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x254affff
    .word 0x2d420100
    .word 0x1040005a
    .word 0x3c02001c
    .word 0x24429db8
    .word 0x01421021
    .word 0x90420000
    .word 0x00024880
    .word 0x240b0010
    .word 0x012f1821
    .word 0x8ce616d4
    .word 0x94680002
    .word 0x01681023
    .word 0x0046102a
    .word 0x50400047
    .word 0x94620000
    .word 0x94650000
    .word 0x94e216d0
    .word 0x00c51804
    .word 0x8ce4001c
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x01621023
    .word 0x00452807
    .word 0x2463fff0
    .word 0xa4e516d0
    .word 0xace316d4
    .word 0x3c02001c
    .word 0x244297e0
    .word 0x01221021
    .word 0x8c480000
    .word 0x5100ff4f
    .word 0x8ce216b0
    .word 0x3c02001c
    .word 0x240b0010
    .word 0x2442a130
    .word 0x8ce416d4
    .word 0x01221021
    .word 0x01681823
    .word 0x8c420000
    .word 0x0064182a
    .word 0x10600017
    .word 0x01425023
    .word 0x94e216d0
    .word 0x008a1804
    .word 0x8ce4001c
    .word 0x3145ffff
    .word 0x00431025
    .word 0x8ce30008
    .word 0xa4e216d0
    .word 0x00641821
    .word 0x24840001
    .word 0x90e216d0
    .word 0x24860001
    .word 0xa0620000
    .word 0x94e316d0
    .word 0x8ce20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xace6001c
    .word 0x8ce216d4
    .word 0x00481821
    .word 0x1000ff2a
    .word 0x01621023
    .word 0x94e216d0
    .word 0x008a1804
    .word 0x00882021
    .word 0x00431025
    .word 0xace416d4
    .word 0x1000ff27
    .word 0xa4e216d0
    .word 0x00c82021
    .word 0x94e316d0
    .word 0x00c21004
    .word 0xace416d4
    .word 0x00621825
    .word 0x1000ffcd
    .word 0xa4e316d0
    .word 0x000a19c2
    .word 0x24429db8
    .word 0x00621821
    .word 0x1000ffa6
    .word 0x90620100
    .word 0x94e216d0
    .word 0x00891804
    .word 0x00882021
    .word 0x00431025
    .word 0xace416d4
    .word 0x1000ff98
    .word 0xa4e216d0
    .word 0x00c82021
    .word 0x94e316d0
    .word 0x00c21004
    .word 0xace416d4
    .word 0x00621825
    .word 0x1000ff68
    .word 0xa4e316d0
    .word 0x0000402d
    .word 0x0000382d
    .word 0x248300a4
    .word 0x24060006
    .word 0x94620000
    .word 0x24c6ffff
    .word 0x24630004
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x04c1fff9
    .word 0x00e23821
    .word 0x24060007
    .word 0x248500c0
    .word 0x94a30000
    .word 0x24c60001
    .word 0x28c20080
    .word 0x24a50004
    .word 0x00000000
    .word 0x00000000
    .word 0x1440fff9
    .word 0x01034021
    .word 0x28c20100
    .word 0x1040000d
    .word 0x00081082
    .word 0x00061080
    .word 0x00441021
    .word 0x244500a4
    .word 0x94a30000
    .word 0x24c60001
    .word 0x28c20100
    .word 0x24a50004
    .word 0x00000000
    .word 0x00000000
    .word 0x1440fff9
    .word 0x00e33821
    .word 0x00081082
    .word 0x0047102b
    .word 0x38420001
    .word 0x03e00008
    .word 0xa0820024
    .word 0x0000182d
    .word 0x30820001
    .word 0x24a5ffff
    .word 0x00621825
    .word 0x00042042
    .word 0x00000000
    .word 0x00000000
    .word 0x1ca0fff9
    .word 0x00031840
    .word 0x03e00008
    .word 0x00031042
    .word 0x8c8316d4
    .word 0x24020010
    .word 0x0080302d
    .word 0x10620012
    .word 0x28640008
    .word 0x1480000e
    .word 0x00000000
    .word 0x8cc3001c
    .word 0x8cc20008
    .word 0x90c416d0
    .word 0x00431021
    .word 0x24630001
    .word 0xa0440000
    .word 0xacc3001c
    .word 0x94c216d0
    .word 0x8cc316d4
    .word 0x00021202
    .word 0x2463fff8
    .word 0xa4c216d0
    .word 0xacc316d4
    .word 0x03e00008
    .word 0x00000000
    .word 0x8cc4001c
    .word 0x8cc20008
    .word 0x90c316d0
    .word 0x00441021
    .word 0x24840001
    .word 0xa0430000
    .word 0x24850001
    .word 0x94c316d0
    .word 0x8cc20008
    .word 0x00031a02
    .word 0x00441021
    .word 0xa0430000
    .word 0xacc5001c
    .word 0xacc016d4
    .word 0x03e00008
    .word 0xa4c016d0
    .word 0x8c8316d4
    .word 0x28620009
    .word 0x14400011
    .word 0x0080302d
    .word 0x8c84001c
    .word 0x8cc20008
    .word 0x90c316d0
    .word 0x00441021
    .word 0x24840001
    .word 0xa0430000
    .word 0x24850001
    .word 0x8cc30008
    .word 0x94c216d0
    .word 0x00641821
    .word 0x00021202
    .word 0xa0620000
    .word 0xacc5001c
    .word 0xacc016d4
    .word 0x03e00008
    .word 0xa4c016d0
    .word 0x5860fffd
    .word 0xacc016d4
    .word 0x8c82001c
    .word 0x8c830008
    .word 0x908416d0
    .word 0x00621821
    .word 0x24420001
    .word 0xa0640000
    .word 0x1000fff4
    .word 0xacc2001c
    .word 0x27bdffb0
    .word 0xffb30030
    .word 0x00a0982d
    .word 0xffb20020
    .word 0x00c0902d
    .word 0xffb10010
    .word 0x0080882d
    .word 0xffb00000
    .word 0xffbf0040
    .word 0x0c066241
    .word 0x00e0802d
    .word 0x24020008
    .word 0x12000014
    .word 0xae2216cc
    .word 0x8e23001c
    .word 0x00122a02
    .word 0x8e220008
    .word 0x00122027
    .word 0x00043a02
    .word 0x00431021
    .word 0x24630001
    .word 0xa0520000
    .word 0x24660003
    .word 0x8e220008
    .word 0x00431021
    .word 0xa0450000
    .word 0x8e220008
    .word 0x00621021
    .word 0xa0440001
    .word 0x8e220008
    .word 0x00621821
    .word 0xa0670002
    .word 0xae26001c
    .word 0x2652ffff
    .word 0x2402ffff
    .word 0x1242000d
    .word 0xdfbf0040
    .word 0x8e23001c
    .word 0x2652ffff
    .word 0x8e220008
    .word 0x92640000
    .word 0x26730001
    .word 0x00431021
    .word 0x24630001
    .word 0xa0440000
    .word 0x2402ffff
    .word 0x1642fff6
    .word 0xae23001c
    .word 0xdfbf0040
    .word 0xdfb30030
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0050
    .word 0x3c02001c
    .word 0x03e00008
    .word 0x2442a240
    .word 0x24020002
    .word 0x3c030042
    .word 0x00441023
    .size stage3p_tail_gap_00197000, .-stage3p_tail_gap_00197000

    /* Exact public EE reconstruction @ 0x00198bfc..0x00198c04
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_00198bfc
    .type stage3p_tail_gap_00198bfc, @function
stage3p_tail_gap_00198bfc:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00198bfc, .-stage3p_tail_gap_00198bfc

    /* Exact public EE reconstruction @ 0x00199170..0x00199178
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_00199170
    .type stage3p_tail_gap_00199170, @function
stage3p_tail_gap_00199170:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00199170, .-stage3p_tail_gap_00199170

    /* Exact public EE reconstruction @ 0x00199288..0x00199290
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_00199288
    .type stage3p_tail_gap_00199288, @function
stage3p_tail_gap_00199288:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_00199288, .-stage3p_tail_gap_00199288

    /* Exact public EE reconstruction @ 0x0019bd5c..0x0019bd64
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_0019bd5c
    .type stage3p_tail_gap_0019bd5c, @function
stage3p_tail_gap_0019bd5c:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0019bd5c, .-stage3p_tail_gap_0019bd5c

    /* Exact public EE reconstruction @ 0x0019bd70..0x0019bd78
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_0019bd70
    .type stage3p_tail_gap_0019bd70, @function
stage3p_tail_gap_0019bd70:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0019bd70, .-stage3p_tail_gap_0019bd70

    /* Exact public EE reconstruction @ 0x0019be18..0x0019be20
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_0019be18
    .type stage3p_tail_gap_0019be18, @function
stage3p_tail_gap_0019be18:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0019be18, .-stage3p_tail_gap_0019be18

    /* Exact public EE reconstruction @ 0x0019be5c..0x0019be64
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_0019be5c
    .type stage3p_tail_gap_0019be5c, @function
stage3p_tail_gap_0019be5c:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0019be5c, .-stage3p_tail_gap_0019be5c

    /* Exact public EE reconstruction @ 0x0019beb4..0x0019becc
     * target span SHA-256: 9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0
     */
    .balign 4
    .globl stage3p_tail_gap_0019beb4
    .type stage3p_tail_gap_0019beb4, @function
stage3p_tail_gap_0019beb4:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0019beb4, .-stage3p_tail_gap_0019beb4

    /* Exact public EE reconstruction @ 0x0019c050..0x0019c05c
     * target span SHA-256: 15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b
     */
    .balign 4
    .globl stage3p_tail_gap_0019c050
    .type stage3p_tail_gap_0019c050, @function
stage3p_tail_gap_0019c050:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0019c050, .-stage3p_tail_gap_0019c050

    /* Exact public EE reconstruction @ 0x0019c0b4..0x0019c0c0
     * target span SHA-256: 15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b
     */
    .balign 4
    .globl stage3p_tail_gap_0019c0b4
    .type stage3p_tail_gap_0019c0b4, @function
stage3p_tail_gap_0019c0b4:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0019c0b4, .-stage3p_tail_gap_0019c0b4

    /* Exact public EE reconstruction @ 0x0019cfb8..0x0019cfc0
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_0019cfb8
    .type stage3p_tail_gap_0019cfb8, @function
stage3p_tail_gap_0019cfb8:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_0019cfb8, .-stage3p_tail_gap_0019cfb8

    /* Exact public EE reconstruction @ 0x0019e364..0x0019e414
     * target span SHA-256: f307710d81d98e536bd7c418bc54d86fca64c28bcfaeeb40bf9558f19bca3ea8
     */
    .balign 4
    .globl stage3p_tail_gap_0019e364
    .type stage3p_tail_gap_0019e364, @function
stage3p_tail_gap_0019e364:
    .word 0x27bdfff0
    .word 0x00c0382d
    .word 0x00a0302d
    .word 0xffbf0000
    .word 0x0c0678b8
    .word 0x24051000
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x27bdff90
    .word 0xffa50038
    .word 0x27a50038
    .word 0xffbf0000
    .word 0xffa60040
    .word 0xffa70048
    .word 0xffa80050
    .word 0xffa90058
    .word 0xffaa0060
    .word 0xffab0068
    .word 0xe7ac0028
    .word 0xe7ae002c
    .word 0xe7b00030
    .word 0x0c067eaa
    .word 0xe7b20034
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0070
    .word 0x27bdffa0
    .word 0xffa60030
    .word 0x27a60030
    .word 0xffbf0000
    .word 0xffa70038
    .word 0xffa80040
    .word 0xffa90048
    .word 0xffaa0050
    .word 0xffab0058
    .word 0xe7ac0020
    .word 0xe7ae0024
    .word 0xe7b00028
    .word 0x0c0678d9
    .word 0xe7b2002c
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0060
    .size stage3p_tail_gap_0019e364, .-stage3p_tail_gap_0019e364

    /* Exact public EE reconstruction @ 0x0019f014..0x0019f018
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_0019f014
    .type stage3p_tail_gap_0019f014, @function
stage3p_tail_gap_0019f014:
    .word 0x00000000
    .size stage3p_tail_gap_0019f014, .-stage3p_tail_gap_0019f014

    /* Exact public EE reconstruction @ 0x0019f594..0x0019f5d0
     * target span SHA-256: cdfb19cd2ed92b4d62c6d1f5d9d5f106be272c7a57f9ad6907893f5d3ec6d633
     */
    .balign 4
    .globl stage3p_tail_gap_0019f594
    .type stage3p_tail_gap_0019f594, @function
stage3p_tail_gap_0019f594:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x24030012
    .word 0x0000000c
    .word 0x03e00008
    .word 0x00000000
    .word 0x24030013
    .word 0x0000000c
    .word 0x03e00008
    .word 0x00000000
    .word 0x2403003e
    .word 0x0000000c
    .word 0x03e00008
    .word 0x00000000
    .size stage3p_tail_gap_0019f594, .-stage3p_tail_gap_0019f594

    /* Exact public EE reconstruction @ 0x001a0694..0x001a06a0
     * target span SHA-256: 15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b
     */
    .balign 4
    .globl stage3p_tail_gap_001a0694
    .type stage3p_tail_gap_001a0694, @function
stage3p_tail_gap_001a0694:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a0694, .-stage3p_tail_gap_001a0694

    /* Exact public EE reconstruction @ 0x001a06a8..0x001a06b0
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a06a8
    .type stage3p_tail_gap_001a06a8, @function
stage3p_tail_gap_001a06a8:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a06a8, .-stage3p_tail_gap_001a06a8

    /* Exact public EE reconstruction @ 0x001a06b8..0x001a06c0
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a06b8
    .type stage3p_tail_gap_001a06b8, @function
stage3p_tail_gap_001a06b8:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a06b8, .-stage3p_tail_gap_001a06b8

    /* Exact public EE reconstruction @ 0x001a06d4..0x001a06dc
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a06d4
    .type stage3p_tail_gap_001a06d4, @function
stage3p_tail_gap_001a06d4:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a06d4, .-stage3p_tail_gap_001a06d4

    /* Exact public EE reconstruction @ 0x001a0700..0x001a0708
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a0700
    .type stage3p_tail_gap_001a0700, @function
stage3p_tail_gap_001a0700:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a0700, .-stage3p_tail_gap_001a0700

    /* Exact public EE reconstruction @ 0x001a1c90..0x001a1c98
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a1c90
    .type stage3p_tail_gap_001a1c90, @function
stage3p_tail_gap_001a1c90:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a1c90, .-stage3p_tail_gap_001a1c90

    /* Exact public EE reconstruction @ 0x001a3464..0x001a3470
     * target span SHA-256: 15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b
     */
    .balign 4
    .globl stage3p_tail_gap_001a3464
    .type stage3p_tail_gap_001a3464, @function
stage3p_tail_gap_001a3464:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a3464, .-stage3p_tail_gap_001a3464

    /* Exact public EE reconstruction @ 0x001a3498..0x001a34a0
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a3498
    .type stage3p_tail_gap_001a3498, @function
stage3p_tail_gap_001a3498:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a3498, .-stage3p_tail_gap_001a3498

    /* Exact public EE reconstruction @ 0x001a3bc4..0x001a3bd0
     * target span SHA-256: 15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b
     */
    .balign 4
    .globl stage3p_tail_gap_001a3bc4
    .type stage3p_tail_gap_001a3bc4, @function
stage3p_tail_gap_001a3bc4:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a3bc4, .-stage3p_tail_gap_001a3bc4

    /* Exact public EE reconstruction @ 0x001a3e28..0x001a3e30
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a3e28
    .type stage3p_tail_gap_001a3e28, @function
stage3p_tail_gap_001a3e28:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a3e28, .-stage3p_tail_gap_001a3e28

    /* Exact public EE reconstruction @ 0x001a4100..0x001a5c58
     * target span SHA-256: 963e41001ec626f85902dd3f6b88c8ac032d5c7173f28d0f1873fb92a3e1a8c1
     */
    .balign 4
    .globl stage3p_tail_gap_001a4100
    .type stage3p_tail_gap_001a4100, @function
stage3p_tail_gap_001a4100:
    .word 0x03e00008
    .word 0x8c82014c
    .word 0x27bdff50
    .word 0xffb10020
    .word 0x24910009
    .word 0xffb00010
    .word 0x0080802d
    .word 0x0220202d
    .word 0xffbe0090
    .word 0xffb70080
    .word 0x0000f02d
    .word 0xffb30040
    .word 0x00a0b82d
    .word 0xffbf00a0
    .word 0x00c0982d
    .word 0xffb60070
    .word 0xffb50060
    .word 0xffb40050
    .word 0x0c06717a
    .word 0xffb20030
    .word 0x92030009
    .word 0x02222821
    .word 0x24020065
    .word 0x1062004c
    .word 0x24a40001
    .word 0x0c068fba
    .word 0x26650530
    .word 0x26650528
    .word 0x0c068fca
    .word 0x0040202d
    .word 0x90430000
    .word 0x0040202d
    .word 0x2402ffff
    .word 0xa262053a
    .word 0x2402007a
    .word 0xa2630538
    .word 0x92230000
    .word 0x10430034
    .word 0x24840001
    .word 0x306200ff
    .word 0x10400017
    .word 0x2414004c
    .word 0x24150052
    .word 0x24160050
    .word 0x306200ff
    .word 0x50540028
    .word 0x90820000
    .word 0x24920001
    .word 0x1055001f
    .word 0x02e0282d
    .word 0x1456001b
    .word 0x26310001
    .word 0x90900000
    .word 0x0c068f8c
    .word 0x0200202d
    .word 0x26670524
    .word 0x0200202d
    .word 0x0040282d
    .word 0x0c068fe2
    .word 0x0240302d
    .word 0x0040202d
    .word 0x92230000
    .word 0x1460ffee
    .word 0x306200ff
    .word 0x0080102d
    .word 0x03de100b
    .word 0xdfbf00a0
    .word 0xdfbe0090
    .word 0xdfb70080
    .word 0xdfb60070
    .word 0xdfb50060
    .word 0xdfb40050
    .word 0xdfb30040
    .word 0xdfb20030
    .word 0xdfb10020
    .word 0xdfb00010
    .word 0x03e00008
    .word 0x27bd00b0
    .word 0x1000fff3
    .word 0x03c0102d
    .word 0x90820000
    .word 0x26310001
    .word 0x0240202d
    .word 0x1000ffe9
    .word 0xa2620539
    .word 0x00000000
    .word 0x26310001
    .word 0x24840001
    .word 0x1000ffe4
    .word 0xa262053a
    .word 0x03a0282d
    .word 0x0c068fba
    .word 0x26310001
    .word 0x8fa30000
    .word 0x0040202d
    .word 0x24020001
    .word 0xa262053b
    .word 0x0083f021
    .word 0x1000ffc4
    .word 0x92230000
    .word 0x92230001
    .word 0x24020068
    .word 0x1462ffb2
    .word 0x00000000
    .word 0x88820003
    .word 0x98820000
    .word 0x2611000b
    .word 0x24a40005
    .word 0x1000ffac
    .word 0xae62053c
    .word 0x27bdfd70
    .word 0x0085102b
    .word 0xffb40270
    .word 0x00c0a02d
    .word 0xffb30260
    .word 0x00a0982d
    .word 0xffb20250
    .word 0x24120001
    .word 0xffb10240
    .word 0x0080882d
    .word 0xffbf0280
    .word 0xffb00230
    .word 0x1040001b
    .word 0xffa70000
    .word 0x92300000
    .word 0x2603fffd
    .word 0x2c620094
    .word 0x10400150
    .word 0x26310001
    .word 0x00031080
    .word 0x3c03001c
    .word 0x2463a82c
    .word 0x00431021
    .word 0x8c440000
    .word 0x00800008
    .word 0x00000000
    .word 0x2602ffd0
    .word 0x0002103c
    .word 0x0002103e
    .word 0xffa20200
    .word 0x2e420040
    .word 0x10400142
    .word 0x26520001
    .word 0xdfa30200
    .word 0x001210c0
    .word 0x005d1021
    .word 0xfc430000
    .word 0x0233102b
    .word 0x5440ffe8
    .word 0x92300000
    .word 0x2652ffff
    .word 0x06400138
    .word 0x001218c0
    .word 0xdfbf0280
    .word 0x007d1821
    .word 0xdfb40270
    .word 0xdc620000
    .word 0xdfb30260
    .word 0xdfb20250
    .word 0xdfb10240
    .word 0xdfb00230
    .word 0x03e00008
    .word 0x27bd0290
    .word 0x92220000
    .word 0x1000ffe6
    .word 0x26310001
    .word 0x1000fffd
    .word 0x82220000
    .word 0x92220001
    .word 0x92230000
    .word 0x26310002
    .word 0x00021238
    .word 0x00431025
    .word 0x1000ffdd
    .word 0x3042ffff
    .word 0x92220001
    .word 0x92230000
    .word 0x26310002
    .word 0x00021238
    .word 0x00431025
    .word 0x0002143c
    .word 0x0002143f
    .word 0x3042ffff
    .word 0x00021400
    .word 0x00021403
    .word 0x1000ffd2
    .word 0xffa20200
    .word 0x8a220003
    .word 0x9a220000
    .word 0x1000ffcb
    .word 0x26310004
    .word 0x8a220003
    .word 0x9a220000
    .word 0x1000fff7
    .word 0x26310004
    .word 0x6a220007
    .word 0x6e220000
    .word 0x1000ffc5
    .word 0x26310008
    .word 0x0220202d
    .word 0x0c068fba
    .word 0x27a50200
    .word 0x1000ffc1
    .word 0x0040882d
    .word 0x0220202d
    .word 0x0c068fca
    .word 0x27a50208
    .word 0x0040882d
    .word 0x1000ffba
    .word 0xdfa20208
    .word 0x1a4000fd
    .word 0x001210c0
    .word 0x005d1021
    .word 0xdc43fff8
    .word 0x1000ffb5
    .word 0xffa30200
    .word 0x2652ffff
    .word 0x0641ffba
    .word 0x0233102b
    .word 0x100000f4
    .word 0x00000000
    .word 0x00000000
    .word 0x2a420002
    .word 0x144000f0
    .word 0x001210c0
    .word 0x005d1021
    .word 0x1000fff3
    .word 0xdc43fff0
    .word 0x92230000
    .word 0x2642ffff
    .word 0x26310001
    .word 0x0062102a
    .word 0x104000e7
    .word 0xffa30218
    .word 0x8fa20218
    .word 0x02421023
    .word 0x1000ffe7
    .word 0x000210c0
    .word 0x2a420003
    .word 0x144000e0
    .word 0x001210c0
    .word 0x005d1021
    .word 0xdc43fff8
    .word 0xdc44fff0
    .word 0xdc45ffe8
    .word 0xfc44fff8
    .word 0xfc43ffe8
    .word 0x1000ff9b
    .word 0xfc45fff0
    .word 0x2652ffff
    .word 0x064000d5
    .word 0x001220c0
    .word 0x92220001
    .word 0x92230000
    .word 0x009d2021
    .word 0x00021238
    .word 0x00431025
    .word 0x0002143c
    .word 0x0002143f
    .word 0x3042ffff
    .word 0x00021400
    .word 0x00021403
    .word 0xffa20218
    .word 0xdc830000
    .word 0x1060ff8a
    .word 0x26310002
    .word 0x8fa20218
    .word 0x1000ff87
    .word 0x02228821
    .word 0x2652fffe
    .word 0x064000c1
    .word 0x2604ffe6
    .word 0x001210c0
    .word 0x005d1021
    .word 0x2c830015
    .word 0xdc460008
    .word 0x106000bb
    .word 0xdc450000
    .word 0x3c03001c
    .word 0x00041080
    .word 0x2463aa7c
    .word 0x00431021
    .word 0x8c440000
    .word 0x00800008
    .word 0x00000000
    .word 0x1000ff6d
    .word 0x00a61024
    .word 0x00a0202d
    .word 0x0c06876e
    .word 0x00c0282d
    .word 0x1000ff69
    .word 0xffa20200
    .word 0x1000ff66
    .word 0x00a6102f
    .word 0x00a0202d
    .word 0x0c069d8e
    .word 0x00c0282d
    .word 0x1000ff62
    .word 0xffa20200
    .word 0x00a0202d
    .word 0x0c0686c8
    .word 0x00c0282d
    .word 0x1000ff5d
    .word 0xffa20200
    .word 0x1000ff5a
    .word 0x00a61025
    .word 0x1000ff58
    .word 0x00a6102d
    .word 0x0006103c
    .word 0x0002103f
    .word 0x1000ff54
    .word 0x00451014
    .word 0x0006103c
    .word 0x0002103f
    .word 0x1000ff50
    .word 0x00451016
    .word 0x0006103c
    .word 0x0002103f
    .word 0x1000ff4c
    .word 0x00451017
    .word 0x1000ff4a
    .word 0x00a61026
    .word 0x00c51026
    .word 0x1000ff47
    .word 0x2c420001
    .word 0x00c5102a
    .word 0x1000ff44
    .word 0x38420001
    .word 0x1000ff42
    .word 0x00a6102a
    .word 0x1000fffb
    .word 0x00a6102a
    .word 0x1000ff3e
    .word 0x00c5102a
    .word 0x00c51026
    .word 0x1000ff3b
    .word 0x0002102b
    .word 0x00000000
    .word 0x92220001
    .word 0x92230000
    .word 0x26310002
    .word 0x00021238
    .word 0x00431025
    .word 0x0002143c
    .word 0x0002143f
    .word 0x3042ffff
    .word 0x00021400
    .word 0x00021403
    .word 0xffa20218
    .word 0x8fa30218
    .word 0x1000ff34
    .word 0x02238821
    .word 0x2602ffb0
    .word 0x00021080
    .word 0x00541021
    .word 0x8c430000
    .word 0xdc640000
    .word 0x1000ff26
    .word 0xffa40200
    .word 0x0220202d
    .word 0x0c068fca
    .word 0x27a50218
    .word 0x0040882d
    .word 0x2602ff90
    .word 0x00021080
    .word 0x00541021
    .word 0x8c440000
    .word 0xdfa20218
    .word 0xdc830000
    .word 0x1000ff64
    .word 0x0062182d
    .word 0x0220202d
    .word 0x0c068fba
    .word 0x27a50210
    .word 0x0040882d
    .word 0xdfa20210
    .word 0x0002103c
    .word 0x0002103f
    .word 0x1000ffe7
    .word 0x00021080
    .word 0x0220202d
    .word 0x0c068fba
    .word 0x27a50210
    .word 0x27a50218
    .word 0x0c068fca
    .word 0x0040202d
    .word 0x0040882d
    .word 0xdfa20210
    .word 0x0002103c
    .word 0x0002103f
    .word 0x1000ffe6
    .word 0x00021080
    .word 0x2652ffff
    .word 0x06400046
    .word 0x001210c0
    .word 0x005d1021
    .word 0xdc430000
    .word 0x2402001f
    .word 0x1202000c
    .word 0xffa30200
    .word 0x2e020020
    .word 0x10400011
    .word 0x24020023
    .word 0x24020006
    .word 0x12020008
    .word 0x00000000
    .word 0x24020019
    .word 0x16020038
    .word 0x00000000
    .word 0x0463fef3
    .word 0x2e420040
    .word 0x1000feef
    .word 0x0003102f
    .word 0x0003183c
    .word 0x0003183f
    .word 0x88620003
    .word 0x98620000
    .word 0x1000fee8
    .word 0x0002103c
    .word 0x12020024
    .word 0x0220202d
    .word 0x2e020024
    .word 0x10400006
    .word 0x24020094
    .word 0x24020020
    .word 0x16020026
    .word 0x00031027
    .word 0x1000fee0
    .word 0xffa20200
    .word 0x16020022
    .word 0x24020002
    .word 0x92240000
    .word 0x0003183c
    .word 0x0003183f
    .word 0x10820012
    .word 0x26310001
    .word 0x28820003
    .word 0x10400006
    .word 0x24020004
    .word 0x24020001
    .word 0x14820017
    .word 0x00000000
    .word 0x1000fed0
    .word 0x90620000
    .word 0x1082ffe2
    .word 0x00000000
    .word 0x24020008
    .word 0x14820010
    .word 0x00000000
    .word 0x68620007
    .word 0x6c620000
    .word 0x1000fec8
    .word 0xffa20200
    .word 0x90620001
    .word 0x1000fee4
    .word 0x90630000
    .word 0x0c068fba
    .word 0x27a50220
    .word 0xdfa30220
    .word 0x0040882d
    .word 0xdfa20200
    .word 0x1000febd
    .word 0x0043102d
    .word 0x00000000
    .word 0x0c041d5e
    .word 0x00000000
    .word 0x27bdff30
    .word 0x0085102b
    .word 0xffbe00b0
    .word 0x03a0f02d
    .word 0xffb600a0
    .word 0x0000b02d
    .word 0xffb50090
    .word 0x00a0a82d
    .word 0xffb40080
    .word 0x00c0a02d
    .word 0xffb30070
    .word 0x00e0982d
    .word 0xffb20060
    .word 0x0080902d
    .word 0xffbf00c0
    .word 0xffb10050
    .word 0xffb00040
    .word 0x1040002e
    .word 0xace00500
    .word 0x8cf00520
    .word 0x8cc20144
    .word 0x0202102b
    .word 0x10400029
    .word 0x00000000
    .word 0x92430000
    .word 0x24020040
    .word 0x306500c0
    .word 0x10a2012a
    .word 0x26520001
    .word 0x24020080
    .word 0x10a20119
    .word 0x3066003f
    .word 0x240200c0
    .word 0x10a20112
    .word 0x00000000
    .word 0x2c620030
    .word 0x10400124
    .word 0x00031080
    .word 0x3c03001c
    .word 0x2463aad0
    .word 0x00431021
    .word 0x8c440000
    .word 0x00800008
    .word 0x00000000
    .word 0x92700539
    .word 0x0280282d
    .word 0x26710520
    .word 0x0c068f8c
    .word 0x0200202d
    .word 0x0220382d
    .word 0x0200202d
    .word 0x0040282d
    .word 0x0c068fe2
    .word 0x0240302d
    .word 0x0040902d
    .word 0x00000000
    .word 0x0255102b
    .word 0x10400006
    .word 0x00000000
    .word 0x8e630520
    .word 0x8e820144
    .word 0x0062102b
    .word 0x1440ffd9
    .word 0x0060802d
    .word 0x03c0e82d
    .word 0xdfbf00c0
    .word 0xdfbe00b0
    .word 0xdfb600a0
    .word 0xdfb50090
    .word 0xdfb40080
    .word 0xdfb30070
    .word 0xdfb20060
    .word 0xdfb10050
    .word 0xdfb00040
    .word 0x03e00008
    .word 0x27bd00d0
    .word 0x92440000
    .word 0x26520001
    .word 0xde650530
    .word 0x00000000
    .word 0x0c0686c8
    .word 0x00000000
    .word 0x0002103c
    .word 0x0002103f
    .word 0x02021021
    .word 0x1000ffe2
    .word 0xae620520
    .word 0x92440001
    .word 0x92420000
    .word 0x26520002
    .word 0x00042238
    .word 0xde650530
    .word 0x00822025
    .word 0x1000fff2
    .word 0x3084ffff
    .word 0x8a440003
    .word 0x9a440000
    .word 0x26520004
    .word 0xde650530
    .word 0x0004203c
    .word 0x1000ffeb
    .word 0x0004203e
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x27c50008
    .word 0x03c0282d
    .word 0x0c068fba
    .word 0x0040202d
    .word 0xdfc40000
    .word 0xde650528
    .word 0x0040902d
    .word 0x0c0686c8
    .word 0x00000000
    .word 0x0040202d
    .word 0x8fc20008
    .word 0x00021100
    .word 0x24030001
    .word 0x00531021
    .word 0xfc440000
    .word 0x1000ffc0
    .word 0xac430008
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x27c50008
    .word 0x0040902d
    .word 0x8fc20008
    .word 0x00021100
    .word 0x00531021
    .word 0x1000ffb7
    .word 0xac400008
    .word 0x0240202d
    .word 0x27c50008
    .word 0x0c068fba
    .word 0x00000000
    .word 0x1000ffb1
    .word 0x0040902d
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x27c50008
    .word 0x27c50010
    .word 0x0c068fba
    .word 0x0040202d
    .word 0x24040002
    .word 0x0040902d
    .word 0xdfc30010
    .word 0x8fc20008
    .word 0x00021100
    .word 0x00531021
    .word 0xfc430000
    .word 0x1000ffa2
    .word 0xac440008
    .word 0x52c00018
    .word 0x27bdfaf0
    .word 0x02c0282d
    .word 0x8ed60500
    .word 0x00a0182d
    .word 0x0260102d
    .word 0x26640500
    .word 0xdc460000
    .word 0xdc470008
    .word 0xdc480010
    .word 0xdc490018
    .word 0xfc660000
    .word 0xfc670008
    .word 0xfc680010
    .word 0xfc690018
    .word 0x24420020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1444fff3
    .word 0x24630020
    .word 0xdc440000
    .word 0xfc640000
    .word 0x1000ff89
    .word 0xae650500
    .word 0x1000ffea
    .word 0x03a0282d
    .word 0x8e650500
    .word 0x0260182d
    .word 0x00a0102d
    .word 0x24a40500
    .word 0xdc490000
    .word 0xdc460008
    .word 0xdc470010
    .word 0xdc480018
    .word 0xfc690000
    .word 0xfc660008
    .word 0xfc670010
    .word 0xfc680018
    .word 0x24420020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1444fff3
    .word 0x24630020
    .word 0xdc490000
    .word 0xfc690000
    .word 0xacb60500
    .word 0x1000ff70
    .word 0x00a0b02d
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x26650510
    .word 0x03c0282d
    .word 0x0c068fba
    .word 0x0040202d
    .word 0xdfc30000
    .word 0x0040902d
    .word 0x24020001
    .word 0xae62051c
    .word 0x1000ff64
    .word 0xfe630508
    .word 0x26650510
    .word 0x0c068fba
    .word 0x0240202d
    .word 0x0040902d
    .word 0x24020001
    .word 0x1000ff5d
    .word 0xae62051c
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x03c0282d
    .word 0x0040902d
    .word 0xdfc20000
    .word 0x1000ff56
    .word 0xfe620508
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x03c0282d
    .word 0x24030002
    .word 0x0040902d
    .word 0xae620518
    .word 0x8fc20000
    .word 0xae63051c
    .word 0x1000ff4c
    .word 0x02429021
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x27c50008
    .word 0x03c0282d
    .word 0x0c068fba
    .word 0x0040202d
    .word 0x24040003
    .word 0x0040902d
    .word 0x8fc30000
    .word 0x8fc20008
    .word 0x00021100
    .word 0x00531021
    .word 0xac520000
    .word 0x1000ff9a
    .word 0x02439021
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x27c50008
    .word 0x27c50018
    .word 0x0c068fca
    .word 0x0040202d
    .word 0xdfc40018
    .word 0xde650528
    .word 0x1000ff6a
    .word 0x0040902d
    .word 0x26650510
    .word 0x0c068fba
    .word 0x0240202d
    .word 0x26650508
    .word 0x0c068fca
    .word 0x0040202d
    .word 0x1000ffcc
    .word 0x0040902d
    .word 0x26650508
    .word 0x0c068fca
    .word 0x0240202d
    .word 0x1000ff26
    .word 0x0040902d
    .word 0x24060010
    .word 0x24050001
    .word 0xffc60008
    .word 0x0000202d
    .word 0x00000000
    .word 0x8fc20008
    .word 0x64c60001
    .word 0x2cc30020
    .word 0xffc60008
    .word 0x00021100
    .word 0x00531021
    .word 0xfc440000
    .word 0x64840004
    .word 0x1460fff7
    .word 0xac450008
    .word 0x1000ff16
    .word 0x0255102b
    .word 0x26850158
    .word 0x1000ff5e
    .word 0x0240202d
    .word 0x0240202d
    .word 0x0c068fba
    .word 0x27c50008
    .word 0x03c0282d
    .word 0x0c068fba
    .word 0x0040202d
    .word 0xdfc40000
    .word 0xde650528
    .word 0x0c0686c8
    .word 0x0040902d
    .word 0x1000ff40
    .word 0x0002202f
    .word 0x00000000
    .word 0x0006103c
    .word 0x0002103f
    .word 0x1000ff47
    .word 0xffc60008
    .word 0x0240202d
    .word 0x03c0282d
    .word 0x0c068fba
    .word 0xffc60008
    .word 0xdfc40000
    .word 0xde650528
    .word 0x0c0686c8
    .word 0x0040902d
    .word 0x0040202d
    .word 0xdfc20008
    .word 0x0002103c
    .word 0x0002103f
    .word 0x1000ff2f
    .word 0x00021100
    .word 0xde650530
    .word 0x1000ff08
    .word 0x3064003f
    .word 0x0c041d5e
    .word 0x00000000
    .word 0x00000000
    .word 0x27bdff60
    .word 0x24060540
    .word 0xffb40050
    .word 0x00a0a02d
    .word 0xffb10020
    .word 0x0000282d
    .word 0x0080882d
    .word 0xffbf0090
    .word 0x0280202d
    .word 0xffb50060
    .word 0xffb70080
    .word 0xffb60070
    .word 0xffb30040
    .word 0xffb20030
    .word 0x0c0670e7
    .word 0xffb00010
    .word 0x8e240144
    .word 0x2625014c
    .word 0xfe200158
    .word 0xae200148
    .word 0x0c069bcc
    .word 0x2484ffff
    .word 0x0040a82d
    .word 0x12a00034
    .word 0x24020005
    .word 0x8ea30004
    .word 0x0220282d
    .word 0x8e220154
    .word 0x0280302d
    .word 0x02a38023
    .word 0xae820520
    .word 0x26160004
    .word 0x0c069042
    .word 0x02c0202d
    .word 0x0040902d
    .word 0x12400028
    .word 0x24020003
    .word 0x8e020004
    .word 0x0240202d
    .word 0x0220302d
    .word 0x0280382d
    .word 0x02c21021
    .word 0x26b30008
    .word 0x24450004
    .word 0x0c069212
    .word 0x26370148
    .word 0x0000902d
    .word 0x0c068f70
    .word 0x92840539
    .word 0x03a0282d
    .word 0x9283053b
    .word 0x00021040
    .word 0x14600023
    .word 0x02629821
    .word 0x9290053a
    .word 0x240200ff
    .word 0x0220282d
    .word 0x12020009
    .word 0x0200202d
    .word 0x0c068f8c
    .word 0x00000000
    .word 0x02e0382d
    .word 0x0260302d
    .word 0x0200202d
    .word 0x0c068fe2
    .word 0x0040282d
    .word 0x0040982d
    .word 0x8ea20000
    .word 0x0272900a
    .word 0x0220302d
    .word 0x0280382d
    .word 0x02a21021
    .word 0x0240202d
    .word 0x0c069212
    .word 0x24450004
    .word 0x0000102d
    .word 0xdfbf0090
    .word 0xdfb70080
    .word 0xdfb60070
    .word 0xdfb50060
    .word 0xdfb40050
    .word 0xdfb30040
    .word 0xdfb20030
    .word 0xdfb10020
    .word 0xdfb00010
    .word 0x03e00008
    .word 0x27bd00a0
    .word 0x00000000
    .word 0x0c068fba
    .word 0x0260202d
    .word 0x0040982d
    .word 0x8fa20000
    .word 0x1000ffd9
    .word 0x02629021
    .word 0x27bdf930
    .word 0x24060160
    .word 0xffb106b0
    .word 0x00a0882d
    .word 0xffb006a0
    .word 0x0000282d
    .word 0x0080802d
    .word 0xffbf06c0
    .word 0x03a0202d
    .word 0x0c0670e7
    .word 0x26100001
    .word 0xafb00144
    .word 0x03a0202d
    .word 0x0c06935e
    .word 0x27a50160
    .word 0x14400022
    .word 0x0000302d
    .word 0x8fa3067c
    .word 0x24020002
    .word 0x1062001e
    .word 0x26250018
    .word 0x24080001
    .word 0x24090002
    .word 0x2624029c
    .word 0x03a0302d
    .word 0x2407004f
    .word 0x90c20168
    .word 0x00021e00
    .word 0x00031e03
    .word 0x1068001a
    .word 0xa0820000
    .word 0x50690019
    .word 0xdcc20160
    .word 0xfca00000
    .word 0x24e7ffff
    .word 0x24a50008
    .word 0x24840001
    .word 0x04e1fff4
    .word 0x24c60010
    .word 0xdfa20668
    .word 0x0220302d
    .word 0x93a40698
    .word 0x97a30670
    .word 0xdfa50158
    .word 0xfe220008
    .word 0x8fa2069c
    .word 0xa6230298
    .word 0xa624029a
    .word 0xfe250010
    .word 0xae220004
    .word 0xdfbf06c0
    .word 0x00c0102d
    .word 0xdfb106b0
    .word 0xdfb006a0
    .word 0x03e00008
    .word 0x27bd06d0
    .word 0xdcc20160
    .word 0x1000ffe8
    .word 0xfca20000
    .word 0x00000000
    .word 0x27bdfe00
    .word 0x0080102d
    .word 0xffbf01f0
    .word 0x03a0182d
    .word 0xffb701e0
    .word 0xffb601d0
    .word 0xffb501c0
    .word 0xffb401b0
    .word 0xffb301a0
    .word 0xffb10180
    .word 0xffb20190
    .word 0x00a0902d
    .word 0xffb00170
    .word 0x0080802d
    .word 0x24840160
    .word 0x00000000
    .word 0xdc450000
    .word 0xdc460008
    .word 0xdc470010
    .word 0xdc480018
    .word 0xfc650000
    .word 0xfc660008
    .word 0xfc670010
    .word 0xfc680018
    .word 0x24420020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1444fff3
    .word 0x24630020
    .word 0x8e43051c
    .word 0x24020001
    .word 0x10620045
    .word 0x24020002
    .word 0x14620057
    .word 0x27a50160
    .word 0x8e420518
    .word 0x0c068fba
    .word 0x0040202d
    .word 0x8fa50160
    .word 0x0000382d
    .word 0x0040202d
    .word 0x0200302d
    .word 0x0c0690ae
    .word 0x00452821
    .word 0x0002983c
    .word 0x0013983f
    .word 0xae130140
    .word 0x0200882d
    .word 0x24140001
    .word 0x24150002
    .word 0x0240802d
    .word 0x24160003
    .word 0x0013b83c
    .word 0x2412004f
    .word 0x00000000
    .word 0x8e020008
    .word 0x5054002a
    .word 0x8e020000
    .word 0x50400006
    .word 0x6652ffff
    .word 0x50550020
    .word 0x8e020000
    .word 0x10560010
    .word 0x27a50168
    .word 0x6652ffff
    .word 0x26310004
    .word 0x0641fff4
    .word 0x26100010
    .word 0xdfbf01f0
    .word 0xdfb701e0
    .word 0xdfb601d0
    .word 0xdfb501c0
    .word 0xdfb401b0
    .word 0xdfb301a0
    .word 0xdfb20190
    .word 0xdfb10180
    .word 0xdfb00170
    .word 0x03e00008
    .word 0x27bd0200
    .word 0x8e020000
    .word 0x0c068fba
    .word 0x0040202d
    .word 0x8fa50168
    .word 0x0017383e
    .word 0x0040202d
    .word 0x03a0302d
    .word 0x0c0690ae
    .word 0x00452821
    .word 0x0002103c
    .word 0x0002103f
    .word 0x00000000
    .word 0x1000ffe4
    .word 0xae220000
    .word 0x00021080
    .word 0x005d1021
    .word 0x8c430000
    .word 0x1000ffdf
    .word 0xae230000
    .word 0x00000000
    .word 0x1000fff7
    .word 0x02621021
    .word 0x8e420510
    .word 0x00021080
    .word 0x00501021
    .word 0x8c430000
    .word 0x54600005
    .word 0xde420510
    .word 0x8e130140
    .word 0x8e420508
    .word 0x1000ffc0
    .word 0x02629821
    .word 0x0002103c
    .word 0x0002103f
    .word 0x00021080
    .word 0x00501021
    .word 0x8c430000
    .word 0xdc640000
    .word 0x0004983c
    .word 0x0013983f
    .word 0x1000fff5
    .word 0x8e420508
    .word 0x0c041d5e
    .word 0x00000000
    .word 0x27bdffd0
    .word 0xffb00000
    .word 0x00a0802d
    .word 0xffbf0020
    .word 0xffb10010
    .word 0x0c0693f8
    .word 0x0080882d
    .word 0xdfbf0020
    .word 0x92020538
    .word 0xdfb00000
    .word 0x00021080
    .word 0x00511021
    .word 0x8c440000
    .word 0x2402fffe
    .word 0xdc830000
    .word 0x0003183c
    .word 0x0003183f
    .word 0x00621824
    .word 0xae230144
    .word 0xdfb10010
    .word 0x03e00008
    .word 0x27bd0030
    .word 0x27bdfa60
    .word 0xffbf0590
    .word 0x27a20590
    .word 0xffb40580
    .word 0x00a0a02d
    .word 0xffb30570
    .word 0x0000282d
    .word 0xffb20560
    .word 0x00c0982d
    .word 0xffb10550
    .word 0x0080902d
    .word 0xffb00540
    .word 0x2411fffe
    .word 0x24060160
    .word 0x02719824
    .word 0x0c0670e7
    .word 0x8c500000
    .word 0x0240202d
    .word 0x03a0282d
    .word 0x02118024
    .word 0x0c06935e
    .word 0xae500144
    .word 0x0240202d
    .word 0x14400010
    .word 0x03a0282d
    .word 0xae540140
    .word 0x24020001
    .word 0xafa2051c
    .word 0xffa00510
    .word 0x0c0693f8
    .word 0xffa00508
    .word 0xae530144
    .word 0xdfbf0590
    .word 0xdfb40580
    .word 0xdfb30570
    .word 0xdfb20560
    .word 0xdfb10550
    .word 0xdfb00540
    .word 0x03e00008
    .word 0x27bd05a0
    .word 0x0c041d5e
    .word 0x00000000
    .word 0x27bdffb0
    .word 0x3c060044
    .word 0xffb30030
    .word 0x0080982d
    .word 0xffb10010
    .word 0x00a0882d
    .word 0xffbf0040
    .word 0xffb20020
    .word 0xffb00000
    .word 0x90c27700
    .word 0x14400052
    .word 0x24c57700
    .word 0x24020004
    .word 0x24030010
    .word 0x24040008
    .word 0xa0c37700
    .word 0xa0a3001e
    .word 0xa0a30001
    .word 0xa0a30002
    .word 0xa0a30003
    .word 0xa0a30004
    .word 0xa0a30005
    .word 0xa0a30006
    .word 0xa0a30007
    .word 0xa0a30008
    .word 0xa0a30009
    .word 0xa0a3000a
    .word 0xa0a3000b
    .word 0xa0a3000c
    .word 0xa0a3000d
    .word 0xa0a3000e
    .word 0xa0a3000f
    .word 0xa0a30010
    .word 0xa0a30011
    .word 0xa0a30012
    .word 0xa0a30013
    .word 0xa0a30014
    .word 0xa0a30015
    .word 0xa0a30016
    .word 0xa0a30017
    .word 0xa0a30018
    .word 0xa0a30019
    .word 0xa0a3001a
    .word 0xa0a3001b
    .word 0xa0a3001c
    .word 0xa0a3001d
    .word 0xa0a20020
    .word 0xa0a20021
    .word 0xa0a20022
    .word 0xa0a20023
    .word 0xa0a20024
    .word 0xa0a20025
    .word 0xa0a20026
    .word 0xa0a20027
    .word 0xa0a20028
    .word 0xa0a20029
    .word 0xa0a2002a
    .word 0xa0a2002b
    .word 0xa0a2002c
    .word 0xa0a2002d
    .word 0xa0a2002e
    .word 0xa0a2002f
    .word 0xa0a20030
    .word 0xa0a20031
    .word 0xa0a20032
    .word 0xa0a20033
    .word 0xa0a20034
    .word 0xa0a20035
    .word 0xa0a20036
    .word 0xa0a20037
    .word 0xa0a20038
    .word 0xa0a20039
    .word 0xa0a2003a
    .word 0xa0a2003b
    .word 0xa0a2003c
    .word 0xa0a2003d
    .word 0xa0a2003e
    .word 0xa0a2003f
    .word 0xa0a40040
    .word 0xa0a40041
    .word 0xa0a40042
    .word 0xa0a20043
    .word 0xa0a20044
    .word 0xa0a2004b
    .word 0xa0a4004e
    .word 0xa0a20045
    .word 0xa0a20046
    .word 0xa0a20047
    .word 0xa0a20048
    .word 0xa0a20049
    .word 0xa0a2004a
    .word 0xa0a4004c
    .word 0xa0a4004d
    .word 0x00a0902d
    .word 0x0000802d
    .word 0x00000000
    .word 0x0010203c
    .word 0x0004203f
    .word 0x66100001
    .word 0x00041080
    .word 0x00511821
    .word 0x00531021
    .word 0x8c630000
    .word 0x10600008
    .word 0x8c420000
    .word 0x10400006
    .word 0x0060282d
    .word 0x00923021
    .word 0x10620003
    .word 0x0040202d
    .word 0x0c0670d9
    .word 0x90c60000
    .word 0x2a02004f
    .word 0x1440ffee
    .word 0xdfbf0040
    .word 0x8e630140
    .word 0x8e220140
    .word 0xde240158
    .word 0x00431023
    .word 0xdfb30030
    .word 0x0044102d
    .word 0xdfb20020
    .word 0xdfb10010
    .word 0xdfb00000
    .word 0x03e00008
    .word 0x27bd0050
    .word 0x27bdfa50
    .word 0xffbf05a0
    .word 0xffb10550
    .word 0xffb50590
    .word 0x24150007
    .word 0xffb40580
    .word 0x24140008
    .word 0xffb30570
    .word 0x24130004
    .word 0xffb20560
    .word 0x0080902d
    .word 0xffb00540
    .word 0x00a0802d
    .word 0x0200202d
    .word 0x0c06935e
    .word 0x03a0282d
    .word 0x0260882d
    .word 0x0040202d
    .word 0xde430018
    .word 0x9e020144
    .word 0x24050002
    .word 0x00431026
    .word 0x14800015
    .word 0x0002880b
    .word 0x8fa20524
    .word 0x36250002
    .word 0x24040001
    .word 0x0240382d
    .word 0x14400009
    .word 0x0200402d
    .word 0x0200202d
    .word 0x16200016
    .word 0x03a0282d
    .word 0x0c069474
    .word 0x00000000
    .word 0x1000ffea
    .word 0x0200202d
    .word 0x00000000
    .word 0x0040f809
    .word 0xde460000
    .word 0x10550003
    .word 0x0040282d
    .word 0x1054fff3
    .word 0x24050002
    .word 0xdfbf05a0
    .word 0x00a0102d
    .word 0xdfb50590
    .word 0xdfb40580
    .word 0xdfb30570
    .word 0xdfb20560
    .word 0xdfb10550
    .word 0xdfb00540
    .word 0x03e00008
    .word 0x27bd05b0
    .word 0x0c041d5e
    .word 0x00000000
    .word 0x27bdf6e0
    .word 0xffa50820
    .word 0x27a208e0
    .word 0xffa40810
    .word 0x27a50920
    .word 0xafa40800
    .word 0x03a0202d
    .word 0xffbf08e0
    .word 0xffbe08d0
    .word 0xffb708c0
    .word 0xffb608b0
    .word 0xffb508a0
    .word 0xffb40890
    .word 0xffb30880
    .word 0xffb20870
    .word 0xffb10860
    .word 0xffb00850
    .word 0xffa70840
    .word 0xffa60830
    .word 0xe7bf091c
    .word 0xe7be0918
    .word 0xe7bd0914
    .word 0xe7bc0910
    .word 0xe7bb090c
    .word 0xe7ba0908
    .word 0xe7b90904
    .word 0xe7b80900
    .word 0xe7b708fc
    .word 0xe7b608f8
    .word 0xe7b508f4
    .word 0xe7b408f0
    .word 0x0c06948a
    .word 0x8c460000
    .word 0x03a0102d
    .word 0x27a40160
    .word 0x0080182d
    .word 0xdc450000
    .word 0xdc460008
    .word 0xdc470010
    .word 0xdc480018
    .word 0xfc650000
    .word 0xfc660008
    .word 0xfc670010
    .word 0xfc680018
    .word 0x24420020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1444fff3
    .word 0x24630020
    .word 0x27a40160
    .word 0x0c06935e
    .word 0x27a502c0
    .word 0x24050001
    .word 0x0040182d
    .word 0x27a80160
    .word 0x24020005
    .word 0x8fa70800
    .word 0x10620011
    .word 0x24040001
    .word 0x1460000e
    .word 0x8fa207e4
    .word 0x50400008
    .word 0x27a40160
    .word 0x0040f809
    .word 0xdce60000
    .word 0x24050006
    .word 0x10450026
    .word 0x24060008
    .word 0x14460005
    .word 0x27a40160
    .word 0x0c069474
    .word 0x27a502c0
    .word 0x1000ffe9
    .word 0x27a40160
    .word 0x24020003
    .word 0x0000182d
    .word 0xdfbf08e0
    .word 0x24630920
    .word 0xdfbe08d0
    .word 0xdfb708c0
    .word 0xdfb608b0
    .word 0xdfb508a0
    .word 0xdfb40890
    .word 0xdfb30880
    .word 0xdfb20870
    .word 0xdfb10860
    .word 0xdfb00850
    .word 0xdfa70840
    .word 0xdfa60830
    .word 0xdfa50820
    .word 0xdfa40810
    .word 0xc7bf091c
    .word 0xc7be0918
    .word 0xc7bd0914
    .word 0xc7bc0910
    .word 0xc7bb090c
    .word 0xc7ba0908
    .word 0xc7b90904
    .word 0xc7b80900
    .word 0xc7b708fc
    .word 0xc7b608f8
    .word 0xc7b508f4
    .word 0xc7b408f0
    .word 0x03e00008
    .word 0x03a3e821
    .word 0x8fa70800
    .word 0x27a40160
    .word 0x9fa202a4
    .word 0x03a0182d
    .word 0xfce00010
    .word 0xfce20018
    .word 0x0080102d
    .word 0x00000000
    .word 0xdc680000
    .word 0xdc650008
    .word 0xdc660010
    .word 0xdc670018
    .word 0xfc880000
    .word 0xfc850008
    .word 0xfc860010
    .word 0xfc870018
    .word 0x24630020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1462fff3
    .word 0x24840020
    .word 0x8fa40800
    .word 0x0c069532
    .word 0x27a50160
    .word 0x0040182d
    .word 0x24020007
    .word 0x10620003
    .word 0x03a0202d
    .word 0x1000ffc4
    .word 0x0060102d
    .word 0x0c0694b4
    .word 0x27a50160
    .word 0x8fa402a4
    .word 0x0002183c
    .word 0x0003183f
    .word 0x1000ffbe
    .word 0xffa408e0
    .word 0x27bdfa20
    .word 0xffbe05c0
    .word 0x241e0007
    .word 0xffb705b0
    .word 0x24170008
    .word 0xffb605a0
    .word 0x24160005
    .word 0xffb50590
    .word 0x2415001a
    .word 0xffb40580
    .word 0xffb30570
    .word 0xffb20560
    .word 0x0080902d
    .word 0xffb10550
    .word 0x00a0882d
    .word 0xffbf05d0
    .word 0xffb00540
    .word 0xdc820010
    .word 0xdc830018
    .word 0x0002a03c
    .word 0x0014a03f
    .word 0x0003983c
    .word 0x0013983f
    .word 0x0220202d
    .word 0x0c06935e
    .word 0x03a0282d
    .word 0x10400003
    .word 0x0040802d
    .word 0x14560020
    .word 0x24030002
    .word 0x3a020005
    .word 0x2405000a
    .word 0xde460000
    .word 0x02a2280a
    .word 0x24040001
    .word 0x0240382d
    .word 0x0220402d
    .word 0x0280f809
    .word 0x0260482d
    .word 0x14400015
    .word 0x24030002
    .word 0x24020005
    .word 0x1202001f
    .word 0x8fa20524
    .word 0x1040000c
    .word 0x0220202d
    .word 0xde460000
    .word 0x24040001
    .word 0x2405000a
    .word 0x0240382d
    .word 0x0040f809
    .word 0x0220402d
    .word 0x105e0015
    .word 0x0040802d
    .word 0x14570006
    .word 0x24030002
    .word 0x0220202d
    .word 0x0c069474
    .word 0x03a0282d
    .word 0x1000ffdc
    .word 0x0220202d
    .word 0xdfbf05d0
    .word 0x0060102d
    .word 0xdfbe05c0
    .word 0xdfb705b0
    .word 0xdfb605a0
    .word 0xdfb50590
    .word 0xdfb40580
    .word 0xdfb30570
    .word 0xdfb20560
    .word 0xdfb10550
    .word 0xdfb00540
    .word 0x03e00008
    .word 0x27bd05e0
    .word 0x1000fff2
    .word 0x0200182d
    .word 0x27bdfc00
    .word 0xffa60310
    .word 0x27a203c0
    .word 0xffa50300
    .word 0xffa402f0
    .word 0xafa402c0
    .word 0x03a0202d
    .word 0xffbf03c0
    .word 0xffbe03b0
    .word 0xffb703a0
    .word 0xffb60390
    .word 0xffb50380
    .word 0xffb40370
    .word 0xffb30360
    .word 0xffb20350
    .word 0xffb10340
    .word 0xffb00330
    .word 0xffa70320
    .word 0xe7bf03fc
    .word 0xe7be03f8
    .word 0xe7bd03f4
    .word 0xe7bc03f0
    .word 0xe7bb03ec
    .word 0xe7ba03e8
    .word 0xe7b903e4
    .word 0xe7b803e0
    .word 0xe7b703dc
    .word 0xe7b603d8
    .word 0xe7b503d4
    .word 0xe7b403d0
    .word 0x8c430000
    .word 0xafa502d0
    .word 0x27a50400
    .word 0xafa602e0
    .word 0x0c06948a
    .word 0x0060302d
    .word 0x03a0102d
    .word 0x27a40160
    .word 0x0080182d
    .word 0x00000000
    .word 0xdc450000
    .word 0xdc460008
    .word 0xdc470010
    .word 0xdc480018
    .word 0xfc650000
    .word 0xfc660008
    .word 0xfc670010
    .word 0xfc680018
    .word 0x24420020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1444fff3
    .word 0x24630020
    .word 0x8fa302d0
    .word 0x8fa402e0
    .word 0x8fa502c0
    .word 0x0003103c
    .word 0x0004183c
    .word 0x0002103e
    .word 0x0003183e
    .word 0xfca20010
    .word 0xfca30018
    .word 0x27a50160
    .word 0x0c0695fa
    .word 0x8fa402c0
    .word 0x24040007
    .word 0x1044001e
    .word 0x0000182d
    .word 0xdfbf03c0
    .word 0x24630400
    .word 0xdfbe03b0
    .word 0xdfb703a0
    .word 0xdfb60390
    .word 0xdfb50380
    .word 0xdfb40370
    .word 0xdfb30360
    .word 0xdfb20350
    .word 0xdfb10340
    .word 0xdfb00330
    .word 0xdfa70320
    .word 0xdfa60310
    .word 0xdfa50300
    .word 0xdfa402f0
    .word 0xc7bf03fc
    .word 0xc7be03f8
    .word 0xc7bd03f4
    .word 0xc7bc03f0
    .word 0xc7bb03ec
    .word 0xc7ba03e8
    .word 0xc7b903e4
    .word 0xc7b803e0
    .word 0xc7b703dc
    .word 0xc7b603d8
    .word 0xc7b503d4
    .word 0xc7b403d0
    .word 0x03e00008
    .word 0x03a3e821
    .word 0x03a0202d
    .word 0x0c0694b4
    .word 0x27a50160
    .word 0x8fa402a4
    .word 0x0002183c
    .word 0x0003183f
    .word 0x1000ffdc
    .word 0xffa403c0
    .word 0x27bdfc20
    .word 0xffa502e0
    .word 0x27a203a0
    .word 0xffa402d0
    .word 0x27a503e0
    .word 0xafa402c0
    .word 0x03a0202d
    .word 0xffbf03a0
    .word 0xffbe0390
    .word 0xffb70380
    .word 0xffb60370
    .word 0xffb50360
    .word 0xffb40350
    .word 0xffb30340
    .word 0xffb20330
    .word 0xffb10320
    .word 0xffb00310
    .word 0xffa70300
    .word 0xffa602f0
    .word 0xe7bf03dc
    .word 0xe7be03d8
    .word 0xe7bd03d4
    .word 0xe7bc03d0
    .word 0xe7bb03cc
    .word 0xe7ba03c8
    .word 0xe7b903c4
    .word 0xe7b803c0
    .word 0xe7b703bc
    .word 0xe7b603b8
    .word 0xe7b503b4
    .word 0xe7b403b0
    .word 0x0c06948a
    .word 0x8c460000
    .word 0x03a0102d
    .word 0x27a40160
    .word 0x0080182d
    .word 0xdc450000
    .word 0xdc460008
    .word 0xdc470010
    .word 0xdc480018
    .word 0xfc650000
    .word 0xfc660008
    .word 0xfc670010
    .word 0xfc680018
    .word 0x24420020
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x1444fff3
    .word 0x24630020
    .word 0x8fa302c0
    .word 0xdc620010
    .word 0x1440002a
    .word 0x8fa402c0
    .word 0x0c069532
    .word 0x27a50160
    .word 0x0040182d
    .word 0x24020007
    .word 0x14620028
    .word 0x27a50160
    .word 0x0c0694b4
    .word 0x03a0202d
    .word 0xc7b403b0
    .word 0x8fa302a4
    .word 0x0002103c
    .word 0x0002103f
    .word 0xdfbe0390
    .word 0xffa303a0
    .word 0x244303e0
    .word 0xdfb70380
    .word 0xdfbf03a0
    .word 0xdfb60370
    .word 0xdfb50360
    .word 0xdfb40350
    .word 0xdfb30340
    .word 0xdfb20330
    .word 0xdfb10320
    .word 0xdfb00310
    .word 0xdfa70300
    .word 0xdfa602f0
    .word 0xdfa502e0
    .word 0xdfa402d0
    .word 0xc7bf03dc
    .word 0xc7be03d8
    .word 0xc7bd03d4
    .word 0xc7bc03d0
    .word 0xc7bb03cc
    .word 0xc7ba03c8
    .word 0xc7b903c4
    .word 0xc7b803c0
    .word 0xc7b703bc
    .word 0xc7b603b8
    .word 0xc7b503b4
    .word 0x03e00008
    .word 0x03a3e821
    .word 0x0c0695fa
    .word 0x27a50160
    .word 0x1000ffd7
    .word 0x0040182d
    .word 0x0c041d5e
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a4100, .-stage3p_tail_gap_001a4100

    /* Exact public EE reconstruction @ 0x001a5c7c..0x001a5cc0
     * target span SHA-256: 58368a52a2aa11fc38be9979f1ae5f45cc3d81bc3ae8b98455977f3dc827c480
     */
    .balign 4
    .globl stage3p_tail_gap_001a5c7c
    .type stage3p_tail_gap_001a5c7c, @function
stage3p_tail_gap_001a5c7c:
    .word 0x00000000
    .word 0x00052880
    .word 0x00a42821
    .word 0x8ca30000
    .word 0x03e00008
    .word 0xdc620000
    .word 0x00000000
    .word 0x00052880
    .word 0x00a42821
    .word 0x8ca20000
    .word 0x03e00008
    .word 0xfc460000
    .word 0x00000000
    .word 0x03e00008
    .word 0x8c820144
    .word 0x03e00008
    .word 0xac850144
    .size stage3p_tail_gap_001a5c7c, .-stage3p_tail_gap_001a5c7c

    /* Exact public EE reconstruction @ 0x001a5d2c..0x001a5d30
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a5d2c
    .type stage3p_tail_gap_001a5d2c, @function
stage3p_tail_gap_001a5d2c:
    .word 0x00000000
    .size stage3p_tail_gap_001a5d2c, .-stage3p_tail_gap_001a5d2c

    /* Exact public EE reconstruction @ 0x001a5d6c..0x001a5d70
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a5d6c
    .type stage3p_tail_gap_001a5d6c, @function
stage3p_tail_gap_001a5d6c:
    .word 0x00000000
    .size stage3p_tail_gap_001a5d6c, .-stage3p_tail_gap_001a5d6c

    /* Exact public EE reconstruction @ 0x001a5f7c..0x001a5f80
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a5f7c
    .type stage3p_tail_gap_001a5f7c, @function
stage3p_tail_gap_001a5f7c:
    .word 0x00000000
    .size stage3p_tail_gap_001a5f7c, .-stage3p_tail_gap_001a5f7c

    /* Exact public EE reconstruction @ 0x001a5fec..0x001a5ff0
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a5fec
    .type stage3p_tail_gap_001a5fec, @function
stage3p_tail_gap_001a5fec:
    .word 0x00000000
    .size stage3p_tail_gap_001a5fec, .-stage3p_tail_gap_001a5fec

    /* Exact public EE reconstruction @ 0x001a6034..0x001a6038
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a6034
    .type stage3p_tail_gap_001a6034, @function
stage3p_tail_gap_001a6034:
    .word 0x00000000
    .size stage3p_tail_gap_001a6034, .-stage3p_tail_gap_001a6034

    /* Exact public EE reconstruction @ 0x001a608c..0x001a6090
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a608c
    .type stage3p_tail_gap_001a608c, @function
stage3p_tail_gap_001a608c:
    .word 0x00000000
    .size stage3p_tail_gap_001a608c, .-stage3p_tail_gap_001a608c

    /* Exact public EE reconstruction @ 0x001a6184..0x001a6188
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a6184
    .type stage3p_tail_gap_001a6184, @function
stage3p_tail_gap_001a6184:
    .word 0x00000000
    .size stage3p_tail_gap_001a6184, .-stage3p_tail_gap_001a6184

    /* Exact public EE reconstruction @ 0x001a6320..0x001a64e8
     * target span SHA-256: b751f6c59aa6accfbd75162b2fc930333a142e2f7f7ae3c0c1934c62482ae8ad
     */
    .balign 4
    .globl stage3p_tail_gap_001a6320
    .type stage3p_tail_gap_001a6320, @function
stage3p_tail_gap_001a6320:
    .word 0x8ca40008
    .word 0x8cc30008
    .word 0x0064102b
    .word 0x14400004
    .word 0x24050001
    .word 0x0083102b
    .word 0x2405ffff
    .word 0x0002280a
    .word 0x03e00008
    .word 0x00a0102d
    .word 0x27bdffa0
    .word 0xffbf0050
    .word 0xffb30040
    .word 0xffb20030
    .word 0x00c0902d
    .word 0xffb10020
    .word 0x26520008
    .word 0xffb00010
    .word 0x0080882d
    .word 0x00a0802d
    .word 0x0220282d
    .word 0xdc840010
    .word 0x26100008
    .word 0x000420fa
    .word 0x308400ff
    .word 0x0c069870
    .word 0x308400ff
    .word 0x0200302d
    .word 0xde240010
    .word 0x03a0382d
    .word 0x0040282d
    .word 0x0040982d
    .word 0x000420fa
    .word 0x308400ff
    .word 0x0c069774
    .word 0x308400ff
    .word 0x0240302d
    .word 0xde240010
    .word 0x0260282d
    .word 0x27a70004
    .word 0x000420fa
    .word 0x308400ff
    .word 0x0c069774
    .word 0x308400ff
    .word 0x8fa30004
    .word 0x8fa40000
    .word 0x0064102b
    .word 0x14400004
    .word 0x24050001
    .word 0x0083102b
    .word 0x2405ffff
    .word 0x0002280a
    .word 0xdfbf0050
    .word 0x00a0102d
    .word 0xdfb30040
    .word 0xdfb20030
    .word 0xdfb10020
    .word 0xdfb00010
    .word 0x03e00008
    .word 0x27bd0060
    .word 0x27bdffa0
    .word 0xffbf0050
    .word 0xffb30040
    .word 0x0080982d
    .word 0xffb20030
    .word 0x00c0902d
    .word 0xffb10020
    .word 0x00a0882d
    .word 0xffb00010
    .word 0x26310008
    .word 0x8ca40004
    .word 0x00a42023
    .word 0x0c069890
    .word 0x24840004
    .word 0x0260282d
    .word 0x305000ff
    .word 0x0c069870
    .word 0x0200202d
    .word 0x0200202d
    .word 0x03a0382d
    .word 0x0220302d
    .word 0x0c069774
    .word 0x0040282d
    .word 0x8e440004
    .word 0x02442023
    .word 0x26520008
    .word 0x0c069890
    .word 0x24840004
    .word 0x0260282d
    .word 0x305000ff
    .word 0x0c069870
    .word 0x0200202d
    .word 0x0240302d
    .word 0x0200202d
    .word 0x0040282d
    .word 0x0c069774
    .word 0x27a70004
    .word 0x8fa30004
    .word 0x8fa40000
    .word 0x0064102b
    .word 0x14400004
    .word 0x24050001
    .word 0x0083102b
    .word 0x2405ffff
    .word 0x0002280a
    .word 0xdfbf0050
    .word 0x00a0102d
    .word 0xdfb30040
    .word 0xdfb20030
    .word 0xdfb10020
    .word 0xdfb00010
    .word 0x03e00008
    .word 0x27bd0060
    .word 0x00000000
    .size stage3p_tail_gap_001a6320, .-stage3p_tail_gap_001a6320

    /* Exact public EE reconstruction @ 0x001a6a6c..0x001a6a70
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a6a6c
    .type stage3p_tail_gap_001a6a6c, @function
stage3p_tail_gap_001a6a6c:
    .word 0x00000000
    .size stage3p_tail_gap_001a6a6c, .-stage3p_tail_gap_001a6a6c

    /* Exact public EE reconstruction @ 0x001a7434..0x001a7438
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a7434
    .type stage3p_tail_gap_001a7434, @function
stage3p_tail_gap_001a7434:
    .word 0x00000000
    .size stage3p_tail_gap_001a7434, .-stage3p_tail_gap_001a7434

    /* Exact public EE reconstruction @ 0x001a757c..0x001a7580
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001a757c
    .type stage3p_tail_gap_001a757c, @function
stage3p_tail_gap_001a757c:
    .word 0x00000000
    .size stage3p_tail_gap_001a757c, .-stage3p_tail_gap_001a757c

    /* Exact public EE reconstruction @ 0x001a7630..0x001a7e30
     * target span SHA-256: 28b29fec16ceef2c3878245f75e097d918e27aedb3b613b084d9cbcdb82a2d0f
     */
    .balign 4
    .globl stage3p_tail_gap_001a7630
    .type stage3p_tail_gap_001a7630, @function
stage3p_tail_gap_001a7630:
    .word 0x00000000
    .word 0x00000000
    .word 0x2402ffff
    .word 0x0002103c
    .word 0x2403ffff
    .word 0x0003183c
    .word 0x01024024
    .word 0x01234824
    .word 0x0004103c
    .word 0x0002103f
    .word 0x0004383f
    .word 0x00021023
    .word 0x0080502d
    .word 0x0002103c
    .word 0x00072023
    .word 0x0002103e
    .word 0x0005583f
    .word 0x01024025
    .word 0x27bdffc0
    .word 0x0005103c
    .word 0x0002103f
    .word 0x0008183c
    .word 0x0003183f
    .word 0x00021023
    .word 0x0003182b
    .word 0x0002103c
    .word 0x00832023
    .word 0x0002103e
    .word 0x0004603c
    .word 0x01224825
    .word 0x000b2023
    .word 0x0009103c
    .word 0x0002103f
    .word 0x3c03ffff
    .word 0x0003183e
    .word 0x0002102b
    .word 0xffb10020
    .word 0x00822023
    .word 0xffbf0030
    .word 0xffb00010
    .word 0x03a0302d
    .word 0x01034024
    .word 0x01234824
    .word 0x0004103c
    .word 0x04e00025
    .word 0x0000882d
    .word 0x05600021
    .word 0x0140202d
    .word 0x0c069de2
    .word 0x00000000
    .word 0x12200016
    .word 0xdfa40000
    .word 0x2403ffff
    .word 0x0003183c
    .word 0x02038024
    .word 0x0004103c
    .word 0x0002103f
    .word 0x0004203f
    .word 0x00021023
    .word 0x00042023
    .word 0x0002103c
    .word 0x0002103e
    .word 0x02028025
    .word 0x3c02ffff
    .word 0x0002103e
    .word 0x0010183c
    .word 0x0003183f
    .word 0x02028024
    .word 0x0003182b
    .word 0x00832023
    .word 0x0004203c
    .word 0x02048025
    .word 0xffb00000
    .word 0xdfa20000
    .word 0xdfbf0030
    .word 0xdfb10020
    .word 0xdfb00010
    .word 0x03e00008
    .word 0x27bd0040
    .word 0x00000000
    .word 0x1000ffdf
    .word 0x01222825
    .word 0x010c5025
    .word 0x1000ffda
    .word 0x2411ffff
    .word 0x00000000
    .word 0x27bdfff0
    .word 0x0005383f
    .word 0x0004403f
    .word 0x0005483c
    .word 0x0009483f
    .word 0x0004683c
    .word 0x000d683f
    .word 0x14e00108
    .word 0xffb00000
    .word 0x0109102b
    .word 0x10400071
    .word 0x3402ffff
    .word 0x0049102b
    .word 0x14400066
    .word 0x3c0200ff
    .word 0x24020008
    .word 0x2d230100
    .word 0x0040282d
    .word 0x0003280b
    .word 0x3c03001c
    .word 0x00a91006
    .word 0x2463abc8
    .word 0x24070020
    .word 0x00431021
    .word 0x90440000
    .word 0x00852021
    .word 0x00e47823
    .word 0x51e00008
    .word 0x00093c02
    .word 0x00ef1023
    .word 0x01e81804
    .word 0x004d1006
    .word 0x01e94804
    .word 0x00624025
    .word 0x01ed6804
    .word 0x00093c02
    .word 0x312affff
    .word 0x0107001b
    .word 0x000d2402
    .word 0x50e00001
    .word 0x000001cd
    .word 0x00001012
    .word 0x00001810
    .word 0x00031c00
    .word 0x00641825
    .word 0x00002812
    .word 0x004a4018
    .word 0x0068102b
    .word 0x5040000c
    .word 0x00681823
    .word 0x00691821
    .word 0x0069102b
    .word 0x14400007
    .word 0x24a5ffff
    .word 0x0068102b
    .word 0x50400005
    .word 0x00681823
    .word 0x24a5ffff
    .word 0x00691821
    .word 0x00000000
    .word 0x00681823
    .word 0x50e00001
    .word 0x000001cd
    .word 0x0067001b
    .word 0x31a4ffff
    .word 0x00001012
    .word 0x00001810
    .word 0x00031c00
    .word 0x00641825
    .word 0x00003812
    .word 0x004a4018
    .word 0x0068102b
    .word 0x1040000c
    .word 0x00051400
    .word 0x00691821
    .word 0x0069102b
    .word 0x14400007
    .word 0x24e7ffff
    .word 0x0068102b
    .word 0x10400005
    .word 0x00051400
    .word 0x00691821
    .word 0x24e7ffff
    .word 0x00000000
    .word 0x00051400
    .word 0x00686823
    .word 0x00475025
    .word 0x0000802d
    .word 0x10c0000b
    .word 0x01ed1006
    .word 0x2403ffff
    .word 0x0003183c
    .word 0x0002103c
    .word 0x01836024
    .word 0x0002103e
    .word 0x3c03ffff
    .word 0x0003183e
    .word 0x01826025
    .word 0x01836024
    .word 0xfccc0000
    .word 0x000a103c
    .word 0x2403ffff
    .word 0x0003183c
    .word 0x0002103e
    .word 0x01c37024
    .word 0x01c27025
    .word 0x0010183c
    .word 0x3c02ffff
    .word 0x0002103e
    .word 0xdfb00000
    .word 0x01c27024
    .word 0x01c37025
    .word 0x01c0102d
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x00000000
    .word 0x24030010
    .word 0x3442ffff
    .word 0x24040018
    .word 0x0049102b
    .word 0x0060282d
    .word 0x1000ff99
    .word 0x0082280b
    .word 0x00000000
    .word 0x15200008
    .word 0x0049102b
    .word 0x24020001
    .word 0x51200001
    .word 0x000001cd
    .word 0x0047001b
    .word 0x00004812
    .word 0x3402ffff
    .word 0x0049102b
    .word 0x14400082
    .word 0x3c0200ff
    .word 0x24020008
    .word 0x2d230100
    .word 0x0040282d
    .word 0x0003280b
    .word 0x3c03001c
    .word 0x00a91006
    .word 0x2463abc8
    .word 0x24070020
    .word 0x00431021
    .word 0x90440000
    .word 0x00852021
    .word 0x00e47823
    .word 0x15e00038
    .word 0x00efc023
    .word 0x01094023
    .word 0x24100001
    .word 0x00095402
    .word 0x3139ffff
    .word 0x010a001b
    .word 0x000d2c02
    .word 0x51400001
    .word 0x000001cd
    .word 0x00002012
    .word 0x00001810
    .word 0x00031c00
    .word 0x00651825
    .word 0x00005812
    .word 0x00993818
    .word 0x0067102b
    .word 0x5040000c
    .word 0x00671823
    .word 0x00691821
    .word 0x0069102b
    .word 0x14400007
    .word 0x248bffff
    .word 0x0067102b
    .word 0x50400005
    .word 0x00671823
    .word 0x256bffff
    .word 0x00691821
    .word 0x00000000
    .word 0x00671823
    .word 0x31a4ffff
    .word 0x006a001b
    .word 0x51400001
    .word 0x000001cd
    .word 0x00001012
    .word 0x00001810
    .word 0x00031c00
    .word 0x00641825
    .word 0x00004012
    .word 0x00593818
    .word 0x0067102b
    .word 0x1040000c
    .word 0x000b1400
    .word 0x00691821
    .word 0x0069102b
    .word 0x14400007
    .word 0x2508ffff
    .word 0x0067102b
    .word 0x10400005
    .word 0x000b1400
    .word 0x00691821
    .word 0x2508ffff
    .word 0x00000000
    .word 0x000b1400
    .word 0x00676823
    .word 0x1000ff8d
    .word 0x00485025
    .word 0x01e94804
    .word 0x03082806
    .word 0x00095402
    .word 0x00aa001b
    .word 0x3139ffff
    .word 0x030d1806
    .word 0x01e81004
    .word 0x00434025
    .word 0x51400001
    .word 0x000001cd
    .word 0x00081c02
    .word 0x0140582d
    .word 0x01ed6804
    .word 0x00002812
    .word 0x00002010
    .word 0x00042400
    .word 0x00831825
    .word 0x0000c012
    .word 0x00b93818
    .word 0x0067102b
    .word 0x1040000b
    .word 0x0320802d
    .word 0x00691821
    .word 0x0069102b
    .word 0x14400007
    .word 0x24b8ffff
    .word 0x0067102b
    .word 0x50400005
    .word 0x00671823
    .word 0x2718ffff
    .word 0x00691821
    .word 0x00000000
    .word 0x00671823
    .word 0x3104ffff
    .word 0x006b001b
    .word 0x51600001
    .word 0x000001cd
    .word 0x00001012
    .word 0x00001810
    .word 0x00031c00
    .word 0x00641825
    .word 0x00002812
    .word 0x00503818
    .word 0x0067102b
    .word 0x1040000c
    .word 0x00181400
    .word 0x00691821
    .word 0x0069102b
    .word 0x14400007
    .word 0x24a5ffff
    .word 0x0067102b
    .word 0x10400005
    .word 0x00181400
    .word 0x24a5ffff
    .word 0x00691821
    .word 0x00000000
    .word 0x00181400
    .word 0x00674023
    .word 0x1000ff92
    .word 0x00458025
    .word 0x24030010
    .word 0x3442ffff
    .word 0x24040018
    .word 0x0049102b
    .word 0x0060282d
    .word 0x1000ff7d
    .word 0x0082280b
    .word 0x00000000
    .word 0x0107102b
    .word 0x1440001f
    .word 0x0000502d
    .word 0x3402ffff
    .word 0x0047102b
    .word 0x1440008c
    .word 0x3c0200ff
    .word 0x24020008
    .word 0x2ce30100
    .word 0x0040282d
    .word 0x0003280b
    .word 0x3c03001c
    .word 0x00a71006
    .word 0x2463abc8
    .word 0x240a0020
    .word 0x00431021
    .word 0x90440000
    .word 0x00852021
    .word 0x01447823
    .word 0x15e0001c
    .word 0x014fc023
    .word 0x00e8102b
    .word 0x14400005
    .word 0x01a92023
    .word 0x01a9102b
    .word 0x14400007
    .word 0x0000502d
    .word 0x01a92023
    .word 0x01071823
    .word 0x01a4102b
    .word 0x240a0001
    .word 0x00624023
    .word 0x0080682d
    .word 0x10c0ff32
    .word 0x0000802d
    .word 0x2403ffff
    .word 0x0003183c
    .word 0x000d103c
    .word 0x0002103e
    .word 0x01836024
    .word 0x01826025
    .word 0x0008183c
    .word 0x3c02ffff
    .word 0x0002103e
    .word 0x01826024
    .word 0x1000ff25
    .word 0x01836025
    .word 0x00000000
    .word 0x01e71804
    .word 0x03091006
    .word 0x03082806
    .word 0x00623825
    .word 0x01e82004
    .word 0x00075402
    .word 0x30f0ffff
    .word 0x00aa001b
    .word 0x030d1006
    .word 0x00824025
    .word 0x51400001
    .word 0x000001cd
    .word 0x00082402
    .word 0x01e94804
    .word 0x00002812
    .word 0x00001810
    .word 0x00031c00
    .word 0x00641825
    .word 0x0000c812
    .word 0x00b05818
    .word 0x006b102b
    .word 0x1040000a
    .word 0x01ed6804
    .word 0x00671821
    .word 0x0067102b
    .word 0x14400006
    .word 0x24b9ffff
    .word 0x006b102b
    .word 0x50400004
    .word 0x006b1823
    .word 0x2739ffff
    .word 0x00671821
    .word 0x006b1823
    .word 0x3104ffff
    .word 0x006a001b
    .word 0x51400001
    .word 0x000001cd
    .word 0x00001012
    .word 0x00001810
    .word 0x00031c00
    .word 0x00642825
    .word 0x00004012
    .word 0x00505818
    .word 0x00ab102b
    .word 0x1040000c
    .word 0x00191400
    .word 0x00a72821
    .word 0x00a7102b
    .word 0x14400007
    .word 0x2508ffff
    .word 0x00ab102b
    .word 0x10400005
    .word 0x00191400
    .word 0x2508ffff
    .word 0x00a72821
    .word 0x00000000
    .word 0x00191400
    .word 0x00ab2823
    .word 0x00485025
    .word 0x01490019
    .word 0x00001810
    .word 0x00005812
    .word 0x00a3102b
    .word 0x1440001b
    .word 0x01692023
    .word 0x10650017
    .word 0x01ab102b
    .word 0x10c0fee0
    .word 0x0000802d
    .word 0x01ab2023
    .word 0x00a32823
    .word 0x01a4182b
    .word 0x01e42006
    .word 0x00a34023
    .word 0x2403ffff
    .word 0x0003183c
    .word 0x03081004
    .word 0x01836024
    .word 0x00441025
    .word 0x01e82806
    .word 0x0002103c
    .word 0x3c03ffff
    .word 0x0003183e
    .word 0x0002103e
    .word 0x0005283c
    .word 0x01826025
    .word 0x01836024
    .word 0x1000fecb
    .word 0x01856025
    .word 0x1040ffe9
    .word 0x01692023
    .word 0x00671823
    .word 0x0164102b
    .word 0x254affff
    .word 0x00621823
    .word 0x1000ffe3
    .word 0x0080582d
    .word 0x00000000
    .word 0x24030010
    .word 0x3442ffff
    .word 0x24040018
    .word 0x0047102b
    .word 0x0060282d
    .word 0x1000ff73
    .word 0x0082280b
    .word 0x00000000
    .size stage3p_tail_gap_001a7630, .-stage3p_tail_gap_001a7630

    /* Exact public EE reconstruction @ 0x001a7e90..0x001a7ea0
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_001a7e90
    .type stage3p_tail_gap_001a7e90, @function
stage3p_tail_gap_001a7e90:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a7e90, .-stage3p_tail_gap_001a7e90

    /* Exact public EE reconstruction @ 0x001a80c8..0x001a80d0
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a80c8
    .type stage3p_tail_gap_001a80c8, @function
stage3p_tail_gap_001a80c8:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a80c8, .-stage3p_tail_gap_001a80c8

    /* Exact public EE reconstruction @ 0x001a8130..0x001a8140
     * target span SHA-256: 374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb
     */
    .balign 4
    .globl stage3p_tail_gap_001a8130
    .type stage3p_tail_gap_001a8130, @function
stage3p_tail_gap_001a8130:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a8130, .-stage3p_tail_gap_001a8130

    /* Exact public EE reconstruction @ 0x001a8330..0x001a8338
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a8330
    .type stage3p_tail_gap_001a8330, @function
stage3p_tail_gap_001a8330:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a8330, .-stage3p_tail_gap_001a8330

    /* Exact public EE reconstruction @ 0x001a84d4..0x001a84e8
     * target span SHA-256: de47c9b27eb8d300dbb5f2c353e632c393262cf06340c4fa7f1b40c4cbd36f90
     */
    .balign 4
    .globl stage3p_tail_gap_001a84d4
    .type stage3p_tail_gap_001a84d4, @function
stage3p_tail_gap_001a84d4:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a84d4, .-stage3p_tail_gap_001a84d4

    /* Exact public EE reconstruction @ 0x001a84f0..0x001a84fc
     * target span SHA-256: 15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b
     */
    .balign 4
    .globl stage3p_tail_gap_001a84f0
    .type stage3p_tail_gap_001a84f0, @function
stage3p_tail_gap_001a84f0:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a84f0, .-stage3p_tail_gap_001a84f0

    /* Exact public EE reconstruction @ 0x001a8534..0x001a8548
     * target span SHA-256: de47c9b27eb8d300dbb5f2c353e632c393262cf06340c4fa7f1b40c4cbd36f90
     */
    .balign 4
    .globl stage3p_tail_gap_001a8534
    .type stage3p_tail_gap_001a8534, @function
stage3p_tail_gap_001a8534:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a8534, .-stage3p_tail_gap_001a8534

    /* Exact public EE reconstruction @ 0x001a8550..0x001a855c
     * target span SHA-256: 15ec7bf0b50732b49f8228e07d24365338f9e3ab994b00af08e5a3bffe55fd8b
     */
    .balign 4
    .globl stage3p_tail_gap_001a8550
    .type stage3p_tail_gap_001a8550, @function
stage3p_tail_gap_001a8550:
    .word 0x00000000
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a8550, .-stage3p_tail_gap_001a8550

    /* Exact public EE reconstruction @ 0x001a9c18..0x001a9c20
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a9c18
    .type stage3p_tail_gap_001a9c18, @function
stage3p_tail_gap_001a9c18:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a9c18, .-stage3p_tail_gap_001a9c18

    /* Exact public EE reconstruction @ 0x001a9c80..0x001a9c88
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a9c80
    .type stage3p_tail_gap_001a9c80, @function
stage3p_tail_gap_001a9c80:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a9c80, .-stage3p_tail_gap_001a9c80

    /* Exact public EE reconstruction @ 0x001a9e80..0x001a9e88
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001a9e80
    .type stage3p_tail_gap_001a9e80, @function
stage3p_tail_gap_001a9e80:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001a9e80, .-stage3p_tail_gap_001a9e80

    /* Exact public EE reconstruction @ 0x001a9f68..0x001a9fa8
     * target span SHA-256: ab43badd654c9b49ff8ccc08cb6f263b2c26bd604a81e3ac06a5e18cb1d327cb
     */
    .balign 4
    .globl stage3p_tail_gap_001a9f68
    .type stage3p_tail_gap_001a9f68, @function
stage3p_tail_gap_001a9f68:
    .word 0x27bdfff0
    .word 0xffbf0000
    .word 0x0c06a7a2
    .word 0x00000000
    .word 0xdfbf0000
    .word 0x03e00008
    .word 0x27bd0010
    .word 0x00000000
    .word 0x2402ffff
    .word 0x10a20004
    .word 0x00000000
    .word 0x0c0696b0
    .word 0x00000000
    .word 0x00000000
    .word 0x0c06a6e2
    .word 0x00000000
    .size stage3p_tail_gap_001a9f68, .-stage3p_tail_gap_001a9f68

    /* Exact public EE reconstruction @ 0x001aade0..0x001aade8
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001aade0
    .type stage3p_tail_gap_001aade0, @function
stage3p_tail_gap_001aade0:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001aade0, .-stage3p_tail_gap_001aade0

    /* Exact public EE reconstruction @ 0x001ab078..0x001ab080
     * target span SHA-256: af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc
     */
    .balign 4
    .globl stage3p_tail_gap_001ab078
    .type stage3p_tail_gap_001ab078, @function
stage3p_tail_gap_001ab078:
    .word 0x00000000
    .word 0x00000000
    .size stage3p_tail_gap_001ab078, .-stage3p_tail_gap_001ab078

    /* Exact public EE reconstruction @ 0x001ab4e4..0x001ab4e8
     * target span SHA-256: df3f619804a92fdb4057192dc43dd748ea778adc52bc498ce80524c014b81119
     */
    .balign 4
    .globl stage3p_tail_gap_001ab4e4
    .type stage3p_tail_gap_001ab4e4, @function
stage3p_tail_gap_001ab4e4:
    .word 0x00000000
    .size stage3p_tail_gap_001ab4e4, .-stage3p_tail_gap_001ab4e4
