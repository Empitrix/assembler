GPIO EQU 0x06

start:
  MOVLW 0b00000111
  MOVWF GPIO
  CLRW
  MOVF GPIO, 0 ; W=0b00000111
  GOTO start
