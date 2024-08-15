# Assembeler
Assembeler for a non-standrad 8-bit CPU

## Build
for GNU make use:
```bash
make
```

If you are using [`mingw`](https://www.mingw-w64.org/) for windows you can use:
```bash
make windows
```

After that, there will be a `assembeler` file, or `assembeler.exe` in windows.


## Usage
After compiling run the `assembeler` followed by given assembly file!
for example:
```bash
./assembeler ./test/gpio.asm
```
after that if everything goes well you have a `out_bin` file which is contains binary data of given assembly file.

## Flags

| Flag            | Name    | Info                                                                         |
|-----------------|---------|------------------------------------------------------------------------------|
| `-v`            | Verbose |  Show more output than usual, such as lines and their correspondent binaries |
| `-o [filename]` | Output  | To specify the output file's path                                            |
| `--help`        | Help    | show Help                                                                    |

