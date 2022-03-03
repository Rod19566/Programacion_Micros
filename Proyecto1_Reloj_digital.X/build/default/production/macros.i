# 1 "macros.s"
# 1 "<built-in>" 1
# 1 "macros.s" 2


  RESET_TMR0 MACRO
    BANKSEL TMR0
    MOVLW 255 ;2ms delay

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
    movlw 245
    movwf PR2
    CLRF TMR2
    BCF TMR2IF
    ENDM
