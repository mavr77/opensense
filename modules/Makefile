# make the opensense server
#

gcc -Wall -fPIC -c *.c
gcc -shared -Wl -o libctest.so.1.0 -lsqlite3 *.o

# OWNER=root
# GROUP=root
# CC=gcc
# # rdymanic for dynamic lib linker, dont worry if something missing at compile time
# CFLAGS=-O2 -Wall -rdynamic
# # user the dynamic lib
# LIBS=-ldl
# SBINDIR=/usr/sbin

# all:	opensense

# run:	clean opensense
# 		./opensense

# opensense:
# 		$(CC) $(CFLAGS) opensense.c n2h2.c -o $@ $(LIBS)

# install:	opensense
# 		install -o $(OWNER) -g $(GROUP) -m 755 opensense $(SBINDIR)
# #		install -o $(OWNER) -g $(GROUP) -m 755 -d /var/cache/opensense

# clean:
# 		rm -f opensense
