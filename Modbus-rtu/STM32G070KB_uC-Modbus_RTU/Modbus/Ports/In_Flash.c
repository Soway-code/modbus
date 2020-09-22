/**@file        ln_Flash.c
* @brief        Flashģ���Ӧ��
* @details      ������G0ϵ��˫��д��ĵ�Ƭ��,д���ȡFlash����
* @author       �����
* @date         2020-09-14
* @version      V1.0.0
* @copyright    2020-2030,��������Ϊ�Ƽ���չ���޹�˾
**********************************************************************************
* @par �޸���־:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2020/09/14  <td>1.0.0    <td>�����    <td>������ʼ�汾
* </table>
*
**********************************************************************************
*/

#include "In_Flash.h"

static uint64_t Flash_Buf[FLASH_PAGE_SIZE / 8];    ///< ��ʱ����


/**@brief       ���ڲ�Flashָ��λ��д��һ�ֽ�����
* @param[in]    RWAddr : д���ַ
* @param[in]    WrData : д������
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
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

/**@brief       ���ڲ�Flashָ��λ�ö�ȡһ�ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н��
* - 1���ֽ�����
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
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

/**@brief       ���ڲ�Flashָ��λ��д����ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : ���ݻ���ָ��
* @param[in]    Wrlen : д���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
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

/**@brief       ���ڲ�Flashָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pRdbuf : ���ݻ���ָ��
* @param[in]    Rdlen : �����ݳ���
* @return       ����ִ�н��
* - None
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
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

/**@brief       ���ڲ�Flashָ��λ�ò���ҳ
* @param[in]    RWAddr : ������ʼ��ַ
* @param[in]    PageNb : ����ҳ��Ŀ
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
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

