CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -ggdb2
DEFINES=
LIBS=
PLATFORM = $(shell ./platform.sh)

ifeq ($(PLATFORM),Pi)
	CFLAGS += -DPLATFORM_RPI -DDATABASE_REDIS
	LIBS += -lwiringPi -lhiredis
endif

SRCDIR=src
BUILDDIR=build

SRC=$(wildcard $(SRCDIR)/*.c)
SRC += $(wildcard $(SRCDIR)/*/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

BINARYNAME=ased
BINARY=$(BUILDDIR)/$(BINARYNAME)

all: always $(BINARY)

$(BINARY): $(BUILDDIR)/$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BINARY) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(DEFINES) -c $< -o $@

always:
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/platform
	mkdir -p $(BUILDDIR)/database

clean:
	rm -rf $(BUILDDIR)/*
