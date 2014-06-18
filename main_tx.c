/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
*
* @defgroup nrf_dev_led_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_led_radio_tx_example
*
* @brief Radio Transmitter Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral.
*
*/

#include <stdint.h>
#include "radio_config.h"
#include "nrf.h"
#include "simple_uart.h"
#include "boards.h"

static uint8_t packet[100];  ///< Packet to transmit

void init()
{
  /* Start 16 MHz crystal oscillator */
  NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
  NRF_CLOCK->TASKS_HFCLKSTART = 1;

  /* Wait for the external oscillator to start up */
  while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
  {
  }

  // Set radio configuration parameters
  

  simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);

  // Set payload pointer
	
	radio_configure(1, 0);  
}

int packet_info(){

	simple_uart_putstring((const uint8_t *)"Digite a ID do atleta: \n");
	for(int i = 0; i <= 3; i++){
		
		packet[i + 2] = simple_uart_get();
	
	}
	
	simple_uart_putstring((const uint8_t *)"Digite a potencia de transmissão: \n");
	if(simple_uart_get_with_timeout(10000, &packet[1])){

	}	else	{

		packet[0] = NRF_RADIO->POWER;

	}

	simple_uart_putstring((const uint8_t *)"Digite o intervalo de transmissão: \n");
	if(simple_uart_get_with_timeout(10000, &packet[2])){

	}	else	{
	
		packet[1] = 200;
	
	}
	
	simple_uart_putstring((const uint8_t *)"Digite a nova ID: \n");
	if(simple_uart_get_with_timeout(10000, &packet[6]) && simple_uart_get_with_timeout(10000, &packet[7]) 
		&& simple_uart_get_with_timeout(10000, &packet[8]) && simple_uart_get_with_timeout(10000, &packet[9])){

	}	else	{
	
		packet[6] = packet[2];
		packet[7] = packet[3];
		packet[8] = packet[4];
		packet[9] = packet[5];
	
	}

}

int reconfig(){
	int tx;
	int ptx;

	simple_uart_putstring((uint8_t *)"Digte uma nova potencia\n");
	ptx = simple_uart_get();

	radio_configure(1, (uint32_t)ptx);
	NRF_RADIO->PACKETPTR = (uint32_t)packet;
	
}

void conversor_int_byte(uint8_t buff[], int pos, uint32_t valor){

				buff[pos] 		= (uint8_t)((valor & 0xFF000000) >> 24 );
				buff[pos + 1] = (uint8_t)((valor & 0x00FF0000) >> 16 );
				buff[pos + 2] = (uint8_t)((valor & 0x0000FF00) >> 8 );
				buff[pos + 3] = (uint8_t)((valor & 0X000000FF));
	
	
}

/**
 * @brief Function for application main entry.
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
	
	init();
	
  while(true)
  {

    //reconfig();
    //packet_info();
		NRF_RADIO->PACKETPTR = (uint32_t)packet;
		packet[0] = 0xFC;
		packet[1] = 0x0A;
		conversor_int_byte((uint8_t *)&packet, 2, 0x00ABCDEF);
		conversor_int_byte((uint8_t *)&packet, 6, 0x00ABCDEF);
		packet[10] = 0x01;
		//conversor_int_byte((uint8_t *)&packet, 2, 0x12345678);
		//conversor_int_byte((uint8_t *)&packet, 6, 0x12345678);
		//simple_uart_putstring((uint8_t *)"To aqui. \n");
		//simple_uart_get();

    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN = 1;
    while (NRF_RADIO->EVENTS_READY == 0U)
    {
    }
    NRF_RADIO->TASKS_START = 1U;
    NRF_RADIO->EVENTS_END = 0U;
    while(NRF_RADIO->EVENTS_END == 0U)
    {
    }
    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;
    while(NRF_RADIO->EVENTS_DISABLED == 0U)
    {
    }
  }
}




/**
 *@}
 **/
