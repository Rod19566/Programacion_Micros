/* Archivo:	    Lab5.s
    ; Dispositivo:  PIC16F887
    ; Autor:	    Dina Rodríguez
    ; Compilador:   pic-as (v2.30), MPLABX V6.00
    ;
    ;Programa:	    Multiple Display con multiplexado
    ;Hardware:	    pic16f887, 7 segments displays, push buttons, transistores
		    y resistencias	
    ;
    ;Creado:			20 de Febrero, 2022
    ;Ultima modificaci�n:	20 de Febrero, 2022
 */   
PROCESSOR 16F887
#include <xc.inc>
#include "macros.s"


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

  
UP	EQU 0
DOWN	EQU 1
/////////////////////////MACROS///////////////////////  
  RESET_TMR0 MACRO TMR_VAR
    BANKSEL TMR0	    ; Banco del TIMER0
    MOVLW   TMR_VAR	    ;literal TMR_VAR
    MOVWF   TMR0	    ; configuramos tiempo de retardo (w = Timer0)
    BCF	    T0IF	    ; limpiamos bandera de interrupción
    ENDM
    

////////////////////////////////////////////////////////
////////////////////VARIABLES//////////////////////////
    PSECT udata_bank0 ;common
    cont4:
	DS 2
    valor:
	DS 1	; Contiene valor a mostrar en los displays de 7-seg
    ban0:
	DS 1	; Indica que display hay que encender
    ban1:
	DS 1	; Indica que display hay que encender
    nibbles:
	DS 3	; Contiene los nibbles alto y bajo de valor
    display:
	DS 3	; Representación de cada nibble en el display de 7-seg
    seg1:
	DS 2	; segundos para el display de 7-seg
    dec1:
	DS 2	; decimales para el display de 7-seg
    cen1:
	DS 2	; centenas para el display de 7-seg
	
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
    btfsc   RBIF
    call    int_ioc
    
    btfsc   T0IF
    call    int_t0
    
    
POP:
    SWAPF   STATUS_TEMP, W  
    MOVWF   STATUS	    ; Recuperamos el valor de reg STATUS
    SWAPF   W_TEMP, F	    
    SWAPF   W_TEMP, W	    ; Recuperamos valor de W
    RETFIE		    ; Regresamos a ciclo principal
  
/////////////////////////////////////////////////////////
/////////////////////////configuracion/////////////////////////

int_t0:
    restart_tmr0		;15 ms
    CLRF    PORTD
    btfsc   ban0, 0
    goto    display1   //01
    
display0:	//00
    movf    display, w
    movwf   PORTC
    bsf	    PORTD, 0
    incf    ban0
    return
    
display1:	//01
    movf    display+1, w
    movwf   PORTC
    bsf	    PORTD, 1  
    btfsc   ban1, 0
    goto    display2
    incf    ban1
    return
    
display2:	//11 
    CLRF    PORTD
    movf    display+2, w
    movwf   PORTC
    bsf	    PORTD, 2  
    incf    ban1
    incf    ban0
    
return_t0:
    return
    
    
int_ioc:
    BANKSEL PORTB
    BTFSS   PORTB, UP
    INCF    PORTA
    BTFSS   PORTB, DOWN
    DECF    PORTA
    BCF	    RBIF
    
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
    call config_tmr0	//TMR0
    call config_io	//I/O
    call config_ioc
    call config_int	//interrupciones
    BANKSEL PORTA
        
    
loop:	    //el código cueanto no hay interrupciones
    
    MOVF    PORTA, W		; Valor del PORTA a W
    MOVWF   valor		; Movemos W a variable valor
    //call    OBTENER_NIBBLE
    //call    compare
    CALL    SET_DISPLAYS
    GOTO    loop
    
//////////////////////////////////////////////////////////
;    
;OBTENER_NIBBLE:			;    Ejemplo:				; Obtenemos nibble bajo
;    MOVF    valor, w		;    Valor = 1101 0101
;    ANDLW   0X0F		;	 AND 0000 1111
;    MOVWF   nibbles		;	     0000 0101	
;			; Obtenemos nibble alto
;    SWAPF   valor, w		;     Valor = 1101 0101
;    ANDLW   0X0F		;	  AND 1111 0000
;    MOVWF   nibbles+1		;	      1101 0000	; Obtenemos nibble alto
;    
;    movlw   0x03		;      PRUEBA para 3
;    MOVWF   nibbles+2		;	      1101 0000
;    RETURN
;    
SET_DISPLAYS:
    call    compare		//se resta
    MOVF    seg1, W		; Movemos nibble bajo a W
    CALL    table		; Buscamos valor a cargar en PORTC
    MOVWF   display		; Guardamos en display
    
    MOVF    dec1, W	    ; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTC
    MOVWF   display+1		; Guardamos en display+1
  
    MOVF    cen1, W	; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTC
    MOVWF   display+2		; Guardamos en display+1
    RETURN
   
    
///////////////////////configuraciones///////////////////////
config_reloj:
    BANKSEL OSCCON	;banco 1
    BSF OSCCON, 0	; SCS -> 1, se usa reloj interno
    BSF OSCCON, 6
    BCF OSCCON, 5
    BSF OSCCON, 4	;IRCF<2:0> -> 101 2MHz
    return
    
    
config_ioc:
    BANKSEL IOCB
    BSF IOCB, UP	    ;habilita interrupt
    BSF IOCB, DOWN
    BANKSEL PORTA
    movf PORTB, w
    BCF RBIF
    return
    
config_io:     
    BANKSEL ANSEL
    CLRF ANSEL	
    CLRF ANSELH	    ;I/O digitales 
    BCF STATUS, 6   ; BANCO 01 WPUB & TRIS
    BSF INTCON, 0   ;RBIF PORTB Change Interrupt Flag bit habilitado
    BSF TRISB, UP    ; RB0 como incremento
    BSF TRISB, DOWN    ; RB1 como decremento
 
    BCF OPTION_REG, 7	;pull-ups are enabled by individual PORT latch values
    BSF WPUB, UP	    ;habilita pull-up en RB0
    BSF WPUB, DOWN	    ;habilita pull-up en RB1
    
    CLRF TRISC	    ;PORTC como salida display
    CLRF TRISA	    ;PORTA como salida contador A
   
    BCF	TRISD, 0		; Apagamos RD0
    BCF	TRISD, 1		; Apagamos RD1
    BCF	TRISD, 2		; Apagamos RD2
    BANKSEL PORTC   ;se selecciona el banco 0 (00)
    CLRF PORTC
    CLRF PORTA
    CLRF    ban0		; Limpiamos GPR
    CLRF    ban1		; Limpiamos GPR
    CLRF    seg1		; Limpiamos GPR
    CLRF    cen1		; Limpiamos GPR
    CLRF    dec1		; Limpiamos GPR
    
    return 
    
 config_tmr0:
    BANKSEL OPTION_REG	;banco 1
    BCF T0CS		;TMR0 como temporizador
    BCF PSA		;se asigna prescaler to Timer0
    BSF PS2
    BSF PS1
    BSF PS0		;PS<2:0> -> 111 prescaler 1 : 256
    BANKSEL PORTA
    restart_tmr0
    return 
      
config_int:
    BANKSEL INTCON
    BSF	    GIE		    ; Habilitamos interrupciones
    BSF	    RBIE	    ; Habilitamos interrupcion TMR0
    BCF	    RBIF	    ; Limpiamos bandera de TMR0
    BSF	    T0IE	    ; Habilitamos interrupcion TMR0
    BCF	    T0IF	    ; Limpiamos bandera de TMR0
    RETURN
 /*
  division  
    */
compare:
    clrf    cen1

    movlw   0x64	     //se le asigna 100 a w
    incf    cen1	    //incrementa cen1
    subwf   valor, w	     //w = valor - 100
    movwf   valor	     //valor = w
    btfsc   STATUS, 0	    //if status,0 = 0, skip
    GOTO    $-5		    //regresa a  "movlw   0x64"
    decf    cen1	    //le resta 1 a las centenas
    
    movlw   0x64	    //se le asigna 100 a w
    addwf   valor, w	    //se le suma 100 a valor porque está negativo
    clrf    dec1	    //limpia dec1
    
    movlw   0x0A	    //se le asigna 10 a w
    incf    dec1	    //incrementa dec1
    subwf   valor, w	    //w = valor - 10
    movwf   valor	     //valor = w
    btfsc   STATUS, 0	    //if status,0 = 0, skip
    GOTO    $-5		    //regresa a  "movlw   0x0A"
    decf    dec1 	    //le resta 1 a las decenas
    
    clrf    seg1
    movlw   0x0A
    addwf   valor, w	    //w = valor + w
    movwf   seg1   
    
    incf    seg1 	    //le resta 1 a las decenas
    incf    seg1 	    //le resta 1 a las decenas
    incf    seg1 	    //le resta 1 a las decenas
    incf    seg1 	    //le resta 1 a las decenas
    
    return
  
