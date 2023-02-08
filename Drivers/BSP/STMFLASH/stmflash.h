/**
 ****************************************************************************************************
 * @file        stmflash.h
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

#ifndef __STMFLASH_H
#define __STMFLASH_H

#include "./SYSTEM/sys/sys.h"


/* FLASH��ʼ��ַ */
#define STM32_FLASH_SIZE        0x80000         /* STM32 FLASH �ܴ�С */
#define STM32_FLASH_BASE        0x08000000      /* STM32 FLASH ��ʼ��ַ */
#define FLASH_WAITETIME         50000           /* FLASH�ȴ���ʱʱ�� */

/* FLASH ��������ʼ��ַ */
#define ADDR_FLASH_SECTOR_0     ((uint32_t )0x08000000)     /* ����0��ʼ��ַ, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t )0x08004000)     /* ����1��ʼ��ַ, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t )0x08008000)     /* ����2��ʼ��ַ, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t )0x0800C000)     /* ����3��ʼ��ַ, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t )0x08010000)     /* ����4��ʼ��ַ, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t )0x08020000)     /* ����5��ʼ��ַ, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t )0x08040000)     /* ����6��ʼ��ַ, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t )0x08060000)     /* ����7��ʼ��ַ, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t )0x08080000)     /* ����8��ʼ��ַ, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t )0x080A0000)     /* ����9��ʼ��ַ, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t )0x080C0000)     /* ����10��ʼ��ַ,128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t )0x080E0000)     /* ����11��ʼ��ַ,128 Kbytes */


uint32_t stmflash_read_word(uint32_t faddr);                                        /* ������ */
void stmflash_write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite);    /* ��ָ����ַ��ʼд��ָ�����ȵ����� */
void stmflash_read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead);       /* ��ָ����ַ��ʼ����ָ�����ȵ����� */
void test_write(uint32_t WriteAddr, uint32_t WriteData);                            /* ����д�� */

#endif


















