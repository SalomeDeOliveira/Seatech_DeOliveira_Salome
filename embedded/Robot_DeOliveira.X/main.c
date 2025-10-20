#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "main.h"
#include "IO.h"
#include "PWM.h"
#include "timer.h"
#include "ADC.h"
#include "Robot.h"

int main(void) {
    /**************************************************************************************************/
    //Initialisation oscillateur
    /***************************************************************************************/
    InitOscillator();

    /***************************************************************************************************/
    // Configuration des input et output (IO)
    /***************************************************************************************************/
    InitIO();
    //    LED_BLANCHE_1 = 1;
    //    LED_BLEUE_1 = 1;
    //    LED_ORANGE_1 = 1;
    //    LED_ROUGE_1 = 1;
    //    LED_VERTE_1 = 1;
    //    
    //    LED_BLANCHE_2 = 1;
    //    LED_BLEUE_2 = 1;
    //    LED_ORANGE_2 = 1;
    //    LED_ROUGE_2 = 1;
    //    LED_VERTE_2 = 1;
    InitADC1();
    InitTimer23();
    InitTimer1();
    InitTimer4();
    InitPWM();
    //PWMSetSpeed(20,MOTEUR_DROIT);
    //PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
    /***************************************************************************************************/
    // Boucle Principale
    /***************************************************************************************************/
    while (1) {
        if (Bouton_1)
        {
            LED_BLANCHE_2 = 1;
        }
        else
            LED_BLANCHE_2 = 0;
        //        unsigned char ADCflag= ADCIsConversionFinished();
        //        unsigned int * result = ADCGetResult();
        //        if (ADCflag==1){
        //            ADCClearConversionFinishedFlag();
        //            unsigned int ADCValue0=result[0];
        //            unsigned int ADCValue1=result[1];
        //            unsigned int ADCValue2=result[2];
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();

            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreExtremeGauche = 34 / volts - 5;
            if (robotState.distanceTelemetreExtremeGauche >= 30)
                LED_BLANCHE_1 = 1;
            else
                LED_BLANCHE_1 = 0;

            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            if (robotState.distanceTelemetreGauche >= 30)
                LED_BLEUE_1 = 1;
            else
                LED_BLEUE_1 = 0;

            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            if (robotState.distanceTelemetreCentre >= 30)
                LED_ORANGE_1 = 1;
            else
                LED_ORANGE_1 = 0;

            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            if (robotState.distanceTelemetreDroit >= 30)
                LED_ROUGE_1 = 1;
            else
                LED_ROUGE_1 = 0;

            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreExtremeDroit = 34 / volts - 5;
            if (robotState.distanceTelemetreExtremeDroit >= 30)
                LED_VERTE_1 = 1;
            else
                LED_VERTE_1 = 0;
        }
    } // fin main
}

unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (Bouton_1) {
                //if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            }
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        default:
            stateRobot = STATE_ATTENTE;
            break;

        case STATE_TOURNE_EXTREME_GAUCHE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_EXTREME_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_EXTREME_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_EXTREME_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_EXTREME_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_EXTREME_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode(void) {
    unsigned char positionObstacle = PAS_D_OBSTACLE;
    //�Dtermination de la position des obstacles en fonction des ���tlmtres
    if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreGauche > 30) //Obstacle �droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreGauche < 30) //Obstacle �gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 40) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreGauche > 30 &&
            robotState.distanceTelemetreExtremeGauche > 30 &&
            robotState.distanceTelemetreExtremeDroit > 30) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;
    else if (robotState.distanceTelemetreGauche > 30 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreExtremeGauche < 30) //Obstacle �gauche
        positionObstacle = OBSTACLE_A_EXTREME_GAUCHE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreExtremeDroit < 30) //Obstacle �droite
        positionObstacle = OBSTACLE_A_EXTREME_DROIT;

    else if (robotState.distanceTelemetreExtremeDroit < 20 &&
            robotState.distanceTelemetreCentre > 40 &&
            robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreGauche > 30 &&
            robotState.distanceTelemetreExtremeDroit < 20) //Obstacle �droite
        positionObstacle = OBSTACLE_PORTE;
    

    //�Dtermination de l�?tat �venir du robot
    if (timestamp>=10000)
        nextStateRobot= STATE_ATTENTE;
    else if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_A_EXTREME_DROIT)
        nextStateRobot = STATE_TOURNE_EXTREME_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_EXTREME_GAUCHE)
        nextStateRobot = STATE_TOURNE_EXTREME_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_PORTE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    
    
    //Si l?on n?est pas dans la transition de l�?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}
