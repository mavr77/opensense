#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
  	// printsomething("Start main");
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
	int status;
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

	printf("%d\n", daemon_flag);
	// CHANGE IT TO 1 Once it start working
  if(daemon_flag == 0) {
  	d_pid = fork();
  	printf("%d\n", d_pid);
  	if(d_pid < 0) {
  		exit(1);
  	} else if(d_pid == 0) {
  		chdir("/");
  		umask(0);
  		close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			sid = setsid();
			signal(SIGHUP, signal_handler); // write tests for signal handling;
  		startmain();
  	} else {
  		printf("%s\n", "Parent waiting");
  		waitpid(d_pid, &status, 0);
  		exit(0);
  	}
 	} else {
 		printf("startmain in main thread when it works");
 		// startmain(); // ::startmain();
 	}

	printf("%s\n", config);
	printf("%d\n", daemon_flag);

	return 0;
}
