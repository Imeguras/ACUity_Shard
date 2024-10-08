/*
 * EthDuo.cpp
 *
 *  Created on: 30/09/2023
 *      Author: micron
 */

#include "EthDuo.h"

  uint8_t g_ip0_arp_cache_memory [G_IP0_ARP_CACHE_SIZE] __attribute__((aligned(4)));
  uint8_t g_ip0_stack_memory  [G_IP0_TASK_STACK_SIZE]  __attribute__ ((aligned(8), section(".stack.g_ip0")));
#ifdef ETHER_BUFFER_PLACE_IN_SECTION
  uint8_t g_packet_pool0_pool_memory[G_PACKET_POOL0_PACKET_NUM *
                                     (G_PACKET_POOL0_PACKET_SIZE +
                                      sizeof(NX_PACKET))] BSP_ALIGN_VARIABLE(4)
      ETHER_BUFFER_PLACE_IN_SECTION;
#else
	#ifdef WIFI_BUFFER_PLACE_IN_SECTION
	  uint8_t g_packet_pool0_pool_memory[G_PACKET_POOL0_PACKET_NUM *
										 (G_PACKET_POOL0_PACKET_SIZE +
										  sizeof(NX_PACKET))] BSP_ALIGN_VARIABLE(4)
		  WIFI_BUFFER_PLACE_IN_SECTION;
	#endif
#endif
EthDuo::EthDuo() {

}

EthDuo::~EthDuo() {
	UINT status = nx_ip_delete(&g_ip0);
	this->error_counter+=status;
}

uint32_t EthDuo::recv(void * data, uint32_t stream_size){

    FSP_PARAMETER_NOT_USED(data);
    FSP_PARAMETER_NOT_USED(stream_size);
	return 0;
}

uint32_t	EthDuo::write(void *data, uint32_t stream_size){
    FSP_PARAMETER_NOT_USED(data);
    FSP_PARAMETER_NOT_USED(stream_size);
    return 0;
}

int  EthDuo::initialization(){
    if(this->initialized){
            //TODO Evaluate risk
            return FSP_ERR_ALREADY_OPEN;
    }
	nx_system_initialize();
	this->g_packet_pool0_quick_setup();
  	this->g_ip0_quick_setup();
  	return (int)this->error_counter;
}



/* Quick setup for g_ip0.
 * - g_packet_pool0 must be setup before calling this function
 *     (See Developer Assistance -> g_ip0 -> g_packet_pool0 -> Quick Setup).
 * - nx_system_initialize() must be called before calling this function.
 * - If using a NetX Duo Wireless Driver variant the quick setup for wireless
 * must be setup (so it can connect to a network) before calling this function
 *     (See Developer Assistance -> g_ip0 -> NetX Duo Wireless Driver -> Quick
 * Setup)
 * - If using NetX Duo with the Embedded Wireless Framework (EWF) and Cellular
 * products then please see the FSP user manual for what to setup prior to
 * calling this function.
 */
void EthDuo::g_ip0_quick_setup() {
	  UINT status;

  /* Create the ip instance. */
  status = nx_ip_create(&g_ip0, (char *)"g_ip0 IP Instance",G_IP0_ADDRESS,
                        G_IP0_SUBNET_MASK, &g_packet_pool0,
                        G_IP0_NETWORK_DRIVER, &g_ip0_stack_memory[0],
                        G_IP0_TASK_STACK_SIZE, G_IP0_TASK_PRIORITY);

  this->error_counter += status;

	/* If using Embedded Wireless Framework then uncomment this out to set the
   * adapter_ptr. See the FSP user manual for more details. */
  // g_ip0.nx_ip_reserved_ptr = adapter_ptr;

  /* Set the gateway address if it is configured. Make sure this is set if using
   * the Embedded Wireless Framework! */
 if (IP_ADDRESS(0, 0, 0, 0) != G_IP0_GATEWAY_ADDRESS) {
    status = nx_ip_gateway_address_set(&g_ip0, G_IP0_GATEWAY_ADDRESS);
    this->error_counter += status;
  }


  status =
      nx_arp_enable(&g_ip0, &g_ip0_arp_cache_memory[0], G_IP0_ARP_CACHE_SIZE);
  this->error_counter += status;



  status = nx_udp_enable(&g_ip0);
  this->error_counter += status;


  status = nx_icmp_enable(&g_ip0);
  this->error_counter += status;
  status = nx_tcp_enable(&g_ip0);
  this->error_counter += status;
  status = nx_ip_fragment_enable(&g_ip0);
  this->error_counter += status;

  status = nx_igmp_enable(&g_ip0);
  this->error_counter += status;
  /*
  status = nx_dhcp_create(&g_dhcp_client0, &g_ip0, (char *)"g_dhcp_client0");

  nx_dhcp_packet_pool_set(&g_dhcp_client0, &g_packet_pool0);
  nx_dhcp_start(&g_dhcp_client0);

  this->error_counter += status;
  /* Wait for the link to be enabled. */
  ULONG current_state;
   status =
      nx_ip_status_check(&g_ip0, NX_IP_LINK_ENABLED, &current_state, 20000);
  this->error_counter += status;

  	//assert(NX_SUCCESS == status);
  	//assert(NX_IP_ADDRESS_RESOLVED == requested_status);
}
/* Quick Setup for g_packet_pool0.
 * - nx_system_initialize() must be called before calling this function.
 */
void EthDuo::g_packet_pool0_quick_setup() {
  /* Create the packet pool. */
  UINT status = nx_packet_pool_create(
      &g_packet_pool0, "g_packet_pool0 Packet Pool", G_PACKET_POOL0_PACKET_SIZE,
      &g_packet_pool0_pool_memory[0],
      G_PACKET_POOL0_PACKET_NUM *
          (G_PACKET_POOL0_PACKET_SIZE + sizeof(NX_PACKET)));
  assert(NX_SUCCESS == status);
}
/*
 * NX_IP EthDuo::g_ip(){
	return g_ip0;
}
 */

