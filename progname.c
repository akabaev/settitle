#ifndef lint
static const char rcsid[] =
	"$Id: progname.c,v 1.2 2002/04/20 14:48:39 andrew Exp $";
#endif

/*
 * Implementation of {get,set}progname for OSs without it.
 */

#include <stddef.h>
#include <string.h>

#include "progname.h"

static const char *internal_name;

const char *getprogname(void) {

	return internal_name;
}

void setprogname(const char *progname) {
	const char *basename;

	if ((basename = strrchr(progname, '/')) == NULL) {
		internal_name = progname;
	} else {
		internal_name = basename + 1;
	}
}
