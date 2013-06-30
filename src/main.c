#include <config.h>
#include <microhttpd.h>
#include <sysexits.h>
#include "signal_flag.h"
#include "logger.h"
#include "answer.h"

int main(int argc, char** argv)
{
	struct MHD_Daemon* daemon;

	/* TODO: get command line args */
	enable_debug = ENABLE_DEBUG;
	port = WEBSERVER_PORT;

	start_logger();
	log_debug("Logger has been started");

	/* Start the signal handlers. */
	enable_signal_flag(SIGTERM);
	enable_signal_flag(SIGINT);
	log_debug("Signal flaggers have been started");


	if (enable_debug) {
		daemon_opts = MHD_USE_DEBUG | MHD_USE_THREAD_PER_CONNECTION;
		log_debug("Debug mode will be enabled for microhttpd");
	} else {
		daemon_opts = MHD_USE_THREAD_PER_CONNECTION;
	}

	connected_cb = NULL;
	connected_cb_arg = NULL;
	answer_cb = &answer;
	answer_cb_arg = NULL;
	completed_cb = NULL;
	completed_cb_arg = NULL;
	log_cb = &log_callback;
	log_cb_arg = NULL;

	log_info("Starting daemon");
	daemon = MHD_start_daemon(
			daemon_opts,
			port,
			connected_cb,
			connected_cb_arg,
			answer_cb,
			answer_cb_arg,
			MHD_OPTION_NOTIFY_COMPLETED,
			completed_cb,
			completed_cb_arg,
			MHD_OPTION_EXTERNAL_LOGGER,
			log_cb,
			log_cb_arg,
			MHD_OPTION_END);
	if (daemon == NULL) {
		log_crit("Unable to start daemon");
		/* TODO: any cleanup */
		stop_logger();
		return EX_SOFTWARE;
	}
	
	log_notice("Daemon started");

	while (sleep_until_signal_flag()
			&& signal_flag_count(SIGTERM) > 0
			&& signal_flag_count(SIGINT) > 0) {
		log_debug("Flag received");
		/* do something? */
		reset_all_signal_flag_counts();
	}

	log_info("Stopping daemon");
	MHD_stop_daemon(daemon);
	/*TODO: any cleanup */
		
	log_notice("Daemon stopped");
	disable_all_signal_flags();
	stop_logger();
	return EX_OK;
}

