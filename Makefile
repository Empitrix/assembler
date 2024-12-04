# To compile for 'linux/windows' using "gcc" compiler
all:
	@ gcc ./main.c -o ./assembler

test: all
	@ ./assembler ./examples/test.asm -v

# To compile for 'windows' using "mingw" compiler
windows:
	@ x86_64-w64-mingw32-gcc ./main.c -lm -o ./assembler.exe

# To compile for 'linux' using "cc" compiler
linux:
	@ cc ./main.c -lm -o ./assembler

# Debug (show diagnostics)
debug:
	@ gcc -g -fanalyzer -Wall -Wextra -pedantic -fsanitize=undefined,address,leak ./main.c -lm -o ./assembler

# Run cppcheck
check: all
	@ cppcheck . --check-level=exhaustive

val: all
	@ valgrind ./assembler ./examples/test.asm

