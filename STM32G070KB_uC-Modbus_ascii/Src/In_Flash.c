/**@file        ln_Flash.c
* @brief        Flash模块的应用
* @details      适用于G0系列双字写入的单片机,写入读取Flash函数
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

#include "In_Flash.h"

static uint64_t Flash_Buf[FLASH_PAGE_SIZE / 8];    ///< 临时缓存


/**@brief       向内部Flash指定位置写入一字节数据
* @param[in]    RWAddr : 写入地址
* @param[in]    WrData : 写入数据
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 FLASH_END
*/
uint8_t InFlash_Write_OneByte(uint16_t RWAddr, uint8_t WrData)
{
    uint32_t WrAddr;
    uint16_t i;
    uint8_t *buf = (uint8_t *)Flash_Buf;
    HAL_StatusTypeDef Halstatus;

    if(IN_FLASH_END < RWAddr)
    {
        return OP_FAILED;
    }
    WrAddr = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    InFlash_Read_MultiBytes(WrAddr, buf, FLASH_PAGE_SIZE);
    buf[RWAddr % FLASH_PAGE_SIZE] = WrData;
    
    if(buf[RWAddr % FLASH_PAGE_SIZE] != 0xFF)
    {
        Halstatus = (HAL_StatusTypeDef)InFlash_Erase_Page(RWAddr, 1);
        if(HAL_OK != Halstatus)
            return OP_FAILED;
    }
   
    HAL_FLASH_Unlock();
    for(i = 0; i < FLASH_PAGE_SIZE; i += 8)
    {
        Halstatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 
                                        (WrAddr + i) + IN_FLASH_BASE_ADDRESS, (uint64_t)Flash_Buf[i / 8]);
        if(HAL_OK != Halstatus)
            break;
    }
    HAL_FLASH_Lock();

    if(HAL_OK != Halstatus)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}

/**@brief       向内部Flash指定位置读取一字节数据
* @param[in]    RWAddr : 读取地址
* @return       函数执行结果
* - 1个字节数据
* @note         地址不能越界,最大值为 FLASH_END
*/
uint8_t InFlash_Read_OneByte(uint16_t RWAddr)
{
  uint8_t RdData;
  uint32_t RealAddr;
  
  RdData = 0;
  RealAddr = IN_FLASH_BASE_ADDRESS + RWAddr;
  assert_param(IS_FLASH_PROGRAM_ADDRESS(RdAddr));
  RdData = *(__IO uint8_t *)RealAddr;
  return RdData;
}

/**@brief       向内部Flash指定位置写多个字节
* @param[in]    RWAddr : 写起始地址
* @param[in]    pWrbuf : 数据缓存指针
* @param[in]    Wrlen : 写数据长度
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 FLASH_END
*/
uint8_t InFlash_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen)
{
    uint16_t WrAddr;
    uint16_t i;
    uint8_t j;
    uint16_t remain;
    uint16_t off;
    uint8_t *wbuf = (uint8_t *)pWrbuf;
    uint8_t *flash_buf = (uint8_t *)Flash_Buf;
    HAL_StatusTypeDef Halstatus;

    if(IN_FLASH_END < RWAddr)
    {
        return OP_FAILED;
    }
    WrAddr = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    off = RWAddr % FLASH_PAGE_SIZE;
    remain = FLASH_PAGE_SIZE - off;
    if(Wrlen <= remain)
        remain = Wrlen;
    for(j = 0; j < (Wrlen / FLASH_PAGE_SIZE) + 1; j++)
    {        
        InFlash_Read_MultiBytes(WrAddr, flash_buf, FLASH_PAGE_SIZE);
        for(i = 0; i < remain; i++)
        {
            flash_buf[i + off] = wbuf[i];
        }
        
        Halstatus = (HAL_StatusTypeDef)InFlash_Erase_Page(WrAddr, 1);
        if(HAL_OK != Halstatus)
            break;
       
        HAL_FLASH_Unlock();                        
        for(i = 0; i < FLASH_PAGE_SIZE; i += 8)
        {
            Halstatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, 
                                        (WrAddr + i) + IN_FLASH_BASE_ADDRESS, (uint64_t)Flash_Buf[i / 8]);
            if(HAL_OK != Halstatus)
            {
                HAL_FLASH_Lock();
                goto __exit;
            }
        }
        HAL_FLASH_Lock();
     
        WrAddr += FLASH_PAGE_SIZE;
        off = 0;
        wbuf += remain;
        Wrlen -= remain;
        if(Wrlen > FLASH_PAGE_SIZE)
            remain = FLASH_PAGE_SIZE;
        else
            remain = Wrlen;
    }
__exit:
    if(HAL_OK != Halstatus)
    {
        return OP_FAILED;
    }

    return OP_SUCCESS;
}

/**@brief       从内部Flash指定位置读多个字节
* @param[in]    RWAddr : 读起始地址
* @param[in]    pRdbuf : 数据缓存指针
* @param[in]    Rdlen : 读数据长度
* @return       函数执行结果
* - None
* @note         地址不能越界,最大值为 FLASH_END
*/
void InFlash_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen)
{
  uint16_t FlashCnt;
  uint16_t RWAddress;
  
  RWAddress = RWAddr;
  for(FlashCnt = 0; FlashCnt < Rdlen; FlashCnt++)
  {
    *(pRdbuf + FlashCnt) = InFlash_Read_OneByte(RWAddress);
    
    RWAddress++;
  }
}

/**@brief       向内部Flash指定位置擦除页
* @param[in]    RWAddr : 擦除起始地址
* @param[in]    PageNb : 擦除页数目
* @return       函数执行结果
* - OP_SUCCESS(成功)
* - OP_FAILED(失败)
* @note         地址不能越界,最大值为 FLASH_END
*/
uint8_t InFlash_Erase_Page(uint16_t RWAddr, uint8_t PageNb)
{
    uint8_t sta = OP_SUCCESS;
    uint32_t EepAddress;
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;
	uint32_t Page;

    EepAddress = (RWAddr / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE;
    EepAddress += IN_FLASH_BASE_ADDRESS;
	Page = (EepAddress - FLASH_BASE) / FLASH_PAGE_SIZE;;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;

	EraseInitStruct.Page	= Page;
    EraseInitStruct.NbPages = PageNb;
    HAL_FLASH_Unlock();    
    sta = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    HAL_FLASH_Lock();    

    return sta;
}

