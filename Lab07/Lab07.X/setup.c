/*
 * File:   setup.c
 * Author: Dina
 *
 * Created on 4 de abril de 2022, 12:53 AM
 */


#include <xc.h>

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    
    ANSEL = 0;
    ANSELH = 0b00000000;        // Usaremos solo I/O digitales
    
    TRISA = 0x00;               // PORTA como salida
    PORTA = 0;                  // Limpiamos PORTA
    TRISC = 0x00;               // PORTC como salida
    PORTC = 0;                  // Limpiamos PORTC
    
    //TRISB = 1;                // RB0 como entrada (configurada en decimal)
    //TRISB = 0b00000001;       // RB0 como entrada (configurada con binario)
    TRISBbits.TRISB0 = 1;       // RB0 como entrada (configurada con control de bits)
    TRISBbits.TRISB1 = 1;       // RB1 como entrada (configurada con control de bits)
    
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;         // Habilitamos resistencia de pull-up de RB0
    WPUBbits.WPUB1 = 1;         // Habilitamos resistencia de pull-up de RB1
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    IOCBbits.IOCB0 = 1;         // Habilitamos interrupci n por cambio de estado para RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos interrupci n por cambio de estado para RB1
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupci n�
}

 
 // Td = Pre * TMR1*Ti
 // N = 65536-(Td/Pre*Ti)
 
 //Ttmr1if = Prescaler * PR2 * Postscaler * (1/(Fosc/4))
 //PR2 = Ttmr2if/Prescaler * Postscaler * (1/(Fosc/4))

void setuptmr0(void){
    
    
}

