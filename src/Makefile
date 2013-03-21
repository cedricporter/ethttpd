
top=../

ethttpd_SOURCES=$(wildcard *.c)
ethttpd_OBJ=$(patsubst %.c, %.o, $(wildcard *.c))

CFLAGS=-I ../include -g -Wall
CC=gcc
C_COMPILE=$(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
C_DEPENDENCIES=-Wp,-MD,.deps/$(*F).P
C_LINK=$(CC) $(CFLAGS) $(LDFLAGS) -L.

top_builddir = ..

DEP_FILES=$(wildcard .deps/*.P)

all: ethttpd

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)
-include $(DEP_FILES)

%.o: %.c
	@echo '$(C_COMPILE) -c $<'; \
	$(C_COMPILE) $(C_DEPENDENCIES) -o $@ -c $<

ethttpd: $(ethttpd_OBJ)
	$(C_LINK) -o $@ $^ $(LDDEPS)

clean:
	rm -f *.mod *.o *.obj .deps/*.P .lo

print-info:
	@echo "objs: " $(ethttpd_OBJ)
	@echo "srcs: " $(ethttpd_SOURCES)
	@echo "deps: " $(DEP_FILES)

check-syntax:
	gcc $(CFLAGS) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES) 


# End of Makefile
