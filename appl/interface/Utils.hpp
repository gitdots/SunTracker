#ifndef UTILS_HPP
#define UTILS_HPP

/*  GPIOs   */

/*  0. SPI - MCP3008    */
/* 
 *  code uses the SPI0 interface with the assigned CS0
 *
 * SPI0_MOSI - GPIO18
 * SPI0_MISO - GPIO9
 * SPI0_SCLK - GPIO11
 * SPI0_CS0  - GPIO8 
*/

#define MCP_SPI_MODE 0
#define MCP_SPI_BPW 8
#define MCP_SPI_SPEED 10000
#define MCP_SPI_CSLOW 0
#define MCP_NO_CH 8
#define MCP_VREF 3.3

/*  1. Servomotors  */
#define SERVO_0X 19
#define SERVO_0Y 4

/*  2. LDRs & Solar Panel*/
#define LDR_UL_CH 0
#define LDR_UR_CH 1
#define LDR_DL_CH 2
#define LDR_DR_CH 3
#define SOLAR_PANEL_CH 7

/*  3. DHT22    */
#define DHT_PIN 18

/*  4. Strings  */
#define PY_BRIDGE "/home/corona/shared/aaa/SunTracker/appl/source/py_bridge.py"
#define SOCKET_NAME "python_bridge.socket"
#define APPL2FLASK_SOCKET_PATH "/home/corona/shared/aaa/SunTracker/webserver/appl2flask.socket"

/*  5. Physical offsets */
#define OX_DELTA 0.0
#define OY_DELTA 0.0

/*  6. Time constants   */
#define TIME_NEW_DATA  5    // represents the period of time for new data aquisition from the senzors (in seconds)
#define TIME_DATABASE 15    // represents the period of time for new data to be pushed into the database (in minutes)


#define INA219_I2C_DEVICE "/dev/i2c-1"
#define INA219_ADDRESS 0x40
#define INA219_REG_CONFIG 0x05
#define INA219_REG_SHUNTVOLTAGE 0x01
#define INA219_REG_BUSVOLTAGE 0x02
#define INA219_CONFIG_BVOLTAGERANGE_16V 0x2000 
#define INA219_CONFIG_GAIN_8_320MV 0x1800
#define INA219_CONFIG_BADCRES_12BIT 0x0800
#define INA219_CONFIG_SADCRES_12BIT_1S_532US 0x0080
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS 0x0007
#endif