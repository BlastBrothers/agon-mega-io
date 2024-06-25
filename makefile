# ----------------------------
# Makefile Options
# ----------------------------

NAME = Joytest
DESCRIPTION = "Agon Mega I/O Test App"
COMPRESSED = NO
LDHAS_ARG_PROCESSING = 0

CFLAGS = -Wall -Wextra -O2 -ferror-limit=0
CXXFLAGS = -Wall -Wextra -O2

#BSSHEAP_LOW=050000
#BSSHEAP_HIGH=0B0000

# ----------------------------

include $(shell cedev-config --makefile)