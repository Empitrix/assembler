STATUS EQU 0x03
GPIO EQU 0x06
Z EQU 0x02

R0 EQU 0x0A
R1 EQU 0x0B

init:
	; Load '100' to R0
	MOVLW 9
	MOVWF R0

	CLRW

	; Load '50' to R1
	MOVLW 50
	MOVWF R1


; Check if R0 is eual to R1 (R0 == R1)
main:
	MOVF R0, F
	SUBWF R1, F
	BTFSS STATUS, Z  ; BIT TEST F SKIP if SET
	CALL notequal
	CALL equal
	
; Do the following things if it's equal (==), print E
equal:
	MOVLW '-'
	CALL PCWS

; Do the following things if it's not equal (!=), print N
notequal:
	MOVLW '.'
	CALL PCWS

PCWS:             ; Put Char W and SLEEP
	MOVWF GPIO
	BSF GPIO, 7
	RETLW 0
