/*
 * File:   stup.c
 * Author: WMGWW
 *
 * Created on 21 de mayo de 2022, 06:10 AM
 */

#include <xc.h>

#define _XTAL_FREQ 8000000      //configuracion 4MHz
#define I2C_SPEED 100000
#define ADDRESS 0x08
#define READ 0b0
#define WRITE 0b1

void resettmr0(void){
   // TMR0 = 5;              //para 15.1 ms
    INTCONbits.T0IF=0;     // ; Limpiamos bandera de TMR0
}

void setuptmr0(void){
    OPTION_REGbits.T0CS=0; // SELECT INTERNAL SOURCE TMR0
    OPTION_REGbits.PSA=0;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS2=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS1=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS0=1;  // PRESCALER ASSIGNED TO TIMER0 256
    resettmr0();
    
}

void configint(void){
    
    INTCONbits.T0IE = 1;     // Habilitamos interrupcion TMR0
    INTCONbits.T0IF = 0;     // ; Limpiamos bandera de TMR0
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;      //se habilitan las interrupciones por adc
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;      // ENABLE peripherial INTERRUPT
    INTCONbits.RBIE = 1;        //interrupciones en PORTB y TMR0
    INTCONbits.RBIF = 0;        //Apagamos banderas

    //Configuracion push button
    TRISBbits.TRISB0 = 1;       //RB0 
    TRISBbits.TRISB1 = 1;       //RB1
    TRISBbits.TRISB2 = 1;       //RB2
    TRISBbits.TRISB3 = 1;       //RB3
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUBbits.WPUB = 0b00001111;       //0011 RB0 and RB1 
    IOCBbits.IOCB = 0b00001111;       //RB0 y RB1 pull ups e interrupciones
    
    // Configuraciones de comunicacion serial
    //SYNC = 0, BRGH = 1, BRG16 = 1, SPBRG=25 <- Valores de tabla 12-5
    TXSTAbits.SYNC = 0;         // Comunicaci n ascincrona (full-duplex)�
    TXSTAbits.BRGH = 1;         // Baud rate de alta velocidad 
    BAUDCTLbits.BRG16 = 1;      // 16-bits para generar el baud rate
    
    SPBRG = 207;                 //8mhz ((8Mhz/9600) / 64
    SPBRGH = 0;                 // Baud rate ~9600, error -> 0.16%
    
    RCSTAbits.SPEN = 1;         // Habilitamos comunicaci n�
    TXSTAbits.TX9 = 0;          // Utilizamos solo 8 bits
    TXSTAbits.TXEN = 1;         // Habilitamos transmisor
    RCSTAbits.CREN = 1;         // Habilitamos receptor
    
    PIE1bits.RCIE = 1;          // Habilitamos Interrupciones de recepci n�

}
void setup(void){
    
//    //configuraciones
//    //configuracion reloj
//    CCP1CONbits.CCP1M = 0b1100;
    //Single output; P1A modulated; P1B, P1C, P1D assigned as port pins
        
    OSCCONbits.IRCF = 0b0111;//0111, Frecuencia de reloj 8 MHz
    OSCCONbits.SCS   = 1;//reloj interno
    configint();
    //configuracion in out
    ANSELH = 0; //Pines digitales
    ANSELbits.ANS0  = 1;//AN0, AN1 y AN2 como pines analogicos
    ANSELbits.ANS1  = 1;
    ANSELbits.ANS2  = 1;
    ANSELbits.ANS3  = 1;
    TRISA  = 0b00001111; //AN0, AN1 y AN2 como inputs y los demas como outputs
    PORTA  = 0;//se limpian los puertos
    TRISC  = 0b00011000;         // SCL and SDA as input
    PORTC  = 0;
    TRISD = 0; 
    PORTD = 0;
    PORTB  = 0;
    //configuracion adc
    ADCON0bits.ADCS = 2;    //10 se selecciona Fosc/32 para conversion 4us full TAD
    ADCON0bits.CHS0 = 0;    //se selecciona el canal AN0
    ADCON1bits.VCFG1 = 0;   //se ponen los voltajes de referencia internos del PIC
    ADCON1bits.VCFG0 = 0;   //0V -> 5V
    ADCON1bits.ADFM = 0;    //pushed to the left, + significativos
    ADCON0bits.ADON = 1;    //ADC ON
    __delay_us(50);         //delay de 50 us
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
    
    
    ADCON0bits.GO = 1;      //ADC conversion
    
}

void setupI2C(void){
    SSPADD = ((_XTAL_FREQ)/(4*I2C_SPEED)) - 1;  // 100 kHz
    SSPSTATbits.SMP = 1;                        // Velocidad de rotaci n�
    SSPCONbits.SSPM = 0b1000;                   // I2C master mode, clock = Fosc/(4*(SSPADD+1))
    SSPCONbits.SSPEN = 1;                       // Habilitamos pines de I2C
    PIR1bits.SSPIF = 0;                         // Limpiamos bandera de interrupci n de I2C�
}
    
void wait_I2C(void){
    while(!PIR1bits.SSPIF);     // Esperamos a que se ejecute instruccion de I2C
    PIR1bits.SSPIF = 0;         // Limpimos bandera
}
void start_I2C(void){
    SSPCON2bits.SEN = 1;        // Inicializar comunicaci n�
    wait_I2C();
}
void restart_I2C(void){
    SSPCON2bits.RSEN = 1;       // Reiniciar de comunicaci n�
    wait_I2C();
}
void stop_I2C(void){
    SSPCON2bits.PEN = 1;        // Finalizar comunicaci n�
    wait_I2C();
}
void send_ACK(void){
    SSPCON2bits.ACKDT = 0;      // Confirmar que se recibi  la data�
    SSPCON2bits.ACKEN = 1;      // Envio de ack al esclavo
    wait_I2C();
}
void send_NACK(void){
    SSPCON2bits.ACKDT = 1;      // Confirmar recepci n al finalizar comunicaci n��
    SSPCON2bits.ACKEN = 1;      // Envio de nack al esclavo
    wait_I2C();
}
__bit write_I2C(uint8_t data){
    SSPBUF = data;              // Cargar dato a enviar en el buffer
    wait_I2C();
    return ACKSTAT;             // Obtener ACK del esclavo
}
uint8_t read_I2C(void){
    SSPCON2bits.RCEN = 1;       // Pedir dato al esclavo  
    wait_I2C();
    return SSPBUF;              // Regresar dato recibido
}

uint8_t read_EEPROM(uint8_t address){
    EEADR = address;
    EECON1bits.EEPGD = 0;       // Lectura a la EEPROM
    EECON1bits.RD = 1;          // Obtenemos dato de la EEPROM
    return EEDAT;               // Regresamos dato 
}

void write_EEPROM(uint8_t address, uint8_t data){
    EEADR = address;
    EEDAT = data;
    EECON1bits.EEPGD = 0;       // Escritura a la EEPROM
    EECON1bits.WREN = 1;        // Habilitamos escritura en la EEPROM
    
    INTCONbits.GIE = 0;         // Deshabilitamos interrupciones
    EECON2 = 0x55;      
    EECON2 = 0xAA;
    
    EECON1bits.WR = 1;          // Iniciamos escritura
    
    EECON1bits.WREN = 0;        // Deshabilitamos escritura en la EEPROM
    INTCONbits.RBIF = 0;
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
}
    

