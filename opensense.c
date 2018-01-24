#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>


int main(int argc, char **argv)
{
	int c;	// var for options
	int daemon_flag = 0;
	char *config;
	/* getoptlong ---- for long key */
	while ((c = getopt (argc, argv, "c:p:vd")) != -1)

	    switch (c)
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
	        else if (isprint (optopt))
	          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	        else
	          fprintf (stderr,
	                   "Unknown option character `\\x%x'.\n",
	                   optopt);
	        return 1;
	      default:
	        abort ();
	      }
	printf("%s\n", config);
	printf("%d\n", daemon_flag);


	return 0;
}