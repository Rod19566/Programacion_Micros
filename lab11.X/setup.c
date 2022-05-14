/*
 * File:   setup.c
 * Author: WMGWW
 *
 * Created on 9 de mayo de 2022, 12:01 PM
 */


#include <xc.h>
#include <stdint.h>
char cont_slave = 0xFF;
char cont_master = 0;
char val_temporal = 0;

#define _XTAL_FREQ 1000000
/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH = 0;                 // I/O digitales
    
    TRISE = 0b00000001;         // RB0 como entrada
    PORTE = 0;
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Reloj interno
    
    //CONDIG TMR2
    PIR1bits.TMR2IF = 0;        //TMR2 interrrupt flag
    T2CONbits.T2CKPS = 0b11;    //prescaler 1:16
    T2CONbits.TMR2ON = 1;       //se enciende el tmr2
    while(PIR1bits.TMR2IF == 0);//esperar un ciclo de tmr2
    PIR1bits.TMR2IF = 0;
    
    // Configuraciones de comunicacion serial
    //SYNC = 0, BRGH = 1, BRG16 = 1, SPBRG=25 <- Valores de tabla 12-5
    TXSTAbits.SYNC = 0;         // Comunicaci n ascincrona (full-duplex)?
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
    
    __delay_us(50);         //delay de 50 us
    ADCON0bits.GO = 1;      //ADC conversion
    
    //MAESTRO
    if(PORTEbits.RE0){ //if maestro
        
    TRISA  = 0b00000111; //AN0, AN1 y AN2 como inputs y los demas como outputs
    PORTA = 0;
    //configuracion adc
    ADCON0bits.ADCS = 2;    //10 se selecciona Fosc/32 para conversion 4us full TAD
    ADCON0bits.CHS0 = 0;    //se selecciona el canal AN0
    ADCON1bits.VCFG1 = 0;   //se ponen los voltajes de referencia internos del PIC
    ADCON1bits.VCFG0 = 0;   //0V -> 5V
    ADCON1bits.ADFM = 0;    //pushed to the left, + significativos
    ADCON0bits.ADON = 1;    //ADC ON
    __delay_us(50);         //delay de 50 us
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;      //se habilitan las interrupciones por adc
    
        TRISC = 0b00010000;         // -> SDI entrada, SCK y SD0 como salida
        PORTC = 0;
        
        // SSPCON <5:0>
        SSPCONbits.SSPM = 0b0000;   // -> SPI Maestro, Reloj -> Fosc/4 (250kbits/s)
        SSPCONbits.CKP = 0;         // -> Reloj inactivo en 0
        SSPCONbits.SSPEN = 1;       // -> Habilitamos pines de SPI
        // SSPSTAT<7:6>
        SSPSTATbits.CKE = 1;        // -> Dato enviado cada flanco de subida
        SSPSTATbits.SMP = 1;        // -> Dato al final del pulso de reloj
        SSPBUF = 0;              // Enviamos un dato inicial
    }
    // Configs del esclavo
    else{
        TRISC = 0b00011000; // -> SDI y SCK entradas, SD0 como salida
        PORTC = 0;
        TRISBbits.TRISB0 = 1;       // RB0 como entrada (configurada con control de bits)
        TRISBbits.TRISB1 = 1;       // RB1 como entrada (configurada con control de bits)
        
        OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
        WPUBbits.WPUB0 = 1;         // Habilitamos resistencia de pull-up de RB0
        WPUBbits.WPUB1 = 1;         // Habilitamos resistencia de pull-up de RB1
    
        INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
        IOCBbits.IOCB0 = 1;         // Habilitamos interrupci n por cambio de estado para RB0
        IOCBbits.IOCB1 = 1;         // Habilitamos interrupci n por cambio de estado para RB1
        INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupci n?
   
        // SSPCON <5:0>
        SSPCONbits.SSPM = 0b0100;   // -> SPI Esclavo, SS hablitado
        SSPCONbits.CKP = 0;         // -> Reloj inactivo en 0
        SSPCONbits.SSPEN = 1;       // -> Habilitamos pines de SPI
        // SSPSTAT<7:6>
        SSPSTATbits.CKE = 1;        // -> Dato enviado cada flanco de subida
        SSPSTATbits.SMP = 0;        // -> Dato al final del pulso de reloj
        
        PIR1bits.SSPIF = 0;         // Limpiamos bandera de SPI
        PIE1bits.SSPIE = 1;         // Habilitamos int. de SPI
    }
}