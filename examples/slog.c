#define PROGRAM_NAME ""
#include "../errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

char const HELP[] =
    "Usage: slog [-f LOG_FILE][-p PROGRAM_NAME] [<level>] <msg>"      "\n"
    ""                                                              "\n"
    "Print a log message if <level> is smaller that LOG_LEVEL. The" "\n"
    "following log levels are defined:"                             "\n"
    ""                                                              "\n"
    "    0: emerg*  2: crit*  4: warning           6: info"         "\n"
    "    1: alert   3: err*   5: notice (default)  7: debug"        "\n"
    ""                                                              "\n"
    "A level can be spefied by name or number. Also the following"  "\n"
    "environment variables are respeced: LOG_LEVEL, LOG_FILE"       "\n";
    

int
main(int _argc, char *_argv[])
{
	char   buffer[1024*5];
	size_t bufpos = 0;
	int    priority = -1, opt;
	char  *env;

	if (_argc > 1 && (!strcmp(_argv[1], "-h") || !strcmp(_argv[1], "--help"))) {
		fputs(HELP, stdout);
		return 1;
	}
	
	while((opt = getopt (_argc, _argv, "f:p:")) != -1) {
		switch (opt) {
		case 'f':
			log_file = fopen(optarg, "a");
			if (!log_file) { perror("fopen"); return 1; }
			break;
		case 'p':
			log_program_name = optarg;
			break;
		case '?':
		default:
			return 1;
		}
	}

	if (optind < _argc && (priority = strprio(_argv[optind])) != -1) {
		optind++;
	} else {
		priority = LOG_NOTICE;
	}

	for (int first = 1; optind < _argc; optind++, first = 0) {
		bufpos += snprintf(
		    buffer + bufpos, sizeof(buffer) - bufpos,
		    "%s%s",
		    (first)?"":" ",
		    _argv[optind]
		);
	}

	if ((env = getenv("LOG_LEVEL"))) {
		log_priority = strprio(env);
		if (log_priority == -1) {
			fprintf(stderr, "Invalid log level: %s\n", env);
			return 1;
		}
	}

	if ((env = getenv("LOG_FILE"))) {
		log_file = fopen(env, "a");
		if (!log_file) { perror("fopen"); return 1; }
	}

	slog(priority, buffer);
	return 0;
}
