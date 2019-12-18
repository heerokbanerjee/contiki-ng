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
#include "sys/stimer.h"
#include "tsch.h"

PROCESS(energest_example_process, "energest example process");
AUTOSTART_PROCESSES(&energest_example_process);
/*---------------------------------------------------------------------------*/

static bool start = false;

static inline unsigned long
to_seconds(uint64_t time)
{
  return (unsigned long)(time / ENERGEST_SECOND);
}

void
tsch_rpl_callback_joining_network_new(void) //gets called when connection is established
{
    printf("\nJOINED NETWORK\n");
    start = true;
}

/*---------------------------------------------------------------------------*/
/*
 * This Process will periodically print energest values for the last minute.
 *
 */
PROCESS_THREAD(energest_example_process, ev, data)
{
  static struct etimer periodic_timer;
  static struct stimer second_timer;
  static bool once = true;

  PROCESS_BEGIN();

  etimer_set(&periodic_timer, CLOCK_SECOND * 10);
  while(1) {


    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);	

    //when joined a network start the timer once
    if(start && once) {
	stimer_set(&second_timer, 600); // Start the timer.
        printf("\nTimer started for 600 seconds\n");
        once = false;
    }
    //if we're in a network and the timer is expired stop the measurements
    if(start && stimer_expired(&second_timer)) {
      printf("\n10 minutes expired\n");
      printf("\nStopping process\n");
      break;
    }



    if (start) {

	//timer
        unsigned long timer = stimer_remaining(&second_timer);
	printf("\nTime left: %ld\n", timer);

	/*
	* Update all energest times. Should always be called before energest
	* times are read.
	*/
	energest_flush();

	printf("\nEnergest:\n");
	printf(" CPU          %4lus LPM      %4lus DEEP LPM %4lus  Total time %lus\n",
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
    }

    


  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
