#ifndef CONSTANTS_H
#define CONSTANTS_H

// ==================== HARDWARE CONSTANTS ====================

#define ROM_BANK_SIZE	0x4000
#define VRAM_SIZE	0x2000
#define EXTERN_RAM_SIZE 0x2000
#define WRAM_SIZE	0x1000
#define CPU_FREQ	4194304
// Cycles per scanline: 456
// Scanlines per frame: 154 (144 visible + 10 vblank lines)
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

// ================ IO REGISTERS ================

// joypad input
#define JOYPAD_INPUT 0xFF00

// serial transfer
#define SERIAL_TRANSFER 0xFF01
#define SERIAL_CONTROL	0xFF02

// timer and divider
#define DIV_ADDR  0xFF04
#define TIMA_ADDR 0xFF05
#define TMA_ADDR  0xFF06
#define TAC_ADDR  0xFF07

// interrputs
#define INTERRUPT_FLAG 0xFF0F

// audio
#define CH1_SWEEP	0xFF10
#define CH1_TIMER	0xFF11
#define CH1_VOLUME	0xFF12
#define CH1_PERIOD_LOW	0xFF13
#define CH1_PERIOD_HIGH 0xFF14
#define CH2_TIMER	0xFF16
#define CH2_VOLUME	0xFF17
#define CH2_PERIOD_LOW	0xFF18
#define CH2_PERIOD_HIGH 0xFF19
#define CH3_DAC_ENABLE	0xFF1A
#define CH3_LEN_TIMER	0xFF1B
#define CH3_OUTPUT_LVL	0xFF1C
#define CH3_PERIOD_LOW	0xFF1D
#define CH3_PERIOD_HIGH 0xFF1E
#define CH4_LEN_TIMER	0xFF20
#define CH4_VOLUME	0xFF21
#define CH4_FREQUENCY	0xFF22
#define CH4_CONTROL	0xFF23
#define MASTER_VOLUME	0xFF24
#define SOUND_PANNING	0xFF25
#define AUDIO_CONTROL	0xFF26

// wave pattern
#define WAVE_PATTERN_RAM 0xFF30

// lcd control, status, position, scrolling, palettes
#define LCD_CONTROL	  0xFF40
#define LCD_STATUS	  0xFF41
#define BG_VIEWPORT_Y	  0xFF42
#define BG_VIEWPORT_X	  0xFF43
#define LCD_Y		  0xFF44
#define LCD_LY_CMP	  0xFF45
#define BG_PALETTE_DATA	  0xFF47
#define OBJ_PALETTE0_DATA 0xFF48
#define OBJ_PALETTE1_DATA 0xFF49
#define WINDOW_Y7	  0xFF4A
#define WINDOW_X7	  0xFF4B

// vram bank select
#define VRAM_BANK 0xFF4F

// boot rom mapping control
#define BOOT_ROM_DISABLE 0xFF50

// vram dma
#define VRAM_DMA_SRC_HIGH  0xFF51
#define VRAM_DMA_SRC_LOW   0xFF52
#define VRAM_DMA_DEST_HIGH 0xFF53
#define VRAM_DMA_DEST_LOW  0xFF54
#define VRAM_DMA_START	   0xFF55

// bg/obj palettes
#define CGB_BG_PALETTE_SPEC	   0xFF68
#define CGB_BG_PALETTE_DATA	   0xFF69
#define CGB_OBJ_COLOR_PALETTE_SPEC 0xFF6A
#define CGB_OBJ_COLOR_PALETTE_DATA 0xFF6B

// wram bank select
#define WRAM_BANK 0xFF70

#endif
