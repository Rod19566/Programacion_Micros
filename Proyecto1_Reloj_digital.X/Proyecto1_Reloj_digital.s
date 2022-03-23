/* Archivo:	    Proyecto1_Reloj_digital
    ; Dispositivo:  PIC16F887
    ; Autor:	    Dina Rodríguez
    ; Compilador:   pic-as (v2.30), MPLABX V6.00
    ;
    ;Programa:	    Proyecto 1 Reloj Digital
    ;Hardware:	    pic16f887, 7 segments displays, transistores
		    y resistencias	
    ;
    ;Creado:			1 de Marzo, 2022
    ;Ultima modificaci�n:	1 de Marzo. 2022
 */   
PROCESSOR 16F887
#include <xc.inc>
#include "macros.s"

    //NOMBRE DE LOS BOTONES
UP	EQU 0	    //INCREMENTAR
DOWN	EQU 1	    //DECREMENTAR
MODE	EQU 2	    //SELECCION DE MODO
ENTER	EQU 3	    //Activar modo
PAUSE	EQU 4	    //pausa
	
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
  

  ////////////////////VARIABLES//////////////////////////
    PSECT udata_bank0 ;common	    0000 
    cont4:
	DS 2
    valor:
	DS 2	; Contiene valor a mostrar en los displays de 7-seg
    halfie:
	DS 1	; control de medio segundo
    ban0:
	DS 1	; Indica que display hay que encender
    ban1:
	DS 1	; Indica que display hay que encender
    display:
	DS 4	; Representación de cada nibble en el display de 7-seg
    HOUR:
	DS 2	;VARIABLE QUE TENDRA VALOR DE LA HORA actual
    HOURS1:
	DS 1	;VARIABLE de la hora centenas y unidades
    HOURS2:
	DS 1	;VARIABLE de la hora centenas y unidades
    MINUTE:
	DS 2	;VARIABLE QUE TENDRA VALOR DE LOS MINUTOS actual
    MINUTES1:
	DS 1	;VARIABLE  MINUTOS centenas y unidades
    MINUTES2:
	DS 1	;VARIABLE  MINUTOS centenas y unidades
    SECOND:
	DS 2	;VARIABLE QUE TENDRA VALOR DE LOS SEGUNDOS
    MODO:
	DS 2	;selección LOS ESTADOS
    MONTH:
	DS 1	;VARIABLE QUE TENDRA EL MES
    MONTH1:
	DS 1	;VARIABLE DIAS centenas y unidades
    MONTH2:
	DS 1	;VARIABLE DIAS centenas y unidades
    DAY:
	DS 1	;VARIABLE DIAS actual
    DAY1:
	DS 2	;VARIABLE DIAS centenas y unidades
    DAY2:
	DS 2	;VARIABLE DIAS centenas y unidades
    DIAMAX:
	DS 1	;VARIABLE MAXIMO DE DIAS POR MES
    temp:
	DS 2	;VARIABLE TEMPORAL
    tEDIT:
	DS 1	;VARIABLE 
    EDIT:
	DS 1	;VARIABLE 
    EDIT1:
	DS 1	;VARIABLE 
    PAUSA:
	DS 1	;VARIABLE 
    CONT:
	DS 1	;VARIABLE 
    tSECOND:
	DS 2	;VARIABLE QUE TENDRA VALOR DE LA HORA actual
    tSECOND1:
	DS 1	;VARIABLE de la hora centenas y unidades
    tSECOND2:
	DS 1	;VARIABLE de la hora centenas y unidades
    tMINUTE:
	DS 2	;VARIABLE QUE TENDRA VALOR DE LOS MINUTOS actual
    tMINUTES1:
	DS 1	;VARIABLE  MINUTOS centenas y unidades
    tMINUTES2:
	DS 1	;VARIABLE  MINUTOS centenas y unidades
	
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
    btfsc   T0IF
    call    int_t0
    btfsc   TMR1IF
    call    int_t1
    btfsc   TMR2IF
    call    int_t2 
    btfsc   RBIF
    call    int_ioc
        
    
POP:
    SWAPF   STATUS_TEMP, W  
    MOVWF   STATUS	    ; Recuperamos el valor de reg STATUS
    SWAPF   W_TEMP, F	    
    SWAPF   W_TEMP, W	    ; Recuperamos valor de W
    RETFIE		    ; Regresamos a ciclo principal

      
/////////////////////////////////////////////////////////
/////////////////////////configuracion/////////////////////////
    
int_ioc:    //INTERRUPCIONES PORTB
    
    BANKSEL PORTB
    BTFSS   PORTB, MODE
    INCF    MODO
    BTFSS   PORTB, ENTER
    INCF    EDIT
    BTFSS   PORTB, PAUSE
    INCF    PAUSA
    
    btfsc   EDIT, 0	//
    call    mode
   
    BCF	    RBIF    //bandera del boton
    return
    
    mode:    //eleccion de modo
    movf    MODO, w
    movwf   temp
    movlw   0
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    goto    modeup_0   //10
    
    movf    MODO, w
    movwf   temp
    movlw   1
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    goto    modeup_1   //10
    
    movf    MODO, w
    movwf   temp
    movlw   2
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    goto    modeup_2   //10
    
    movf    MODO, w
    movwf   temp
    movlw   3
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    goto    modeup_3   //11
        
    return
    
    modeup_0:	//minuto para hora
    btfsc   PAUSA, 0
    call    incminute
    btfss   PAUSA, 0
    call    inchour
    
    return
    
    incminute: ///incrementa o baja minuto
    BTFSS   PORTB, UP
    incf    MINUTE
    BTFSS   PORTB, DOWN
    DECF    MINUTE
    return
 
    inchour: ///incrementa o baja hora
    BTFSS   PORTB, UP
    incf    HOUR
    BTFSS   PORTB, DOWN
    DECF    HOUR
    return
    
    modeup_1:	///fecha dia/mes
    btfsc   PAUSA, 0
    call    incday
    btfss   PAUSA, 0
    call    incmonth
    return
    
    incday: ///incrementa o baja minuto
    BTFSS   PORTB, UP
    incf    DAY
    BTFSS   PORTB, DOWN
    DECF    DAY
    return
    
    incmonth: ///incrementa o baja minuto
    BTFSS   PORTB, UP
    incf    MONTH
    BTFSS   PORTB, DOWN
    DECF    MONTH
    return
    
    modeup_2:	//contador
    
    movf    PAUSA, w
    movwf   temp
    movlw   0
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    goto    inctminute  //10
    
    movf    PAUSA, w
    movwf   temp
    movlw   1
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    goto    inctsecond   //10
    
    inctminute:
    clrf    tEDIT
    BTFSS   PORTB, UP
    incf    tMINUTE
    BTFSS   PORTB, DOWN
    DECF    tMINUTE
    return
  
    inctsecond:
    clrf    tEDIT
    BTFSS   PORTB, UP
    incf    tSECOND
    BTFSS   PORTB, DOWN
    DECF    tSECOND
    return 
    
    modeup_3:
    //CLRF PORTD
    
    return
    
    
    
int_t1:	    //SUBRUTINA DEL TMR1    (CUENTA DEL TIEMPO) 1 segundo
    RESET_TMR1
    incf    SECOND
    
    movf    PAUSA, w
    movwf   temp
    movlw   2
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    decf    tSECOND
    
 
return_t1:
    return

   
int_t2:	    //SUBRUTINA DEL TMR2    (MEDIO SEGUNDO)
    incf    halfie
    bcf	    TMR2IF
     
return_t2:
    return
    
    
int_t0:
    RESET_TMR0	    	;15 ms
     
    btfsc   EDIT, 0	    //si modo edición está activado, parpadea RD4 (EDIT)
    bsf	    RA4
    
    btfss   EDIT, 0	    //si modo edición está activado, parpadea RD4 (EDIT)
    bcf	    RA4
    
    movf    MODO, w	    //overflow de MODO
    movwf   temp
    movlw   3
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    CLRF    MODO	//MODO = 0
    
    CLRF    PORTE
    BCF	    RC7
    CLRF    PORTD
    
    btfsc   ban0, 0
    goto    display1   //01
    
    btfsc   ban0, 1
    goto    display2   //10
    
    btfsc   ban0, 2
    goto    display3   //11
    goto    display0
    return
    //then goes to display0
    
display0:	//00
    
    movf    display+0, w	    //w = display
    movwf   PORTD	    //PORTD = w
    bsf	    PORTE, 0	    //se prende el display 1
    
    bsf	ban0,   0
    bcf	ban0,   1
    bcf	ban0,   2
    return
    
display1:	//01
    movf    SECOND, w
    movwf   PORTD  
    movf    display+1, w    //w = display+1
    movwf   PORTD	    //PORTC = w
    bsf	    PORTE, 1 	    //se prende el display 2 3
    btfsc   halfie, 0
    bsf	    RD7 
    
    bcf	ban0,   0
    bsf	ban0,   1
    bcf	ban0,   2
    return
    
display2:	//11
    CLRF    PORTD
    movf    display+2, w    //w = display+2
    movwf   PORTD	    //PORTC = w
    bsf	    PORTE, 2 	    //se prende el display 3 
    btfsc   halfie, 0
    bsf	    RD7    
    
    bcf	ban0,   0
    bcf	ban0,   1
    bsf	ban0,   2
    return
    
display3:	//11
    bsf	    RC7	    //se prende el display 3
    movf    display+3, w    //w = display+1
    movwf   PORTD	    //PORTC = w
    
    bcf	ban0,   0
    bcf	ban0,   1
    bcf	ban0,   2
    return
    
return_t0:
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
    

max28days:
    movlw   28
    movwf   DIAMAX
    movwf   temp
    return
    
max29days:
    movlw   29
    movwf   DIAMAX
    movwf   temp
    return
    
max30days:
    movlw   30
    movwf   DIAMAX
    movwf   temp
    return
    
max31days:
    movlw   31
    movwf   DIAMAX
    movwf   temp
    return
    
maxdaysmonths:	    //regresa # de dias del mes
    
    movf    MONTH, w //REVISA en qué mes está
    movwf   temp
    movlw   1	    //ENERO
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max31days		//w = 31	DIAMAX = w
    
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   2	    //FEBRERO
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max28days
    
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   3	    //MARZO
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max31days
        
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   4	    //ABRIL
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max30days
        
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   5	    //MAYO
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max31days
        
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   6	    //JUNIO
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max30days
         
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   7	    //JULIO
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max31days
        
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   8	    //AGOSTO
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max31days
        
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   9	    //SEPTIEMBRE
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max30days
        
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   10	    //OCTUBRE
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max31days
        
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   11	    //NOVIEMBRE
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max30days
        
    movf    MONTH, w //REVISA # DE DIAS
    movwf   temp
    movlw   12	    //DICIEMBRE
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    call    max31days
    
    return
        
main:   
    call config_reloj	//oscilador
    call config_io	//I/O
    call config_ioc	//PORTB PULLUP INTERRUPT
    call config_tmr0	//TMR0
    call config_tmr1	//TMR1
    call config_tmr2	//TMR2
    call config_int	//interrupciones
    BANKSEL PORTA  
    
 loop:	    //el código cueanto no hay interrupciones
    //CLRF    PORTE
   // CLRF    PORTD
    BCF	    RC7
    BCF	    RA4
    
    call checkmode
    GOTO    loop

checkmode: //elige qué display poner
    CLRF    PORTA
    CLRF    PORTD
    
    movf    MODO, w //IF MODO = 00
    movwf   temp
    movlw   0
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    goto    m0   //10
    
    movf    MODO, w //IF MODO = 01
    movwf   temp
    movlw   1
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    goto    m1   //10
    
    movf    MODO, w //IF MODO = 10
    movwf   temp
    movlw   2
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    goto    m2   //10
    
    movf    MODO, w	//IF MODO = 11
    movwf   temp
    movlw   3
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    goto    m3   //11
    
    return
    
    check_pausa2:
    movf    PAUSA, w	    //PAUSA OVERFLOW //modo 3
    movwf   temp
    movlw   2
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    CLRF    PAUSA
    return
    
    check_pausa3:
    movf    PAUSA, w	    //PAUSA OVERFLOW //modo 3
    movwf   temp
    movlw   3
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende    
    CLRF    PAUSA
    return
    
    m0:	//minuto para hora
    bsf	    PORTA, 0
    CALL    SET_DISPLAYS00
    call    check_pausa2
    
    return
    
    m1:	///
    CALL    SET_DISPLAYS01
    bsf	    PORTA, 1
    call    check_pausa2
    return
    
    m2:
    CALL    SET_DISPLAYS10
    bsf	    PORTA, 2
    call    check_pausa3
    return
    
    m3:
    CALL    SET_DISPLAYS11
    bsf	    PORTA, 3
    call    check_pausa2
    
    return
    
 ////////////////////////////   
SET_DISPLAYS00:
    CLRF    temp
    call    checkseconds	//checks for overflow and underflow seconds (60)
    call    checkminutes	//checks for overflow and underflow minutes (60)
    call    checkhours	//checks for overflow and underflow hours   (24)
    DIVISION	MINUTE, MINUTES1, MINUTES2, temp    //MINUTE1 = unidades    MINUTE2 = decena
    DIVISION	HOUR, HOURS1, HOURS2, temp
    
    MOVF    HOURS1 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+3		; Guardamos en display+3
    
    MOVF    HOURS2 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+2		; Guardamos en display+2
    
    MOVF    MINUTES1 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+1		; Guardamos en display+1
    
    MOVF    MINUTES2  , w		; Movemos nibble bajo a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+0		; Guardamos en display
    RETURN

checkseconds:
    movf    SECOND, w
    movwf   temp	    //asigna un valor temporal para no modificar el
			    //valor original
    movlw   60
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 0	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    alarma60s
    return
   
alarma60s:
    CLRF    SECOND
    incf    MINUTE
    return  
   
checkminutes: 
    movf    MINUTE, w
    movwf   temp	    //asigna un valor temporal para no modificar el
	  
    movlw   60
    subwf   temp, w	; Se resta w a MINUTE
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    alarma60m
    
    movf    MINUTE, w
    movwf   temp	    //asigna un valor temporal para no modificar el
	
    movlw   60
    subwf   temp, w	; Se resta w a MINUTE
    btfsc   STATUS, 0	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    underflowm
    
    return

alarma60m:
    CLRF    MINUTE
    incf    HOUR
    return
    
underflowm:
    CLRF    MINUTE
    movlw   59
    movwf   MINUTE
    
    movf    HOUR, w	///si hay underflow del h
    movwf   temp  
    movlw   0
    subwf   temp, w
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    underflowh
    decf    HOUR
    
    return
 
////////    
checkhours:
    movf    HOUR, w
    movwf   temp  	  
    movlw   24
    subwf   temp, w	; Se resta w a HOUR
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    alarma24h
    
    movf    HOUR, w
    movwf   temp  	    //asigna un valor temporal para no modificar el
    movlw   24
    subwf   temp, w	; Se resta w a HOUR
    btfsc   STATUS, 0	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    underflowh
    
    return

alarma24h:  ///si h = 25
    CLRF    HOUR
    return
    
underflowh: ///si h = -1
    movlw   23
    movwf   HOUR
    return
    
    
    
//////////////////////////
 SET_DISPLAYS01:	///modo fecha
    CLRF    temp
    call    checkxd
    call    check12m
    DIVISION	DAY, DAY1, DAY2, temp
    DIVISION	MONTH, MONTH1, MONTH2, temp
    
    MOVF    DAY1 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+3		; Guardamos en display+3
    
    MOVF    DAY2 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+2		; Guardamos en display+2
    
    MOVF    MONTH1 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+1		; Guardamos en display+1
    
    MOVF    MONTH2  , w		; Movemos nibble bajo a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+0		; Guardamos en display
    RETURN
    
    
check12m: //revisa underflow and overflow de los meses
    movf    MONTH, w
    movwf   temp	    //asigna un valor temporal para no modificar el
			    // temp = MONTH
    movlw   13		//overflow
    subwf   temp, w	; Se resta w a MINUTE	temp = temp - 12
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    alarma12m
    
    movf    MONTH, w
    movwf   temp	    //asigna un valor temporal para no modificar el
		    // temp = MONTH
    movlw   13		//underflow
    subwf   temp, w	; Se resta w a MINUTE
    btfsc   STATUS, 0	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    underflowmon
    
    return

alarma12m:
    CLRF    MONTH
    return
    
underflowmon:
    movlw   12
    movwf   MONTH
    return
    
 checkxd:
    
    call    maxdaysmonths   //maxdays according to month	DIAMAX = temp = # dias max segun mes
    movf    DAY,    w	    //w = DAY
    subwf   temp, w	    //DIAMAX = DIAMAX - w (DAY)
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    alarmaxd
    
    call    maxdaysmonths   //maxdays according to month
    movf    DAY,    w    
    subwf   temp, w	; Se resta w a MINUTE	temp = temp - 12
    btfsc   STATUS, 0	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    underflowxd
    
    return

alarmaxd:
    CLRF    DAY
    return
    
underflowxd:
    movf    DIAMAX, w
    movwf   DAY	    //DAY = w
    return   
 
   
  ///////////////////////////////////  3er modo display
  SET_DISPLAYS10:
    CLRF    temp
    call    tcheck60s
    call    tcheck99m
    DIVISION	tMINUTE, tMINUTES1, tMINUTES2, temp
    DIVISION	tSECOND, tSECOND1, tSECOND2, temp
    
    MOVF    tMINUTES1 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+3		; Guardamos en display+3
    
    MOVF    tMINUTES2 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+2		; Guardamos en display+2
    
    MOVF    tSECOND1 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+1		; Guardamos en display+1
    
    MOVF    tSECOND2  , w		; Movemos nibble bajo a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+0		; Guardamos en display
    RETURN

tcheck60s:
    movf    tSECOND, w
    movwf   temp	    //asigna un valor temporal para no modificar el
			    //valor original
    movlw   60
    subwf   temp, w	; Se resta w a SECOND
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    talarma60s
    
    movf    tSECOND, w
    movwf   temp	    //asigna un valor temporal para no modificar el
			    //valor original
    movlw   60
    subwf   temp, w	; Se resta w a MINUTE
    btfsc   STATUS, 0	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    tunderflows
    
    return
   
talarma60s:
    CLRF    tSECOND
    incf    tMINUTE
    return 
    
tunderflows:
    CLRF    tSECOND
    movlw   59
    movwf   tSECOND
    decf   tMINUTE
    return  
   
tcheck99m: 
    movf    tMINUTE, w
    movwf   temp	    //asigna un valor temporal para no modificar el
	  
    movlw   100
    subwf   temp, w	; Se resta w a MINUTE
    btfsc   STATUS, 2	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    talarma99m
    
    movf    tMINUTE, w
    movwf   temp	    //asigna un valor temporal para no modificar el
	
    movlw   100
    subwf   temp, w	; Se resta w a MINUTE
    btfsc   STATUS, 0	; si la resta da 0 significa que son iguales entonces la zero flag se enciende
    call    tunderflowm
    
    return

talarma99m:
    CLRF    tMINUTE
    CLRF    tSECOND
    return
    
tunderflowm:
    movlw   99
    movwf   tMINUTE
  
    return
  
SET_DISPLAYS11:
;    CLRF    temp
;    call    tcheck60s
;    call    tcheck99m
;    DIVISION	tMINUTE, tMINUTES1, tMINUTES2, temp
;    DIVISION	tSECOND, tSECOND1, tSECOND2, temp
    
    MOVF    0 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+3		; Guardamos en display+3
    
    MOVF    0 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+2		; Guardamos en display+2
    
    MOVF    0 , w		; Movemos nibble alto a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+1		; Guardamos en display+1
    
    MOVF    0  , w		; Movemos nibble bajo a W
    CALL    table		; Buscamos valor a cargar en PORTD
    MOVWF   display+0		; Guardamos en display
    RETURN
   
///////////////////////configuraciones///////////////////////
config_reloj:
    BANKSEL OSCCON	;banco 1
    BSF OSCCON, 0	; SCS -> 1, se usa reloj interno
    BCF OSCCON, 6	//  IRCF 011 -- seleccion de oscilador
    BSF OSCCON, 5
    BSF OSCCON, 4	;IRCF<2:0> -> 5 kHz
    
    return
    
config_ioc:	//CONFIGURACION DEL PORTB INTERRUPCIONES
    BANKSEL IOCB
    BSF IOCB, UP	    ;habilita interrupt
    BSF IOCB, DOWN
    BSF IOCB, MODE	    ;habilita interrupt
    BSF IOCB, ENTER
    BSF IOCB, PAUSE	    ;habilita interrupt
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
    BSF TRISB, UP    ; RB0 como entrada
    BSF TRISB, DOWN    ;RB1 como entrada
    BSF TRISB, MODE    ; RB2 como entrada
    BSF TRISB, ENTER    ;RB3 como entrada
    BSF TRISB, PAUSE    ; RB4 como entrada
    
    BCF OPTION_REG, 7	;pull-ups are enabled by individual PORT latch values
    BSF WPUB, UP	    ;habilita pull-up en RB0
    BSF WPUB, DOWN	    ;habilita pull-up en RB1
    BSF WPUB, MODE	    ;habilita pull-up en RB2
    BSF WPUB, ENTER	    ;habilita pull-up en RB3
    BSF WPUB, PAUSE	    ;habilita pull-up en RB4
    
    BANKSEL TRISD
    CLRF TRISD			;PORTD como salida display
    CLRF TRISA			;PORTA como salida MUESTRA EL MODO
    BCF	TRISE, 0		; SELECTOR DISPLAY0
    BCF	TRISE, 1		; SELECTOR DISPLAY1
    BCF	TRISE, 2		; SELECTOR DISPLAY2
    BCF	TRISE, 3		; SELECTOR DISPLAY3
    BCF	TRISC, 7		; SELECTOR DISPLAY3
    
    BANKSEL PORTC   ;se selecciona el banco 0 (00)
    CLRF PORTD
    CLRF PORTA
    CLRF    ban0
    CLRF    ban1		; Limpiamos GPR
    
    CLRF    MODO		; Limpiamos modo
    CLRF    EDIT		; Limpiamos edit
    CLRF    PAUSA		; Limpiamos second
    CLRF    tEDIT		; Limpiamos second
    CLRF    SECOND		; Limpiamos second
    CLRF    tSECOND1		; Limpiamos secondtimer
    CLRF    tSECOND2		; Limpiamos secondtimer
    CLRF    MINUTE		; Limpiamos miunte
    CLRF    DAY		; Limpiamos miunte
    CLRF    MONTH	
    incf    DAY		; Limpiamos miunte
    incf    MONTH		; Limpiamos miunte
    CLRF    HOUR		; Limpiamos hour
    CLRF    HOURS1
    CLRF    MINUTES1		; Limpiamos hour
    CLRF    HOURS2
    CLRF    MINUTES2		; Limpiamos hour
    CLRF    tMINUTE		; Limpiamos miunte
    CLRF    tMINUTES1		; Limpiamos hour
    CLRF    tMINUTES2		; Limpiamos hour
        
    return 
    
  config_tmr0:
    BANKSEL OPTION_REG	;banco 1
    BCF T0CS		;TMR0 como temporizador
    BCF PSA		;se asigna prescaler to Timer0
    BSF PS2
    BSF PS1
    BSF PS0		;PS<2:0> -> 111 prescaler 1 : 256
    BANKSEL PORTA
    RESET_TMR0
    return 
    
 config_tmr1:		;PS<2:0> -> 111 prescaler 1 : 256
    BANKSEL PORTA
   // bcf	    TMR1GE
    bsf	    T1OSCEN	//internal clock
    bcf	    TMR1CS
    bsf	    TMR1ON	//TIMER1  ON
    bsf	    T1CKPS1	//prescaler 8:1
    bsf	    T1CKPS0
    RESET_TMR1 
    return 
    
config_tmr2:
    BANKSEL PORTA	//prescaler 1:16
    bsf	    TOUTPS3	//TOUTPS<3:0>: 1111
    bsf	    TOUTPS2
    bsf	    TOUTPS1
    bsf	    TOUTPS0
    
    bsf	    T2CKPS1	//prescaler 1:16
    bcf	    T2CKPS0
    bsf	    TMR2ON
    
    RESET_TMR2 
    return
    
config_int:
    BANKSEL TRISA
    bsf	    TMR1IE	    //tmr1 interrupt
    bsf	    TMR2IE	    //tmr2 interrupt
    BANKSEL INTCON
    BSF	    T0IE	    ; Habilitamos interrupcion TMR0
    BCF	    T0IF	    ; Limpiamos bandera de TMR0
    bcf	    TMR1IF	    //bandera tmr
    bcf	    TMR2IF	    //bandera tmr2
    bsf	    PEIE
    BSF	    RBIE	    ; Habilitamos interrupcion PORTB
    BCF	    RBIF	    ; Limpiamos bandera de PORTB
    BSF	    GIE		    ; Habilitamos interrupciones
    RETURN
  
    
 