    ; Archivo:	    Lab4.s
    ; Dispositivo:  PIC16F887
    ; Autor:	    Dina Rodríguez
    ; Compilador:   pic-as (v2.30), MPLABX V6.00
    ;
    ;Programa:	    Interrupciones
    ;Hardware:		
    ;
    ;Creado:		12 de Febrero, 2022
    ;Ultima modificaci�n:	12 de Febrero, 2022
    
PROCESSOR 16F887


; PIC16F887 Configuration Bit Settings

; Assembly source line config statements

; CONFIG1
  CONFIG  FOSC = INTRC_NOCLKOUT ; Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
  CONFIG  WDTE = OFF            ; Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
  CONFIG  PWRTE = ON            ; Power-up Timer Enable bit (PWRT enabled)
  CONFIG  MCLRE = OFF           ; RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
  CONFIG  CP = OFF              ; Code Protection bit (Program memory code protection is disabled)
  CONFIG  CPD = OFF             ; Data Code Protection bit (Data memory code protection is disabled)
  CONFIG  BOREN = OFF           ; Brown Out Reset Selection bits (BOR disabled)
  CONFIG  IESO = OFF            ; Internal External Switchover bit (Internal/External Switchover mode is disabled)
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
  CONFIG  LVP = ON              ; Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

; CONFIG2
  CONFIG  BOR4V = BOR40V        ; Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
  CONFIG  WRT = OFF             ; Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.inc>
  
UP	EQU 0
DOWN	EQU 1
/////////////////////////MACROS///////////////////////  
  RESET_TMR0 MACRO TMR_VAR
    BANKSEL TMR0	    ; Banco del TIMER0
    MOVLW   TMR_VAR	    ;literal TMR_VAR
    MOVWF   TMR0	    ; configuramos tiempo de retardo (w = Timer0)
    BCF	    T0IF	    ; limpiamos bandera de interrupción
    ENDM
    
  restart_tmr0 macro 
    BANKSEL TMR0
    MOVLW   227		;15ms delay
    // N = 256 - [(T * Fosc) / (4 * PS)]
    MOVWF   TMR0
    BCF	    T0IF
    ENDM
////////////////////////////////////////////////////////
////////////////////VARIABLES//////////////////////////
    PSECT udata_bank0 ;common
    cont4:
	DS 2
PSECT udata_shr ;memoria compartida   
    W_TEMP:	
	DS 1
    STATUS_TEMP:
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
    INCF    cont4
    movf    cont4, w
    sublw   67		    ;1000ms
    BTFSS   ZERO	   ;status, 2
    GOTO    return_t0	    ;1000ms
    CLRF    cont4
    incf    PORTD
    
return_t0:
    return
    
    
int_ioc:
    BANKSEL PORTA
    BTFSS   PORTB, UP
    INCF    PORTC
    BTFSS   PORTB, DOWN
    DECF    PORTC
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
    
    GOTO loop
    
//////////////////////////////////////////////////////////
///////////////////////subrutina///////////////////////
config_reloj:
    BANKSEL OSCCON	;banco 1
    BSF OSCCON, 0	; SCS -> 1, se usa reloj interno
    BSF OSCCON, 6
    BCF OSCCON, 5
    BSF OSCCON, 4	;IRCF<2:0> -> 101 2MHz
    return
    
 
 
 
 
  reset_tmr0:
    INCF PORTB
    call compare
    BANKSEL TMR0
    MOVLW   227		;15ms delay
    // N = 256 - [(T * Fosc) / (4 * PS)]
    MOVWF   TMR0
    BCF T0IF
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
    BSF TRISB, UP    ; RA0 como entrada
    BSF TRISB, DOWN    ; RA1 como entrada
    
    BCF OPTION_REG, 7	;pull-ups are enabled by individual PORT latch values
    BSF WPUB, UP	    ;habilita pull-up en RB0
    BSF WPUB, DOWN	    ;habilita pull-up en RB1
    
    CLRF TRISC	    ;PORTC como salida contador
    CLRF TRISD	    ;PORTD como salida 
    CLRF TRISE	    ;PORTE como salida
    BANKSEL PORTC   ;se selecciona el banco 0 (00)
    CLRF PORTC
    CLRF PORTD
    CLRF PORTE
    return 
    
   config_tmr0:
    BANKSEL OPTION_REG	;banco 1
    BCF T0CS		;TMR0 como temporizador
    BCF PSA		;se asigna prescaler to Timer0
    BSF PS2
    BSF PS1
    BSF PS0		;PS<2:0> -> 111 prescaler 1 : 256
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
    
antirrebotes1:
    call checkbutton1
    INCF PORTC   ; incremento de contador
    return 
    
checkbutton1:
    BTFSS PORTB, 0
    GOTO $-1
    return

antirrebotes2:
    call checkbutton2
    DECF PORTC   ; incremento de contador
    return 
    
checkbutton2:
    BTFSS PORTB, 1
    GOTO $-1
    return

    buttons:
    btfss PORTB, 0
    call antirrebotes1
    btfss PORTB, 1 
    call antirrebotes2
    return

        
compare:
    call table
    subwf   W_TEMP, w
    btfss   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    alarma	; cuando la bandera de cero se activa se llama a alarma
    return
    
alarma:
    INCF PORTD
    return