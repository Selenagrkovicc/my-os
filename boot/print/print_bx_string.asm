print_bx_string:
    pusha

ispis:
    mov al, [bx]  ; pokazivac na jedan karakter
    cmp al, 0     ; ako je nula onda je kraj stringa
    je done                         

    mov ah, 0x0e  ; sa ovom fjom poziva prekid 0x10
    int 0x10      ; ispisuje jedan karakter na ekran 

    inc bx        ; bx pokazuje na sledeci
    jmp ispis

done:
    popa
    ret
