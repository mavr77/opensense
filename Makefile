# make the opensense server
#
OWNER=root
GROUP=root
CC=gcc
# rdymanic for dynamic lib linker, dont worry if something missing at compile time
CFLAGS=-O2 -Wall -rdynamic
# user the dynamic lib
LIBS=-ldl -lconfig
SBINDIR=/usr/sbin

all:	opensense

run:	clean opensense
		./opensense -c opensense.cfg

opensense:
		$(CC) $(CFLAGS) opensense.c n2h2.c -o $@ $(LIBS)

install:	opensense
		install -o $(OWNER) -g $(GROUP) -m 755 opensense $(SBINDIR)

clean:
		rm -f opensense


