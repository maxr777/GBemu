#ifndef CONSTANTS_H
#define CONSTANTS_H

// ================ HARDWARE CONSTANTS ================

#define ROM_BANK_SIZE	0x4000
#define VRAM_SIZE	0x2000
#define EXTERN_RAM_SIZE 0x2000
#define WRAM_SIZE	0x1000
#define CPU_FREQ	4194304
// cycles per scanline: 456
// scanlines per frame: 154 (144 visible + 10 vblank lines)
#define CYCLES_PER_SCANLINE 456 * 154 // 70224
// CPU clock: 4.194304 MHz
// 4194304 / 70224 = 59.7275Hz (Vertical sync)
#define TARGET_FRAMETIME 1.0f / (4194304.0f / 70224)

#endif
