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
  for(;;){
  	printf("%s\n", "str");
  	sleep(3);
  	exit(1);
  }
}

// void printsomething(char []str)
// {
// 	printf("%s\n", str);
// }

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