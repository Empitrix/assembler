; PRESS 1 to 8 in your keyboard
; Only the last 4 shuld work (with red color)

start:
  MOVLW 0b11110000
  TRIS 6
  GOTO start
