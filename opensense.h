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
#define URL_SIZE 65535
#define REQ_SIZE 65535

// request struct and defs
#define UNKNOWN 65535

struct uf_request {
    uint16_t type;
    char srcip[16];
    char dstip[16];
    char url[URL_SIZE];
    char usr[URL_SIZE];
};
