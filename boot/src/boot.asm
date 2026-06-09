[org 0x7c00]                                ;sve pocinje 

start:
    [bits 16]

    cli ; iskljucujem prekide

    xor ax, ax                  ; ax = 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov [BOOT_DRIVE], dl      ; dl dobija vrednost diska sa kojeg bootujem

    mov bp, 0x9000
    mov sp, bp                ; postavljam stek sa sigurnu udaljenost da moze da raste

    sti                         ; Ponovo uključi prekide

    mov bx, REALMODE_MSG      ; ispisi da smo u realnom modu
    call print_bx_string


    call load_kernel

   call switch_to_pm



jmp $ ;beskonacna petlja


%include "../print/print_bx_string.asm"           ; dodajem funkciju
%include "../data/data.asm"   
%include "../disk/disk.asm"   
%include "../protected_mode/gdt_start.asm" 
%include "../protected_mode/switch_to_pm.asm" 
%include "../protected_mode/print_string_protectedm.asm" 
%include "../kernel/load_kernel.asm"   

;-------PRELAZAK U 32-OBITNI MOD------
    [bits 32]

BEGIN_PM:

    mov ebx, PROTECTEDMODE_MSG
    call print_string_protectedm
 


    jmp KERNEL_OFFSET

    jmp $ ;beskonacna petlja

;------KRAJ 32BITNOG---------


times 510 - ($ - $$) db 0   
dw 0xaa55                   
