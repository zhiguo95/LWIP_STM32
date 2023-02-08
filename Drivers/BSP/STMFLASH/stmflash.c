/**
 ****************************************************************************************************
 * @file        stmflash.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-28
 * @brief       STM32�ڲ�FLASH��д ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨������ԭ�� F407���������
 * ������Ƶ��www.yuanzige.com
 * ������̳��http://www.openedv.com/forum.php
 * ��˾��ַ��www.alientek.com
 * �����ַ��zhengdianyuanzi.tmall.com
 *
 * �޸�˵��
 * V1.0 20211028
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/STMFLASH/stmflash.h"

/**
 * @brief       ��ָ����ַ��ȡһ���� (32λ����)
 * @param       faddr   : ��ȡ��ַ (�˵�ַ����Ϊ4����!!)
 * @retval      ��ȡ�������� (32λ)
 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief       ��ָ����ַ��ʼ����ָ�����ȵ�����
 * @param       raddr : ��ʼ��ַ
 * @param       pbuf  : ����ָ��
 * @param       length: Ҫ��ȡ����(32λ)��,��4���ֽڵ�������
 * @retval      ��
 */
void stmflash_read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead)
{
    uint32_t  i;
    for (i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = stmflash_read_word(ReadAddr);/* ��ȡ4���ֽ� */
        ReadAddr += 4;/* ƫ��4���ֽ� */
    }
}

/**
 * @brief       ��ȡĳ����ַ���ڵ�flash����
 * @param       addr    : lash��ַ
 * @retval      0~11,��addr���ڵ�����
 */
uint8_t  stmflash_get_flash_sector(uint32_t addr)
{
    if (addr < ADDR_FLASH_SECTOR_1) return FLASH_SECTOR_0;
    else if (addr < ADDR_FLASH_SECTOR_2) return FLASH_SECTOR_1;
    else if (addr < ADDR_FLASH_SECTOR_3) return FLASH_SECTOR_2;
    else if (addr < ADDR_FLASH_SECTOR_4) return FLASH_SECTOR_3;
    else if (addr < ADDR_FLASH_SECTOR_5) return FLASH_SECTOR_4;
    else if (addr < ADDR_FLASH_SECTOR_6) return FLASH_SECTOR_5;
    else if (addr < ADDR_FLASH_SECTOR_7) return FLASH_SECTOR_6;
    else if (addr < ADDR_FLASH_SECTOR_8) return FLASH_SECTOR_7;
    else if (addr < ADDR_FLASH_SECTOR_9) return FLASH_SECTOR_8;
    else if (addr < ADDR_FLASH_SECTOR_10) return FLASH_SECTOR_9;
    else if (addr < ADDR_FLASH_SECTOR_11) return FLASH_SECTOR_10;
    return FLASH_SECTOR_11;
}

/**
 * @brief       ��ָ����ַ��ʼд��ָ�����ȵ�����
 *   @note      �ر�ע�⣺��ΪSTM32F4������ʵ��̫��,û�취���ر�����������,���Ա�����
 *              д��ַ�����0XFF,��ô���Ȳ������������Ҳ�������������.����
 *              д��0XFF�ĵ�ַ,�����������������ݶ�ʧ.����д֮ǰȷ��������
 *              û����Ҫ����,��������������Ȳ�����,Ȼ����������д
 *              �ú�����OTP����Ҳ��Ч!��������дOTP��!
 *              OTP�����ַ��Χ:0X1FFF7800~0X1FFF7A0F(ע�⣺���16�ֽڣ�����OTP���ݿ�����������д����)
 * @param       WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ4�ı���!!)
 * @param       pBuffer:����ָ��
 * @param       NumToWrite:��(32λ)��(����Ҫд���32λ���ݵĸ���)
 * @retval      ��
 */
void stmflash_write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus = HAL_OK;
    uint32_t SectorError = 0;
    uint32_t  addrx = 0;
    uint32_t  endaddr = 0;
    if (WriteAddr < STM32_FLASH_BASE || WriteAddr%4) return;    /* �Ƿ���ַ */

    HAL_FLASH_Unlock();     /* ���� */
    addrx = WriteAddr;      /* д�����ʼ��ַ */
    endaddr = WriteAddr + NumToWrite * 4;    /* д��Ľ�����ַ */

    if (addrx < 0X1FFF0000)
    {
        while (addrx < endaddr)     /* ɨ��һ���ϰ�.(�Է�FFFFFFFF�ĵط�,�Ȳ���) */
        {
            if (stmflash_read_word(addrx) != 0XFFFFFFFF)                    /* �з�0XFFFFFFFF�ĵط�,Ҫ����������� */
            {
                FlashEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;         /* �������ͣ��������� */
                FlashEraseInit.Sector = stmflash_get_flash_sector(addrx);   /* Ҫ���������� */
                FlashEraseInit.NbSectors = 1;                               /* һ��ֻ����һ������ */
                FlashEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;        /* ��ѹ��Χ��VCC=2.7~3.6V֮��!! */
                if (HAL_FLASHEx_Erase(&FlashEraseInit,&SectorError) != HAL_OK)
                {
                    break;  /* ���������� */
                }
            } 
            else
            {
               addrx += 4;
            }
            FLASH_WaitForLastOperation(FLASH_WAITETIME);                    /* �ȴ��ϴβ������ */
        }
    }
    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);                /* �ȴ��ϴβ������ */
    if (FlashStatus == HAL_OK)
    {
        while (WriteAddr < endaddr) /* д���� */
        {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr, *pBuffer) != HAL_OK)    /* д������ */
            {
                break;              /* д���쳣 */
            }
            WriteAddr += 4;
            pBuffer++;
        }
    }
    HAL_FLASH_Lock();               /* ���� */
}

/******************************************************************************************/
/* �����ô��� */

/**
 * @brief       ����д����(д1����)
 * @param       waddr : ��ʼ��ַ
 * @param       wdata : Ҫд�������
 * @retval      ��ȡ��������
 */
void test_write(uint32_t WriteAddr, uint32_t WriteData)
{
    stmflash_write(WriteAddr, &WriteData, 1);/* д��һ���� */
}

