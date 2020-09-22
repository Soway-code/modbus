/**@file        MB_BSP.h
* @details      MB_BSP的头文件,定义了串口宏定义,声明了串口初始化应用的API函数
* @author       马灿林
* @date         2020-09-14
* @version      V1.0.0
* @copyright    2020-2030,深圳市信为科技发展有限公司
**********************************************************************************
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/09/14  <td>1.0.0    <td>马灿林    <td>创建初始版本
* </table>
*
**********************************************************************************
*/

#ifndef __MB_BSP_H
#define __MB_BSP_H
#ifdef __cplusplus
 extern "C" {
#endif
#include "main.h"
#include "cpu.h"
#include "mb.h"
#include "In_Flash.h"

#include "stdio.h"

#define USART_BAUDRATE_4800_CODE        2   ///< 波特率代码,代表4800 bit/s
#define USART_BAUDRATE_9600_CODE        3   ///< 波特率代码,代表9600 bit/s
#define USART_BAUDRATE_19200_CODE       4   ///< 波特率代码,代表19200 bit/s
#define USART_BAUDRATE_38400_CODE       5   ///< 波特率代码,代表38400 bit/s
#define USART_BAUDRATE_57600_CODE       6   ///< 波特率代码,代表57600 bit/s
#define USART_BAUDRATE_115200_CODE      7   ///< 波特率代码,代表115200 bit/s

#define USART_PARITY_NONE_CODE          3   ///< 奇偶校验代码,代表无奇偶校验
#define USART_PARITY_ODD_CODE           1   ///< 奇偶校验代码,代表奇校验
#define USART_PARITY_EVEN_CODE          2   ///< 奇偶校验代码,代表偶校验

/** 默认波特率 9600 bit/s */
#define DEFAULT_UART_BAUD               9600   

/**@brief       读取Flash信息和串口初始化
* @param[in]    无 
* @return       无
*/
void  ModBus_Init(void);

#endif
