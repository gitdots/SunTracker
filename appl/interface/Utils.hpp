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

/*  1. Servomotors  */
#define SERVO_X 3
#define SERVO_Y 27

/*  2. LDRs */
#define LDR_UL 12
#define LDR_UR 16
#define LDR_DL 20
#define LDR_DR 21

/*  3. DHT22    */
#define DHT 18

/*  4. Strings  */
#define SM_PATH "/home/corona/shared/aaa/SunTracker/appl/source/sm.py"
#define DHT22_PATH "/home/corona/shared/aaa/SunTracker/appl/source/dht22.py"
#define SOCKET_NAME "th_data.socket"

#endif