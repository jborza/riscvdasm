## RISC-V disassembler

### Features

- disassembling flat binaries
- register ABI names (`ra`, `sp`, `a0`, ...)
- CSR register names
- (some) pseudoinstruction decoding (`mv`, `j`, `li`, `...`) 

### Build instructions:

```sh
make
```

### Building the web version:

Requirements: 
- Install `emscripten`: https://emscripten.org/docs/getting_started/index.html

```sh
make web
```

### Usage:
```
./riscvdasm < test/simple.bin
```

### Sample output:

```
       0:       2000006f                j 0x200
       4:       34011173                csrrw sp,mscratch,sp
       8:       1a010c63                beqz sp,0x1b8
       c:       02a12423                sw a0,40(sp)
      10:       02b12623                sw a1,44(sp)
      14:       342025f3                csrrs a1,mcause,zero
      18:       0805d263                bgez a1,0x84
      1c:       00159593                slli a1,0x407765
      20:       00e00513                li a0,14
      24:       02b51263                bne a0,a1,0x24
```

### TODO
- more pseudoinstructions according to https://riscv.org/wp-content/uploads/2017/05/riscv-spec-v2.2.pdf (chapter 20)
- floating point extension
- lui -> addi combo (track register value if these two are chained?)
- elf binaries

### Note:
To compare results with the canonical riscv32 `objdump`, use

```sh
riscv32-unknown-elf-objdump -D -b binary -m riscv FILE
```