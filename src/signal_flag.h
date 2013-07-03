#ifndef _TEMPSITE_SIGNAL_FLAG_H_
#define _TEMPSITE_SIGNAL_FLAG_H_

int enable_signal_flag(int signum);

int sleep_until_signal_flag();

int signal_flag_count(int signum);

int lower_signal_flag_count(int signum);

int reset_signal_flag_count(int signum);

int reset_all_signal_flag_counts();

int disable_signal_flag(int signum);

int disable_all_signal_flags();



#define SIGNAL_FLAG_PRESENT      2
#define SIGNAL_FLAG_OKAY         1

#define SIGNAL_FLAG_DISALLOWED  -1
#define SIGNAL_FLAG_NOT_PRESENT -2
#define SIGNAL_FLAG_NOMEM       -3

#endif
