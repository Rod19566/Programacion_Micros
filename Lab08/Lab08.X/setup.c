/*
 * File:   setup.c
 * Author: Dina
 *
 * Created on 4 de abril de 2022, 12:53 AM
 */


#include <xc.h>
#define _XTAL_FREQ 4000000      //configuracion 4MHz
/*------------------------------------------------------------------------------
 * CONFIGURACION 
 ------------------------------------------------------------------------------*/

void setup(void){
    ANSEL   = 0b00000011;       //AN0 AN1 analógicos
    ANSELH  = 0;                //Digitales I/O

    TRISA   = 0b00000011;       //AN0 AN1 entradas
    
    TRISC   = 0;
    TRISD   = 0;
    
    PORTA   = 0;                //CLEAR
    PORTC   = 0;                //CLEAR
    PORTD   = 0;                //CLEAR
    
    //RELOJ INTERNO
    OSCCONbits.IRCF     = 0b0110;   //4MHZ
    OSCCONbits.SCS      = 1;        //OSC INTERNO
    
    //ADC
    ADCON0bits.ADCS     = 0b01;     //Fosc/8
    ADCON1bits.VCFG0    = 0;        //VDD
    ADCON1bits.VCFG1    = 0;        //VSS
    ADCON0bits.CHS      = 0b0000;   //AN0
    ADCON1bits.ADFM     = 0;        //LEFT ADJUSTED
    ADCON0bits.ADON     = 1;        //MODULO ADC HABILITADO 
    __delay_us(40);                 //SAMPLE TIME
   
    
}//setup

void resettmr0(void){
    TMR0 = 253;              //para 15 ms
    INTCONbits.T0IF=0;     // ; Limpiamos bandera de TMR0
}

void configint(void){
    /*
    INTCONbits.T0IE = 1;     // Habilitamos interrupcion TMR0
    INTCONbits.T0IF = 0;     // ; Limpiamos bandera de TMR0*/
    PIR1bits.ADIF   = 0;
    PIE1bits.ADIE   = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE  = 1;
    
}

int display_table(int x){       //funcion que asigna los valores para encender
   switch (x){                  //los leds de los displays segun el numero
       case 0: return 0b00111111;
       //break;
       case 1: return 0b00000110;
      // break;
       case 2: return 0b01011011;
      // break;
       case 3: return 0b01001111;
       //break;
       case 4: return 0b01100110;
       ///break;
       case 5: return 0b01101101;
       //break;
       case 6: return 0b01111101;
     // break;
       case 7: return 0b00000111;
       //break;
       case 8: return 0b01111111;
       //break;
       case 9: return 0b01101111;
       //break;
       default:  return 0b00000000; //control
               break;
   }//switch(x)
}// display_table

int cen(int x){         //funcion que revisa cuantos cienes hay
    int temp = x;       //asigna una variable local temporal
    int cont = 0;       //contador local
    while( temp-100 >= 0){      //resta para comprobar si ha habido underflow
        temp = temp - 100;      //le resta 100 al temporal si no hay underflow
        cont++;                 //+1 cien
    }
    return cont;        //regresa el valor de contador
}

int dec(int x){         //funcion que revisa cuantas decenas hay
    int temp = x;
    int cont = 0;
    while( temp-100 >= 0){      //se eliminan cienes
        temp = temp - 100;
    }//while
    while( temp-10 >= 0){
        temp = temp - 10;
        cont++;
    }
    return cont;
}

int uni(int x){
    int temp = x;
    while( temp-100 >= 0){     //se eliminan cienes
        temp = temp - 100;
    }
    while( temp-10 >= 0){     //se eliminan decenas
        temp = temp - 10;
    }
    return temp;
}

void setuptmr0(void){
    OPTION_REGbits.T0CS=0; // SELECT INTERNAL SOURCE TMR0
    OPTION_REGbits.PSA=0;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS2=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS1=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS0=1;  // PRESCALER ASSIGNED TO TIMER0 256
    resettmr0();
    
}
