/*
 * File:   ProyectoOjosMASTER.c
 * Author: WMGWW
 *
 * Created on 21 de mayo de 2022, 06:03 AM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
        
#include <xc.h>
#include <stdio.h>
#include "stup.h"

//prototipos
void configint(void);
void resettmr0(void);
void setuptmr0(void);

//constantes
#define MODEbut PORTBbits.RB0     // Asignamos un alias a RB0
#define SAVE1but PORTBbits.RB1     // Asignamos un alias a RB1
#define SAVE2but PORTBbits.RB2     // Asignamos un alias a RB2
#define SAVE3but PORTBbits.RB3     // Asignamos un alias a RB3
//#define I2C_SPEED 100000
#define ADDRESS 0x08
#define LEN_MSG 7   
#define READ 0b0
#define WRITE 0b1
#define _XTAL_FREQ 4000000      //configuracion 8MHz
//variables
char mensaje[LEN_MSG] = {'P', 'o', 't', '1', ':', ' ', ' '};
//char mensaje[LEN_MSG] = {'D', 'a', 't', 'o', ':', ' ', ' ', , 0x0A};
uint8_t indice = 0;             // Variable para saber que posici n del mensaje �enviar al serial
uint8_t valor_old = 0, potsel = 0;          // Variable para guardar el valor anterior recibido

uint8_t potvalue1 = 0, potvalue2 = 0, potvalue3 = 0, potvalue4 = 0;
uint8_t index = 0;             // Variable para saber que posici n del mensaje 
uint8_t mode = 1;       //variable for mode
uint8_t oldvalue = 0;          // Variable para guardar el valor anterior recibido
uint8_t  data = 0, response = 0, amount = 0;
uint8_t addeeprom, cont = 0;

//----------------------interrupciones------------------------------------------
void __interrupt() isr(void){   
    //ADC INTERRUPT
    if (ADIF == 1 && mode == 1) {
        if(!ADCON0bits.CHS){
            potvalue1 = (ADRESH>>1)+124;
            CCPR1L = potvalue1; 
            ADCON0bits.CHS = 1; 
        }
        //canal pwm2
        else if(ADCON0bits.CHS == 1 && mode == 1) {
            potvalue2 = (ADRESH>>1)+124;
            CCPR2L = potvalue2; 
            
            ADCON0bits.CHS = 0b0010;//se cambia a canal del tercer pot
        }
        else if(ADCON0bits.CHS == 2 && mode == 1) {
            potvalue3 = (ADRESH>>1)+124;
            ADCON0bits.CHS = 3;//se cambia a canal del tercer pot
            
        } 
        else if(ADCON0bits.CHS == 3 && mode == 1) {  
            potvalue4 = (ADRESH>>1)+124;
            
            ADCON0bits.CHS = 0;//se cambia a canal del primer pot
           // CCP2CONbits.DC2B0 = (ADRESL>>7);
            
        } 
        __delay_us(40);   
        PIR1bits.ADIF = 0;
        ADCON0bits.GO = 1;
        
    } //adif
    if(INTCONbits.T0IF){    //se revisa bandera del timer0
       
        resettmr0();  
        
      
    }//tmr0
    
    if(INTCONbits.RBIF){            // Fue interrupci n del PORTB�
        if(!MODEbut){                 // Verificamos si fue RB0 quien gener  la �interrupci n�
            mode++;
        }
        if (mode == 1){
            
            if(!SAVE1but){                 // Verificamos si fue RB1 quien gener  la �interrupci n�
                //PORTD = read_EEPROM(1);
                            // Verificamos si fue RB1 quien gener  la �interrupci n�
                write_EEPROM(1, potvalue1);
                write_EEPROM(2, potvalue2);
                write_EEPROM(3, potvalue3);
                write_EEPROM(4, potvalue4);
                INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
            }
            if(!SAVE2but){                 // Verificamos si fue RB2 quien gener  la �interrupci n�
            //PORTD = read_EEPROM(1);
            
                INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
            }
            if(!SAVE3but){                 // Verificamos si fue RB3 quien gener  la �interrupci n�
            //PORTD = read_EEPROM(1);
            
                INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
            }
        } //if mode manual
       if (mode == 2){
            
            if(!SAVE1but){                 // Verificamos si fue RB1 quien gener  la �interrupci n�
                //PORTD = read_EEPROM(1);
                            // Verificamos si fue RB1 quien gener  la �interrupci n�
                potvalue1 = read_EEPROM(1);
                potvalue2 = read_EEPROM(2);
                potvalue3 = read_EEPROM(3);
                potvalue4 = read_EEPROM(4);
                
                
                INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
            }
            if(!SAVE2but){                 // Verificamos si fue RB2 quien gener  la �interrupci n�
            //PORTD = read_EEPROM(1);
                CCPR1L = read_EEPROM(1);
                INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
            }
            if(!SAVE3but){                 // Verificamos si fue RB3 quien gener  la �interrupci n�
            //PORTD = read_EEPROM(1);
            
                INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
            }
        } 
       // INTCONbits.RBIF = 0;    // Limpiamos bandera de interrupci n�
    } //RBIF
    if(PIR1bits.RCIF && mode == 3){          // Hay datos recibidos?
        //__delay_ms(50);
        if (RCREG<=4 && RCREG>=1){
            mensaje[3] = RCREG; // ELECCION de potenciometro
            potsel = mensaje[3];
        } else mensaje[6] = RCREG; 
        
    }
    } //interrupciones
    

void main(void) {
    setup();
    setupI2C();
    //LOOP
    while (1){ 
    
    if (mode == 4) mode = 1; //mode reseter
    
    PORTD = mode; //SHOWS MODE
    if (mode == 3) { //consola
        indice = 0;                             // Reiniciamos indice para enviar todo el mensaje
        if (valor_old != mensaje[6]){           // Verificamos que el nuevo valor recibido en el serial 
                                                //   sea diferente al anterior, para imprimir solo 
            while(indice<LEN_MSG){              // Loop para imprimir el mensaje completo
                if (PIR1bits.TXIF){             // Esperamos a que est  libre el �TXREG para poder enviar por el serial
                    TXREG = mensaje[indice];    // Cargamos caracter a enviar
                    indice++;                   // Incrementamos indice para enviarsigiente caracter
                }
            }
            valor_old = mensaje[6];             // Guardamos valor recibido para comparar en siguiente iteraci n�
                                                //   si el nuevo valor recibido es diferente al anterior. 
        }
        switch (potsel){
            case 1:
                CCPR1L = mensaje[3];
                break;
            case 2:
                CCPR2L = mensaje[3];
                break;
            case 3:
                //CCPR2L = mensaje[3];
                break;
            case 4:
                //CCPR2L = mensaje[3];
                break;
        }
    }
    if (mode == 2) {
        
    }
    if (mode == 1) {
        
    }
    PORTD = mode; //SHOWS MODE
    
    data = (uint8_t)((ADDRESS<<1)+READ);
        start_I2C();  
        write_I2C(data);                // Enviamos direcci n de esclavo a recibir datos�
        write_I2C(potvalue3);            // Enviamos dato al esclavo
        stop_I2C();                 // Finalizamos la comunicaci n� 
        //PORTA = amount;
//        PORTA = cont;               // Mostramos valor enviado en PORTA
//        cont++;
        __delay_ms(100);
    
   }//WHILE LOOP
    return;   
}