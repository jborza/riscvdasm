## RISC-V disassembler

### Build instructions:

```sh
make
```

### Usage:
```
./riscvdasm < test/simple.bin
```

### Sample output:

```
       0:       02A00513        addi x10,x0,0x0000002a
       4:       05D00893        addi x17,x0,0x0000005d
       8:       00000073        ecall
```

### TODO
- rewrite registers to conventional names
- decimal vs hexadecimal arguments
- parse CSR registers according to csr.h
- floating point extension

### Note:
To compare results with canonical `objdump`, use

```sh
riscv32-unknown-elf-objdump -D -b binary -m riscv FILE
```