GPIO EQU 0x06  ; Define a constant for the GPIO register address
start:
	MOVLW 03H    ; Loads a literal (immediate) value into the W register.
	MOVWF 06H    ; Moves the contents of the W register to a specified register or memory location.
	CLRF 06H     ; Clears (sets to 0) a specified register or memory location.
	CLRW         ; Clears the W register.
	SLEEP        ; Puts the CPU into a standby mode.
