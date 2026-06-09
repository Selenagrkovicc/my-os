disk_load:
    pusha

    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    mov dl, [BOOT_DRIVE]

    int 0x13
    jc disk_error

    popa
    ret

disk_error:
    mov bx, DISK_GRESKA_MSG
    call print_bx_string
    jmp $