# make the opensense server
#

#gcc -Wall -fPIC -c *.c
#gcc -shared -Wl -o libctest.so.1.0 -lsqlite3 *.o

#gcc -shared -o libhello.so -fPIC hello.c

OWNER=root
GROUP=root
CC=gcc
# rdymanic for dynamic lib linker, dont worry if something missing at compile time
CFLAGS=-Wall -shared -fPIC
# user the dynamic lib
LIBS=-lsqlite3
SBINDIR=/usr/sbin



mod_db.so:
		$(CC) $(CFLAGS) mod_db.c -o $@ $(LIBS)

#install:	opensense
#		install -o $(OWNER) -g $(GROUP) -m 755 opensense $(SBINDIR)
#		install -o $(OWNER) -g $(GROUP) -m 755 -d /var/cache/opensense

clean:
		rm -f *.o *.so
