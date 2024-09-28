GPIO EQU 0x06   ; Register 0x06 is GPIO

LOOP:
  CALL on       ; Turn ON the LED
  NOP
  CALL off      ; Turn OFF the LED
  GOTO LOOP     ; Do this again (LOOP)

on:
  BSF GPIO, 0   ; Turn on the LED
  RETLW 0       ; Back to the next instruction and load 'W' with 0

off:
  BCF GPIO, 0   ; Turn off the LED
  RETLW 0       ; Back to the next instruction and load 'W' with 0
