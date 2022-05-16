/*
 * File:   setup.c
 * Author: WMGWW
 *
 * Created on 16 de mayo de 2022, 01:05 AM
 */
#define _XTAL_FREQ 1000000

#include <xc.h>
#include <stdint.h>

 
void setup(void){
    ANSELH = 0x00;
    ANSEL = 0b00000100;      //AN2
    
    TRISA = 0b00000100;     //RA2 potenciometro
    PORTA = 0;
    
    TRISD = 0;
    PORTD = 0;
    
    TRISC = 0;         // -> SDI entrada, SCK y SD0 como salida
    PORTC = 0;
        
    OSCCONbits.IRCF = 0b0100;   //1MHz
    OSCCONbits.SCS = 1;
             
        
    //Configuracion push button
    TRISBbits.TRISB0 = 1;       //RB0 
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB = 0x01;       //0001 RB0 
    IOCBbits.IOCB = 0x01;       //RB0 y RB1 pull ups e interrupciones
    
    OSCCONbits.IRCF = 0b0100;   //1MHz
    OSCCONbits.SCS = 1;
      
    //Configuraciones de ADC
    ADCON0bits.ADCS = 0b00;     // Fosc/2

    ADCON1bits.VCFG0 = 0;       //VDD *Referencias internas
    ADCON1bits.VCFG1 = 0;       //VSS

    ADCON0bits.CHS = 0b0010;    //canal AN0
    ADCON1bits.ADFM = 0;        //justificacion Izquierda
    ADCON0bits.ADON = 1;        //habilitar modulo ADC
    __delay_us(40);
        
    //Configuraciones de interrupcioens
    PIR1bits.ADIF = 0;          //bandera int. ADC
    PIE1bits.ADIE = 1;          //habilitar int. ADC
    INTCONbits.PEIE = 1;        //habilitar int. perifericos
    INTCONbits.GIE = 1;         //habilitar int. globales
    INTCONbits.RBIE = 1;        //interrupciones en PORTB y TMR0
    INTCONbits.RBIF = 0;        //Apagamos banderas
}
