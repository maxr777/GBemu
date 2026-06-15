#ifndef CONSTANTS_H
#define CONSTANTS_H

// ==================== HARDWARE CONSTANTS ====================

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

// ==================== MEMORY MAP ====================

#define ROM_BANK_0_ADDR 0x0000
#define ROM_BANK_N_ADDR 0x4000
#define VRAM_ADDR	0x8000
#define EXTERN_RAM_ADDR 0xA000
#define WRAM_0_ADDR	0xC000
#define WRAM_N_ADDR	0xD000 // DMG = extension of WRAM_0, CGB = switchable bank 1-7
#define ECHO_RAM_ADDR	0xE000 // Prohibited, mirror of both WRAMs
#define OAM_ADDR	0xFE00
#define INVAL_MEM_ADDR	0xFEA0 // Prohibited
#define IO_REGS_ADDR	0xFF00
#define HRAM_ADDR	0xFF80
#define INT_ENABLE_ADDR 0xFFFF

#endif
