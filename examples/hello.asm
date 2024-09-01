GPIO EQU 0x06     ; Define a constant for the GPIO register address
start:
    ; H
    BSF GPIO, 3
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H

    ; e
    BSF GPIO, 0
    BSF GPIO, 2
    BSF GPIO, 5
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H

    ; l
    BSF GPIO, 2
    BSF GPIO, 3
    BSF GPIO, 5
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H

    ; l
    BSF GPIO, 2
    BSF GPIO, 3
    BSF GPIO, 5
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H


    ; o
    BSF GPIO, 0
    BSF GPIO, 1
    BSF GPIO, 2
    BSF GPIO, 3
    BSF GPIO, 5
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H


    ; >,<
    BSF GPIO, 2
    BSF GPIO, 3
    BSF GPIO, 5

    BSF GPIO, 7
    CLRF 06H

    ; > <
    BSF GPIO, 5

    BSF GPIO, 7
    CLRF 06H



    ; W
    BSF GPIO, 0
    BSF GPIO, 1
    BSF GPIO, 2
    BSF GPIO, 4
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H


    ; o
    BSF GPIO, 0
    BSF GPIO, 1
    BSF GPIO, 2
    BSF GPIO, 3
    BSF GPIO, 5
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H

    ; r
    BSF GPIO, 1
    BSF GPIO, 4
    BSF GPIO, 5
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H


    ; l
    BSF GPIO, 2
    BSF GPIO, 3
    BSF GPIO, 5
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H


    ; l
    BSF GPIO, 2
    BSF GPIO, 5
    BSF GPIO, 6

    BSF GPIO, 7
    CLRF 06H

    ; >!<
    BSF GPIO, 0
    BSF GPIO, 5

    BSF GPIO, 7
    CLRF 06H

    ; >\n<
    BSF GPIO, 1
    BSF GPIO, 3

    BSF GPIO, 7
    CLRF 06H

    NOP           ; Do nothing (introduce a small delay)
    BCF GPIO, 0   ; Clear bit 0 of the GPIO register (turn off the LED)
    GOTO start    ; Jump back to the beginning (infinite loop)
