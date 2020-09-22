/**@file        ln_Flash.h
* @details      Flash.c��ͷ�ļ�,������Flash��ַ�궨��,������FlashӦ�õ�API����
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
#ifndef __IN_FLASH_H
#define __IN_FLASH_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

#define  OP_SUCCESS                	 	0x00                                	//�����ɹ�
#define  OP_FAILED                  	0x01                                	//����ʧ��

#define IN_FLASH_BASE_ADDRESS           0x0801E800                          	//����Flash����ַ

#define IN_FLASH_START                  0x0000                             	 	//Flash��ʼ��ַ
#define IN_FLASH_END                    0x17FF                              	//Flash������ַ

#define IN_FLASH_WR_ENABLE              0x0F                                	//Flashд����ʹ��
#define IN_FLASH_WR_DISABLE             0x00                                	//Flashд������ֹ
	
#define WRITE_FLASH_ERR_MAX             0x03                               		//дFlash����������
	

/***************************** �豸�������ڲ�Flash�е�ӳ���ַ ****************************/
#define RUN_ADDR_BASE                     0x00                                  //ϵͳ������ʼ����־/�ڲ�Flash��ʼ��ַ
#define DEVICE_ADDR                       (RUN_ADDR_BASE + 0x01)                //�豸��ַ
#define BAUDRATE                          (DEVICE_ADDR   + 0x01)                //������
#define PARITY                            (BAUDRATE      + 0x01)                //��żУ��
#define FILTER                            (PARITY        + 0x01)                //�˲�ϵ��
#define AUTO_UPLOAD                       (FILTER        + 0x01)                //�Զ��ϴ�����
#define COMPENSATE                        (AUTO_UPLOAD   + 0x01)                //����ʹ��
#define FREEZE                            (COMPENSATE    + 0x01)                //�Ƿ񶳽��豸
#define OUTPUTMODE                        (FREEZE        + 0x01)  				//�����ʽ

#define CORRECT_K                         (OUTPUTMODE    + 0x02)               	//����ϵ��K
#define CORRECT_B                         (CORRECT_K     + 0x02)               	//����ϵ��B
#define UPPERTEMP						  (CORRECT_B     + 0x02)				//�¶��Ϸ�ֵ
#define LOWERTEMP						  (UPPERTEMP     + 0x02)				//�¶��·�ֵ

#define ORGANIZATION                       0x0400                               ///< ��֯��������
#define PRODUCTION                        (ORGANIZATION  + 0x30)                ///< ��Ʒ����
#define HARDWAREVER                       (PRODUCTION    + 0x30)                ///< Ӳ���汾
#define SOFTWAREVER                       (HARDWAREVER   + 0x30)                ///< ����汾
#define DEVICENUM                         (SOFTWAREVER   + 0x20)                ///< �豸ID
#define CUSTOMER                          (DEVICENUM     + 0x30)                ///< �ͻ�����

/**@brief       ���ڲ�Flashָ��λ��д��һ�ֽ�����
* @param[in]    RWAddr : д���ַ
* @param[in]    WrData : д������
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
uint8_t InFlash_Write_OneByte(uint16_t RWAddr, uint8_t WrData);

/**@brief       ���ڲ�Flashָ��λ�ö�ȡһ�ֽ�����
* @param[in]    RWAddr : ��ȡ��ַ
* @return       ����ִ�н��
* - 1���ֽ�����
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
uint8_t InFlash_Read_OneByte(uint16_t RWAddr);

/**@brief       ���ڲ�Flashָ��λ��д����ֽ�
* @param[in]    RWAddr : д��ʼ��ַ
* @param[in]    pWrbuf : ���ݻ���ָ��
* @param[in]    Wrlen : д���ݳ���
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
uint8_t InFlash_Write_MultiBytes(uint16_t RWAddr, uint8_t const *pWrbuf, uint16_t Wrlen);

/**@brief       ���ڲ�Flashָ��λ�ö�����ֽ�
* @param[in]    RWAddr : ����ʼ��ַ
* @param[in]    pRdbuf : ���ݻ���ָ��
* @param[in]    Rdlen : �����ݳ���
* @return       ����ִ�н��
* - None
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
void InFlash_Read_MultiBytes(uint16_t RWAddr, uint8_t *pRdbuf, uint16_t Rdlen);

/**@brief       ���ڲ�Flashָ��λ�ò���ҳ
* @param[in]    RWAddr : ������ʼ��ַ
* @param[in]    PageNb : ����ҳ��Ŀ
* @return       ����ִ�н��
* - OP_SUCCESS(�ɹ�)
* - OP_FAILED(ʧ��)
* @note         ��ַ����Խ��,���ֵΪ FLASH_END
*/
uint8_t InFlash_Erase_Page(uint16_t RWAddr, uint8_t PageNb);
  
#ifdef __cplusplus
}
#endif
#endif

