start:
	MOVLW 80H  ; Write to register W
	MOVWF 06H  ; Set the value of register W to 0x06 register
	MOVLW C0H
	MOVWF 06H
	MOVLW E0H
	MOVWF 06H
	MOVLW F0H
	MOVWF 06H
	MOVLW F8H
	MOVWF 06H
	MOVLW FCH
	MOVWF 06H
	MOVLW FEH
	MOVWF 06H
	MOVLW FFH
	MOVWF 06H
	CLRF 06H   ; Clear register 0x06
	CLRW       ; Clear register W
  SLEEP      ; Sleep
