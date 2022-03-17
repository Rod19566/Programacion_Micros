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
 DIVISION MACRO VAR, VAR1, VAR2, TEM







    movf VAR, w
    movwf TEM


    clrf VAR1
    decf VAR1

    movlw 10
    incf VAR1
    subwf TEM, w
    movwf TEM
    btfsc STATUS, 0
    GOTO $-5

    clrf VAR2

    movlw 10
    addwf TEM, w
    movwf VAR2

    ENDM
