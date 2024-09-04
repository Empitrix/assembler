GPIO EQU 0x06     ; Define a constant for the GPIO register address
start:
  BSF GPIO, 7     ; Set GPIO to 10000000 (x80)
  DECF GPIO, 1    ; Decrement GPIO by 1 (store to the address)
  DECF GPIO, 0    ; Decrement GPIO by 1 (store to working register (W))
  DECFSZ GPIO, 0  ; Decrement GPIO by 1 (store to working register (W))
  CLRF GPIO       ; set GPIO to 00000000
  BSF GPIO, 0     ; set GPIO to 00000001
  DECFSZ GPIO, 1  ; decrement and store to GPIO
  SLEEP           ; bypass
  SLEEP           ; SLEEP
