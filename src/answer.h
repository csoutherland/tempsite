#ifndef _TEMPSITE_ANSWER_H_
#define _TEMPSITE_ANSWER_H_

int answer(
		void* daemon_state,
		struct MHD_Connection* connection,
		const char* url,
		const char* method,
		const char* version,
		const char* upload_data,
		size_t* upload_data_size,
		void** connection_state);

#endif
