/*
*********************************************************************************************************
*
*                                     MODBUS NO_OS LAYER INTERFACE
* 
* Filename      : mb_app.c
* Version       : V1.0
* Programmer(s) : YW
*********************************************************************************************************
* Note(s)       :
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "mb_app.h"
///#include "stm32f10x.h"
#include "main.h"

/*
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

volatile CPU_BOOLEAN Queue[MODBUS_CFG_MAX_CH];


/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/
#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
static  void                 MB_InitMaster  (void);
static  void                 MB_ExitMaster  (void);
#endif

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
static  void                 MB_InitSlave   (void);
static  void                 MB_ExitSlave   (void);
#endif

/*
*********************************************************************************************************
*                                          MB_***()
*
* Description : This function initializes and creates the kernel objectes needed for Modbus Salve
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : ***
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  MB_OS_Init (void)
{
	#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
			MB_InitMaster();
	#endif

	#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
			MB_InitSlave();
	#endif
}

void  MB_OS_Exit (void)
{
	#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
			MB_ExitMaster();
	#endif

	#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
			MB_ExitSlave();
	#endif
}

#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
static  void  MB_InitMaster (void)
{    

}
#endif

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
static  void  MB_InitSlave (void)
{ 
	
}
#endif

#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
static  void  MB_ExitMaster (void)
{

}
#endif

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
void  MB_ExitSlave (void)
{
                 
}
#endif

void  MB_OS_RxSignal (MODBUS_CH *pch)
{
	if (pch != (MODBUS_CH *)0) 
	{
		switch (pch->MasterSlave) 
		{
			#if (MODBUS_CFG_MASTER_EN == DEF_ENABLED)
      case MODBUS_MASTER:

      break;
			#endif

			#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
      case MODBUS_SLAVE:
      default:
							Queue[pch->Ch] = 1;	//收到完整的帧
      break;
			#endif
		}
	}
}

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
void  MB_OS_RxTask (void)
{
	CPU_INT08U 	ch=0;
	MODBUS_CH   *pch;
	
//	for (ch = 0; ch < MODBUS_CFG_MAX_CH; ch++)
//	{
		if ( Queue[0] )
		{
			Queue[ch] = 0;
			pch = &MB_ChTbl[ch];
			MB_RxTask(pch);
		}
//	}
}
#endif
