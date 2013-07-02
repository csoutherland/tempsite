#include <config.h>
#include "answer.h"
#include <string.h>

#define SMATCH(string,literal) 0==strncmp(string,""literal,strlen(literal)+1)

extern unsigned char _binary_stuph_logo_png_start[];
extern size_t _binary_stuph_logo_png_size;

static int redirect(struct MHD_Connection* connection)
{
	char* page = "<html><head><title>Stuph Labs (Redirecting...)</title>"
		"</head><body><img src=\"http://stuph.net/stuph_logo.png\" />"
		"<h1>Redirecting to Stuph Labs...<h1><p>To go to the main "
		"Stuph Labs website, <a href=\"http://stuph.net/\">click "
		"here</a>.</p></body></html>";

	struct MHD_Response* response = MHD_create_response_from_buffer(
			strlen(page),
			(void*)page,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		log_crit("Unable to create response for redirect");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_LOCATION,
			"http://stuph.net/")) {
		log_crit("Unable to add header for redirect");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"text/html")) {
		log_crit("Unable to add content type for redirect");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_TEMPORARY_REDIRECT,
			response)) {
		log_crit("Unable to queue response for redirect");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_footer(
			response,
			MHD_HTTP_HEADER_CONNECTION,
			"close")) {
		log_crit("Unable to close connection for redirect");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int options(struct MHD_Connection* connection)
{
	struct MHD_Response* response = MHD_create_response_from_buffer(
			0,
			NULL,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		log_crit("Unable to create response for options");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_ALLOW,
			MHD_HTTP_METHOD_GET","
			MHD_HTTP_METHOD_HEAD","
			MHD_HTTP_METHOD_OPTIONS)) {
		log_crit("Unable to add header for options");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_OK,
			response)) {
		log_crit("Unable to queue response for options");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_footer(
			response,
			MHD_HTTP_HEADER_CONNECTION,
			"close")) {
		log_crit("Unable to close connection for options");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int not_allowed(struct MHD_Connection* connection)
{
	char* page = "<html><head><title>Stuph Labs (Method Not Allowed)"
		"</title></head><body><img src=\"/stuph_logo.png\" /><h1>"
		"Method Not Allowed<h1><p>The request method that was sent to "
		"the server is not allowed for this particular URL. Please "
		"use the HTTP GET, HEAD, or OPTIONS methods to attempt to "
		"access this URL.</p></body></html>";

	struct MHD_Response* response = MHD_create_response_from_buffer(
			strlen(page),
			(void*)page,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		log_crit("Unable to create response for not allowed");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_ALLOW,
			MHD_HTTP_METHOD_GET","
			MHD_HTTP_METHOD_HEAD","
			MHD_HTTP_METHOD_OPTIONS)) {
		log_crit("Unable to add header for not allowed");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"text/html")) {
		log_crit("Unable to add content type for not allowed");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_METHOD_NOT_ALLOWED,
			response)) {
		log_crit("Unable to queue response for not allowed");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_footer(
			response,
			MHD_HTTP_HEADER_CONNECTION,
			"close")) {
		log_crit("Unable to close connection for not allowed");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int not_found(struct MHD_Connection* connection)
{
	char* page = "<html><head><title>Stuph Labs (Page Not Found)</title>"
		"</head><body><img src=\"http://stuph.net/stuph_logo.png\" />"
		"<h1>Page Not Found<h1><p>To go to the main Stuph Labs "
		"website, <a href=\"http://stuph.net/\">click here</a>.</p>"
		"</body></html>";

	struct MHD_Response* response = MHD_create_response_from_buffer(
			strlen(page),
			(void*)page,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		log_crit("Unable to create response for not found");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"text/html")) {
		log_crit("Unable to add content type for not found");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_NOT_FOUND,
			response)) {
		log_crit("Unable to queue response for not found");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_footer(
			response,
			MHD_HTTP_HEADER_CONNECTION,
			"close")) {
		log_crit("Unable to close connection for not found");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int index(struct MHD_Connection* connection)
{
	char* page = "<html><head><title>Stuph Labs</title></head><body>"
		"<img src=\"http://stuph.net/stuph_logo.png\" /><h1>Stuph "
		"Labs' New Website<h1><p>New websites for Stuph Labs and "
		"several of its projects are currently being planned,  Stay "
		"tuned...</p></body></html>";

	struct MHD_Response* response = MHD_create_response_from_buffer(
			strlen(page),
			(void*)page,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		log_crit("Unable to create response for index");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"text/html")) {
		log_crit("Unable to add content type for index");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_OK,
			response)) {
		log_crit("Unable to queue response for index");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int logo(struct MHD_Connection* connection)
{
	unsigned char* logo = _binary_stuph_logo_png_start;
	size_t logo_size = _binary_stuph_logo_png_size;

	struct MHD_Response* response = MHD_create_response_from_buffer(
			logo_size,
			(void*)logo,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		log_crit("Unable to create response for logo");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"image/png")) {
		log_crit("Unable to add content type for logo");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_OK,
			response)) {
		log_crit("Unable to queue response for logo");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

int answer(
		void* cls,
		struct MHD_Connection* connection,
		const char* url,
		const char* method,
		const char* version,
		const char* upload_data,
		size_t* upload_data_size,
		void** con_cls)
{
	if (*con_cls == NULL) {
		if (SMATCH(
				MHD_lookup_connection_value(
					connection,
					NULL,
					"Host"),
				"stuph.net")) {
			return redirect(connection);
		}
	}

	if (SMATCH(url, "/")
			|| SMATCH(url, "/index.htm")
			|| SMATCH(url, "/index.html")) {
		/* main page */
		if (SMATCH(method, MHD_HTTP_METHOD_OPTIONS)) {
			return options(connection);
		} else if (!(SMATCH(method, MHD_HTTP_METHOD_GET)
				|| SMATCH(method, MHD_HTTP_METHOD_HEAD))) {
			return not_allowed(connection);
		} else if (*con_cls == NULL) {
			*con_cls = connection;
			return MHD_YES;
		} else {
			return index();
		}
	} else if (SMATCH(url, "/stuph_logo.png")) {
		/* logo */
		if (SMATCH(method, MHD_HTTP_METHOD_OPTIONS)) {
			return options(connection);
		} else if (!(SMATCH(method, MHD_HTTP_METHOD_GET)
				|| SMATCH(method, MHD_HTTP_METHOD_HEAD))) {
			return not_allowed(connection);
		} else if (*con_cls == NULL) {
			*con_cls = connection;
			return MHD_YES;
		} else {
			/* serve page */
		}
	} else if (SMATCH(url, "*")
			&& SMATCH(method, MHD_HTTP_METHOD_OPTIONS)) {
		return options(connection);
	} else {
		return not_found();
	}
}


