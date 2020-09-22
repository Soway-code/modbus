
#ifndef 	_MBAPP_H
#define   _MBAPP_H

#include "mb.h"

#if (MODBUS_CFG_SLAVE_EN == DEF_ENABLED)
void  MB_OS_RxTask (void);
#endif


#endif
