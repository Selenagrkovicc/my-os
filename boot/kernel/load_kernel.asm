[bits 16]

load_kernel:
    mov bx, LOAD_KERNEL_MSG
    call print_bx_string

    xor ax, ax
    mov es, ax
    mov bx, KERNEL_OFFSET

    mov dh, 40
    call disk_load

    ret