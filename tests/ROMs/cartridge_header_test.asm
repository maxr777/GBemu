SECTION "Header", ROM0[$100]
    nop
    jp Start

SECTION "Code", ROM0[$150]
Start:
    jp Start
