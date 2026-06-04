SECTION "Header", ROM0[$100]
    nop
    jp Start

SECTION "Logo", ROM0[$104]
    db $00, $01, $02, $03, $04, $05, $06, $07
    db $08, $09, $0A, $0B, $0C, $0D, $0E, $0F
    db $10, $11, $12, $13, $14, $15, $16, $17
    db $18, $19, $1A, $1B, $1C, $1D, $1E, $1F
    db $20, $21, $22, $23, $24, $25, $26, $27
    db $28, $29, $2A, $2B, $2C, $2D, $2E, $2F

SECTION "Code", ROM0[$150]
Start:
    jp Start
