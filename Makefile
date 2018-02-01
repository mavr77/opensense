# make the opensense server
#
OWNER=root
GROUP=root
CC=gcc
CFLAGS=-O2 -Wall
#LIBS=-ldb
SBINDIR=/usr/sbin

all:	opensense

run:	clean opensense
		./opensense

opensense:
		$(CC) $(CFLAGS) opensense.c n2h2.c -o $@ $(LIBS)

install:	opensense
		install -o $(OWNER) -g $(GROUP) -m 755 opensense $(SBINDIR)
#		install -o $(OWNER) -g $(GROUP) -m 755 -d /var/cache/opensense

clean:
		rm -f opensense


