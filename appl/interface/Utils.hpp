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

/*  1. Servomotors  */
#define SERVO_0X 19
#define SERVO_0Y 3

/*  2. LDRs */
#define LDR_UL_CH 0
#define LDR_UR_CH 1
#define LDR_DL_CH 2
#define LDR_DR_CH 3

/*  3. DHT22    */
#define DHT 18

/*  4. Strings  */
#define SM_PATH "/home/corona/shared/aaa/SunTracker/appl/source/sm.py"
#define DHT22_PATH "/home/corona/shared/aaa/SunTracker/appl/source/dht22.py"
#define SOCKET_NAME "th_data.socket"

#endif