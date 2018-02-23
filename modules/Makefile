# make the opensense blacklist logic modules
#

# WHY ROOT
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
mod_txt.so:
		$(CC) $(CFLAGS) mod_txt.c -o $@ # <- whats this

clean:
		rm -f *.o *.so
