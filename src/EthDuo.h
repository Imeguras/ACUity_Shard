/*
 * ethernet_logic.cpp
 *
 *  Created on: 30/09/2023
 *      Author: João Vieira
 */

#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
 #include "r_sci_uart.h"
#ifndef ETHDUO_H_
#define ETHDUO_H_

class EthDuo {
	public:
		EthDuo();
		virtual ~EthDuo();
	private:

		/* IP instance */
		NX_IP g_ip0;
		/* Stack memory for g_ip0. */
		void g_ip0_quick_setup();
		void g_packet_pool0_quick_setup();
		/* Stack memory for g_ip0. */
		uint8_t g_ip0_stack_memory[G_IP0_TASK_STACK_SIZE]; //BSP_PLACE_IN_SECTION(".stack.g_ip0") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);

		/* ARP cache memory for g_ip0. */
		uint8_t g_ip0_arp_cache_memory[G_IP0_ARP_CACHE_SIZE] BSP_ALIGN_VARIABLE(4);
		#if defined(ETHER_BUFFER_PLACE_IN_SECTION)
		uint8_t g_packet_pool0_pool_memory[G_PACKET_POOL0_PACKET_NUM * (G_PACKET_POOL0_PACKET_SIZE + sizeof(NX_PACKET))] BSP_ALIGN_VARIABLE(4) ETHER_BUFFER_PLACE_IN_SECTION;
		#elif defined(WIFI_BUFFER_PLACE_IN_SECTION)
		uint8_t g_packet_pool0_pool_memory[G_PACKET_POOL0_PACKET_NUM * (G_PACKET_POOL0_PACKET_SIZE + sizeof(NX_PACKET))] BSP_ALIGN_VARIABLE(4) WIFI_BUFFER_PLACE_IN_SECTION;
		#endif
		/* Packet pool instance (If this is a Trustzone part, the memory must be placed in Non-secure memory). */
		NX_PACKET_POOL g_packet_pool0;


};





#endif /* ETHDUO_H_ */












