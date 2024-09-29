GPIO EQU 0x06      ; Define GPIO as 0x06

MOVLW 0b11111111   ; Move 0xFF to W register
MOVWF GPIO         ; Move content of W register to GPIO

start:             ; 'start' lable
  DECF GPIO, 1     ; Decrement from GPIO and place the result back into GPIO
  BTFSS GPIO, 7    ; Test bit 7 of GPIO, If it's 1 skip the next instruction
  SLEEP            ; Sleep
  GOTO start       ; Go back to start

