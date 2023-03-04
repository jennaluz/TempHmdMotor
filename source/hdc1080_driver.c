/*
 * SensorMotor/include/private/hdc1080_driver.h
 */

#include <hardware/i2c.h>
#include <math.h>
#include <pico/binary_info.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "hdc1080_driver.h"

const uint8_t HDC_ADDR          = 0x40;
const uint8_t HDC_TMP_REG       = 0x00;
const uint8_t HDC_HMD_REG       = 0x01;
const uint8_t HDC_CONFIG_REG    = 0x02;

/*
 * Initialize I2C port, and SDA and SCL pins.
 */
void vHDC1080Init()
{
    // initialize default port on default SDA and SCL pins
    i2c_init(PICO_DEFAULT_I2C_INSTANCE, 100 * 1000);    // put I2C hardware into known state and enable

    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // make I2C pins available to picotool
    //bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    return;
}

/*
 *
 */
void vHDC1080ReadTmp()
{
    uint8_t uiTmpValue[2] = {};
    int8_t iReturn = 0;
    uint16_t uiReturnValue = 0;
    float fFahrenheit = 0;

    // point to temperature register
    iReturn = i2c_write_blocking(PICO_DEFAULT_I2C_INSTANCE, HDC_ADDR, &HDC_TMP_REG, 1, false);
    sleep_ms(25);

    // read from temperature register
    iReturn = i2c_read_blocking(PICO_DEFAULT_I2C_INSTANCE, HDC_ADDR, uiTmpValue, 2, false);
    uiReturnValue = (uiTmpValue[0] << 8) | uiTmpValue[1];

    // convert return value to fahrenheit
    fFahrenheit = (((uiReturnValue / pow(2, 16)) * 165 - 40) * 9 / 5) + 32;
    //printf("%f degrees F\n", fFahrenheit);

    return;
}

/*
 *
 */
void vHDC1080ReadHmd()
{
  uint8_t uiHmdValue[2] = {};
  int8_t iReturn = 0;
  uint16_t uiReturnValue = 0;
  float fRelativeHumdity = 0;

  // point to humidity register
  iReturn = i2c_write_blocking(PICO_DEFAULT_I2C_INSTANCE, HDC_ADDR, &HDC_HMD_REG, 1, false);
  sleep_ms(25);

  // read from humidity register
  iReturn = i2c_read_blocking(PICO_DEFAULT_I2C_INSTANCE, HDC_ADDR, uiHmdValue, 2, false);
  uiReturnValue = (uiHmdValue[0] << 8) | uiHmdValue[1];

  // convert return value to relative humidity
  fRelativeHumdity = (uiReturnValue / pow(2, 16)) * 100;
  //printf("RH %f\n", fRelativeHumdity);

  return;
}
