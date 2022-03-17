
/////////////////////////MACROS///////////////////////  
  RESET_TMR0 MACRO 
    BANKSEL TMR0
    MOVLW   255		;2 ms delay
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
    
   RESET_TMR2 MACRO  //500 ms
    banksel TRISB
    movlw   244
    movwf   PR2
    CLRF    TMR2
    BCF	    TMR2IF
    ENDM  
    ////////
    
  /*  SUMA MACRO	VAR
    INCF    VAR
    ENDM 
    
   RESTA MACRO	VAR 
    DECF    VAR
    ENDM
   
*/
     
 DIVISION MACRO	 VAR, VAR1, VAR2, TEM	//LOS PARTE EN 2
 /*
     VAR1 = valor total
     VAR2 = valor de las decenas
     VAR3 = valor de unidades
     
     */
    /////////////HORA //////////////
    movf    VAR, w
    movwf   TEM		    //asigna un valor temporal para no modificar el
			    //valor original
    
    clrf    VAR1      
    decf    VAR1 	    //le resta 1 a la cuenta de las decenas
      //
    movlw   10		     //se le asigna 10 a w
    incf    VAR1	    //incrementa dec1
    subwf   TEM, w	    //w = valor - 10
    movwf   TEM		    //TEM = w
    btfsc   STATUS, 0	    //if status,0 = 0, skip
    GOTO    $-5		    //regresa a  "movlw   0x0A"
    
    clrf    VAR2	    //limpia VAR2
    
    movlw   10		    //w = 10
    addwf   TEM, w	    //se le suma 10 a valor porque está negativo (w = valor + 10)
    movwf   VAR2	    //VAR2 = w
    
    ENDM
    
    
   /* 
     
 SET_DISPLAYS MACRO	    VAR0, VAR1, VAR2, VAR3
    /////////////HORA //////////////
    MOVF    VAR0 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+3		; Guardamos en display+3
    
    MOVF    VAR1 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+2		; Guardamos en display+2
    
    MOVF    VAR2 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+1		; Guardamos en display+1
    
    MOVF    VAR3  , w		; Movemos nibble bajo a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display		; Guardamos en display
    ENDM
    
    SET_V MACRO	    V0, V1, V2, V3
    /////////////HORA //////////////
    MOVF    V0 , w		; Movemos nibble alto a W
    MOVWF   V		; Buscamos valor a cargar en PORTD
    
    MOVF    V1 , w		; Movemos nibble alto a W
    MOVWF   V+1		; Buscamos valor a cargar en PORTD
    
    MOVF    V2 , w		; Movemos nibble alto a W
    MOVWF   V+2		; Buscamos valor a cargar en PORTD
    
    MOVF    V3 , w		; Movemos nibble alto a W
    MOVWF   V+3		; Buscamos valor a cargar en PORTD
    ENDM
    
    
*/
