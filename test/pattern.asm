GPIO EQU 0x06    ; GPIO in 0x06 (Register)
start:
    BSF GPIO, 7  ; Set bit 7 to 1
    BSF GPIO, 6  ; Set bit 6 to 1 
    BSF GPIO, 5  ; Set bit 5 to 1
    BSF GPIO, 4  ; Set bit 4 to 1
    BSF GPIO, 3  ; Set bit 3 to 1
    BSF GPIO, 2  ; Set bit 2 to 1
    BSF GPIO, 1  ; Set bit 1 to 1
    BSF GPIO, 0  ; Set bit 0 to 1
    NOP          ; Do Nothing (Delay)
    BCF GPIO, 0  ; Set bit 0 to 0 
    BCF GPIO, 1  ; Set bit 1 to 0 
    BCF GPIO, 2  ; Set bit 2 to 0 
    BCF GPIO, 3  ; Set bit 3 to 0
    BCF GPIO, 4  ; Set bit 4 to 0
    BCF GPIO, 5  ; Set bit 5 to 0
    BCF GPIO, 6  ; Set bit 6 to 0
    BCF GPIO, 7  ; Set bit 7 to 0
    GOTO start   ; Back to stasrt (jump)
