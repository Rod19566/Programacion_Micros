# 1 "macros.s"
# 1 "<built-in>" 1
# 1 "macros.s" 2



  restart_tmr0 macro
    BANKSEL TMR0
    MOVLW 236 ;10ms delay

    MOVWF TMR0
    BCF T0IF
    ENDM
