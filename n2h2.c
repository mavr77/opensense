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

struct uf_request n2h2_validate(struct n2h2_req *n2h2_request, int msgsize)
{

  struct uf_request request = {
    0, {0}, {0}, "", ""
  };

  struct in_addr srcip, dstip;

  request.type = UNKNOWN;

  if(msgsize == N2H2_ALIVE_SIZE && htons(n2h2_request->code) == N2H2_ALIVE)
  {
    request.type = N2H2_ALIVE;
    return request;
  }

  if(msgsize > N2H2_REQ_SIZE && htons(n2h2_request->code) == N2H2_REQ && htons(n2h2_request->urlsize) < URL_SIZE)
  {
    request.type = N2H2_REQ;
    srcip.s_addr = n2h2_request->srcip;
    dstip.s_addr = n2h2_request->dstip;
    snprintf(request.srcip, sizeof(request.srcip), "%s", inet_ntoa(srcip)); // inet_ntoa ipv4 representation
    snprintf(request.dstip, sizeof(request.dstip), "%s", inet_ntoa(dstip));
    for(int i = 0; i < ntohs(n2h2_request->urlsize); i++)
      request.url[i] = n2h2_request->url[i];
    return request;
  }
  return request;
}

void n2h2_deny(int fd, struct n2h2_req *n2h2_request, char *redirect_url)
{
  int urlsize = 0;
  int i; // counter for url bytes copy
  struct n2h2_resp n2h2_resp_deny;
  n2h2_resp_deny.code = htons(N2H2_REQ_DENY);
  n2h2_resp_deny.serial = n2h2_request->serial;
  n2h2_resp_deny.unknown = htons(0);
  if(redirect_url != NULL){
    urlsize = strlen(redirect_url) + 1;
    if(urlsize < URL_SIZE){
      n2h2_resp_deny.urlsize = htons(urlsize);
      for(i=0;i<urlsize;i++){
        n2h2_resp_deny.url[i] = redirect_url[i]; // strncpy
      }
    } // no nwed to check for urlsize > URL_SIZE cause it needs to send response anyways and it will fill it in with zeros which is expected reply
  }
  printf("%d\n", (int)send(fd, &n2h2_resp_deny, N2H2_HDR + urlsize, 0));
}
