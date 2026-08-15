.text
.set noreorder

.globl p53_type_info_D2
.type p53_type_info_D2, @function
p53_type_info_D2:
    lui $2,%hi(p53_vt_type_info)
    addiu $2,$2,%lo(p53_vt_type_info)
    jr $31
    sw $2,0($4)
.size p53_type_info_D2, .-p53_type_info_D2

.globl p53_type_info_D1
.type p53_type_info_D1, @function
p53_type_info_D1:
    lui $2,%hi(p53_vt_type_info)
    addiu $2,$2,%lo(p53_vt_type_info)
    jr $31
    sw $2,0($4)
.size p53_type_info_D1, .-p53_type_info_D1

.globl p53_type_info_D0
.type p53_type_info_D0, @function
p53_type_info_D0:
    lui $2,%hi(p53_vt_type_info)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_type_info)
    sd $31,0($29)
    jal p53_call_operator_delete
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_type_info_D0, .-p53_type_info_D0

.globl p53_bad_cast_D2
.type p53_bad_cast_D2, @function
p53_bad_cast_D2:
    lui $2,%hi(p53_vt_bad_cast)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_bad_cast)
    sd $31,0($29)
    jal p53_call_exception_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_bad_cast_D2, .-p53_bad_cast_D2

.globl p53_bad_cast_D1
.type p53_bad_cast_D1, @function
p53_bad_cast_D1:
    lui $2,%hi(p53_vt_bad_cast)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_bad_cast)
    sd $31,0($29)
    jal p53_call_exception_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_bad_cast_D1, .-p53_bad_cast_D1

.globl p53_bad_cast_D0
.type p53_bad_cast_D0, @function
p53_bad_cast_D0:
    lui $2,%hi(p53_vt_bad_cast)
    addiu $29,$29,-32
    addiu $2,$2,%lo(p53_vt_bad_cast)
    sd $16,0($29)
    sw $2,0($4)
    sd $31,16($29)
    jal p53_call_exception_D2
    daddu $16,$4,$zero
    jal p53_call_operator_delete
    daddu $4,$16,$zero
    ld $16,0($29)
    ld $31,16($29)
    jr $31
    addiu $29,$29,32
.size p53_bad_cast_D0, .-p53_bad_cast_D0

.globl p53_bad_typeid_D2
.type p53_bad_typeid_D2, @function
p53_bad_typeid_D2:
    lui $2,%hi(p53_vt_bad_typeid)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_bad_typeid)
    sd $31,0($29)
    jal p53_call_exception_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_bad_typeid_D2, .-p53_bad_typeid_D2

.globl p53_bad_typeid_D1
.type p53_bad_typeid_D1, @function
p53_bad_typeid_D1:
    lui $2,%hi(p53_vt_bad_typeid)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_bad_typeid)
    sd $31,0($29)
    jal p53_call_exception_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_bad_typeid_D1, .-p53_bad_typeid_D1

.globl p53_bad_typeid_D0
.type p53_bad_typeid_D0, @function
p53_bad_typeid_D0:
    lui $2,%hi(p53_vt_bad_typeid)
    addiu $29,$29,-32
    addiu $2,$2,%lo(p53_vt_bad_typeid)
    sd $16,0($29)
    sw $2,0($4)
    sd $31,16($29)
    jal p53_call_exception_D2
    daddu $16,$4,$zero
    jal p53_call_operator_delete
    daddu $4,$16,$zero
    ld $16,0($29)
    ld $31,16($29)
    jr $31
    addiu $29,$29,32
.size p53_bad_typeid_D0, .-p53_bad_typeid_D0

.globl std_type_info_is_pointer_p
.type std_type_info_is_pointer_p, @function
std_type_info_is_pointer_p:
    jr $31
    daddu $2,$zero,$zero
.size std_type_info_is_pointer_p, .-std_type_info_is_pointer_p

.globl std_type_info_is_function_p
.type std_type_info_is_function_p, @function
std_type_info_is_function_p:
    jr $31
    daddu $2,$zero,$zero
.size std_type_info_is_function_p, .-std_type_info_is_function_p

.globl std_type_info_do_catch
.type std_type_info_do_catch, @function
std_type_info_do_catch:
    lw $2,4($4)
    lw $3,4($5)
    xor $2,$2,$3
    jr $31
    sltiu $2,$2,1
.size std_type_info_do_catch, .-std_type_info_do_catch

.globl std_type_info_do_upcast
.type std_type_info_do_upcast, @function
std_type_info_do_upcast:
    jr $31
    daddu $2,$zero,$zero
.size std_type_info_do_upcast, .-std_type_info_do_upcast

.globl p53_class_type_info_D2
.type p53_class_type_info_D2, @function
p53_class_type_info_D2:
    lui $2,%hi(p53_vt_class_type_info)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_class_type_info)
    sd $31,0($29)
    jal p53_call_type_info_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_class_type_info_D2, .-p53_class_type_info_D2

.globl p53_class_type_info_D1
.type p53_class_type_info_D1, @function
p53_class_type_info_D1:
    lui $2,%hi(p53_vt_class_type_info)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_class_type_info)
    sd $31,0($29)
    jal p53_call_type_info_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_class_type_info_D1, .-p53_class_type_info_D1

.globl p53_class_type_info_D0
.type p53_class_type_info_D0, @function
p53_class_type_info_D0:
    lui $2,%hi(p53_vt_class_type_info)
    addiu $29,$29,-32
    addiu $2,$2,%lo(p53_vt_class_type_info)
    sd $16,0($29)
    sw $2,0($4)
    sd $31,16($29)
    jal p53_call_type_info_D2
    daddu $16,$4,$zero
    jal p53_call_operator_delete
    daddu $4,$16,$zero
    ld $16,0($29)
    ld $31,16($29)
    jr $31
    addiu $29,$29,32
.size p53_class_type_info_D0, .-p53_class_type_info_D0

.globl p53_si_class_type_info_D2
.type p53_si_class_type_info_D2, @function
p53_si_class_type_info_D2:
    lui $2,%hi(p53_vt_si_class_type_info)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_si_class_type_info)
    sd $31,0($29)
    jal p53_call_class_type_info_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_si_class_type_info_D2, .-p53_si_class_type_info_D2

.globl p53_si_class_type_info_D1
.type p53_si_class_type_info_D1, @function
p53_si_class_type_info_D1:
    lui $2,%hi(p53_vt_si_class_type_info)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_si_class_type_info)
    sd $31,0($29)
    jal p53_call_class_type_info_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_si_class_type_info_D1, .-p53_si_class_type_info_D1

.globl p53_si_class_type_info_D0
.type p53_si_class_type_info_D0, @function
p53_si_class_type_info_D0:
    lui $2,%hi(p53_vt_si_class_type_info)
    addiu $29,$29,-32
    addiu $2,$2,%lo(p53_vt_si_class_type_info)
    sd $16,0($29)
    sw $2,0($4)
    sd $31,16($29)
    jal p53_call_class_type_info_D2
    daddu $16,$4,$zero
    jal p53_call_operator_delete
    daddu $4,$16,$zero
    ld $16,0($29)
    ld $31,16($29)
    jr $31
    addiu $29,$29,32
.size p53_si_class_type_info_D0, .-p53_si_class_type_info_D0

.globl p53_vmi_class_type_info_D2
.type p53_vmi_class_type_info_D2, @function
p53_vmi_class_type_info_D2:
    lui $2,%hi(p53_vt_vmi_class_type_info)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_vmi_class_type_info)
    sd $31,0($29)
    jal p53_call_class_type_info_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_vmi_class_type_info_D2, .-p53_vmi_class_type_info_D2

.globl p53_vmi_class_type_info_D1
.type p53_vmi_class_type_info_D1, @function
p53_vmi_class_type_info_D1:
    lui $2,%hi(p53_vt_vmi_class_type_info)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_vmi_class_type_info)
    sd $31,0($29)
    jal p53_call_class_type_info_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_vmi_class_type_info_D1, .-p53_vmi_class_type_info_D1

.globl p53_vmi_class_type_info_D0
.type p53_vmi_class_type_info_D0, @function
p53_vmi_class_type_info_D0:
    lui $2,%hi(p53_vt_vmi_class_type_info)
    addiu $29,$29,-32
    addiu $2,$2,%lo(p53_vt_vmi_class_type_info)
    sd $16,0($29)
    sw $2,0($4)
    sd $31,16($29)
    jal p53_call_class_type_info_D2
    daddu $16,$4,$zero
    jal p53_call_operator_delete
    daddu $4,$16,$zero
    ld $16,0($29)
    ld $31,16($29)
    jr $31
    addiu $29,$29,32
.size p53_vmi_class_type_info_D0, .-p53_vmi_class_type_info_D0

.globl class_type_info_find_public_src
.type class_type_info_find_public_src, @function
class_type_info_find_public_src:
    xor $8,$8,$6
    addiu $3,$zero,1
    addiu $2,$zero,6
    jr $31
    movn $2,$3,$8
.size class_type_info_find_public_src, .-class_type_info_find_public_src

.globl std_uncaught_exception
.type std_uncaught_exception, @function
std_uncaught_exception:
    addiu $29,$29,-16
    sd $31,0($29)
    jal p53_call_cxa_get_globals
    nop
    lw $2,4($2)
    ld $31,0($29)
    sltu $2,$zero,$2
    jr $31
    addiu $29,$29,16
.size std_uncaught_exception, .-std_uncaught_exception

.globl p53_exception_D2
.type p53_exception_D2, @function
p53_exception_D2:
    lui $2,%hi(p53_vt_exception)
    addiu $2,$2,%lo(p53_vt_exception)
    jr $31
    sw $2,0($4)
.size p53_exception_D2, .-p53_exception_D2

.globl p53_exception_D1
.type p53_exception_D1, @function
p53_exception_D1:
    lui $2,%hi(p53_vt_exception)
    addiu $2,$2,%lo(p53_vt_exception)
    jr $31
    sw $2,0($4)
.size p53_exception_D1, .-p53_exception_D1

.globl p53_exception_D0
.type p53_exception_D0, @function
p53_exception_D0:
    lui $2,%hi(p53_vt_exception)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_exception)
    sd $31,0($29)
    jal p53_call_operator_delete
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_exception_D0, .-p53_exception_D0

.globl p53_bad_exception_D2
.type p53_bad_exception_D2, @function
p53_bad_exception_D2:
    lui $2,%hi(p53_vt_bad_exception)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_bad_exception)
    sd $31,0($29)
    jal p53_call_exception_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_bad_exception_D2, .-p53_bad_exception_D2

.globl p53_bad_exception_D1
.type p53_bad_exception_D1, @function
p53_bad_exception_D1:
    lui $2,%hi(p53_vt_bad_exception)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_bad_exception)
    sd $31,0($29)
    jal p53_call_exception_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_bad_exception_D1, .-p53_bad_exception_D1

.globl p53_bad_exception_D0
.type p53_bad_exception_D0, @function
p53_bad_exception_D0:
    lui $2,%hi(p53_vt_bad_exception)
    addiu $29,$29,-32
    addiu $2,$2,%lo(p53_vt_bad_exception)
    sd $16,0($29)
    sw $2,0($4)
    sd $31,16($29)
    jal p53_call_exception_D2
    daddu $16,$4,$zero
    jal p53_call_operator_delete
    daddu $4,$16,$zero
    ld $16,0($29)
    ld $31,16($29)
    jr $31
    addiu $29,$29,32
.size p53_bad_exception_D0, .-p53_bad_exception_D0

.globl p53_bad_alloc_D2
.type p53_bad_alloc_D2, @function
p53_bad_alloc_D2:
    lui $2,%hi(p53_vt_bad_alloc)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_bad_alloc)
    sd $31,0($29)
    jal p53_call_exception_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_bad_alloc_D2, .-p53_bad_alloc_D2

.globl p53_bad_alloc_D1
.type p53_bad_alloc_D1, @function
p53_bad_alloc_D1:
    lui $2,%hi(p53_vt_bad_alloc)
    addiu $29,$29,-16
    addiu $2,$2,%lo(p53_vt_bad_alloc)
    sd $31,0($29)
    jal p53_call_exception_D2
    sw $2,0($4)
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_bad_alloc_D1, .-p53_bad_alloc_D1

.globl p53_bad_alloc_D0
.type p53_bad_alloc_D0, @function
p53_bad_alloc_D0:
    lui $2,%hi(p53_vt_bad_alloc)
    addiu $29,$29,-32
    addiu $2,$2,%lo(p53_vt_bad_alloc)
    sd $16,0($29)
    sw $2,0($4)
    sd $31,16($29)
    jal p53_call_exception_D2
    daddu $16,$4,$zero
    jal p53_call_operator_delete
    daddu $4,$16,$zero
    ld $16,0($29)
    ld $31,16($29)
    jr $31
    addiu $29,$29,32
.size p53_bad_alloc_D0, .-p53_bad_alloc_D0

.globl std_exception_what
.type std_exception_what, @function
std_exception_what:
    lw $2,0($4)
    lw $3,-4($2)
    jr $31
    lw $2,4($3)
.size std_exception_what, .-std_exception_what

.globl cxa_get_globals_fast
.type cxa_get_globals_fast, @function
cxa_get_globals_fast:
    lui $2,%hi(p53_cxa_globals)
    jr $31
    addiu $2,$2,%lo(p53_cxa_globals)
.size cxa_get_globals_fast, .-cxa_get_globals_fast

.globl cxa_get_globals
.type cxa_get_globals, @function
cxa_get_globals:
    lui $2,%hi(p53_cxa_globals)
    jr $31
    addiu $2,$2,%lo(p53_cxa_globals)
.size cxa_get_globals, .-cxa_get_globals

.globl std_set_new_handler
.type std_set_new_handler, @function
std_set_new_handler:
    lui $3,%hi(p53_new_handler)
    lw $2,%lo(p53_new_handler)($3)
    jr $31
    sw $4,%lo(p53_new_handler)($3)
.size std_set_new_handler, .-std_set_new_handler

.globl operator_delete
.type operator_delete, @function
operator_delete:
    addiu $29,$29,-16
    beq $4,$zero,1f
    sd $31,0($29)
    jal p53_call_free
    nop
    1:
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size operator_delete, .-operator_delete

.globl operator_delete_array
.type operator_delete_array, @function
operator_delete_array:
    addiu $29,$29,-16
    sd $31,0($29)
    jal p53_call_operator_delete
    nop
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size operator_delete_array, .-operator_delete_array

.globl p53_size_encoded
.type p53_size_encoded, @function
p53_size_encoded:
    addiu $29,$29,-16
    andi $4,$4,0xff
    addiu $2,$zero,0xff
    sd $31,0($29)
    beq $4,$2,.Lsize_ret
    daddu $3,$zero,$zero
    andi $4,$4,7
    addiu $2,$zero,2
    beq $4,$2,.Lsize_ret
    addiu $3,$zero,2
    slti $2,$4,3
    beq $2,$zero,.Lsize_hi
    addiu $2,$zero,3
    beq $4,$zero,.Lsize_ret
    addiu $3,$zero,4
    .Lsize_abort:
    jal p53_call_abort
    nop
    .Lsize_ret:
    ld $31,0($29)
    .Lsize_move:
    daddu $2,$3,$zero
    jr $31
    addiu $29,$29,16
    nop
    .Lsize_hi:
    beq $4,$2,.Lsize_ret
    addiu $3,$zero,4
    addiu $2,$zero,4
    bne $4,$2,.Lsize_abort
    addiu $3,$zero,8
    b .Lsize_move
    ld $31,0($29)
.size p53_size_encoded, .-p53_size_encoded

.globl p53_terminate_helper
.type p53_terminate_helper, @function
p53_terminate_helper:
    addiu $29,$29,-16
    sd $31,0($29)
    jalr $31,$4
    nop
    jal p53_call_abort
    nop
    ld $31,0($29)
    jr $31
    addiu $29,$29,16
.size p53_terminate_helper, .-p53_terminate_helper

.globl p53_std_terminate
.type p53_std_terminate, @function
p53_std_terminate:
    lui $2,%hi(p53_terminate_handler)
    addiu $29,$29,-16
    lw $4,%lo(p53_terminate_handler)($2)
    sd $31,0($29)
    jal p53_call_terminate_helper
    nop
.size p53_std_terminate, .-p53_std_terminate

.globl p53_unexpected_helper
.type p53_unexpected_helper, @function
p53_unexpected_helper:
    addiu $29,$29,-16
    sd $31,0($29)
    jalr $31,$4
    nop
    jal p53_call_std_terminate
    nop
.size p53_unexpected_helper, .-p53_unexpected_helper

.globl p53_std_unexpected
.type p53_std_unexpected, @function
p53_std_unexpected:
    lui $2,%hi(p53_unexpected_handler)
    addiu $29,$29,-16
    lw $4,%lo(p53_unexpected_handler)($2)
    sd $31,0($29)
    jal p53_call_unexpected_helper
    nop
.size p53_std_unexpected, .-p53_std_unexpected

.globl p53_set_terminate
.type p53_set_terminate, @function
p53_set_terminate:
    lui $3,%hi(p53_terminate_handler)
    lw $2,%lo(p53_terminate_handler)($3)
    jr $31
    sw $4,%lo(p53_terminate_handler)($3)
.size p53_set_terminate, .-p53_set_terminate

.globl p53_set_unexpected
.type p53_set_unexpected, @function
p53_set_unexpected:
    lui $3,%hi(p53_unexpected_handler)
    lw $2,%lo(p53_unexpected_handler)($3)
    jr $31
    sw $4,%lo(p53_unexpected_handler)($3)
.size p53_set_unexpected, .-p53_set_unexpected

.globl p53_read_uleb
.type p53_read_uleb, @function
p53_read_uleb:
    daddu $6,$zero,$zero
    daddu $7,$zero,$zero
    addiu $8,$zero,-128
    nop
    .Luleb_loop:
    lbu $3,0($4)
    addiu $4,$4,1
    andi $2,$3,0x7f
    and $3,$3,$8
    sllv $2,$2,$6
    addiu $6,$6,7
    bne $3,$zero,.Luleb_loop
    or $7,$2,$7
    sd $7,0($5)
    jr $31
    daddu $2,$4,$zero
.size p53_read_uleb, .-p53_read_uleb

.globl p53_read_sleb
.type p53_read_sleb, @function
p53_read_sleb:
    daddu $7,$zero,$zero
    daddu $8,$zero,$zero
    addiu $9,$zero,-128
    nop
    .Lsleb_loop:
    lbu $6,0($4)
    addiu $4,$4,1
    andi $2,$6,0x7f
    and $3,$6,$9
    sllv $2,$2,$7
    addiu $7,$7,7
    bne $3,$zero,.Lsleb_loop
    or $8,$2,$8
    sltiu $2,$7,64
    beql $2,$zero,.Lsleb_store
    sd $8,0($5)
    andi $2,$6,0x40
    beql $2,$zero,.Lsleb_store
    sd $8,0($5)
    addiu $2,$zero,-1
    dsllv $2,$2,$7
    or $8,$8,$2
    sd $8,0($5)
    .Lsleb_store:
    jr $31
    daddu $2,$4,$zero
.size p53_read_sleb, .-p53_read_sleb

