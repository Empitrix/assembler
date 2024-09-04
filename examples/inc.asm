GPIO EQU 0x06     ; Define a constant for the GPIO register address
start:
	BSF GPIO, 6     ; Set GPIO to 10000000 (x80)
	INCF GPIO, 1    ; Increment GPIO by 1 (store to the address)
	INCF GPIO, 0    ; Increment GPIO by 1 (store to working register (W))
	INCFSZ GPIO, 0  ; Increment GPIO by 1 (store to working register (W))
	CLRF GPIO       ; Set GPIO to 00000000

	CLRW
	MOVLW 255       ; load 255 to register W
	MOVWF GPIO      ; Move the value of register W to GPIO

	INCFSZ GPIO, 1  ; Increment and store to GPIO
	SLEEP           ; Bypass
	SLEEP           ; SLEEP
