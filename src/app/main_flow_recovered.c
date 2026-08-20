/*
 * High-level recovery of SNES Station v0.23 main() after startup.
 * Original range covered here: 0x00105060 .. 0x0010569c.
 *
 * This intentionally does not invent names for still-unknown routines.
 */

#include <stdint.h>
#include <string.h>

extern void sub_00104a54(void);
extern void load_config_00106054(void);
extern int  sub_00106824(void);
extern void sub_001060dc(void);
extern void sub_00101ef0(void);
extern void restart_embedded_mod_00105d78(void);
extern int  memory_card_setup_0010689c(void);
extern void top_level_gui_00102ab0(char *selected_rom);
extern int  CMemory_Init_recovered(void *memory);
extern int  CMemory_LoadROM_001513bc(void *memory, const char *rom);
extern char *build_sram_path_00105750(void);
extern int  CMemory_LoadSRAM_recovered(void *memory, const char *path);
extern int  apu_buffers_init_0010a840(void);
extern void rom_cleanup_00151330(void *memory);
extern int  puts(const char *s);

extern uint8_t g_Settings_blob[0x148];        /* EE VA 0x003454e0 */
extern void   *g_Memory;                      /* logical VA 0x0035e2b0 */
extern int     g_memory_card_available;
extern char    g_selected_rom_path[];         /* EE VA 0x00428180 */

extern uint32_t embedded_disclaimer_xor[];     /* EE VA 0x002ec200 */
extern int      embedded_disclaimer_bytes;    /* EE VA 0x002ec53c = 0x33a */
extern uint32_t embedded_credits_xor[];        /* EE VA 0x002ebc30 */
extern int      embedded_credits_bytes;       /* EE VA 0x002ec1f4 = 0x5c1 */

/* The target intentionally leaves a one- or two-byte tail outside the loop. */
static void xor_words(uint32_t *p, int byte_count)
{
    int words = byte_count / 4;
    int i;
    for (i = 0; i < words; ++i)
        p[i] ^= 0x96695aa5u;
}

void main_after_cdvd_recovered(void)
{
    /* The original initializes its current directory to "/" here. */
    sub_00104a54();

    memset(g_Settings_blob, 0, 0x148);

    /* Exact main() order at 0x001050b4 and 0x00105104. */
    xor_words(embedded_disclaimer_xor, embedded_disclaimer_bytes);
    xor_words(embedded_credits_xor, embedded_credits_bytes);

    load_config_00106054();
    if (sub_00106824())
        sub_001060dc();

    sub_00101ef0();
    /* Restarts the embedded ProTracker module "can't stop coming" by Azazel. */
    restart_embedded_mod_00105d78();
    g_memory_card_available = memory_card_setup_0010689c();

rom_selector:
    top_level_gui_00102ab0(g_selected_rom_path);
    puts("Returned from GUI");

    /* 0x105210..0x105318 writes the original Snes9x 1.41 Settings
       defaults field-by-field. Keep the raw assembly until the 1.41
       SSettings layout is fully recovered. */

    if (!CMemory_Init_recovered(g_Memory) || !apu_buffers_init_0010a840()) {
        puts("Failed to Init Memory & APU");
        /* original exits through sub_00104e58 */
        return;
    }

    if (!CMemory_LoadROM_001513bc(g_Memory, g_selected_rom_path)) {
        puts("Memory.LoadROM returned FALSE");
        rom_cleanup_00151330(g_Memory);
        goto rom_selector;
    }

    if (g_memory_card_available == 1)
        CMemory_LoadSRAM_recovered(g_Memory, build_sram_path_00105750());

    /* Renderer/audio/game loop setup occupies 0x105384..0x105574.
       It is intentionally left address-labelled for the next milestone. */

    rom_cleanup_00151330(g_Memory);
    restart_embedded_mod_00105d78();
    goto rom_selector;
}
