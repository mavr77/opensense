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

int startmain(void)
{
	int opensense_fd;
  int local_port = 0;
	struct sockaddr_in opensense_addr;

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

  for(;;){
  	sleep(3);
  	exit(1);
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
	int c;	// var for options
	int daemon_flag = 0;
	char *config;
	pid_t d_pid, sid;

	/* getoptlong ---- for long key */
	while ((c = getopt(argc, argv, "c:vd")) != -1)
    switch(c)
      {
      case 'd':
        daemon_flag = 1;
        break;
      case 'c':
        config = optarg;
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
        return 1;
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
