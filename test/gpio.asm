GPIO EQU 0x06   ; Define a constant for the GPIO register address
start:
    BSF GPIO, 0   ; Set bit 0 of the GPIO register (turn on the LED)
    NOP           ; Do nothing (introduce a small delay)
    BCF GPIO, 0   ; Clear bit 0 of the GPIO register (turn off the LED)
    GOTO start    ; Jump back to the beginning (infinite loop)
