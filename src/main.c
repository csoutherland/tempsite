#include <config.h>
#include <microhttpd.h>
#include <sysexits.h>
#include <syslog.h>
#include <signal.h>
#include <stdarg.h>
#include "signal_flag.h"
#include "answer.h"

int main()
{
	struct MHD_Daemon* daemon;

	openlog("stuph_temp_httpd", LOG_PERROR | LOG_PID, LOG_DAEMON);
	syslog(LOG_DEBUG, "Logger has been started");

	/* Start the signal handlers. */
	enable_signal_flag(SIGTERM);
	enable_signal_flag(SIGINT);
	syslog(LOG_DEBUG, "Signal flaggers have been started");


	syslog(LOG_DEBUG, "Debug mode will be enabled for microhttpd");

	syslog(LOG_INFO, "Starting daemon");
	daemon = MHD_start_daemon(
			MHD_USE_DEBUG | MHD_USE_THREAD_PER_CONNECTION,
			8080,
			NULL,
			NULL,
			&answer,
			NULL,
			MHD_OPTION_EXTERNAL_LOGGER,
			&vsyslog,
			LOG_DEBUG,
			MHD_OPTION_END);
	if (daemon == NULL) {
		syslog(LOG_CRIT, "Unable to start daemon");
		/* TODO: any cleanup */
		closelog();
		return EX_SOFTWARE;
	}
	
	syslog(LOG_NOTICE, "Daemon started");

	while (sleep_until_signal_flag()
			&& signal_flag_count(SIGTERM) > 0
			&& signal_flag_count(SIGINT) > 0) {
		syslog(LOG_DEBUG, "Flag received");
		/* do something? */
		reset_all_signal_flag_counts();
	}

	syslog(LOG_INFO, "Stopping daemon");
	MHD_stop_daemon(daemon);
	/*TODO: any cleanup */
		
	syslog(LOG_NOTICE, "Daemon stopped");
	disable_all_signal_flags();
	closelog();
	return EX_OK;
}

