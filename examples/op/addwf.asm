GPIO EQU 0x06
start:
  BSF GPIO, 0
  BSF GPIO, 1
  ADDWF GPIO, 0
  NOP
  ADDWF GPIO, 1
  GOTO start