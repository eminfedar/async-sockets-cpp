#!make
# This make needs:
# g++
#

include ../MakeHelper

# Project name
NAME 		:= $(patsubst %/,%,$(notdir $(CURDIR)))

#The Target Binary Program
TARGET			:= $(call MakeLib,$(NAME))

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR		?= $(CURDIR)/src
INCDIR		?= $(CURDIR)/include
BUILDDIR	?= $(CURDIR)/obj/$(BUILD)
TARGETDIR	?= $(CURDIR)/bin/$(BUILD)
RESDIR		?= $(CURDIR)/rsc
SRCEXT	:= .cpp
OBJEXT	:= .o

#Compiler and Linker
BUILD  		?= release
CC			:= g++

#Flags, Libraries and Includes
CFLAGS.common	:= -std=c++17 -fPIC -Wall -Wextra -DBUILD_EASYSOCKET
CFLAGS.debug 	:= $(CFLAGS.common) -g
CFLAGS.release	:= $(CFLAGS.common) -Werror -O3
CFLAGS			?= $(CFLAGS.$(BUILD))
LIB		:= -L$(TARGETDIR)
INC		:= -I$(INCDIR)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES	:= $(wildcard $(SRCDIR)/*$(SRCEXT))
OBJECTS	:= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:$(SRCEXT)=$(OBJEXT)))

#Default Make
all: resources $(TARGET) tests docs

#Remake
remake: cleaner all

#Create template folder structure
template: directories
	$(call MD,$(SRCDIR))
	$(call MD,$(INCDIR))
	$(call MD,$(RESDIR))

#Make the Directories
directories:
	$(call MD,$(TARGETDIR))
	$(call MD,$(BUILDDIR))

#Copy Resources from Resources Directory to Target Directory
resources: template
#	@$(call CP,$(RESDIR)/*,$(TARGETDIR)/)

#Clean only Objecst
clean:
	$(call RM,$(BUILDDIR))

#Full Clean, Objects and Binaries
cleaner: clean
	$(call RM,$(TARGETDIR))

docs:
#TODO

tests:
#TODO

#Link the final executable/library
$(TARGET): $(OBJECTS)
	$(CC) -shared -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%$(OBJEXT): $(SRCDIR)/%$(SRCEXT)
	$(call MD,$(dir $@))
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

#Non-File Targets
.PHONY: all remake template directories resources clean cleaner
