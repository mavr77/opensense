#include "opensense.h"

char *blacklist_filename;
char *blacklist_lib_filename;
int local_port = 4005;

int check_access(struct uf_request req)
{
  void *lib_handle;
  int (*blacklist)(char *, char[URL_SIZE]);
  char *error;
  int return_code = 0;

  // loadind needed lib source file/code
  lib_handle = dlopen(blacklist_lib_filename, RTLD_LAZY);
  if (!lib_handle)
  {
    fprintf(stderr, "%s\n", dlerror());
    exit(1);
  }

  // symlinking function from the needed lib that we loaded in dlopen function above.
  blacklist = dlsym(lib_handle, "blacklist_logic");
  if ((error = dlerror()) != NULL)
  {
    fprintf(stderr, "%s\n", error);
    exit(1);
  }

  return_code = blacklist(blacklist_filename, req.url);
  dlclose(lib_handle);
  return return_code;
}

int startmain(void)
{
  int opensense_fd;
  struct sockaddr_in opensense_addr;
  pid_t connection_pid;

  opensense_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(opensense_fd < 0) {
    printf("%s\n", "Failed to open socket");
  }

  int optval = 1;
  if (setsockopt(opensense_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
      close(opensense_fd);
      printf("opensense v%s: setsockopt failed.\n", VERSION);
      exit(1);
  }

  bzero(&opensense_addr, sizeof(opensense_addr));

  opensense_addr.sin_family = AF_INET;
  opensense_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  opensense_addr.sin_port = htons(local_port);
  if (bind(opensense_fd, (struct sockaddr *)&opensense_addr, sizeof(opensense_addr)) < 0) {
    close(opensense_fd);
    printf("opensense v%s: bind failed.\n", VERSION);
    exit(1);
  }

  if (listen(opensense_fd, 4096) < 0) {
    close(opensense_fd);
    printf("opensense v%s: listen failed.\n", VERSION);
    exit(1);
  }

  struct sockaddr_in cli_addr;
  socklen_t cli_size;
  int cli_fd;

  for(;;){
    cli_size = sizeof(cli_addr);
    if((cli_fd = accept(opensense_fd, (struct sockaddr *)&cli_addr, &cli_size)) < 0){
      printf("opensense v%s: failed to accept connection.\n", VERSION);
      exit(1);
    }

    if ((connection_pid = fork()) == 0) {

      struct n2h2_req *n2h2_request = NULL;
      struct uf_request request;

      close(opensense_fd);

      char msg[REQ_SIZE]; // this where we will recieve the whole packet
      int msgsize = 0;

      for(;;){
        bzero(&msg, sizeof(msg));
        msgsize = recvfrom(cli_fd, msg, REQ_SIZE, 0, (struct sockaddr *)&cli_addr, &cli_size);
        if(msgsize == 0){
          printf("opensense msg: %s\n", "connection has been closed");
          close(cli_fd);
          exit(1);
        } else if(msgsize < 0) {
          printf("opensense msg: %s\n", "receiving data has failed");
          close(cli_fd);
          exit(1);
        }

        printf("opensense msg: %s\n", msg);
        n2h2_request = (struct n2h2_req *)msg;
        request = n2h2_validate(n2h2_request, msgsize);
        if(request.type == UNKNOWN)
        {
          close(cli_fd);
          printf("Request UNKNOWN");
          exit(1);
        }

        if(request.type == N2H2_ALIVE)
        {
          printf("receive alive packet, replying on it");
          n2h2_alive(cli_fd, n2h2_request);
        }
        if(request.type == N2H2_REQ)
        {
          if(check_access(request))
          {
            n2h2_deny(cli_fd, n2h2_request, "http://google.com");
            printf("receive req packet, denied it");
          } else {
            n2h2_accept(cli_fd, n2h2_request);
            printf("receive req packet from: %s, accepted it", request.url);
          }
        }

      }
    }
    close(cli_fd);
    printf("%s\n", "main thread close cli_fd");
  }

}

void signal_handler(int sig)
{
  switch(sig)
  {
    case SIGHUP:
      syslog(LOG_WARNING, "Received SIGHUP signal.");
      break;
    case SIGINT:
    case SIGTERM:
      syslog(LOG_INFO, "Daemon exiting");
      // daemonShutdown();
      exit(EXIT_SUCCESS);
      break;
    default:
      syslog(LOG_WARNING, "Unhandled signal %s", strsignal(sig));
      break;
  }
}

int main(int argc, char **argv)
{
  int c;  // var for options
  int daemon_flag = 0;
  char *config;
  pid_t d_pid, sid;

  config_t cfg;
  config_setting_t *setting;
  char *cfg_param;
  char *engine;

  /* getoptlong ---- for long key */
  static struct option longopts[] = {
    { "config",  required_argument, NULL,        'c' },
    { "help",    no_argument,       NULL,        'h' },
    { "verbose", no_argument,       NULL,        'v' },
    { "daemon" , no_argument,       NULL,         1  },
    { 0, 0, 0, 0 }
  };

  while ((c = getopt_long(argc, argv, "c:vd", longopts, NULL)) != -1)
    switch(c)
      {
      case 'd':
        daemon_flag = 1;
        break;
      case 'c':
        config = optarg;
        config_init(&cfg);
        if(! config_read_file(&cfg, config))
        {
          fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                  config_error_line(&cfg), config_error_text(&cfg));
          config_destroy(&cfg);
          return(EXIT_FAILURE);
        }
        /* Get the store name. */
        if(config_lookup_int(&cfg, "port", &local_port)) {
          printf("Local port: %d\n", local_port);
        } else {
          fprintf(stderr, "No 'port' setting in configuration file.\n");
          return(EXIT_FAILURE);
        }

        if(config_lookup_string(&cfg, "engine", &cfg_param)) {
          printf("Engine: %s\n\n", cfg_param);
          engine = cfg_param;
          setting = config_lookup(&cfg, engine);
          if(setting != NULL) {
            if(!config_setting_lookup_string(setting, "module", &blacklist_lib_filename)) {
              fprintf(stderr, "No 'module file' setting in configuration file.\n");
              return(EXIT_FAILURE);
            }
            if(!config_setting_lookup_string(setting, "blacklist", &blacklist_filename)) {
              fprintf(stderr, "No 'blacklist filename' setting in configuration file.\n");
              return(EXIT_FAILURE);
            }
            printf("lib & file: %s\n%s\n", blacklist_lib_filename, blacklist_filename);
          }
        } else {
          fprintf(stderr, "No 'engine' setting in configuration file.\n");
          return(EXIT_FAILURE);
        }


        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return EXIT_FAILURE;
      default: // it will nevet get to default.
        abort();
      }

  if(daemon_flag == 1) {
    // Because this main process should just launch the fork and quit
    // if child will send something to parent and that something wont get processed
    // then it will become zombie process, so we need to ignore fork signals.
    signal(SIGCHLD, SIG_IGN);
    if((d_pid = fork()) == 0) {
      printf("%s\n", "Started...");
      chdir("/");
      umask(0);
      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      close(STDERR_FILENO);
      sid = setsid();
      signal(SIGHUP, signal_handler); // write tests for signal handling;
      startmain();
    }
    if(d_pid == -1) {
      printf("%s\n", "Daemon failed to launch");
      exit(1);
    }
  } else {
    startmain();
  }

  printf("%s\n", config);
  printf("%d\n", daemon_flag);

  return 0;
}
