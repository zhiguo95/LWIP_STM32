/**
 ****************************************************************************************************
 * @file        lwip_comm.c
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
 
#include "lwip_comm.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/init.h"
#include "ethernetif.h"
#include "lwip/timeouts.h"
#include "lwip/tcpip.h"
#include "./MALLOC/malloc.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include <stdio.h>


__lwip_dev lwipdev;                 /* lwip���ƽṹ�� */
struct netif lwip_netif;            /* ����һ��ȫ�ֵ�����ӿ� */

#if LWIP_DHCP
uint32_t DHCPfineTimer = 0;         /* DHCP��ϸ�����ʱ�� */
uint32_t DHCPcoarseTimer = 0;       /* DHCP�ֲڴ����ʱ�� */
#endif


/**
 * @breif       lwip Ĭ��IP����
 * @param       lwipx  : lwip���ƽṹ��ָ��
 * @retval      ��
 */
void lwip_comm_default_ip_set(__lwip_dev *lwipx)
{
    /* Ĭ��Զ��IPΪ:192.168.1.134 */
    lwipx->remoteip[0] = 192;
    lwipx->remoteip[1] = 168;
    lwipx->remoteip[2] = 1;
    lwipx->remoteip[3] = 27;
    
    /* MAC��ַ����(�����ֽڹ̶�Ϊ:2.0.0,�����ֽ���STM32ΨһID) */
    lwipx->mac[0] = 0xB8;       /* �����ֽ�(IEEE��֮Ϊ��֯ΨһID,OUI)��ַ�̶�Ϊ:2.0.0 */
    lwipx->mac[1] = 0xAE;
    lwipx->mac[2] = 0x1D;
    lwipx->mac[3] = 0x00;       /* �����ֽ���STM32��ΨһID 28.0.36 */   
    lwipx->mac[4] = 0x07;
    lwipx->mac[5] = 0x00;
    
    /* Ĭ�ϱ���IPΪ:192.168.1.30 */
    lwipx->ip[0] = 192;
    lwipx->ip[1] = 168;
    lwipx->ip[2] = 1;
    lwipx->ip[3] = 30;
    /* Ĭ����������:255.255.255.0 */
    lwipx->netmask[0] = 255;
    lwipx->netmask[1] = 255;
    lwipx->netmask[2] = 255;
    lwipx->netmask[3] = 0;
    
    /* Ĭ������:192.168.1.1 */
    lwipx->gateway[0] = 192;
    lwipx->gateway[1] = 168;
    lwipx->gateway[2] = 1;
    lwipx->gateway[3] = 1;
    lwipx->dhcpstatus = 0; /* û��DHCP */
}

/**
 * @breif       LWIP��ʼ��(LWIP������ʱ��ʹ��)
 * @param       ��
 * @retval      0,�ɹ�
 *              1,�ڴ����
 *              2,��̫��оƬ��ʼ��ʧ��
 *              3,�������ʧ��.
 */
uint8_t lwip_comm_init(void)
{
    uint8_t retry = 0;
    struct netif *netif_init_flag;              /* ����netif_add()����ʱ�ķ���ֵ,�����ж������ʼ���Ƿ�ɹ� */
    ip_addr_t ipaddr;                           /* ip��ַ */
    ip_addr_t netmask;                          /* �������� */
    ip_addr_t gw;                               /* Ĭ������ */

    if (ethernet_mem_malloc())return 1;         /* �ڴ�����ʧ��*/

    lwip_comm_default_ip_set(&lwipdev);         /* ����Ĭ��IP����Ϣ */

    while (ethernet_init())                     /* ��ʼ����̫��оƬ,���ʧ�ܵĻ�������5�� */
    {
        retry++;

        if (retry > 5)
        {
            retry = 0;                          /* ��̫��оƬ��ʼ��ʧ�� */
            return 3;
        }
    }

    lwip_init();                                /* ��ʼ��LWIP�ں� */

#if LWIP_DHCP                                   /* ʹ�ö�̬IP */
    ipaddr.addr = 0;
    netmask.addr = 0;
    gw.addr = 0;
#else   /* ʹ�þ�̬IP */
    IP4_ADDR(&ipaddr, lwipdev.ip[0], lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);
    IP4_ADDR(&netmask, lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
    IP4_ADDR(&gw, lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
    printf("����en��MAC��ַΪ:................%d.%d.%d.%d.%d.%d\r\n", lwipdev.mac[0], lwipdev.mac[1], lwipdev.mac[2], lwipdev.mac[3], lwipdev.mac[4], lwipdev.mac[5]);
    printf("��̬IP��ַ........................%d.%d.%d.%d\r\n", lwipdev.ip[0], lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);
    printf("��������..........................%d.%d.%d.%d\r\n", lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
    printf("Ĭ������..........................%d.%d.%d.%d\r\n", lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
#endif  /* �������б������һ������ */
    netif_init_flag = netif_add(&lwip_netif, (const ip_addr_t *)&ipaddr, (const ip_addr_t *)&netmask, (const ip_addr_t *)&gw, NULL, &ethernetif_init, &ethernet_input);


    if (netif_init_flag == NULL)
    {
        return 4;                           /* �������ʧ�� */
    }
    else                                    /* ������ӳɹ���,����netifΪĬ��ֵ,���Ҵ�netif���� */
    {
        netif_set_default(&lwip_netif);     /* ����netifΪĬ������ */

        if (netif_is_link_up(&lwip_netif))
        {
            netif_set_up(&lwip_netif);      /* ��netif���� */
        }
        else
        {
            netif_set_down(&lwip_netif);
        }
    }

#if LWIP_DHCP                               /* ���ʹ��DHCP�Ļ� */
    lwipdev.dhcpstatus = 0;                 /* DHCP���Ϊ0 */
    dhcp_start(&lwip_netif);                /* ����DHCP���� */
#endif
    return 0;                               /* ����OK. */
}

/**
 * @breif       �����յ����ݺ����
 * @param       ��
 * @retval      ��
 */
void lwip_pkt_handle(void)
{
    /* �����绺�����ж�ȡ���յ������ݰ������䷢�͸�LWIP���� */
    ethernetif_input(&lwip_netif);
}

/**
 * @breif       LWIP��ѯ����
 * @param       ��
 * @retval      ��
 */
void lwip_periodic_handle()
{
    sys_check_timeouts();
#if LWIP_DHCP       /* ���ʹ��DHCP */

    /* ÿ500ms����һ��dhcp_fine_tmr() */
    if (sys_now() - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)    /* DHCP_FINE_TIMER_MSECS(500) lwip_localtime���� T3 ���뵥λ */
    {
        DHCPfineTimer =  sys_now();
        dhcp_fine_tmr();

        if ((lwipdev.dhcpstatus != 2) && (lwipdev.dhcpstatus != 0XFF))
        {
            lwip_dhcp_process_handle();  /* DHCP���� */
        }
    }

    /* ÿ60sִ��һ��DHCP�ֲڴ��� */
    if (sys_now() - DHCPcoarseTimer >= DHCP_COARSE_TIMER_MSECS)
    {
        DHCPcoarseTimer =  sys_now();
        dhcp_coarse_tmr();
    }
#endif
}



/* ���ʹ��DHCP */
#if LWIP_DHCP

struct dhcp gdhcp;

/**
 * @breif       DHCP��������
 * @param       ��
 * @retval      ��
 */
void lwip_dhcp_process_handle(void)
{
    uint32_t ip = 0, netmask = 0, gw = 0;

    switch (lwipdev.dhcpstatus)
    {
        case 0:     /* ����DHCP */
            dhcp_start(&lwip_netif);
            lwipdev.dhcpstatus = 1;             /* �ȴ�ͨ��DHCP��ȡ���ĵ�ַ */
            printf("���ڲ���DHCP������,���Ե�...........\r\n");
            break;

        case 1:     /* �ȴ���ȡ��IP��ַ */
        {
            ip = lwip_netif.ip_addr.addr;       /* ��ȡ��IP��ַ */
            netmask = lwip_netif.netmask.addr;  /* ��ȡ�������� */
            gw = lwip_netif.gw.addr;            /* ��ȡĬ������ */

            if (ip != 0)    /*��ȷ��ȡ��IP��ַ��ʱ��*/
            {
                lwipdev.dhcpstatus = 2;         /* DHCP�ɹ� */
                printf("����en��MAC��ַΪ:................%d.%d.%d.%d.%d.%d\r\n", lwipdev.mac[0], lwipdev.mac[1], lwipdev.mac[2], lwipdev.mac[3], lwipdev.mac[4], lwipdev.mac[5]);
                /* ������ͨ��DHCP��ȡ����IP��ַ */
                lwipdev.ip[3] = (uint8_t)(ip >> 24);
                lwipdev.ip[2] = (uint8_t)(ip >> 16);
                lwipdev.ip[1] = (uint8_t)(ip >> 8);
                lwipdev.ip[0] = (uint8_t)(ip);
                printf("ͨ��DHCP��ȡ��IP��ַ..............%d.%d.%d.%d\r\n", lwipdev.ip[0], lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);
                /* ����ͨ��DHCP��ȡ�������������ַ */
                lwipdev.netmask[3] = (uint8_t)(netmask >> 24);
                lwipdev.netmask[2] = (uint8_t)(netmask >> 16);
                lwipdev.netmask[1] = (uint8_t)(netmask >> 8);
                lwipdev.netmask[0] = (uint8_t)(netmask);
                printf("ͨ��DHCP��ȡ����������............%d.%d.%d.%d\r\n", lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
                /* ������ͨ��DHCP��ȡ����Ĭ������ */
                lwipdev.gateway[3] = (uint8_t)(gw >> 24);
                lwipdev.gateway[2] = (uint8_t)(gw >> 16);
                lwipdev.gateway[1] = (uint8_t)(gw >> 8);
                lwipdev.gateway[0] = (uint8_t)(gw);
                printf("ͨ��DHCP��ȡ����Ĭ������..........%d.%d.%d.%d\r\n", lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
            }
            else if (gdhcp.tries > LWIP_MAX_DHCP_TRIES)    /* ͨ��DHCP�����ȡIP��ַʧ��,�ҳ�������Դ��� */
            {
                lwipdev.dhcpstatus = 0XFF;  /* DHCP��ʱʧ��. */
                /* ʹ�þ�̬IP��ַ */
                IP4_ADDR(&(lwip_netif.ip_addr), lwipdev.ip[0], lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);
                IP4_ADDR(&(lwip_netif.netmask), lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
                IP4_ADDR(&(lwip_netif.gw), lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
                printf("DHCP����ʱ,ʹ�þ�̬IP��ַ!\r\n");
                printf("����en��MAC��ַΪ:................%d.%d.%d.%d.%d.%d\r\n", lwipdev.mac[0], lwipdev.mac[1], lwipdev.mac[2], lwipdev.mac[3], lwipdev.mac[4], lwipdev.mac[5]);
                printf("��̬IP��ַ........................%d.%d.%d.%d\r\n", lwipdev.ip[0], lwipdev.ip[1], lwipdev.ip[2], lwipdev.ip[3]);
                printf("��������..........................%d.%d.%d.%d\r\n", lwipdev.netmask[0], lwipdev.netmask[1], lwipdev.netmask[2], lwipdev.netmask[3]);
                printf("Ĭ������..........................%d.%d.%d.%d\r\n", lwipdev.gateway[0], lwipdev.gateway[1], lwipdev.gateway[2], lwipdev.gateway[3]);
            }
        }
        break;

        default :
            break;
    }
}
#endif
