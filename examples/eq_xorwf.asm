STATUS EQU 0x03
GPIO EQU 0x06
Z EQU 0x02

R1 EQU 0x0A  ; RAM [pos: 0]
R2 EQU 0x0B  ; RAM [pos: 1]

MOVLW 25
MOVWF R1

MOVLW 24
MOVWF R2

MOVF    R1, 0           ; Load R1 into W register
XORWF   R2, 0           ; XOR W with VALUE2 (W = R1 ^ R2)
BTFSC   STATUS, Z       ; Check Zero flag in STATUS register
GOTO    EQUAL           ; If Z is set, R1 == R2, so go to EQUAL

NOT_EQUAL:
  MOVLW 'N'
  MOVWF GPIO
  BSF GPIO, 7
  GOTO    END

EQUAL:
  MOVLW 'E'
  MOVWF GPIO
  BSF GPIO, 7

END:
  SLEEP
