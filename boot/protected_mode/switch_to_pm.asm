[bits 16]

switch_to_pm:
    cli                      ; Gasi hardverske interrupte kao sto su mis, disk, tastatura
    lgdt [gdt_descriptor]    ; ucitava adresu GDT-a

    mov eax, cr0             ; kontrolni registar procesora   
    or eax, 0x1              ; pravim masku da bi prvi bit postao 1 i tako prelazi u protected mod
    mov cr0, eax             ; CPU (cpu nam je mozak racunara on ucitava, obradjuje i salje rez) U JE UPROTECTED MODU!!




    jmp CODE_SEG:init_pm     ; prelazimo na drugu lokaciju jmp SEGMENT(koji segment u gdt):OFFSET(gde unutar segmenta kod pocinje-ovde labela)

[bits 32]

init_pm:

    mov ax, DATA_SEG         
    mov ds, ax               ; data segment
    mov ss, ax               ; stek segment
    mov es, ax               ; ostali segmenti
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000         ; postavljam stek
    mov esp, ebp

    call BEGIN_PM