GPIO EQU 0x06

start:
  COMF GPIO, 1 ; 255 or 0 (in cycle)
  GOTO start
