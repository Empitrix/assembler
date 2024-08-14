
; Set the start address
#ORG     0x8000

; Initialize the stack
LDI     0xFE
STA     0xFFFF

start:
        LDI     <string
        PHS
        LDI     >string
        PHS
        JPS     PrintText
        PLS
        PLS
        JPA     start

; String to be printed
string:  'Hello, World!', 10, 0

; 16-bit pointer
ptr:     0x0000

PrintText:
        LDS     3
        STA     ptr + 1
        LDS     4
        STA     ptr + 0
loop:
        LDR     ptr
        CPI     0
        BEQ     exit
        OUT
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        INW     ptr
        JPA     loop
exit:
        RTS


#begin
	I: 'A'
#end
