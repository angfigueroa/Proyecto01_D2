/*
 * File:   Temperatura.c
 * Author: ANGELA
 *
 * Created on 12 de agosto de 2023, 17:28
 */


// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
//#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include "ADC_T.h"
#include "SPI.h"
#define _XTAL_FREQ (8000000)
#define led_r RD2
#define led_a RD1
#define led_g RD0

//VARIABLES
int term;
int adc_fin;

//PROTOTIPOS
void conf_but(void);

//MAIN
void main(void){
    conf_but(); //Llamo mi setup principal
    adc_fin = 0; //Bandera termino ADC
    ADC_CONF();//LLamo a la config del ADC
    term = 0; //Incializa mi variable qie guarda Adresh
    conf_ch(0);
    
    while(1){
        if (adc_fin == 0){
            adc_fin = 1;
            __delay_ms(10); 
            ADCON0bits.GO = 1; //Enciende la conversion
        }
        if (term < 100){ //Si mi valor no para de los 100, no paso los 25C
            led_g = 1;//Luz verde
            led_a = 0;
            led_r = 0;
        }
        else if (100 < term & term < 113){ //Si mi valor no para de los 100, 25C a 36C
            led_a = 1;
            led_g = 0;//Luz amarilla
            led_r = 0;
        }
        else if (113 <= term){ //Si mi valor no para de los 100, arriba 36C
            led_a = 0;
            led_g = 0;
            led_r = 1;//Luz rojo 
        }
    }
    
}

//FUNCIONES
void conf_but(void){
    //Configurar los puertos
    INTCONbits.GIE = 1;//Habilita las interrupciones
    INTCONbits.PEIE = 1;//Interrupciones perifericas
    ANSEL = 0;
    ANSELH = 0;
    ANSELbits.ANS0 = 1; //Excepto el pin AN0
    TRISB = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    TRISA = 0;
    TRISC = 0;
    TRISC4 = 1;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA5 = 1;
    PORTD = 0;
    PORTB = 0;
    PORTC = 0;
    PORTE = 0;
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
   
}
//INTERRUPCION
void __interrupt() ISR(void){
    if (PIR1bits.ADIF == 1){
        //revisa la bandera del ADC
        term = ADRESH;
        adc_fin = 0;
    }
    PIR1bits.ADIF = 0;
        
    if (SSPIF == 1){
        spiWrite(term);//Mando mi conversion al termometro
        SSPIF = 0;
    }
    
}

