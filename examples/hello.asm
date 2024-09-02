; Print "Hello, World" in console

GPIO EQU 0x06   ; Define a ;Constant for the GPIO register address

start:
	MOVLW 'H'    ; Load the given letter to register W as it's value
	MOVWF GPIO   ; Move the value of register W to GPIO
	BSF GPIO, 7  ; Set bit 7 of GPIO to 1

	MOVLW 'e'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW 'l'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW 'l'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW 'o'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW ','
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW ' '
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW 'W'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW 'o'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW 'r'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW 'l'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW 'd'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW '!'
	MOVWF GPIO
	BSF GPIO, 7

	MOVLW '\n'
	MOVWF GPIO
	BSF GPIO, 7

	SLEEP

