GPIO EQU 0x06   ; Define a constant for the GPIO register address
start:
	DECF GPIO, 1
	DECF GPIO, 0
	DECF 06H, 1
	DECFSZ 06H, 0

