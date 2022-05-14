/*
 * File:   setup.c
 * Author: WMGWW
 *
 * Created on 14 de mayo de 2022, 04:40 AM
 */

#define _XTAL_FREQ 1000000
#include <xc.h>
#include <stdint.h>

uint8_t potvalue = 0;

void setup(void){
    ANSELH = 0x00;
    ANSEL =0b00000100;      //AN2
    
    TRISA = 0b00000100;     //RA2 potenciometro
    PORTA = 0X00;
    TRISD = 0x00;
    PORTD = 0x00;
    
    TRISC = 0b00010000;         // -> SDI entrada, SCK y SD0 como salida
    PORTC = 0x00;
    
    OSCCONbits.IRCF = 0b0100;   //1MHz
    OSCCONbits.SCS = 1;
                
    //SSPCON 
    SSPCONbits.SSPM = 0b0000;   // -> SPI Maestro, Reloj -> Fosc/4 (250kbits/s)
    SSPCONbits.CKP = 0;         // -> Reloj inactivo en 0
    SSPCONbits.SSPEN = 1;       // -> Habilitamos pines de SPI
    // SSPSTAT
    SSPSTATbits.CKE = 1;        // -> Dato enviado cada flanco de subida
    SSPSTATbits.SMP = 1;        // -> Dato al final del pulso de reloj
    SSPBUF = potvalue;        // Enviamos un dato inicial
       
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
    
}