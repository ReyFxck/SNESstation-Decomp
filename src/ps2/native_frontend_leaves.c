/* Small target-proven PS2 frontend functions formerly carried as .word data. */

typedef unsigned int uint32_t;

extern void frontend_leaf_00101814(uint32_t, uint32_t);
extern void frontend_irq_helper_00194498(uint32_t);
extern void frontend_leaf_001041c0(uint32_t, uint32_t);
extern int sprintf(char *, const char *, ...);
extern char DAT_001b0f70[];
extern char DAT_00429288[];
extern char snes_data_001b0000[];
extern uint32_t snes_vtable_00426c28[];

void frontend_control_00101838(void)
{
    frontend_leaf_00101814(1, 0xffff);
}

void snes_leaf_00101858(void) {}
void snes_leaf_00101860(void) {}
void snes_leaf_00101868(void) {}
void snes_leaf_00101870(void) {}
void snes_leaf_00101878(void) {}

void frontend_clear_irq_001018e0(uint32_t source)
{
    frontend_irq_helper_00194498(source);
}

void snes_leaf_001018fc(void) {}
void snes_leaf_00101904(void) {}
void snes_leaf_0010190c(void) {}
int snes_leaf_00101914(void) { return 0; }
void snes_leaf_0010191c(void) {}
int snes_leaf_001019a0(void) { return 0; }

void frontend_control_001041e4(void)
{
    frontend_leaf_001041c0(1, 0xffff);
}

typedef struct FrontendLeafObject {
    uint32_t vtable;
    uint32_t word1;
    uint32_t word2;
} FrontendLeafObject;

void frontend_object_init_00104204(FrontendLeafObject *object)
{
    object->word1 = 0;
    object->word2 = 0;
    object->vtable = (uint32_t)snes_vtable_00426c28;
}

void frontend_object_init_0010421c(FrontendLeafObject *object)
{
    object->word1 = 0;
    object->word2 = 0;
    object->vtable = (uint32_t)snes_vtable_00426c28;
}

void snes_leaf_001056b0(void) {}
int snes_leaf_001056b8(void) { return 1; }

char *frontend_drawsync_001056e0(void)
{
    sprintf(DAT_00429288, DAT_001b0f70);
    return DAT_00429288;
}

int frontend_audio_compare_00105e38(const int *left, const int *right)
{
    return *left - *right;
}

int snes_leaf_0010e33c(void)
{
    return (int)(snes_data_001b0000 + 0x1b98);
}

int snes_leaf_0010e348(void)
{
    return (int)(snes_data_001b0000 + 0x1be0);
}

int snes_leaf_0010e354(void)
{
    return (int)(snes_data_001b0000 + 0x1c00);
}
