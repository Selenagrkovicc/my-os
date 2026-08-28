disk_load:
    pusha

    mov ah, 0x02  ;postoje puno skupa fja a ova kaze da se cita sektor sa diska
    mov al, dh    ; 40 sektor diska
    mov ch, 0x00  ; cilindar
    mov cl, 0x02  ; sektor dva(sektor 1 je bootloader)
    mov dh, 0x00  ;head
    mov dl, [BOOT_DRIVE]  ; disk sa kojeg citam

    int 0x13      ;prekid za rad sa diskom disk ucitava u ES:BX
    jc disk_error ;ako cistanje nije uspesno dolazi do cerry flaga

    popa
    ret

disk_error:
    mov bx, DISK_GRESKA_MSG
    call print_bx_string
    jmp $