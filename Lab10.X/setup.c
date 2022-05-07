/*
 * File:   setup.c
 * Author: WMGWW
 *
 * Created on 2 de mayo de 2022, 08:10 AM
 */


#include <xc.h>

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    TRISD = 0;
    PORTD = 0;                  // PORTD como salida
    
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    
    // Configuraciones de comunicacion serial
    //SYNC = 0, BRGH = 1, BRG16 = 1, SPBRG=25 <- Valores de tabla 12-5
    TXSTAbits.SYNC = 0;         // Comunicaci n ascincrona (full-duplex)�
    TXSTAbits.BRGH = 1;         // Baud rate de alta velocidad 
    BAUDCTLbits.BRG16 = 1;      // 16-bits para generar el baud rate
    
    SPBRG = 25;
    SPBRGH = 0;                 // Baud rate ~9600, error -> 0.16%
    
    RCSTAbits.SPEN = 1;         // Communication on 
    TXSTAbits.TX9 = 0;          //  8 bits use
    TXSTAbits.TXEN = 1;         // Transmisor
    RCSTAbits.CREN = 1;         // Receptor 
    
    // Configuraciones de interrupciones
    INTCONbits.GIE = 1;         // Global Interrupt
    INTCONbits.PEIE = 1;        // Perifierical Interruption
    PIE1bits.RCIE = 1;          // Reception Interrupt
}