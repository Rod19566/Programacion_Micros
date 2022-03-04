# 1 "macros.s"
# 1 "<built-in>" 1
# 1 "macros.s" 2


  RESET_TMR0 MACRO
    BANKSEL TMR0
    MOVLW 255 ;2 ms delay

    MOVWF TMR0
    BCF T0IF
    ENDM

  RESET_TMR1 MACRO
    movlw 194
    movwf TMR1H
    movlw 98
    movwf TMR1L
    bcf TMR1IF
    ENDM

   RESET_TMR2 MACRO
    banksel TRISB
    movlw 244
    movwf PR2
    CLRF TMR2
    BCF TMR2IF
    ENDM
# 38 "macros.s"
 DIVISION MACRO VAR1, VAR2, VAR3

    movf VAR1+0, w
    movwf VAR1+1
    CLRF VAR2

    movlw 10
    incf VAR2
    subwf VAR1+1, w
    movwf VAR1+1
    btfsc STATUS, 0
    GOTO $-5

    decf VAR2
    CLRF VAR3

    movlw 10
    addwf VAR1+1, w
    movwf VAR3
    ENDM
