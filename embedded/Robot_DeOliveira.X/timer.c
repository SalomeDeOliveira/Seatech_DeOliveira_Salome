#include <xc.h>
#include "timer.h"
#include "main.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"

unsigned long timestamp;

//Initialisation d?un timer 16 bits
void InitTimer1(void) {
    //Timer1 pour horodater les mesures (1ms)
    T1CONbits.TON = 0; // Disable Timer
//    T1CONbits.TCKPS = 0b10; //Prescaler
    //11 = 1:256 prescale value
    //10 = 1:64 prescale value
    //01 = 1:8 prescale value
    //00 = 1:1 prescale value
    T1CONbits.TCS = 0; //clock source = internal clock
    SetFreqTimer1(50);
    //PR1 = 0x1D4C; // en hexadecimal soit 7500 sec donc frequence= 60 MHz(frequence du qwartz a l'interieur)/(7500*prescaler)
//    PR1 = 0x249F; // on teste une autre periode pour avoir une frequence de 6kHz
    IFS0bits.T1IF = 0; // Clear Timer Interrupt Flag
    IEC0bits.T1IE = 1; // En able Timer interrupt
    T1CONbits.TON = 1; // Enable Timer
}
//Interruption du timer 1
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
//    LED_BLEUE_1 = !LED_BLEUE_1;
    PWMUpdateSpeed();
    ADC1StartConversionSequence();
}
//Initialisation d?un timer 32 bits
void InitTimer23(void) {
    T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
    T2CONbits.T32 = 1; // Enable 32-bit Timer mode
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR3 = 0x00; // Clear 32-bit Timer (msw) en hexadecimal
    TMR2 = 0x00; // Clear 32-bit Timer (lsw)en hexadecimal
    PR3 = 0x727; // Load 32-bit period value (msw) en hexadécimal car PR=60000000 et en hexa c'est 0393 8700 donc PR2 et PR3
    PR2 = 0xE00; // Load 32-bit period value (lsw) en hexadecimal
    IPC2bits.T3IP = 0x01; // Set Timer3 Interrupt Priority Level
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
    IEC0bits.T3IE = 1; // Enable Timer3 interrupt
    T2CONbits.TON = 1; // Start 32-bit Timer
}
//Interruption du timer 32 bits sur 2-3
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
//    LED_ORANGE_1 = !LED_ORANGE_1;
}

unsigned char toggle = 0;
//Interruption du timer 32 bits sur 2-3
//void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
//    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
//    if(toggle == 0)
//    {
//    PWMSetSpeed(20, MOTEUR_DROIT);
//    PWMSetSpeed(20, MOTEUR_GAUCHE);
//    toggle = 1;
//    }
//    else
//    {
//    PWMSetSpeed(-20, MOTEUR_DROIT);
//    PWMSetSpeed(-20, MOTEUR_GAUCHE);
//    toggle = 0;
//    }
//}
void SetFreqTimer1(float freq)
{
    float FCY=60000000;
    T1CONbits.TCKPS = 0b00; //00 = 1:1 prescaler value
    if(FCY /freq > 65535)
    {
        T1CONbits.TCKPS = 0b01; //01 = 1:8 prescaler value
        if(FCY /freq / 8 > 65535)
        {
            T1CONbits.TCKPS = 0b10; //10 = 1:64 prescaler value
            if(FCY /freq / 64 > 65535)
            {
                T1CONbits.TCKPS = 0b11; //11 = 1:256 prescaler value
                PR1 = (int)(FCY / freq / 256);
            }
            else
            PR1 = (int)(FCY / freq / 64);
        }
        else
        PR1 = (int)(FCY / freq / 8);
    }
    else
    PR1 = (int)(FCY / freq);
}

//Initialisation d?un timer 16 bits
void InitTimer4(void) {
    T4CONbits.TON = 0; // Disable Timer
    T4CONbits.TCS = 0; //clock source = internal clock
    SetFreqTimer1(1000);
    IFS1bits.T4IF = 0; // Clear Timer Interrupt Flag
    IEC1bits.T4IE = 1; // En able Timer interrupt
    T4CONbits.TON = 1; // Enable Timer
}
//Interruption du timer 4
void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void) {
    IFS1bits.T4IF = 0;
    timestamp=timestamp+1;
    OperatingSystemLoop();
}