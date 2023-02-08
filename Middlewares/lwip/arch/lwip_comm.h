/**
 ****************************************************************************************************
 * @file        lwip_comm.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-12-02
 * @brief       LWIP������������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� F407���������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211202
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#ifndef _LWIP_COMM_H
#define _LWIP_COMM_H 
#include "./BSP/ETHERNET/ethernet.h"


#define LWIP_MAX_DHCP_TRIES   4     /* DHCP������������Դ��� */

/*lwip���ƽṹ��*/
typedef struct  
{
    uint8_t mac[6];                 /* MAC��ַ */
    uint8_t remoteip[4];            /* Զ������IP��ַ */ 
    uint8_t ip[4];                  /* ����IP��ַ */
    uint8_t netmask[4];             /* �������� */
    uint8_t gateway[4];             /* Ĭ�����ص�IP��ַ */
    uint8_t dhcpstatus;             /* dhcp״̬ */
                                        /* 0, δ��ȡDHCP��ַ;*/
                                        /* 1, ����DHCP��ȡ״̬*/
                                        /* 2, �ɹ���ȡDHCP��ַ*/
                                        /* 0XFF,��ȡʧ�� */
}__lwip_dev;

extern __lwip_dev lwipdev;          /* lwip���ƽṹ�� */
 

void lwip_packet_handler(void);                     /* �����յ����ݺ���� */
void lwip_periodic_handle(void);                    /* lwip_periodic_handle */
void lwip_comm_default_ip_set(__lwip_dev *lwipx);   /* lwip Ĭ��IP���� */
uint8_t lwip_comm_mem_malloc(void);                 /* �ڴ�� ram_heap ���ڴ�� memp_memory ���ڴ���� */
void lwip_comm_mem_free(void);                      /* lwip��mem��memp�ڴ��ͷ� */
uint8_t lwip_comm_init(void);                       /* LWIP��ʼ��(LWIP������ʱ��ʹ��) */
void lwip_dhcp_process_handle(void);                /* DHCP�������� */

#endif













