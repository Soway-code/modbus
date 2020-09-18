
/*
*********************************************************************************************************
*
*                         uC/MODBUS TARGET SPECIFIC DATA ACCESS FUNCTIONS 
*
* Filename      : mb_data.c
* Version       : V2.12
* Programmer(s) : JJL
*********************************************************************************************************
* Note(s)       :
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <mb.h>
#include "main.h"
/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
#define 	REG_HOLDING_START		0x0000		//保持寄存器起始地址
#define		REG_HOLDING_NREGS		125				//保持寄存器数量

volatile uint16_t MB_DATA[125] = {0};
volatile uint16_t* Ptr = &MB_DATA[0];

volatile uint16_t MB_DATA_04[125] = {0x0,0xBE2,0x0,0x12C,0x4,0x5,0x6,0x7,0x8,0x9,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21,0x23};
volatile uint16_t* Ptr_04 = &MB_DATA_04[0];
	
/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/

#if (MODBUS_CFG_FC01_EN == DEF_ENABLED)
CPU_BOOLEAN
MB_CoilRd (CPU_INT16U   coil, CPU_INT16U  *perr)
{
	*perr = MODBUS_ERR_RANGE;
   return (DEF_FALSE);
}
#endif

#if (MODBUS_CFG_FC05_EN == DEF_ENABLED) || \
    (MODBUS_CFG_FC15_EN == DEF_ENABLED)
void
MB_CoilWr (CPU_INT16U coil, CPU_BOOLEAN coil_val, CPU_INT16U *perr)
{
	*perr = MODBUS_ERR_RANGE;
}
#endif

#if (MODBUS_CFG_FC02_EN == DEF_ENABLED)
CPU_BOOLEAN
MB_DIRd (CPU_INT16U  di, CPU_INT16U  *perr)
{
	*perr = MODBUS_ERR_RANGE;
	return DEF_FALSE;
}
#endif

#if (MODBUS_CFG_FC04_EN == DEF_ENABLED)
CPU_INT16U
MB_InRegRd (CPU_INT16U reg, CPU_INT16U *perr)
{
   CPU_INT16U  val;
	reg &= 0x00FF; 
	if ( reg <= REG_HOLDING_START+REG_HOLDING_NREGS-1 )	//有效地址
	{
		*perr = MODBUS_ERR_NONE;
		val = *(Ptr_04+reg);
	}
	else 
	{
		*perr = MODBUS_ERR_RANGE;
		val = 0;
	}
	return (val);
}
#endif

#if (MODBUS_CFG_FP_EN   == DEF_ENABLED)
#if (MODBUS_CFG_FC04_EN == DEF_ENABLED)
CPU_FP32
MB_InRegRdFP (CPU_INT16U reg, CPU_INT16U *perr)
{
	*perr = MODBUS_ERR_RANGE;
	return ((CPU_FP32)0);
}
#endif
#endif

#if (MODBUS_CFG_FC03_EN == DEF_ENABLED)
CPU_INT16U
MB_HoldingRegRd (CPU_INT16U reg, CPU_INT16U  *perr)
{
	CPU_INT16U  val;
	reg = reg & 0x00FF;
	if ( reg <= REG_HOLDING_START+REG_HOLDING_NREGS-1 )	//有效地址
	{
		*perr = MODBUS_ERR_NONE;
		val = *(Ptr+reg);
	}
	else 
	{
		*perr = MODBUS_ERR_RANGE;
		val = 0;
	}
	return (val);
}
#endif

#if (MODBUS_CFG_FP_EN   == DEF_ENABLED)
#if (MODBUS_CFG_FC03_EN == DEF_ENABLED)
CPU_FP32
MB_HoldingRegRdFP (CPU_INT16U reg, CPU_INT16U *perr)
{
	*perr = MODBUS_ERR_RANGE;
	return ((CPU_FP32)0);
}
#endif
#endif

#if (MODBUS_CFG_FC06_EN == DEF_ENABLED) || \
    (MODBUS_CFG_FC16_EN == DEF_ENABLED)
void
MB_HoldingRegWr (CPU_INT16U  reg, CPU_INT16U  reg_val_16, CPU_INT16U  *perr)
{
	uint8_t buf;
	reg = reg & 0x00FF;
	if ( reg < REG_HOLDING_START+REG_HOLDING_NREGS )		//有效地址
	{
		*perr = MODBUS_ERR_NONE;
		*(Ptr+reg) = reg_val_16;
		buf = (uint8_t)reg_val_16;
		switch (reg)
        {
        	case 0x30:{
			InFlash_Write_OneByte(DEVICE_ADDR,buf);			//设备地址
			//HAL_NVIC_SystemReset();
			break;
			}
        		
        	case 0x31:{
				InFlash_Write_OneByte(BAUDRATE,buf);		//串口波特率
			//	HAL_NVIC_SystemReset();
			break;				
			}
			
			case 0x32:{
				InFlash_Write_OneByte(PARITY,buf);			//串口奇偶校验    
			//	HAL_NVIC_SystemReset();
			break;				
			}
			
			case 0x34:{
				InFlash_Write_OneByte(COMPENSATE,buf);		// 补偿使能
				break;				
			}
			
			case 0x35:{
				InFlash_Write_OneByte(FILTER,buf);			// 滤波系数
				break;				
			}
			
			case 0x36:{
				InFlash_Write_OneByte(AUTO_UPLOAD,buf);			//自动上传周期
				break;				
			}
        		
			case 0x37:{
				InFlash_Write_OneByte(CORRECT_K,buf);			//修正系数K
				break;				
			}
			
			case 0x38:{
				InFlash_Write_OneByte(CORRECT_B,buf);			//修正系数B
				break;				
			}
			
			case 0x40:{
				InFlash_Write_OneByte(UPPERTEMP,buf);			//温度上阀值
				break;				
			}
			
			case 0x41:{
				InFlash_Write_OneByte(LOWERTEMP,buf);			//温度下阀值
				break;				
			}
			
        	default:*perr = MODBUS_ERR_RANGE;
        		break;
        }
	
//		MB_ASCII_Tx(pch);                     /* Send back reply.                                                */
//
	//	HAL_NVIC_SystemReset();
		
		
	}
	else 
	{
		*perr = MODBUS_ERR_RANGE;
	}
}
#endif

#if (MODBUS_CFG_FP_EN    == DEF_ENABLED)
#if (MODBUS_CFG_FC06_EN == DEF_ENABLED) || \
    (MODBUS_CFG_FC16_EN == DEF_ENABLED)
void
MB_HoldingRegWrFP (CPU_INT16U reg, CPU_FP32 reg_val_fp, CPU_INT16U *perr)
{
	*perr = MODBUS_ERR_RANGE;
}
#endif
#endif

#if (MODBUS_CFG_FC20_EN == DEF_ENABLED)
CPU_INT16U
MB_FileRd (CPU_INT16U  file_nbr, 
           CPU_INT16U  record_nbr, 
           CPU_INT16U  ix, 
           CPU_INT08U  record_len, 
           CPU_INT16U  *perr)
{
    (void)file_nbr;
    (void)record_nbr;
    (void)ix;
    (void)record_len;
    *perr  = MODBUS_ERR_NONE;
    return (0);
}
#endif

#if (MODBUS_CFG_FC21_EN == DEF_ENABLED)
void
MB_FileWr (CPU_INT16U   file_nbr, 
					 CPU_INT16U   record_nbr, 
					 CPU_INT16U   ix, 
					 CPU_INT08U   record_len, 
					 CPU_INT16U   val, 
					 CPU_INT16U  *perr)
{
    (void)file_nbr;
    (void)record_nbr;
    (void)ix;
    (void)record_len;
    (void)val;
    *perr = MODBUS_ERR_NONE;
}
#endif
