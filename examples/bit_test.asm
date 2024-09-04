GPIO EQU 0x06     ; Define a constant for the GPIO register address
start:
	BTFSS GPIO, 3   ; Test bit 3 of GPIO, skip next instruction if it is 1
	BSF GPIO, 3     ; Set bit 3 of GPIO to 1
	BTFSS GPIO, 3   ; Test bit 3 of GPIO, skip next instruction if it is 1
	SLEEP           ; Sleep (as NOP)

	CLRF GPIO       ; Clear the GPIO

	BSF GPIO, 3     ; Set bit 3 of GPIO to 1
	BTFSC GPIO, 3   ; Test bit 3 of GPIO, skip next instruction if it is 0
	BTFSC GPIO, 2   ; Test bit 3 of GPIO, skip next instruction if it is 0
	SLEEP           ; Sleep (as NOP)
	SLEEP           ; Sleep

