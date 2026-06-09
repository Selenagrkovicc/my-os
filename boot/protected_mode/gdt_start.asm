
;----------GDT----------------

gdt_start:

gdt_null :
    dd 0x0                            ; nul mora biti prvo
    dd 0x0

gdt_code :
    dw 0xffff                   ;donjih 16 bitova limit
    dw 0x0                      ; baza donji deo
    db 0x0                      ; baza srednji deo
    db 10011010b                ; bajt
    db 11001111b                ; bajt
    db 0x0                      ; baza gornja

gdt_data :
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end :

gdt_descriptor :
    dw gdt_end - gdt_start - 1       ; dobija se adresa i velicina gdt
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start    ; definisali smo code seg
DATA_SEG equ gdt_data - gdt_start    


;gdt tabela kaze gde pocinje segment,njegova velicina, njegove dozvole, da li je kod ili data segment