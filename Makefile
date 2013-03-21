
CPPFLAGS=-I include

CC=gcc
C_COMPILE=$(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
# C_DEPENDENCIES=-Wp,-MD,src/.deps/$(*F).P
C_LINK=$(CC) $(CFLAGS) $(LDFLAGS) -L.

DEP_FILES=main.P
VPATH=src

ETHTTPD_OBJ=$(shell echo src/*.c)

all: ethttpd

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)
-include $(DEP_FILES)

build/%.o: %.c
	@echo '$(C_COMPILE) -c $<';
	$(C_COMPILE) -MM -MF '$(*F).P' -MT '$@' $< 
	$(C_COMPILE) -o $@ -c $<

ethttpd: build/main.o
	$(C_LINK) -o $@ $^ $(LDDEPS)

clean:
	rm -f ethttpd
	rm -f build/*
	rm -f *.mod *.o *.obj .deps/*.P .lo
	rm -f src/.deps/*.P

check-syntax:
        gcc $(CPPFLAGS) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES) 
