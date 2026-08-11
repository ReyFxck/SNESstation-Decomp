# Progress 8 runtime map — libsupc++ RTTI and exception core

Progress 8 crosses the clean `0x001a9fa8` frontier and closes the contiguous
GNU C++ runtime corridor through `0x001ab3bc`. Target assembly and target data
remain authoritative; historical GCC material is only a secondary cross-check.
Nothing in this document is a byte-identical **MATCHING** claim.

## Hard boundaries

- `0x001a9fa8` — first `std::type_info` destructor leaf.
- `0x001aa100` — first `std::type_info` virtual query/catch leaves.
- `0x001aa130` — `__cxxabiv1::__class_type_info` destructor family.
- `0x001aa1b8` — `__si_class_type_info` destructor family.
- `0x001aa240` — `__vmi_class_type_info` destructor family.
- `0x001aae70` — public Itanium ABI `__dynamic_cast`.
- `0x001aafb8` — `__cxa_allocate_exception`.
- `0x001ab0f0` — `__cxa_begin_catch`.
- `0x001ab158` — `__cxa_end_catch`.
- `0x001ab200` — `std::uncaught_exception()`.
- `0x001ab228` — standard-exception destructor/`what()` family.
- `0x001ab308` — EH globals and `std::set_new_handler` leaves.
- `0x001ab3bc` — final return of `std::bad_alloc` deleting destructor.
- `0x001ab3c0` — next clean frontier; code immediately switches to an EE
  CP0/cache-maintenance routine (`mfc0`, `cache`, `sync`, `DIntr/EIntr`).

The focused target extract is kept in
`analysis/functions/libsupcxx_rtti_001a9fa8.asm`.

## RTTI data proves the class hierarchy

The target's read-only type-info objects contain these exact mangled names:

| Target string | Decoded class |
|---|---|
| `7IniFile` | application `IniFile` class |
| `N10__cxxabiv121__vmi_class_type_infoE` | `__vmi_class_type_info` |
| `N10__cxxabiv120__si_class_type_infoE` | `__si_class_type_info` |
| `N10__cxxabiv117__class_type_infoE` | `__class_type_info` |
| `St10bad_typeid` | `std::bad_typeid` |
| `St8bad_cast` | `std::bad_cast` |
| `St9type_info` | `std::type_info` |
| `St13bad_exception` | `std::bad_exception` |
| `St9exception` | `std::exception` |
| `St9bad_alloc` | `std::bad_alloc` |

The corresponding target vtable address points are:

| Address point | Type |
|---|---|
| `0x00426d08` | `std::type_info` |
| `0x00426ca8` | `__class_type_info` |
| `0x00426c78` | `__si_class_type_info` |
| `0x00426c48` | `__vmi_class_type_info` |
| `0x00426cf0` | `std::bad_cast` |
| `0x00426cd8` | `std::bad_typeid` |
| `0x00426d98` | `std::exception` |
| `0x00426d80` | `std::bad_exception` |
| `0x00426dc8` | `std::bad_alloc` |

That data makes the virtual-slot mapping unusually strong. After the destructor
slots, `__class_type_info` exposes the expected pointer/function tests, catch,
upcast, dynamic-cast and public-source helpers. This is why the complex VMI
walkers can be named with very-high confidence even when they remain only
`IDENTIFIED` in the manifest.

## Reconstructed RTTI leaves

The independent model in `src/ps2/libsupcxx_rtti_recovered.c` reconstructs:

- all D2/D1/D0 destructor triples for `std::type_info`,
  `__class_type_info`, `__si_class_type_info`, `__vmi_class_type_info`,
  `std::bad_cast`, `std::bad_typeid`, `std::exception`,
  `std::bad_exception`, and `std::bad_alloc`;
- `std::type_info::__is_pointer_p()` and `__is_function_p()`;
- `std::type_info::__do_catch()` and its always-false base upcast leaf;
- `__class_type_info::__do_catch()`;
- the public two-argument `__do_upcast` wrapper;
- the base `__do_find_public_src` leaf;
- protected three-argument upcast for `__class_type_info` and
  `__si_class_type_info`.

The large `__vmi_class_type_info` base walkers and the three `__do_dyncast`
implementations are intentionally **IDENTIFIED, not reconstructed**. Their
control flow, base-info layout and virtual dispatch are mapped, but marking them
green before rewriting and testing the complete ambiguity/virtual-base state
machine would overstate progress.

## `__dynamic_cast`

`0x001aae70` has the canonical Itanium ABI shape directly in the target:

1. read the source object's vptr;
2. obtain whole-object RTTI from `vtable[-1]`;
3. obtain offset-to-top from `vtable[-2]`;
4. build a dynamic-cast result record;
5. dispatch through the whole type's `__do_dyncast` virtual slot;
6. apply public-path/result tests and, for the slow case, dispatch
   `__do_find_public_src` on the destination type.

The function stays `IDENTIFIED` until the full VMI state machine is represented
in independent source.

## Exception allocation and catch state

`__cxa_allocate_exception @ 0x001aafb8` proves a target
`__cxa_exception` size of **0x50 bytes**. It first calls the target allocator for
`thrown_size + 0x50`. On allocation failure it uses four **512-byte** emergency
slots and a 32-bit used-bitmask; requests larger than 512 bytes cannot use the
pool. The 0x50-byte exception header is zeroed before returning the thrown
object at `header + 0x50`.

`__cxa_free_exception @ 0x001ab080` recognizes pointers inside that 0x800-byte
emergency range and clears the corresponding bit; normal exceptions free
`thrown_object - 0x50`.

`__cxa_begin_catch @ 0x001ab0f0` and `__cxa_end_catch @ 0x001ab158` expose the
old ABI bookkeeping cleanly: `handlerCount`, the caught-exception chain, and
`uncaughtExceptions` are all visible at fixed offsets. `std::uncaught_exception
@ 0x001ab200` simply tests whether the globals object's uncaught count is
nonzero.

Both `__cxa_get_globals_fast @ 0x001ab308` and `__cxa_get_globals @ 0x001ab318`
return the same static address `0x004481d0` in this linked runtime.

## Standard exceptions and `what()`

`std::exception`, `std::bad_exception`, and `std::bad_alloc` finish the
corridor. The same `what()` body at `0x001ab2f8` is installed in the related
standard-exception vtables. The machine code resolves the dynamic object's
vtable metadata to its RTTI object and returns that object's name pointer; it
does not use a separate hard-coded literal per function.

`std::set_new_handler @ 0x001ab328` is a two-instruction state swap around the
global handler slot at `0x00426b1c`.

## Progress accounting

After this checkpoint the conservative 1,137-target proxy is:

- **530 reconstructed** — **46.61%**;
- **595 mapped** — **52.33%**;
- **0 matching** — **0.00%**.

Matching stays at zero until the historical EE compiler/runtime environment is
reproduced and complete target functions compare byte-for-byte after relocation
normalization.
