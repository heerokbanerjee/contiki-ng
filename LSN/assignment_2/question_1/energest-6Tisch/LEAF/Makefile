CONTIKI_PROJECT = leaf
all: $(CONTIKI_PROJECT)

MODULES += os/services/shell
MODULES += os/net/mac/tsch/sixtop


MAKE_MAC = MAKE_MAC_TSCH
MAKE_NET = MAKE_NET_IPV6
MAKE_ROUTING = MAKE_ROUTING_RPL_LITE

CONTIKI = ../../../../..
include $(CONTIKI)/Makefile.include
