typedef struct OpCode {
	char *code;
	int value;
} OpCode;


OpCode opcodes[] = {
	{"NOP", 0},  {"BNK", 1},  {"OUT", 2},  {"CLC", 3},
	{"SEC", 4},  {"LSL", 5},  {"ROL", 6},  {"LSR", 7},
	{"ROR", 8},  {"ASR", 9},  {"INP", 10}, {"NEG", 11},
	{"INC", 12}, {"DEC", 13}, {"LDI", 14}, {"ADI", 15},
	{"SBI", 16}, {"CPI", 17}, {"ACI", 18}, {"SCI", 19},
	{"JPA", 20}, {"LDA", 21}, {"STA", 22}, {"ADA", 23},
	{"SBA", 24}, {"CPA", 25}, {"ACA", 26}, {"SCA", 27},
	{"JPR", 28}, {"LDR", 29}, {"STR", 30}, {"ADR", 31},
	{"SBR", 32}, {"CPR", 33}, {"ACR", 34}, {"SCR", 35},
	{"CLB", 36}, {"NEB", 37}, {"INB", 38}, {"DEB", 39},
	{"ADB", 40}, {"SBB", 41}, {"ACB", 42}, {"SCB", 43},
	{"CLW", 44}, {"NEW", 45}, {"INW", 46}, {"DEW", 47},
	{"ADW", 48}, {"SBW", 49}, {"ACW", 50}, {"SCW", 51},
	{"LDS", 52}, {"STS", 53}, {"PHS", 54}, {"PLS", 55},
	{"JPS", 56}, {"RTS", 57}, {"BNE", 58}, {"BEQ", 59},
	{"BCC", 60}, {"BCS", 61}, {"BPL", 62}, {"BMI", 63},
};

