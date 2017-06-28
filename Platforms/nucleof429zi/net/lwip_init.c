/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 * Filename: lwip_init.c
 * ============================================================================
 */

#include "lwip_init.h"
struct netif xnetif; /* network interface structure */

/**
 * @brief  Initializes the lwIP stack
 * @param  None
 * @retval None
 */
void LwIP_Init(void) {
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
#ifndef USE_DHCP 
#ifdef DEBUG
    uint8_t iptab[4];
    uint8_t iptxt[20];
#endif
#endif
    /* Create tcp_ip stack thread */
    tcpip_init(NULL, NULL);
    
#ifndef USE_DHCP

    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
#else
    IP4_ADDR(&ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
    
#endif

    /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
              struct ip_addr *netmask, struct ip_addr *gw,
              void *state, err_t (* init)(struct netif *netif),
              err_t (* input)(struct pbuf *p, struct netif *netif))
    
    Adds your network interface to the netif_list. Allocate a struct
    netif and pass a pointer to this structure as the first argument.
    Give pointers to cleared ip_addr structures when using DHCP,
    or fill them with sane numbers otherwise. The state pointer may be NULL.

    The init function pointer must point to a initialization function for
    your ethernet netif interface. The following code illustrates it's use.*/

    netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

    /*  Registers the default network interface. */
    netif_set_default(&xnetif);

    /*  When the netif is fully configured this function must be called.*/
    netif_set_up(&xnetif);

    dhcp_start(&xnetif);
}