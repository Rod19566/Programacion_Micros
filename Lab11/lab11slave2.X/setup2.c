/*
 * File:   setup.c
 * Author: WMGWW
 *
 * Created on 13 de mayo de 2022, 05:14 AM
 */
#define _XTAL_FREQ 1000000

#include <xc.h>
#include <stdint.h>

void setup(void){
    ANSELH = 0x00;
    ANSEL = 0x00;
    
    TRISA = 0b00100000; //RA5 slave select
    PORTA = 0X00;
    
    TRISC = 0b00011000; // -> SDI y SCK entradas, SD0 como salida
    PORTC = 0x00;
        
    OSCCONbits.IRCF = 0b0100;   //1MHz
    OSCCONbits.SCS = 1;
     
    // SSPCON <5:0>
    SSPCONbits.SSPM = 0b0100;   // -> SPI Esclavo, SS hablitado
    SSPCONbits.CKP = 0;         // -> Reloj inactivo en 0
    SSPCONbits.SSPEN = 1;       // -> Habilitamos pines de SPI
    // SSPSTAT<7:6>
    SSPSTATbits.CKE = 1;        // -> Dato enviado cada flanco de subida
    SSPSTATbits.SMP = 0;        // -> Dato al final del pulso de reloj
    
    //Configuracion PWM
    TRISCbits.TRISC2 = 1;       //Deshabilitar salida CCP1
    PR2 = 155;                  //periodo 10ms
    
    //Configuracion CCP
    CCP1CON = 0;                //Apagamos CCP1
    CCP1CONbits.P1M = 0;        //modo single output
    CCP1CONbits.CCP1M = 0b1100; //PWM CCP1
    
    CCPR1L = 125>>2;
    CCP1CONbits.DC1B = 125 & 0b11; //2ms ancho de pulso, 20% duty cycle
    
    PIR1bits.TMR2IF = 0;        //bandera TMR2
    T2CONbits.T2CKPS = 0b11;    //prescaler 1:16
    T2CONbits.TMR2ON = 1;       //encender TMR2
    while(!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = 0;        //Esperar ciclo TMR2
    
    TRISCbits.TRISC2 = 0;       //Habilitar salida CCP1
    
    //Configuraciones de interrupcioens
    PIR1bits.SSPIF = 0;         // Limpiamos bandera de SPI
    PIE1bits.SSPIE = 1;         // Habilitamos int. de SPI
    INTCONbits.PEIE = 1;        //habilitar int. perifericos
    INTCONbits.GIE = 1;         //habilitar int. globales

}
