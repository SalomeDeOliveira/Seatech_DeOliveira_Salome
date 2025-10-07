#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "PWM.h"
#include "timer.h"
#include "ADC.h"

int main (void){
    /**************************************************************************************************/
    //Initialisation oscillateur
    /***************************************************************************************/
    InitOscillator();
     
    /***************************************************************************************************/
    // Configuration des input et output (IO)
    /***************************************************************************************************/
    InitIO();
    LED_BLANCHE_1 = 1;
    LED_BLEUE_1 = 1;
    LED_ORANGE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;
    
    LED_BLANCHE_2 = 1;
    LED_BLEUE_2 = 1;
    LED_ORANGE_2 = 1;
    LED_ROUGE_2 = 1;
    LED_VERTE_2 = 1;
    
    InitADC1();
    InitTimer23();
    InitTimer1();
    
    InitPWM();
    //PWMSetSpeed(20,MOTEUR_DROIT);
    PWMSetSpeedConsigne (20,1);
 
    /***************************************************************************************************/
    // Boucle Principale
    /***************************************************************************************************/
    while(1)
    {
        unsigned char ADCflag= ADCIsConversionFinished();
        unsigned int * result = ADCGetResult();
        if (ADCflag==1){
            ADCClearConversionFinishedFlag();
            int ADCValue0=result[0];
            int ADCValue1=result[1];
            int ADCValue2=result[2];
            if 
        }
     } // fin main
    
    
}
