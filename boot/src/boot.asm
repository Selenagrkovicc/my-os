[org 0x7c00]                  ;ponasa se kao da je na adresi ovoj jer ga je bios stvarno stavio tu
                              ;stavi bajtove na ovu adresu, tj prvih 512 bajtova kad nadje 0xaa55 i dalje nastavlja moj kod 
start:
    [bits 16]                 ; racunar se uvek pokrece u 16bitnom modu

    cli                       ; iskljucujem prekide
; sve segmente na nula
    xor ax, ax                ; ax = 0
    mov ds, ax
    mov es, ax                ; es je nula
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov [BOOT_DRIVE], dl      ; dl dobija vrednost diska sa kojeg bootujem

    mov bp, 0x9000
    mov sp, bp                ; postavljam stek sa sigurnu udaljenost da moze da raste

    sti                       ; Ponovo uključi prekide

    mov bx, REALMODE_MSG      ; ispisi da smo u realnom modu
    call print_bx_string


    call load_kernel          

    call switch_to_pm       



jmp $ ;beskonacna petlja


%include "../print/print_bx_string.asm"         
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
 


    jmp KERNEL_OFFSET ; sad skace na adresu na koju smo ucitali kernel

    jmp $ ;beskonacna petlja

;------KRAJ 32BITNOG---------


times 510 - ($ - $$) db 0   
dw 0xaa55                   
