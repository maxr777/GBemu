#ifndef CONSTANTS_H
#define CONSTANTS_H

// cycles per scanline: 456
// scanlines per frame: 154 (144 visible + 10 vblank lines)
#define CYCLES_PER_SCANLINE 456 * 154 // 70224
// CPU clock: 4.194304 MHz
// 4194304 / 70224 = 59.7275Hz (Vertical sync)
#define TARGET_FRAMETIME 1.0f / (4194304.0f / 70224)

#endif
