#include "opensense.h"

void n2h2_alive(int fd, struct n2h2_req *n2h2_request)
{
  struct n2h2_resp n2h2_resp_alive;
  n2h2_resp_alive.code = htons(N2H2_ALIVE_RESP);
  n2h2_resp_alive.serial = n2h2_request->serial;
  n2h2_resp_alive.unknown = htons(0);
  n2h2_resp_alive.urlsize = htons(0);

  printf("%d\n", (int)send(fd, &n2h2_resp_alive, N2H2_HDR, 0));

  // n2h2_resp_alive.code = htons(N2H2_ALIVE_RESP);
  // n2h2_resp_alive.serial =
  // n2h2_resp_alive.srcip =
  // n2h2_resp_alive.dstip =
  // n2h2_resp_alive.urlsize =
  // n2h2_resp_alive.usrsize =

}

void n2h2_accept(int fd, struct n2h2_req *n2h2_request)
{

}

void n2h2_deny(int fd, struct n2h2_req *n2h2_request, char *redirect_url)
{

}
