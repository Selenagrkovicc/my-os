print_bx_string:
    pusha

ispis:
    mov al, [bx]
    cmp al, 0
    je done

    mov ah, 0x0e
    int 0x10

    inc bx
    jmp ispis

done:
    popa
    ret
