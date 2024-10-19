#ifndef lint
static const char rcsid[] =
	"$Id: settitle.c,v 1.5 2005/07/07 14:04:31 andrew Exp $";
#endif

#include <sys/param.h>

#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sysexits.h>

#include "settitle.h"

/* OS portability */
#ifndef HAVE_PROGNAME
#include "progname.h"
#endif

static void usage(void);

int main(int argc, char *argv[]) {
	int c;
	char *both_title,
		 *window_title,
		 *icon_title;

	/* initialise progname for later possible use by usage */
	setprogname(argv[0]);

	/* note that these aren't yet set */
	both_title = window_title = icon_title = NULL;

	/* process switches */
	while ((c = getopt(argc, argv, "i:t:")) != -1) {
		switch (c) {
			case 'i':
				/* set icon name */
				icon_title = optarg;
				break;
			case 't':
				/* set window title */
				window_title = optarg;
				break;
			case '?':
			default:
				usage();
				break;
		}
	}
	argc -= optind;
	argv += optind;

	/* if one argument is given then it is the both_title */
	if (argc == 1) {
		both_title = argv[0];
	} else if (argc > 1) {
		usage();
	}

	/* check we haven't been asked for both and one */
	if (((icon_title != NULL) || (window_title != NULL)) &&
			(both_title != NULL)) {
		usage();
	}

	/* if no titles have been specified then we use the hostname as the
	   both_title */
	if ((icon_title == NULL) && (window_title == NULL) && (both_title == NULL)) {
		if (! getshorthostname(&both_title)) {
			err(EX_UNAVAILABLE, "getshorthostname");
		}
	}

	/* do the setting */
	if (icon_title != NULL) {
		(void)printf("%c]1;%s%c", ESC, icon_title, BEL);
	}

	if (window_title != NULL) {
		(void)printf("%c]2;%s%c", ESC, window_title, BEL);
	}

	if (both_title != NULL) {
		(void)printf("%c]0;%s%c", ESC, both_title, BEL);
	}

	/* we haven't freed both_title - and we have no way of knowing if we
	   should have but it doesn't matter as we are about to exit */
	return 0;
}

int control(int c) {

	return(c - '@');
}

static void usage(void)  {

	fprintf(stderr, "usage: %s [-i icon_title] [-t window_title] [icon_and_window_title]\n", getprogname());
	exit(EX_USAGE);
}

/* get the short hostname */
int getshorthostname(char **hostname) {
	char *domain_separator;
	long maxhostname;
	
	/* find how long a hostname can be */
#ifdef _SC_HOST_NAME_MAX
	/* we can use sysconf to get the value */
	errno = 0;
	if ((maxhostname = sysconf(_SC_HOST_NAME_MAX)) == -1) {
		if (errno != 0) {
			/* sysconf failed for some reason but we shouldn't give up */
			warn("sysconf");
		}
#else
	{
#endif
		/* sysconf isn't available so use a compiled in default */
		maxhostname = (long)MAXHOSTNAMELEN;
	}

	/* allocate some space - this may be dodgy on platforms (reportedly such
	   as HURD) that can have unlimited hostnames but it will do for now */
	if ((*hostname = (char *)malloc((size_t)maxhostname + 1)) == NULL) {
		err(EX_UNAVAILABLE, "malloc");
	}

	/* fetch the hostname */
	if (gethostname(*hostname, (size_t)(maxhostname + 1)) == -1) {
		err(EX_UNAVAILABLE, "gethostname");
	}

	/* truncate at the first . */
	if ((domain_separator = strchr(*hostname, '.')) != NULL) {
		*domain_separator = '\0';
	}

	return 1;
}
