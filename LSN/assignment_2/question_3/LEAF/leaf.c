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

PROCESS(energest_example_process, "energest example process");
AUTOSTART_PROCESSES(&energest_example_process);
static bool start = false;
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
/*
 * This Process will periodically print energest values for the last minute.
 *
 */
PROCESS_THREAD(energest_example_process, ev, data)
{
  static struct etimer periodic_timer;
  const linkaddr_t MAC_ROOT = {{ 0x00, 0x12, 0x4b, 0x00, 0x19, 0x32, 0xe3, 0x20 }};
  //static struct stimer second_timer;
   static bool once = true;

  PROCESS_BEGIN();
  tsch_schedule_init();
  tsch_queue_add_nbr(MAC_LEAF);




  etimer_set(&periodic_timer, CLOCK_SECOND * 10);
  while(1) {



    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);



    /*
     * Update all energest times. Should always be called before energest
     * times are read.
     */
    energest_flush();
    printf("\n\n");
 
    if (once) {
	struct tsch_slotframe *sf = tsch_schedule_get_slotframe_by_handle(0); //shared slotframe
	if (sf == NULL){
	printf("NO such slotframe found"); // so node is not yet set to root-node (coordinator)
	} else {
	printf("Handle: %d", sf->handle);
	struct tsch_link *tl = tsch_schedule_add_link(sf, 0,LINK_TYPE_NORMAL, &MAC_ROOT, 1, 0);
	if (tl == NULL){
	    printf("NO such link found");
	} else {
	    printf("handel: %d", tl->handle);
	    printf("LINK CREATED FOR 0012.4b00.1932.e169");
	    once = false;

	}

	}
    }
    
    
	printf("\nSCHEDULE:\n");
	tsch_schedule_print();

    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
