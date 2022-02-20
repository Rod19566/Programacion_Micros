


  restart_tmr0 macro 
    BANKSEL TMR0
    MOVLW   227		;15ms delay
    // N = 256 - [(T * Fosc) / (4 * PS)]
    MOVWF   TMR0
    BCF	    T0IF
    ENDM