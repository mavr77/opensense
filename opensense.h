#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <getopt.h>

#include "n2h2.h"

#define VERSION "0.1"
#define REQ_SIZE 100
