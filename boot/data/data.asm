HELLO_WORLD_MSG:
    db 'Hello world', 0

DISK_GRESKA_MSG:
    db 'greska u citanju diska', 0x0D, 0x0A, 0

REALMODE_MSG:
    db 'dobrodosli u REAL MODE', 0x0D, 0x0A, 0

PROTECTEDMODE_MSG:
    db 'dobrodosli u PROTECTED MODE', 0x0D, 0x0A, 0

LOAD_KERNEL_MSG:
    db 'ucitavam kernel....', 0x0D, 0x0A, 0



BOOT_DRIVE: 
    db 0

KERNEL_OFFSET equ 0x1000 ; gde lodujemo kernel