[bits 16]

load_kernel:
    mov bx, LOAD_KERNEL_MSG
    call print_bx_string

    xor ax, ax
    mov es, ax             ; bios fja za citanje diska se uvek upisuje na es:bx pa stavljamo da je nula
    mov bx, KERNEL_OFFSET  ;lokacija 0x1000
                      
    mov dh, 40
    call disk_load          ;kernel se ucitava na ES:BX = 0x0000:KERNEL_OFFSET

    ret