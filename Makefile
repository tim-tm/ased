CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -ggdb2
DEFINES=
LIBS=

PLATFORM=$(shell ./platform.sh)

SRCDIR=src
BUILDDIR=build

SRC=$(wildcard $(SRCDIR)/*.c)

ifeq ($(PLATFORM),Pi)
	CFLAGS += -DPLATFORM_RPI -DDATABASE_SQLITE -Ibuild
	LIBS += -lwiringPi -lsqlite3
	SRC += $(SRCDIR)/database/database_sqlite.c
	SRC += $(SRCDIR)/platform/platform_rpi.c
else
	CFLAGS += -DPLATFORM_GENERIC -DDATABASE_REDIS -Ibuild
	LIBS += -Lbuild/hiredis -l:libhiredis.a -l:libhiredis_ssl.a -lssl -lcrypto
	SRC += $(SRCDIR)/database/database_sqlite.c
	SRC += $(SRCDIR)/platform/platform_generic.c
endif

OBJ=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))

BINARYNAME=ased
BINARY=$(BUILDDIR)/$(BINARYNAME)

all: always $(BINARY)

$(BINARY): $(BUILDDIR)/$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BINARY) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(DEFINES) -c $< -o $@

deps:
	git clone https://github.com/redis/hiredis.git build/hiredis
	make -C build/hiredis USE_SSL=1

always:
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/platform
	mkdir -p $(BUILDDIR)/database

clean:
	rm -rf $(BUILDDIR)/platform
	rm -rf $(BUILDDIR)/database
	rm -rf $(BINARY)
	rm -rf $(OBJ)

destroy:
	rm -rf $(BUILDDIR)
