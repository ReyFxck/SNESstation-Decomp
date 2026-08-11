/*
 * SNES Station v0.23 -- SPC700/APU memory access leaves.
 * Target: 0x001ac994..0x001acd03.
 *
 * Independent target-driven model of S9xAPUGet/SetByte and direct-page Z
 * variants. The f0..ff hardware window, timer read-clear behavior, control/DSP
 * callbacks and the ffc0 IPL-ROM shadow rule are all visible in target code.
 */
#include <stdint.h>

typedef uint8_t (*SnesApuDspRead)(void *opaque);
typedef void (*SnesApuDspWrite)(uint8_t value, void *opaque);
typedef void (*SnesApuControlWrite)(uint8_t value, void *opaque);

typedef struct {
    uint8_t *ram;
    uint8_t *direct_page;
    uint8_t out_ports[4];
    uint16_t timer_target[3];
    uintptr_t wait_address1;
    uintptr_t wait_address2;
    uint8_t show_rom;
    uint8_t extra_ram[64];
    SnesApuDspRead dsp_read;
    SnesApuDspWrite dsp_write;
    SnesApuControlWrite control_write;
    void *opaque;
} SnesApuMemModel;

static void apu_wait_touch(SnesApuMemModel *a)
{
    uintptr_t old = a->wait_address1;
    a->wait_address1 = (uintptr_t)a->ram;
    a->wait_address2 = old;
}

static uint8_t apu_io_read(SnesApuMemModel *a, uint16_t address)
{
    if (address >= 0xf4 && address <= 0xf7) {
        apu_wait_touch(a);
        return a->ram[address];
    }
    if (address >= 0xfd && address <= 0xff) {
        uint8_t value;
        apu_wait_touch(a);
        value = a->ram[address];
        a->ram[address] = 0;
        return value;
    }
    if (address == 0xf3 && a->dsp_read != 0)
        return a->dsp_read(a->opaque);
    return a->ram[address];
}

static void apu_io_write(SnesApuMemModel *a, uint8_t value, uint16_t address)
{
    if (address >= 0xf4 && address <= 0xf7) {
        a->out_ports[address - 0xf4] = value;
        return;
    }
    if (address == 0xf1) {
        if (a->control_write != 0) a->control_write(value, a->opaque);
        return;
    }
    if (address == 0xf3) {
        if (a->dsp_write != 0) a->dsp_write(value, a->opaque);
        return;
    }
    if (address >= 0xfa && address <= 0xfc) {
        a->ram[address] = value;
        a->timer_target[address - 0xfa] = value == 0 ? 0x100u : value;
        return;
    }
    if (address >= 0xfd && address <= 0xff)
        return;
    a->ram[address] = value;
}

/* 0x001ac994 */
uint8_t S9xAPUGetByte_001ac994(SnesApuMemModel *a, uint8_t address)
{
    if (address < 0xf0 || a->direct_page != a->ram)
        return a->ram[address];
    return apu_io_read(a, address);
}

/* 0x001aca50 */
void S9xAPUSetByte_001aca50(SnesApuMemModel *a, uint8_t value, uint8_t address)
{
    if (address < 0xf0 || a->direct_page != a->ram) {
        a->ram[address] = value;
        return;
    }
    apu_io_write(a, value, address);
}

/* 0x001acb3c */
uint8_t S9xAPUGetByteZ_001acb3c(SnesApuMemModel *a, uint16_t address)
{
    if (address >= 0xf0 && address <= 0xff)
        return apu_io_read(a, address);
    return a->direct_page[address];
}

/* 0x001acbf0 */
void S9xAPUSetByteZ_001acbf0(SnesApuMemModel *a, uint8_t value, uint16_t address)
{
    if (address >= 0xf0 && address <= 0xff) {
        apu_io_write(a, value, address);
        return;
    }

    if (address >= 0xffc0) {
        /*
         * Target keeps the 64-byte IPL overlay shadow separately from the
         * active direct page. The shadow is always updated; when ShowROM is
         * true the visible direct-page byte is deliberately left untouched.
         */
        a->extra_ram[address - 0xffc0u] = value;
        if (a->show_rom)
            return;
    }

    a->direct_page[address] = value;
}
