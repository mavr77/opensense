# make the opensense blacklist logic modules
#

# ROOT for other user to not be able to modify change or delete.
OWNER=root
GROUP=root
CC=gcc
CFLAGS=-Wall -shared -fPIC
# user the dynamic lib
LIBS=-lsqlite3
SBINDIR=/usr/sbin

all: mod_db.so mod_txt.so

mod_db.so:
		$(CC) $(CFLAGS) mod_db.c -o $@ $(LIBS)
mod_txt.so: # this will be $@
		$(CC) $(CFLAGS) mod_txt.c -o $@ # <- this is it

clean:
		rm -f *.o *.so
