R1 EQU 0x06    ; unique
R2 EQU 0x02    ; unique
R3 EQU 0x02    ; same
R3 EQU 0x01    ; never used

start:
	BSF R1, 0
	BSF R2, 0
	BSF R3, 0
	BSF 0x03, 0  ; unique
	SLEEP
