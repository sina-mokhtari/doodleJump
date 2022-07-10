//
// Created by Sina on 7/9/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "requisite.h"
#include "game.h"
#include "lcd.h"
#include "buzzer.h"
#include "LiquidCrystal.h"
#include "menu.h"
#include "uart.h"


void introKeypadHandle() {
    if (keypadNum == 0) {
//            if (programState == LoseState)
//                return;
        programState = MenuState;

        // if (osThreadGetState(updateLcdTskHandle) == osThreadBlocked) {
        menuStart();
        //   osThreadResume(updateLcdTskHandle);
        //}
    }
}


void introHandle() {
    if (!buzzerPlayingMelody)
        melodyQueueSend(MelodyIntro);

    osMutexAcquire(lcdMutexHandle, osWaitForever);

    setCursor(12, 0);
    print("  ");

    setCursor(5, 3);
    write(Monster);

    setCursor(12, 2);
    write(DoodlerUp);
    write(DoodlerDown);

    osDelay(300);

    setCursor(12, 2);
    print("  ");

    setCursor(12, 0);
    write(DoodlerUp);
    write(DoodlerDown);

    osDelay(300);

    osMutexRelease(lcdMutexHandle);

}