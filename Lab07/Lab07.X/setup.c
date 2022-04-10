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
    TRISD = 0x00;               // PORTD como salida
    PORTD = 0;                  // Limpiamos PORTD
    
    TRISE = 0x00;               // PORTE como salida (SELECCION DISPLAYS)
    PORTE = 0;                  // Limpiamos PORTE
    
    //TRISB = 1;                // RB0 como entrada (configurada en decimal)
    //TRISB = 0b00000001;       // RB0 como entrada (configurada con binario)
    TRISBbits.TRISB0 = 1;       // RB0 como entrada (configurada con control de bits)
    TRISBbits.TRISB1 = 1;       // RB1 como entrada (configurada con control de bits)
    
    OPTION_REGbits.nRBPU = 0;   // Habilitamos resistencias de pull-up del PORTB
    WPUBbits.WPUB0 = 1;         // Habilitamos resistencia de pull-up de RB0
    WPUBbits.WPUB1 = 1;         // Habilitamos resistencia de pull-up de RB1
    
    INTCONbits.RBIE = 1;        // Habilitamos interrupciones del PORTB
    IOCBbits.IOCB0 = 1;         // Habilitamos interrupci n por cambio de estado para RB0
    IOCBbits.IOCB1 = 1;         // Habilitamos interrupci n por cambio de estado para RB1
    INTCONbits.RBIF = 0;        // Limpiamos bandera de interrupci n�
    
    //configuracion del reloj
    OSCCONbits.IRCF = 0b0101; // 2MHz
    OSCCONbits.SCS = 1;       // Oscilador interno

}//setup
 
 // Td = Pre * TMR1*Ti
 // N = 65536-(Td/Pre*Ti)
 
 //Ttmr1if = Prescaler * PR2 * Postscaler * (1/(Fosc/4))
 //PR2 = Ttmr2if/Prescaler * Postscaler * (1/(Fosc/4))


void resettmr0(void){
    TMR0 = 253;              //para 15 ms
    INTCONbits.T0IF=0;     // ; Limpiamos bandera de TMR0
}

void configint(void){
    
    INTCONbits.T0IE = 1;     // Habilitamos interrupcion TMR0
    INTCONbits.T0IF = 0;     // ; Limpiamos bandera de TMR0
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;      // ENABLE peripherial INTERRUPT
    
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
    while( temp-100 >= 0){
        temp = temp - 100;
    }
    while( temp-10 >= 0){
        temp = temp - 10;
        cont++;
    }
    return cont;
}

int uni(int x){
    int temp = x;
    int cont = 0;
    while( temp-100 >= 0){
        temp = temp - 100;
    }
    while( temp-10 >= 0){
        temp = temp - 10;
    }
    return temp;
}

void setuptmr0(void){
    //OSCCON|=0x70;      // SELECT 8MHz Internal Oscillator
    OPTION_REGbits.T0CS=0; // SELECT INTERNAL SOURCE TMR0
    OPTION_REGbits.PSA=0;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS2=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS1=1;  // PRESCALER ASSIGNED TO TIMER0
    OPTION_REGbits.PS0=1;  // PRESCALER ASSIGNED TO TIMER0 256
    //OPTION_REG&=0xF8;  // SELECT 1:2 PRESCALER
    
    resettmr0();
    
}



