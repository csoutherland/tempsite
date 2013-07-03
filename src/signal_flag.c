#include <config.h>
#include "signal_flag.h"
#include <signal.h>
#include <stdlib.h>

typedef struct _signal_flag_struct_ {
	int signum;
	unsigned int count;
	struct sigaction old;
	struct sigaction current;
	struct _signal_flag_struct_* next;
} * signal_flag_t;

static int total_count = 0;
static signal_flag_t flags = NULL;

void signal_handler(int signum)
{
	signal_flag_t tflag = flags;

	while (tflag != NULL) {
		if (tflag->signum == signum) {
			tflag->count++;
			total_count++;
			return;
		} else {
			tflag = tflag->next;
		}
	}
}

int enable_signal_flag(const int signum)
{
	signal_flag_t nflag = flags;
	int errcode = 0;

	/* Make sure the signal isn't one that cannot be caught. */
	if (signum == SIGKILL || signum == SIGSTOP) {
		return SIGNAL_FLAG_DISALLOWED;
	}

	/* Check to see if the signal flag already exists. */
	while (nflag != NULL) {
		if (nflag->signum == signum) {
			return SIGNAL_FLAG_PRESENT;
		} else {
			nflag = nflag->next;
		}
	}
	
	/* Create a new flag. */
	nflag = (signal_flag_t)malloc(sizeof(struct _signal_flag_struct_));
	if (nflag == NULL) {
		return SIGNAL_FLAG_NOMEM;
	}

	/* Populate the new flag. */
	nflag->signum = signum;
	nflag->count = 0;
	nflag->next = flags;

	/* Put the new flag at the front. */
	flags = nflag;

	/* Prepare the new sigaction struct. */
	nflag->current.sa_handler = &signal_handler;
	sigemptyset(&(nflag->current.sa_mask));
	nflag->current.sa_flags = 0;

	/* Enable the signal handler. */
	errcode = sigaction(signum, &nflag->current, &nflag->old);

	if (errcode < 0) {
		flags = flags->next;
		free(nflag);
		return SIGNAL_FLAG_DISALLOWED;
	} else {
		return SIGNAL_FLAG_OKAY;
	}
}

int sleep_until_signal_flag()
{
	sigset_t emptyset;
	sigemptyset(&emptyset);

	if (total_count > 0) {
		return total_count;
	}

	sigsuspend(&emptyset);

	return total_count;
}

int signal_flag_count(const int signum)
{
	signal_flag_t tflag = flags;

	while (tflag != NULL) {
		if (tflag->signum == signum) {
			return tflag->count;
		} else {
			tflag = tflag->next;
		}
	}

	return SIGNAL_FLAG_NOT_PRESENT;
}

int lower_signal_flag_count(const int signum)
{
	signal_flag_t tflag = flags;

	while (tflag != NULL) {
		if (tflag->signum == signum) {
			if (tflag->count > 0) {
				tflag->count--;
				total_count--;
			}
			return tflag->count;
		} else {
			tflag = tflag->next;
		}
	}

	return SIGNAL_FLAG_NOT_PRESENT;
}

int reset_signal_flag_count(const int signum)
{
	signal_flag_t tflag = flags;

	while (tflag != NULL) {
		if (tflag->signum == signum) {
			int count = tflag->count;
			if (tflag->count > 0) {
				tflag->count = 0;
				total_count -= count;
			}
			return count;
		} else {
			tflag = tflag->next;
		}
	}

	return SIGNAL_FLAG_NOT_PRESENT;
}

int reset_all_signal_flag_counts()
{
	signal_flag_t tflag = flags;
	int count = 0;

	if (flags == NULL) {
		return SIGNAL_FLAG_NOT_PRESENT;
	}

	while (tflag != NULL) {
		if (tflag->count > 0) {
			tflag->count = 0;
		}
		tflag = tflag->next;
	}

	count = total_count;
	total_count = 0;
	return count;
}

int disable_signal_flag(const int signum)
{
	signal_flag_t tflag = flags;

	if (tflag->signum == signum) {
		int count = 0;
		sigaction(signum, &tflag->old, NULL);
		flags = tflag->next;
		count = tflag->count;
		free(tflag);
		return count;
	}

	while (tflag != NULL && tflag->next != NULL) {
		if (tflag->next->signum == signum) {
			int count = 0;
			signal_flag_t dflag = tflag->next;
			sigaction(signum, &dflag->old, NULL);
			tflag->next = dflag->next;
			count = dflag->count;
			free(dflag);
			return count;
		} else {
			tflag = tflag->next;
		}
	}

	return SIGNAL_FLAG_NOT_PRESENT;
}

int disable_all_signal_flags()
{
	int count = 0;

	if (flags == NULL) {
		return SIGNAL_FLAG_NOT_PRESENT;
	}

	while (flags != NULL) {
		signal_flag_t tflag = flags;
		sigaction(flags->signum, &tflag->old, NULL);
		flags = tflag->next;
		free(tflag);
	}

	count = total_count;
	total_count = 0;
	return count;
}

