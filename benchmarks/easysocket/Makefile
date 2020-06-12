#!make
# This make needs:
# g++
#

include ../../MakeHelper

#The Directories, Source, Includes, Objects, Binary and Resources
BUILDDIR	?= $(CURDIR)/obj/$(BUILD)
TARGETDIR	?= $(CURDIR)/bin/$(BUILD)

#Compiler and Linker
BUILD	?= release
BITS	?= 64
CC		:= g++

#Flags, Libraries and Includes
CFLAGS.common	:= -std=c++17 -m$(BITS) -Wall -Wextra
CFLAGS.debug 	:= $(CFLAGS.common) -g
CFLAGS.release	:= $(CFLAGS.common) -Werror -O3
CFLAGS			?= $(CFLAGS.$(BUILD))
LIB		:= -L$(TARGETDIR) -leasysocket -lpthread -Wl,-rpath='$${ORIGIN}'
INC		:= -I$(CURDIR)/../../easysocket/include

build:
	(cd ../../easysocket && make TARGETDIR="$(TARGETDIR)")
	$(CC) $(CFLAGS) $(INC) udp-client.cpp -o $(TARGETDIR)/$(call MakeExe,udp-client) $(LIB)
	$(CC) $(CFLAGS) $(INC) udp-server.cpp -o $(TARGETDIR)/$(call MakeExe,udp-server) $(LIB)

template:
	$(call MD,$(TARGETDIR))
	$(call MD,$(BUILDDIR))

clean:
	$(call RM,$(BUILDDIR))

cleaner: clean
	$(call RM,$(TARGETDIR))

clean-deps: clean
	(cd ../../easysocket && make clean)

cleaner-deps: cleaner
	(cd ../../easysocket && make cleaner)

.PHONY: build template clean cleaner clean-deps cleaner-deps
