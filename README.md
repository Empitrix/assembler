# Assembler
assembler for a non-standrad 8-bit CPU

## Build
for GNU make use:
```bash
make
```

If you are using [`mingw`](https://www.mingw-w64.org/) for windows you can use:
```bash
make windows
```

After that, there will be a `assembler` file, or `assembler.exe` in windows.


## Usage
After compiling run the `assembler` followed by given assembly file!
for example:
```bash
./assembler ./test/gpio.asm
```
after that if everything goes well you have a `out_bin` file which is contains binary data of given assembly file.

## Flags

| Flag            | Name    | Info                                                                         |
|-----------------|---------|------------------------------------------------------------------------------|
| `-v`            | Verbose | Show more output than usual, such as lines and their correspondent binaries  |
| `-o [filename]` | Output  | To specify the output file's path                                            |
| `--help`        | Help    | Show help menu                                                               |

