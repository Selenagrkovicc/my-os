VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

print_string_protectedm:
    pusha
    mov edx, VIDEO_MEMORY     ; edx postaje podecat video memorije

print_string_pm_loop:
    mov al, [ebx]             ; Uzimamo karakter na koji pokazuje EBX
    mov ah, WHITE_ON_BLACK    ; Postavljamo boju, bela slova i crnu pozadinu
    
    cmp al, 0                 
    je print_string_pm_gotovo   ; Ako je slovo kraj stringa onda zavrsavamo
    
    mov [edx], ax             ; Ispisujemo na ekran
    
    inc ebx                   ; Sledeći karakter u stringu
    add edx, 2                ; Sledeće mesto na ekranu
    jmp print_string_pm_loop  ; Ponovi petlju

print_string_pm_gotovo:
    popa
    ret