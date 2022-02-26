/* Archivo:	    Lab6.s
    ; Dispositivo:  PIC16F887
    ; Autor:	    Dina Rodríguez
    ; Compilador:   pic-as (v2.30), MPLABX V6.00
    ;
    ;Programa:	    Laboratorio 6. Temporizadores
    ;Hardware:	    pic16f887, 7 segments displays, transistores
		    y resistencias	
    ;
    ;Creado:			25 de Febrero, 2022
    ;Ultima modificaci�n:	25 de Febrero, 2022
 */   
PROCESSOR 16F887
#include <xc.inc>
    
 // Td = Pre * TMR1*Ti
 // N = 65536-(Td/Pre*Ti)
 
 //Ttmr1if = Prescaler * PR2 * Postscaler * (1/(Fosc/4))
 //PR2 = Ttmr2if/Prescaler * Postscaler * (1/(Fosc/4))

; PIC16F887 Configuration Bit Settings

; Assembly source line config statements

; CONFIG1
  CONFIG  FOSC = INTRC_NOCLKOUT ; Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
  CONFIG  WDTE = OFF            ; Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
  CONFIG  PWRTE = OFF            ; Power-up Timer Enable bit (PWRT enabled)
  CONFIG  MCLRE = OFF           ; RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
  CONFIG  CP = OFF              ; Code Protection bit (Program memory code protection is disabled)
  CONFIG  CPD = OFF             ; Data Code Protection bit (Data memory code protection is disabled)
  CONFIG  BOREN = OFF           ; Brown Out Reset Selection bits (BOR disabled)
  CONFIG  IESO = OFF            ; Internal External Switchover bit (Internal/External Switchover mode is disabled)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
  CONFIG  LVP = OFF              ; Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

; CONFIG2
  CONFIG  BOR4V = BOR40V        ; Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
  CONFIG  WRT = OFF             ; Flash Program Memory Self Write Enable bits (Write protection off)

/////////////////////////MACROS///////////////////////  
  RESET_TMR1 MACRO
    movlw   0xB
    movwf   TMR1H
    movlw   0x47
    movwf   TMR1L
    bcf	    TMR1IF
    ENDM
////////////////////////////////////////////////////////
    ////////////////////VARIABLES//////////////////////////
    PSECT udata_bank0 ;common	    0000 
    cont4:
	DS 2
    valor:
	DS 2	; Contiene valor a mostrar en los displays de 7-seg
    ban0:
	DS 1	; Indica que display hay que encender
    ban1:
	DS 1	; Indica que display hay que encender
    nibbles:
	DS 3	; Contiene los nibbles alto y bajo de valor
    display:
	DS 3	; Representación de cada nibble en el display de 7-seg
    seg1:
	DS 1	; segundos para el display de 7-seg
    dec1:
	DS 1	; decimales para el display de 7-seg
    cen1:
	DS 1	; centenas para el display de 7-seg
	
PSECT udata_shr ;memoria compartida   
    W_TEMP:	
	DS 1
    STATUS_TEMP:
	DS 1
    seg7:	; variable 7 seg
	DS 1
	
PSECT resVect, class = code, abs, delta = 2
ORG 00h	    ;posición para reset
/////////////////////////////////////////////////////////
;/////////////////vector reset///////////////////////////
resVect:
    PAGESEL main	;cambio de banco
    GOTO main 
    PSECT inVect, class = code, abs, delta=2
 
ORG 04h	    ;posición para interrupciones
 /////////////////////////////////////////////////////////
 ;/////////////////vector interrupciones///////////////////////////
PUSH:
    MOVWF   W_TEMP	    ; w = W_TEMP
    SWAPF   STATUS, W
    MOVWF   STATUS_TEMP	    ; Guardamos STATUS
    
ISR:    
    btfsc   TMR1IF
    call    int_t1
    
    
POP:
    SWAPF   STATUS_TEMP, W  
    MOVWF   STATUS	    ; Recuperamos el valor de reg STATUS
    SWAPF   W_TEMP, F	    
    SWAPF   W_TEMP, W	    ; Recuperamos valor de W
    RETFIE		    ; Regresamos a ciclo principal
  
/////////////////////////////////////////////////////////
/////////////////////////configuracion/////////////////////////
//RESET_TMR1 

 int_t1:
    RESET_TMR1 
    incf    PORTA
    /*
    CLRF    PORTD
    btfsc   ban0, 0
    goto    display1   //01
    
display0:	//00
    movf    display, w	    //w = display
    movwf   PORTC	    //PORTC = w
    bsf	    PORTD, 0	    //se prende el display 1
    incf    ban0	    //se levanta la bandera 1
    return
    
display1:	//01
    movf    display+1, w    //w = display+1
    movwf   PORTC	    //PORTC = w
    bsf	    PORTD, 1 	    //se prende el display 2 
    btfsc   ban1, 0	    //se revisa la bandera 2
    goto    display2	
    incf    ban1	    //se levanta la bandera 2
    return
    
display2:	//11 
    CLRF    PORTD	    //le limpia el PORTD
    movf    display+2, w    //w = display+2
    movwf   PORTC	    //PORTC = w
    bsf	    PORTD, 2 	    //se prende el display 3  
    incf    ban1	    //se levanta la bandera 2
    incf    ban0	    //se levanta la bandera 1
   */ 
return_t1:
    return
    
    
PSECT code, delta=2, abs
ORG 100h
table:
    clrf PCLATH
    bsf PCLATH, 0
    andlw 0x0F
    addwf PCL	    ; suma (add) PCL = PCL + PCLATH + w
    retlw   00111111B	; 0
    retlw   00000110B	; 1
    retlw   01011011B	; 2
    retlw   01001111B	; 3
    retlw   01100110B	; 4
    retlw   01101101B	; 5
    retlw   01111101B	; 6
    retlw   00000111B	; 7
    retlw   01111111B	; 8
    retlw   01101111B	; 9
    retlw   01110111B	; A
    retlw   01111100B	; b
    retlw   00111001B	; C
    retlw   01011110B	; d
    retlw   01111001B	; E
    retlw   01110001B	; F
    
main:   
    call config_reloj	//oscilador
    call config_io	//I/O
    call config_tmr1
    call config_int	//interrupciones
    BANKSEL PORTA
        
    
loop:	    //el código cueanto no hay interrupciones
    clrf    valor
    MOVF    PORTA, w		; Valor del PORTA a W
    MOVWF   valor		; Movemos W a variable valor
   // CALL    SET_DISPLAYS
    GOTO    loop
    
//////////////////////////////////////////////////////////
  
SET_DISPLAYS:
    //call    compare		//se resta
  
    MOVF    cen1, W		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTC
    MOVWF   display+2		; Guardamos en display+1
    
    MOVF    dec1, W	    ; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTC
    MOVWF   display+1		; Guardamos en display+1
    
    MOVF    seg1, w		; Movemos nibble bajo a W
    CALL    table		; Buscamos valor a cargar en PORTC
    MOVWF   display		; Guardamos en display
    RETURN
   
    
///////////////////////configuraciones///////////////////////
config_reloj:
    BANKSEL OSCCON	;banco 1
    BSF OSCCON, 0	; SCS -> 1, se usa reloj interno
    BSF OSCCON, 6
    BSF OSCCON, 5
    BCF OSCCON, 4	;IRCF<2:0> -> 101 2MHz
    
    return
       
config_io:     
    BANKSEL ANSEL
    CLRF ANSEL	
    CLRF ANSELH	    ;I/O digitales 
    BANKSEL TRISC
    CLRF TRISC	    ;PORTC como salida display
    CLRF TRISA	    ;PORTA como salida contador A
    BCF	TRISD, 0		; Apagamos RD0
    BCF	TRISD, 1		; Apagamos RD1
    BANKSEL PORTC   ;se selecciona el banco 0 (00)
    CLRF PORTC
    CLRF PORTA
    CLRF    ban0		; Limpiamos GPR
    CLRF    ban1		; Limpiamos GPR
    CLRF    seg1		; Limpiamos seg1
    CLRF    cen1		; Limpiamos cen1
    CLRF    dec1		; Limpiamos dec1
        
    return 
    
 config_tmr1:		;PS<2:0> -> 111 prescaler 1 : 256
    BANKSEL PORTA
    bcf	    TMR1GE
    bsf	    T1CKPS1	//prescale 8:1
    bsf	    T1CKPS0
    bcf	    T1OSCEN	//internal clock
    bcf	    TMR1CS
    bsf	    TMR1ON	//TIMER1  ON
    RESET_TMR1 
    return 
      
config_int:
    BANKSEL TRISA
    bsf	    TMR1IE	    //tmr1 interrupt
    BANKSEL INTCON
    bcf	    TMR1IF
    bsf	    PEIE
    BSF	    GIE		    ; Habilitamos interrupciones
    RETURN
 /*
  division  
    */
compare:
   
    clrf    cen1       
    decf    cen1 	    //le resta 1 a la cuenta de las centenas
    
    movlw   100	    	     //se le asigna 100 a w
    incf    cen1	    //incrementa cen1
    subwf   valor, w	     //w = valor - 100
    movwf   valor	     //valor = w
    btfsc   STATUS, 0	    //if status,0 = 0, skip
    GOTO    $-5		    //regresa a  "movlw   0x64"
    
    movlw   100	    	    //se le asigna 100 a w
    addwf   valor, w	    //se le suma 100 a valor porque está negativo
    movwf   valor	    //valor = w
    clrf    dec1	    //limpia dec1
    
    movlw   10		     //se le asigna 10 a w
    incf    dec1	    //incrementa dec1
    subwf   valor, w	    //w = valor - 10
    movwf   valor	     //valor = w
    btfsc   STATUS, 0	    //if status,0 = 0, skip
    GOTO    $-5		    //regresa a  "movlw   0x0A"
    
    decf    dec1
    clrf    seg1	    //limpia seg1
    
    movlw   10		    //w = 10
    addwf   valor, w	    //se le suma 10 a valor porque está negativo (w = valor + 10)
    movwf   seg1	    //seg1 = w
	
    return
  
    