# make the opensense server
#
OWNER=root
GROUP=root
CC=gcc
CFLAGS=-O2 -Wall
#LIBS=-ldb
SBINDIR=/usr/sbin

all:	opensense

opensense:
		$(CC) $(CFLAGS) opensense.c -o $@ $(LIBS)

install:	opensense
		install -o $(OWNER) -g $(GROUP) -m 755 opensense $(SBINDIR)
#		install -o $(OWNER) -g $(GROUP) -m 755 -d /var/cache/openufp

clean:
		rm -f opensense