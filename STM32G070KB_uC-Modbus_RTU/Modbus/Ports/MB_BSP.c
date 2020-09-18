/**@file        MB_BSP.c
* @brief        串口和定时器模块的应用
* @details      声明了串口应用的API函数 串口初始化和定时器初始化函数，
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

#include "MB_BSP.h"

#define 	COM1		USART1 

extern volatile uint16_t* Ptr;
uint8_t aRxBuffer[1];	

UART_HandleTypeDef huart1;
TIM_HandleTypeDef htim3;

/**@brief       读取Flash信息和串口初始化
* @param[in]    无 
* @return       无
*/
void  ModBus_Init(void)
{
	uint8_t Device_Addr,BaudRate,Parity;
	
	Device_Addr = InFlash_Read_OneByte(DEVICE_ADDR);			//设备地址
    if(0xFF == Device_Addr)
		Device_Addr = 0X01;
	Ptr[48] = Device_Addr;
	
	BaudRate = InFlash_Read_OneByte(BAUDRATE);  				//串口波特率
	Ptr[49] = BaudRate;

	Parity = InFlash_Read_OneByte(PARITY);  				 	//串口奇偶校验      
    Ptr[50] = Parity;
	
	Ptr[54] = InFlash_Read_OneByte(AUTO_UPLOAD);  				//自动上传周期
	
	InFlash_Read_OneByte(OUTPUTMODE);  							//输出模式

	Ptr[52] = InFlash_Read_OneByte(COMPENSATE);					// 补偿使能
	
	Ptr[53] = InFlash_Read_OneByte(FILTER);						// 滤波系数
	
	Ptr[55] = InFlash_Read_OneByte(CORRECT_K);  				//修正系数K
	
	Ptr[56] = InFlash_Read_OneByte(CORRECT_B);  				//修正系数B
	
	Ptr[64] = InFlash_Read_OneByte(UPPERTEMP);  				//温度上阀值
	
	Ptr[65] = InFlash_Read_OneByte(LOWERTEMP);  				//温度下阀值
	
	MB_Init(1000);
	MB_CfgCh(Device_Addr,MODBUS_SLAVE,MODBUS_MODE_RTU,1,BaudRate,8,Parity,1,MODBUS_WR_EN);
}

/**@brief       串口初始化
* @param[in]    MODBUS_CH *pch: 指向要配置的通信通道的指针
* @param[in] 	CPU_INT08U port_nbr：使用的串口物理号
* @param[in] 	CPU_INT32U baud：波特率
* @param[in] 	CPU_INT08U bits：位数
* @param[in] 	CPU_INT08U Parity：校验方式
* @param[in] 	CPU_INT08U stops：停止位数
* @note      
*/
void MB_CommPortCfg (	MODBUS_CH *pch, 			// a pointer to the communication channel to configure;
						CPU_INT08U port_nbr, 		// 使用的串口物理号
						CPU_INT32U baud, 			// 波特率
						CPU_INT08U bits, 			// 位数
						CPU_INT08U Parity, 			// 校验方式
						CPU_INT08U stops )			// 停止位数
{
	uint32_t baudrate;
    uint32_t parity;
	__HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
	
	 __HAL_RCC_USART1_CLK_DISABLE();
	
	pch->PortNbr        = port_nbr;			// Store configuration in channel
	pch->BaudRate       = baud;
	pch->Parity         = Parity;
	pch->Bits           = bits;
	pch->Stops          = stops;
	
	switch(baud)
    {
        case USART_BAUDRATE_4800_CODE : baudrate = 4800;
        break;
        
        case USART_BAUDRATE_9600_CODE : baudrate = 9600;
        break;
        
        case USART_BAUDRATE_19200_CODE : baudrate = 19200;
        break;
        
        case USART_BAUDRATE_38400_CODE : baudrate = 38400;
        break;
        
        case USART_BAUDRATE_57600_CODE : baudrate = 57600;
        break;
        
        case USART_BAUDRATE_115200_CODE : baudrate = 115200;
        break;
        
        default : baudrate = DEFAULT_UART_BAUD;
        break;
    }
    switch(Parity)
    {
        case USART_PARITY_NONE_CODE : parity = UART_PARITY_NONE;
        break;
        
        case USART_PARITY_ODD_CODE : parity = UART_PARITY_ODD;
        break;
        
        case USART_PARITY_EVEN_CODE : parity = UART_PARITY_EVEN;
        break;
        
        default : parity = UART_PARITY_NONE;
        break;
    }
	
	huart1.Instance = USART1;
	huart1.Init.BaudRate = baudrate;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = parity;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
	Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
	{
	Error_Handler();
	}

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
	
  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 


/*
*********************************************************************************************************
*                                       MB_CommRxTxISR_Handler()
*
* Description : This function is the ISR for either a received or transmitted character.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is a ISR
*
* Note(s)     : (1) The pseudo-code for this function should be:  
*
*               if (Rx Byte has been received) {
*                  c = get byte from serial port;
*                  Clear receive interrupt;
*                  pch->RxCtr++;                      Increment the number of bytes received
*                  MB_RxByte(pch, c);                 Pass character to Modbus to process
*              }
*
*              if (Byte has been transmitted) {
*                  pch->TxCtr++;                      Increment the number of bytes transmitted
*                  MB_TxByte(pch);                    Send next byte in response
*                  Clear transmit interrupt           Clear Transmit Interrupt flag
*              }
*********************************************************************************************************
*/

//void MB_CommRxTxISR_1_Handler (void) 
//{ 
//	CPU_INT08U c; 
//	CPU_INT08U ch; 
//	MODBUS_CH *pch; 
//	
//	pch = &MB_ChTbl[0];
//	for (ch = 0; ch < MODBUS_CFG_MAX_CH; ch++) 
//	{ 
//		if ( pch->PortNbr == 1 ) 
//		{ 

//				c = aRxBuffer[0];
//				pch->RxCtr++; 
//				MB_RxByte(pch, c); 	// Pass byte to Modbus to process 
//				printf("2\n");
//			if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET ) 
//			{ 
//				c = USART_ReceiveData(USART1); 
//				USART_ClearFlag(USAR 1, USART_FLAG_RXNE);
//				pch->RxCtr++; 
//				MB_RxByte(pch, c); 	// Pass byte to Modbus to process 
//			} 
//			if ( USART_GetFlagStatus(USART1, USART_FLAG_TC) == SET ) 
//			{ 
//				pch->TxCtr++; 
//				MB_TxByte(pch); 		// Send next byte in response 
//				USART_ClearFlag(USART1, USART_FLAG_TC); 
//			} 
//			break; 
//		} 
//		else 
//		{ 
//			pch++; 
//		} 
		//HAL_UART_Receive_IT(&huart1,aRxBuffer,1);		// 重新使能串口1接收中断
//	}
//}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	MODBUS_CH *pch; 
	pch = &MB_ChTbl[0];
	
	if(huart == &huart1)
	{
		pch->RxCtr++; 
		MB_RxByte(pch, aRxBuffer[0]); 
	}
	if((aRxBuffer[0] == 0x0a) || (aRxBuffer[0] == 0x0d))
	{		
		pch->TxCtr++; 
		MB_TxByte(pch); 							// Send next byte in response 
	}
	
	HAL_UART_Receive_IT(&huart1,aRxBuffer,1);		// 重新使能串口1接收中断

}

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
} 

void MB_CommRxTxISR_2_Handler (void) 
{ 

}


/*
*********************************************************************************************************
*                                             MB_CommTx1()
*
* Description : This function is called to obtain the next byte to send from the transmit buffer.  When
*               all bytes in the reply have been sent, transmit interrupts are disabled and the receiver
*               is enabled to accept the next Modbus request.
*
* Argument(s) : c     is the byte to send to the serial port
*
* Return(s)   : none.
*
* Caller(s)   : MB_TxByte()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void MB_CommTx1 (	MODBUS_CH  *pch, 
									CPU_INT08U c	) 
{ 
	HAL_UART_Transmit(&huart1,&c,1,100);
}

/*
*********************************************************************************************************
*                                         MB_CommTxIntEn()
*
* Description : This function enables Tx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_Tx()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void MB_CommTxIntEn(MODBUS_CH *pch) 
{ 
	HAL_UART_Receive_IT(&huart1,aRxBuffer,1);		// 重新使能串口1接收中断
}
void MB_CommRxIntEn(MODBUS_CH *pch) 
{ 
	__HAL_USART_ENABLE_IT(&huart1, USART_IT_RXNE);
}

/*
*********************************************************************************************************
*                                         MB_CommTxIntDis()
*
* Description : This function disables Tx interrupts.
*
* Argument(s) : pch        is a pointer to the Modbus channel
*
* Return(s)   : none.
*
* Caller(s)   : MB_CommExit()
*               MB_TxByte()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void MB_CommTxIntDis(MODBUS_CH *pch) 
{ 
	__HAL_USART_DISABLE_IT(&huart1, USART_IT_TC);
}

void MB_CommRxIntDis(MODBUS_CH *pch) 
{ 
	__HAL_USART_DISABLE_IT(&huart1, USART_IT_RXNE);

}

/*
*********************************************************************************************************
*                                           MB_RTU_TmrInit()
*
* Description : This function is called to initialize the RTU timeout timer.
*
* Argument(s) : freq          Is the frequency of the modbus RTU timer interrupt.
*
* Return(s)   : none.
*
* Caller(s)   : MB_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
void  MB_RTU_TmrInit (void)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = (uint16_t) MB_RTU_Freq;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 64;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }


}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
} 

#endif
/*
*********************************************************************************************************
*                                           MB_RTU_TmrExit()
*
* Description : This function is called to disable the RTU timeout timer.
*
* Argument(s) : none.
* 
* Return(s)   : none.
*
* Caller(s)   : MB_Exit()
*
* Note(s)     : none.
*********************************************************************************************************
*/
#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
void  MB_RTU_TmrExit (void)
{
//	TIM_Cmd(TIM4, DISABLE);
//	TIM_SetCounter(TIM4,0x0000); 
//	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
//	TIM_ClearITPen dingBit(TIM4, TIM_IT_Update);
}
#endif
/*
*********************************************************************************************************
*                                       MB_RTU_TmrISR_Handler()
*
* Description : This function handles the case when the RTU timeout timer expires.
*
* Arguments   : none.
*
* Returns     : none.
*
* Caller(s)   : This is a ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/
#if (MODBUS_CFG_RTU_EN == DEF_ENABLED)
void  MB_RTU_TmrISR_Handler (void)
{
	MB_RTU_TmrCtr++;         // Indicate that we had activities on this interrupt
	MB_RTU_TmrUpdate();      // Check for RTU timers that have expired
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//添加回调后的程序逻辑　
    if (htim->Instance == htim3.Instance) //判断是否定时器2
    {
		
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4);
		MB_RTU_TmrISR_Handler();
    }
}
#endif
