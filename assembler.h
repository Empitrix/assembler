#ifndef __ASM_ASSEMBLER_STD__
#define __ASM_ASSEMBLER_STD__
#include "src/types.h"
#include "src/asm.h"
#include "src/utils.h"

void assemble(ASMBL *, TBL *);
void show_err(ASM_ERR *, char*);

#endif
