CC=gcc
CFLAGS=-Wall -Wextra -pedantic -g -ggdb2
DEFINES=
LIBS=

PLATFORM=$(shell ./platform.sh)

SRCDIR=src
BUILDDIR=build

TESTDIR=test
BUILDTESTDIR=$(BUILDDIR)/test

SRC=$(wildcard $(SRCDIR)/*.c)
TEST=$(wildcard $(TESTDIR)/*.c)
TESTOSRC=

ifeq ($(PLATFORM),Pi)
	CFLAGS += -DPLATFORM_RPI -DDATABASE_REDIS -Ibuild
	LIBS += -lwiringPi -Lbuild/hiredis -l:libhiredis.a -l:libhiredis_ssl.a -lssl -lcrypto
	SRC += $(SRCDIR)/database/database_redis.c
	SRC += $(SRCDIR)/platform/platform_rpi.c
	TEST += $(TESTDIR)/test_platform_rpi.c
	TESTOSRC += $(SRCDIR)/database/database_redis.c
	TESTOSRC += $(SRCDIR)/platform/platform_rpi.c
else
	CFLAGS += -DPLATFORM_GENERIC -DDATABASE_REDIS -Ibuild
	LIBS += -Lbuild/hiredis -l:libhiredis.a -l:libhiredis_ssl.a -lssl -lcrypto
	SRC += $(SRCDIR)/database/database_redis.c
	SRC += $(SRCDIR)/platform/platform_generic.c
	TEST += $(TESTDIR)/test_platform_generic.c
	TESTOSRC += $(SRCDIR)/database/database_redis.c
	TESTOSRC += $(SRCDIR)/platform/platform_generic.c
endif

OBJ=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC))
TESTOBJ=$(patsubst $(TESTDIR)/%.c,$(BUILDTESTDIR)/%,$(TEST))

BINARYNAME=ased
BINARY=$(BUILDDIR)/$(BINARYNAME)

all: always $(BINARY)

$(BINARY): $(BUILDDIR)/$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BINARY) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(DEFINES) -c $< -o $@

$(TESTOBJ): always

$(BUILDTESTDIR)/%: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -I$(SRCDIR) $(DEFINES) $(TESTOSRC) $< -o $@ $(LIBS)

test: $(TESTOBJ)
	sh test.sh

deps:
	git clone https://github.com/redis/hiredis.git build/hiredis
	make -C build/hiredis USE_SSL=1

always:
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/platform
	mkdir -p $(BUILDDIR)/database
	mkdir -p $(BUILDTESTDIR)

clean:
	rm -rf $(BUILDDIR)/platform
	rm -rf $(BUILDDIR)/database
	rm -rf $(BUILDTESTDIR)
	rm -rf $(BINARY)
	rm -rf $(OBJ)

destroy:
	rm -rf $(BUILDDIR)
