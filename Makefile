#
# Makefile for 'litra'
# 2022 Chris Jones 
# @IPv6Freely
#


HIDAPI_DIR ?= ../hidapi

UNAME := $(shell uname -s)
ARCH := $(shell uname -m)

ifeq "$(UNAME)" "Darwin"
	OS=macos
endif
ifeq "$(OS)" "Windows_NT"
	OS=windows
endif
ifeq "$(UNAME)" "Linux"
	OS=linux
endif

# deal with stupid Windows not having 'cc'
ifeq (default,$(origin CC))
  CC = gcc
endif


#############  Mac
ifeq "$(OS)" "macos"

CFLAGS+=-arch x86_64 -arch arm64
LIBS=-framework IOKit -framework CoreFoundation -framework AppKit
OBJS=$(HIDAPI_DIR)/mac/hid.o
EXE=

endif

############# Windows
ifeq "$(OS)" "windows"

LIBS += -lsetupapi -Wl,--enable-auto-import -static-libgcc -static-libstdc++
OBJS = $(HIDAPI_DIR)/windows/hid.o
EXE=.exe

endif

############ Linux (hidraw)
ifeq "$(OS)" "linux"

LIBS = `pkg-config libudev --libs`
OBJS = $(HIDAPI_DIR)/linux/hid.o
EXE=

endif


############# common

CFLAGS+=-I $(HIDAPI_DIR)/hidapi
OBJS += litra.o

all: litra

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

litra: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o litra$(EXE) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f litra$(EXE)

package: litra$(EXE)
	@echo "Packaging up litra for '$(OS)-$(ARCH)'"
	zip litra-$(OS)-$(ARCH).zip litra$(EXE)
