/*
 * Copyright (c) 2017, RISE SICS.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>

#include "contiki.h"
#include "sys/energest.h"
#include "tsch.h"
#include "tsch-types.h"
#include "net/nullnet/nullnet.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (8 * CLOCK_SECOND)


PROCESS(energest_example_process, "energest example process");
AUTOSTART_PROCESSES(&energest_example_process);

static bool start = false;
static bool send = false;
static unsigned count;

/*---------------------------------------------------------------------------*/
static inline unsigned long
to_seconds(uint64_t time)
{
  return (unsigned long)(time / ENERGEST_SECOND);
}

void
tsch_rpl_callback_joining_network_new(void) 
{
    printf("\nJOINED NETWORK\n");
    start = true;
}
/*---------------------------------------------------------------------------*/
void input_callback(const void *data, uint16_t len,
  const linkaddr_t *src, const linkaddr_t *dest)
{
  if(len == sizeof(unsigned)) {
    memcpy(&count, data, sizeof(count));
    printf("Received %u from ", count);
    LOG_INFO_LLADDR(src);
    LOG_INFO_("\n");
    send = true;
  }
}
/*---------------------------------------------------------------------------*/
/*
 * This Process will periodically print energest values for the last minute.
 *
 */
PROCESS_THREAD(energest_example_process, ev, data)
{
        static struct etimer periodic_timer;
  //const linkaddr_t MAC_ROOT = {{ 0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe3, 0x20 }};
        const linkaddr_t MAC_ROOT = {{ 0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe1, 0x69 }};
  //static struct stimer second_timer;

        NETSTACK_RADIO.set_value(RADIO_PARAM_TXPOWER, 7);

  	PROCESS_BEGIN();

  	nullnet_set_input_callback(input_callback);

	tsch_set_coordinator(0);
	printf("I'm leaf");

	etimer_set(&periodic_timer, CLOCK_SECOND * 1);
	while(1) {



		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
		etimer_reset(&periodic_timer);

			/*
			     * Update all energest times. Should always be called before energest
			     * times are read.
			     */
			    energest_flush();

				printf("\n---------------------ENERGY-------------------------\n");
			    printf("\n CPU          %4lus LPM      %4lus DEEP LPM %4lus  Total time %lus\n",
			       to_seconds(energest_type_time(ENERGEST_TYPE_CPU)),
			       to_seconds(energest_type_time(ENERGEST_TYPE_LPM)),
			       to_seconds(energest_type_time(ENERGEST_TYPE_DEEP_LPM)),
			       to_seconds(ENERGEST_GET_TOTAL_TIME()));
			    printf(" Radio LISTEN %4lus TRANSMIT %4lus OFF      %4lus\n",
			       to_seconds(energest_type_time(ENERGEST_TYPE_LISTEN)),
			       to_seconds(energest_type_time(ENERGEST_TYPE_TRANSMIT)),
			       to_seconds(ENERGEST_GET_TOTAL_TIME()
				      - energest_type_time(ENERGEST_TYPE_TRANSMIT)
				      - energest_type_time(ENERGEST_TYPE_LISTEN)));
				printf("\n----------------------------------------------------\n");



		if (send) {
			LOG_INFO("Sending %u to ", count);
			LOG_INFO_LLADDR(&MAC_ROOT);
			LOG_INFO_("\n");

			nullnet_buf = (uint8_t *)&count;
			nullnet_len = sizeof(count);

			NETSTACK_NETWORK.output(&MAC_ROOT);
			send = false;
		}

		if(!tsch_is_associated){
			printf("\nNot connected\n");
		} else {
			printf("\nConnected\n");
		}


		
		/*
		* Update all energest times. Should always be called before energest
		* times are read.
		*/
		//energest_flush();
		//printf("\n\n");


		//printf("\nSCHEDULE:\n");
		//tsch_schedule_print();


	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
