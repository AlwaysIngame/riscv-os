# Hobby RISC-V OS

This is a small hobby operating system written for **riscv64**.  
It is designed to be booted using the [Limine bootloader](https://limine-bootloader.org/) under UEFI.

## Building

You will need:
- LLVM and `clang` with RISC-V cross-compilation support (`-target riscv64-unknown-elf`)
- `make`
- `wget`
- `qemu-system-riscv64`

To build the kernel:

```sh
make kernel
````
The file will be `build/kernel.elf`.

## Running

The Makefile provides a `run` target that:

* Downloads the latest RISC-V UEFI firmware (`OVMF`) and Limine bootloader.
* Prepares a FAT disk containing the kernel and `limine.conf` from `misc/limine.conf`.
* Boots the OS in qemu-system-riscv64.

Run with:

```sh
make run
```

This starts QEMU in **headless (`-nographic`) mode** with 4 cores.
The OS is loaded by **Limine** and executed under UEFI.

## Cleaning

To remove build artifacts:

```sh
make clean
```

## License

Certain header files contain their licensing details within them. The rest of the software is distributed under GPL v3, as outlined in the LICENSE file.