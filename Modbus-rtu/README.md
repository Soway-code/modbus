#                         uC-Modbus

####                                                                  -----代码维护人：马灿林


通过修改以下这个函数modbus_mode参数来选择Modbus通信模式
	MODBUS_CH  *MB_CfgCh (CPU_INT08U  node_addr,	
                      CPU_INT08U  master_slave,
                      CPU_INT08U  modbus_mode,
                      CPU_INT08U  port_nbr,
                      CPU_INT32U  baud,
                      CPU_INT08U  bits,
                      CPU_INT08U  parity,
                      CPU_INT08U  stops,
                      CPU_INT08U  wr_en)
					  
	默认配置：Device_Addr(设备地址 01)
			  RTU模式
			  9600波特率
			  校验方式：None	  
	MB_CfgCh(Device_Addr,MODBUS_SLAVE,MODBUS_MODE_RTU,1,BaudRate,8,Parity,1,MODBUS_WR_EN);
	
移植时拷贝整个uC-Modbus文件夹既可，使用时需要初始化ModBus调用ModBus_Init();。
MB_OS_RxTask();这个函数需要放到一个任务在循环调用。

目前ln_Flash.c适用于Flash双字写入的stm32,这个Flash文件和设备参数在内部Flash地址需要根据使用的产品来修改，
ln_Flash.c设备参数在内部Flash中的映射地址参考于soway3.32.3上位机中的温度传感器，
​                