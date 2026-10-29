/*
  _____     ___ ____
   ____|   |    ____|      PS2Lib/ps2sdk
  |     ___|   |____       (c) 2003 Marcus R. Brown (mrbrown@0xd6.org)
  ------------------------------------------------------------------------
  glue.c                   EE kernel glue and utility routines.

*/

#include "glue_legacy_compat.h"

/*
 * SNES Station target trace:
 *   0x0019fb00 EnableDmac
 *   0x0019fb78 DisableDmac
 *
 * Historical PS2LIB built glue.c once per F_* selector.  With no selector,
 * compile only the two DMAC wrappers present in this recovery corridor.
 *
 * ========================================================================
 * REGISTER CONVENTIONS (MIPS EE / GCC 3.2.2):
 * ------------------------------------------------------------------------
 * COP0 Register Access:
 *   mfc0 rt, rd    - Move from Coprocessor 0 (rt = CP0[rd])
 *   mtc0 rt, rd    - Move to Coprocessor 0 (CP0[rd] = rt)
 *
 * COP0 Register 12 = Status Register:
 *   Bit 0 (0x1):    IE  - Interrupt Enable (current)
 *   Bit 1 (0x2):    EXL - Exception Level
 *   Bit 2 (0x4):    ERL - Error Level
 *   Bit 16 (0x10000): IM - Interrupt Mask bits (actually bits 8-15)
 *   Bit 16 (0x10000): IE  - Interrupt Enable (in some contexts)
 *   Actually on EE: Bit 0 = IEc (current IE), Bit 1 = IEp (previous IE), Bit 2 = IEo (old IE)
 *
 * Interrupt Instructions:
 *   di  - Disable Interrupts (clear IE bit in Status)
 *   ei  - Enable Interrupts (set IE bit in Status)
 *   sync.p - Pipeline sync (wait for all pending operations)
 *
 * ========================================================================
 * INLINE ASSEMBLY PATTERNS:
 * ------------------------------------------------------------------------
 * - mfc0 %0, $12 : Read COP0 Status into output register
 * - di / ei : Simple interrupt control
 * - "memory" clobber: Prevents compiler reordering around interrupt changes
 * - volatile: Prevents compiler optimization/elimination
 */

#if !defined(F_DIntr) && !defined(F_EIntr) && !defined(F_EnableIntc) && \
    !defined(F_DisableIntc) && !defined(F_EnableDmac) && \
    !defined(F_DisableDmac) && !defined(F_iEnableIntc) && \
    !defined(F_iDisableIntc) && !defined(F_iEnableDmac) && \
    !defined(F_iDisableDmac) && !defined(F_SyncDCache) && \
    !defined(F_iSyncDCache) && !defined(F_InvalidDCache) && \
    !defined(F_iInvalidDCache)
#define F_EnableDmac
#define F_DisableDmac
#endif

#ifdef F_DIntr
/* Disable Interrupts (DIntr)
 * Returns: 1 if interrupts were enabled, 0 if already disabled
 *
 * Register usage:
 *   eie = COP0 Status & 0x10000 (IE bit)
 *
 * Algorithm:
 * 1. Read COP0 Status register
 * 2. Extract IE bit (bit 16 = 0x10000)
 * 3. If IE=0, return 0 (already disabled)
 * 4. Execute DI (disable interrupts) + SYNC.P (pipeline sync)
 * 5. Poll Status until IE bit clears
 * 6. Return 1 (was enabled)
 */
int DIntr()
{
	int eie, res;

	__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;
	res = eie != 0;

	if (!eie)
		return 0;

	asm (".p2align 3");
	do {
		__asm__ volatile ("di");           /* Disable interrupts */
		__asm__ volatile ("sync.p");       /* Pipeline sync */
		__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));  /* Read Status */
		eie &= 0x10000;                    /* Check IE bit */
	} while (eie);                         /* Loop until IE=0 */

	return res;
}
#endif

#ifdef F_EIntr
/* Enable Interrupts (EIntr)
 * Returns: 1 if interrupts were enabled, 0 if they were disabled
 *
 * Register usage:
 *   eie = COP0 Status & 0x10000 (IE bit)
 *
 * Algorithm:
 * 1. Read COP0 Status register
 * 2. Extract IE bit (bit 16 = 0x10000)
 * 3. Execute EI (enable interrupts)
 * 4. Return previous IE state
 */
int EIntr()
{
	int eie;

	__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;
	__asm__ volatile ("ei");

	return eie != 0;
}
#endif

#ifdef F_EnableIntc
/* EnableIntc: Enable INTC interrupt line with interrupt protection
 * Args: intc = interrupt line number
 * Returns: Result from _EnableIntc
 *
 * Algorithm:
 * 1. Read COP0 Status, save IE bit
 * 2. If interrupts enabled, disable them (DI)
 * 3. Call _EnableIntc(intc)
 * 4. EE_SYNC (memory barrier)
 * 5. If interrupts were enabled, re-enable them (EI)
 * 6. Return result
 */
int EnableIntc(int intc)
{
	int eie, res;

	__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();  /* Disable interrupts if they were enabled */

	res = _EnableIntc(intc);
	EE_SYNC();

	if (eie)
		EI();  /* Restore interrupt state */

	return res;
}
#endif

#ifdef F_DisableIntc
/* DisableIntc: Disable INTC interrupt line with interrupt protection
 * Args: intc = interrupt line number
 * Returns: Result from _DisableIntc
 *
 * Algorithm: Same as EnableIntc but calls _DisableIntc
 */
int DisableIntc(int intc)
{
	int eie, res;

	__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _DisableIntc(intc);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#ifdef F_EnableDmac
/* EnableDmac: Enable DMAC channel with interrupt protection
 * Args: dmac = DMAC channel number
 * Returns: Result from _EnableDmac
 *
 * Algorithm: Same pattern - save IE, DI, call, sync, restore IE
 */
int EnableDmac(int dmac)
{
	int eie, res;

	__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _EnableDmac(dmac);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#ifdef F_DisableDmac
/* DisableDmac: Disable DMAC channel with interrupt protection
 * Args: dmac = DMAC channel number
 * Returns: Result from _DisableDmac
 *
 * Algorithm: Same pattern
 */
int DisableDmac(int dmac)
{
	int eie, res;

	__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	res = _DisableDmac(dmac);
	EE_SYNC();

	if (eie)
		EI();

	return res;
}
#endif

#ifdef F_iEnableIntc
/* iEnableIntc: Enable INTC interrupt line (interrupt context - no DI/EI)
 * Args: intc = interrupt line number
 * Returns: Result from _iEnableIntc
 *
 * Note: "i" prefix = interrupt context (called from ISR, interrupts already disabled)
 * Just calls the internal function and syncs
 */
int iEnableIntc(int intc)
{
	int res = _iEnableIntc(intc);
	EE_SYNC();

	return res;
}
#endif

#ifdef F_iDisableIntc
/* iDisableIntc: Disable INTC interrupt line (interrupt context)
 * Args: intc = interrupt line number
 * Returns: Result from _iDisableIntc
 */
int iDisableIntc(int intc)
{
	int res = _iDisableIntc(intc);
	EE_SYNC();

	return res;
}
#endif

#ifdef F_iEnableDmac
/* iEnableDmac: Enable DMAC channel (interrupt context)
 * Args: dmac = DMAC channel number
 * Returns: Result from _iEnableDmac
 */
int iEnableDmac(int dmac)
{
	int res = _iEnableDmac(dmac);
	EE_SYNC();

	return res;
}
#endif

#ifdef F_iDisableDmac
/* iDisableDmac: Disable DMAC channel (interrupt context)
 * Args: dmac = DMAC channel number
 * Returns: Result from _iDisableDmac
 */
int iDisableDmac(int dmac)
{
	int res = _iDisableDmac(dmac);
	EE_SYNC();

	return res;
}
#endif

#ifdef F_SyncDCache
/* SyncDCache: Synchronize data cache with memory (with interrupt protection)
 * Args: start = start address, end = end address
 * Returns: void
 *
 * Algorithm:
 * 1. Read COP0 Status, save IE bit
 * 2. If interrupts enabled, disable them (DI)
 * 3. Align addresses to 64-byte cache lines (start & ~0x3f, end & ~0x3f)
 * 4. Call _SyncDCache(aligned_start, aligned_end)
 * 5. If interrupts were enabled, re-enable them (EI)
 *
 * Note: Cache lines on EE are 64 bytes (0x40), so align down to 64-byte boundary
 */
void SyncDCache(void *start, void *end)
{
	int eie;

	__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	_SyncDCache((void *)((u32)start & 0xffffffc0), (void *)((u32)end & 0xffffffc0));

	if (eie)
		EI();
}
#endif

#ifdef F_iSyncDCache
/* iSyncDCache: Synchronize data cache (interrupt context - no DI/EI)
 * Args: start = start address, end = end address
 * Returns: void
 */
void iSyncDCache(void *start, void *end)
{
	_SyncDCache((void *)((u32)start & 0xffffffc0), (void *)((u32)end & 0xffffffc0));
}
#endif

#ifdef F_InvalidDCache
/* InvalidDCache: Invalidate data cache (with interrupt protection)
 * Args: start = start address, end = end address
 * Returns: void
 *
 * Algorithm: Same as SyncDCache but calls _InvalidDCache
 */
void InvalidDCache(void *start, void *end)
{
	int eie;

	__asm__ volatile ("mfc0\t%0, $12" : "=r" (eie));
	eie &= 0x10000;

	if (eie)
		DI();

	_InvalidDCache((void *)((u32)start & 0xffffffc0), (void *)((u32)end & 0xffffffc0));

	if (eie)
		EI();
}
#endif

#ifdef F_iInvalidDCache
/* iInvalidDCache: Invalidate data cache (interrupt context)
 * Args: start = start address, end = end address
 * Returns: void
 */
void iInvalidDCache(void *start, void *end)
{
	_InvalidDCache((void *)((u32)start & 0xffffffc0), (void *)((u32)end & 0xffffffc0));
}
#endif
