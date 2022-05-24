/*
 * File:   setup.c
 * Author: WMGWW
 *
 * Created on 16 de mayo de 2022, 11:20 AM
 */


//SETUP SLAVE
#define _XTAL_FREQ 8000000

#include <xc.h>
#include <stdint.h>

//uint8_t address, cont = 0;

/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    OSCCONbits.IRCF = 0b111;    // 4MHz
    OSCCONbits.SCS = 1;         // Reloj interno
    
    TRISC = 0b00011000;         // SCL and SDA as input
    PORTC = 0;
    
    TRISD = 0;
    PORTD = 0x00;
    
    TRISA = 0;
    PORTA = 0x00;
    
    //configuracion pwm
    //CCP1
    TRISCbits.TRISC2 = 1;      //CCP1 como entrada;
    PR2 = 250;                 //valor para que el periodo pwm sea 2 ms 
    CCP1CONbits.P1M = 0;       //config pwm
    CCP1CONbits.CCP1M = 0b1100;
    CCPR1L = 0x0f;             //ciclo de trabajo inicial
    CCP1CONbits.DC1B = 0;
    
    //CCP2
    TRISCbits.TRISC1 = 1;       //CCP2 como entrada;
    CCP2CONbits.CCP2M = 0b1100; //config pwm
    CCPR2L = 0x0f;              //ciclo de trabajo inicial
    CCP2CONbits.DC2B1 = 0;
    
    //CONDIG TMR2
    PIR1bits.TMR2IF = 0;        //TMR2 interrrupt flag
    T2CONbits.T2CKPS = 0b11;    //prescaler 1:16
    T2CONbits.TMR2ON = 1;       //se enciende el tmr2
    while(PIR1bits.TMR2IF == 0);//esperar un ciclo de tmr2
    PIR1bits.TMR2IF = 0;
    
    TRISCbits.TRISC2 = 0;       //out pwm2
    TRISCbits.TRISC1 = 0;       //out pwm1
    
    
    
    SSPADD = 0x10;              // Direcci n de esclavo: 0x08 0b0001 000x�
    SSPSTATbits.SMP = 1;        // Velocidad de rotaci n�
    SSPCONbits.SSPM = 0b0110;   // I2C slave mode, 7-bit address
    SSPCONbits.SSPEN = 1;       // Habilitamos pines de I2C
    PIR1bits.SSPIF = 0;         // Limpiamos bandera de interrupci n de I2C�
    PIE1bits.SSPIE = 1;         // Habilitamos interrupcion de I2C
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de perifericos
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
}