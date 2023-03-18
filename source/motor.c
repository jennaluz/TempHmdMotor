/*
 * SensorMotor/source/motor.c
 *
 * Controls the Stepper Motor status based on motor_queue values.
 */


#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdlib.h>
#include <stdio.h>

#include "error.h"
#include "motor.h"
#include "motor_driver.h"
#include "sensor.h"
#include "system_code.h"


QueueHandle_t motor_queue = NULL;


/*
 * Reads in information from motor_queue to control the status of the queue.
 * The status of the queue is defined by the enumeratored eStatus varaible.
 */
void motor_handle()
{
    system_code_e motor_code = MOTOR_CLOCKWISE;
    system_code_e motor_status = motor_code;
    int old_tmp = 70;
    int new_tmp = 0;
    int old_hmd = 45;
    int new_hmd = 0;

    while (true) {
        xQueuePeek(motor_queue, &motor_code, 0);

        switch (motor_code) {
            case MOTOR_CLOCKWISE:
                vMotorClockwise();
                break;
            case MOTOR_COUNTERCLOCKWISE:
                vMotorCounterclockwise();
                break;
            case MOTOR_ALTERNATE:
                vMotorAlternate();
                break;
            case MOTOR_TEMPERATURE:
                xQueuePeek(xTemperatureQueue, &new_tmp, 0);

                if (new_tmp == old_tmp) {
                    vMotorHalt();
                } else {
                    if (new_tmp > old_tmp) {
                        vMotorIncrement();
                    } else {
                        vMotorDecrement();
                    }
                }

                old_tmp = new_tmp;
                break;
            case MOTOR_HUMIDITY:
                xQueuePeek(xHumidityQueue, &new_hmd, 0);

                if (new_hmd == old_hmd) {
                    vMotorHalt();
                } else {
                    if (new_hmd > old_hmd) {
                        vMotorIncrement();
                    } else {
                        vMotorDecrement();
                    }
                }

                old_hmd = new_hmd;
                break;
            case MOTOR_HALT:
                vMotorHalt();
                break;
            case MOTOR_STATUS:
                break;
            default:
                vError(ERROR_UNKNOWN_INPUT);
        }

        //vTaskDelay(100);
    }
}
