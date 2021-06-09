TARGET_BASE = recno
TARGET = lib${TARGET_BASE}

# BSD doesn't recognize pattern matches (with %),
# so if also target BSD, you'll have to use .SUFFIXES
.SUFFIXES:

# File locations
PREFIX ?= /usr/local
SRC = src

# Compiler flags
CFLAGS = -Wall -Werror -std=c99 -pedantic -m64 -ggdb
LDFLAGS = 

# CFLAGS modifications
CFLAGS += -Wpadded

# Save the accumulated CFLAGS value before adding library-building options
CFLAGS_LINK := ${CFLAGS}

CFLAGS += -fPIC                # relocatable function addresses for use in libraries
CFLAGS += -fvisibility=hidden  # restrict access to explicitely-revealed functions

# default rule should come before _most_, if not all, includes:
all: ${TARGET}.so ${TARGET}.a

# Create list of prerequisites for building the TARGET:
MODULES != ls -1 ${SRC}/*.c | grep -v  ^${SRC}/test_ | sed 's/\.c/.o/g'

TESTS != ls -1 ${SRC}/*.c | grep ^${SRC}/test_ | sed 's/\.c//g'
LINKS != ls -1 ${SRC}/*.c | grep ^${SRC}/link_ | sed 's/\.c//g'



#########
# Includes here in case they prompt changes to MODULES
#########

${TARGET}.a : ${MODULES}
	ar rcs $@ ${MODULES}

${TARGET}.so : ${MODULES}
	${CC} --shared -o $@ ${MODULES} ${LDFLAGS}

# Remove duplicates that may have crept in:
MODULES != echo ${MODULES} | xargs -n1 | sort -u | xargs

# Project build rules

${TARGET} : ${MODULES}
	${CC} -o $@ ${MODULES} ${LDFLAGS}

# GNU Make pattern matching.  This won't be matched in BSD.
%o: %c
	${CC} ${CFLAGS} -c -o $@ $<

test_% : test_%.c 
	${CC} ${CFLAGS} -o $@ $<

link_% : link_%.c ${TARGET}.so
	${CC} ${CFLAGS_LINK} -o $@ $< ./${TARGET}.so  /usr/local/lib/libreadargs.a

# Other project rules:

install:
	install -D --mode=755 ${TARGET} ${PREFIX}/bin

clean:
	rm -f ${TARGET}.a
	rm -f ${TARGET}.so
	rm -f ${SRC}/*.o
	rm -f ${TESTS}
	rm -f ${LINKS}

tests: ${TESTS}

links: ${LINKS}

show:
	@echo CFLAGS is ${CFLAGS}
	@echo CFLAGS_LINK is ${CFLAGS_LINK}
	@echo MODULES is ${MODULES}
	@echo TEST is ${TESTS}
