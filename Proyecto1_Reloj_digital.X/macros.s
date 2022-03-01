
/////////////////////////MACROS///////////////////////  
  RESET_TMR0 macro 
    BANKSEL TMR0
    MOVLW   255		;2ms delay
    // N = 256 - [(T * Fosc) / (4 * PS)]
    MOVWF   TMR0
    BCF	    T0IF
    ENDM
    
  RESET_TMR1 MACRO  //1 segundo
    movlw   194		//64911	// N = 65536-(Td/Pre*Ti)
    movwf   TMR1H
    movlw   98	 //PR2 = Ttmr2if/Prescaler * Postscaler * (1/(Fosc/4))
    movwf   TMR1L
    bcf	    TMR1IF
    ENDM  
    
   RESET_TMR2 MACRO  //1 segundo
    banksel TRISB
    movlw   245
    movwf   PR2
    CLRF    TMR2
    BCF	    TMR2IF
    ENDM
////////////////////////////////////////////////////////


