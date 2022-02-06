    ; Archivo:	    Lab3.s
    ; Dispositivo:  PIC16F887
    ; Autor:	    Dina Rodríguez
    ; Compilador:   pic-as (v2.30), MPLABX V6.00
    ;
    ;Programa:	    Botones y Timer0
    ;Hardware:		
    ;
    ;Creado:		6 de Febrero, 2022
    ;Ultima modificaci�n:	6 de Febrero, 2022
    
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

// config statements should precede project file includes.
#include <xc.inc>
PSECT resVect, class = code, abs, delta = 2
;---------------------------------vector reset----------------------------------
ORG 00h
resVect:
    PAGESEL main	;cambio de banco
    GOTO main 
    
PSECT code, delta = 2, abs
;-----------------------------------------------------------
;-----------------------------configuracion------------------
ORG 100
main:   
    call config_reloj
    call config_tmr0
    call config_io
    
    
    
loop:
    BTFSS T0IF	    ;bandera de interrupcion, verifica TMR0
    GOTO loop
    
    call reset_tmr0
    INCF PORTB
    GOTO loop
    
;-----------subrutina-----------
config_reloj:
    BANKSEL OSCCON	;banco 1
    BSF OSCCON, 0	; SCS -> 1, se usa reloj interno
    BSF OSCCON, 6
    BCF OSCCON, 5
    BSF OSCCON, 4	;IRCF<2:0> -> 101 2MHz
    return
    
  config_tmr0:
    BANKSEL OPTION_REG	;banco 1
    BCF T0CS		;TMR0 como temporizador
    BCF PSA		;se asigna prescaler to Timer0
    BSF PS2
    BSF PS1
    BSF PS0		;PS<2:0> -> 111 prescaler 1 : 256
    call reset_tmr0  
 
 reset_tmr0:   
    BANKSEL TMR0
    MOVLW   61		;100ms delay
    // N = 256 - [(T * Fosc) / (4 * PS)]
    MOVWF   TMR0
    BCF T0IF
    return  
    
config_io:     
    BANKSEL ANSEL
    CLRF ANSELH	    ;I/O digitales 
    BCF STATUS, 6   ; BANCO 01
    CLRF TRISB	    ;PORTB como salida
    BANKSEL PORTB   ;se selecciona el banco 0 (00)
    CLRF PORTB
    return 
    
    


