# SelenaOS

A small educational operating system for 32-bit x86, written in NASM assembly and freestanding C. It boots through the BIOS, enters protected mode, and runs in QEMU.

## Included

- BIOS boot sector and 16-bit to 32-bit protected-mode transition
- IDT, CPU exception handlers, PIC remapping, and timer/keyboard IRQ handling
- VGA text-mode output and a small interactive shell
- PCI scanning and a legacy VirtIO block driver
- Sector-based persistent storage on a separate VirtIO disk image

## Build and run

Requirements: `make`, `nasm`, a 32-bit capable GCC toolchain, GNU `ld`, and QEMU (`qemu-system-i386`).

```sh
make
make run
```

`make` creates `build/os-image.bin`, a 1.44 MB boot image. `make run` also creates `virtio-test.img` (16 MB) when needed and attaches it as the VirtIO block device.

## Shell commands

`help`, `clear`, `ticks`, `about`, `save`, and `load`.

`save` writes the shell's demonstration text to sector 2 of the VirtIO image; `load` reads and displays that sector. The current shell is intentionally minimal and does not accept an argument for `save`.

## Layout

- `boot/` — boot sector, BIOS disk loading, protected-mode setup, and kernel sources
- `boot/kernel/cpu/` — IDT, ISR, IRQ, and PIC code
- `boot/kernel/driver/` — VGA, keyboard, timer, PCI, disk, and VirtIO drivers
- `linker.ld` and `Makefile` — kernel layout and build targets

The longer Serbian-language project report is available as `dokumentacijaosfinalno.pdf` and `dokumentacija.odt`.
