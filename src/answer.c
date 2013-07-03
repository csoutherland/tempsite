#include <config.h>
#include "answer.h"
#include <string.h>
#include <syslog.h>
#include <microhttpd.h>

#define SMATCH(string,literal) 0==strncmp(string,""literal,strlen(literal)+1)

#define CSS "body{background-color:#000000;color:#FFFFFF;}" \
	"div#page{margin-left:auto;margin-right:auto;width:400;" \
	"vertical-align:middle;}div#content{width:400;}"

#define JS "document.onload = function(){" \
	"if(window.innerWidth>800&&window.innerWidth>=window.innerHeight){" \
	"document.getElementById(\"logo\").style.float=\"left\";" \
	"document.getElementById(\"content\").style.float=\"right\";}};" \

#define START2SUBTITLE "<html><head><title>Stuph Labs"
#define SUBTITLE2IMGDOMAIN "</title><style type=\"text/css\">"CSS"</style>" \
	"</head><body><div id=\"page\"><img alt=\"Stuph Labs\" id=\"logo\" " \
	"src=\""
#define IMGDOMAIN2H1 "/stuph_logo.png\"/><div id=\"content\"><h1>"
#define H12P "</h1><p>"
#define P2JS "</p></div></div><script type=\"text/javascript\">"
#define JS2END "</script></body></html>"

extern unsigned char _binary_stuph_logo_png_start;
extern unsigned char _binary_stuph_logo_png_end;

/*
static int respond_redirect(struct MHD_Connection* connection)
{
	char* page = START2SUBTITLE " (Redirecting...)"
		SUBTITLE2IMGDOMAIN "http://stuph.net"
		IMGDOMAIN2H1 "Redirecting to Stuph Labs..."
		H12P "To go to the main Stuph Labs website, <a "
		"href=\"http://stuph.net/\">click here</a>." P2JS JS JS2END;

	struct MHD_Response* response = MHD_create_response_from_buffer(
			strlen(page),
			(void*)page,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		syslog(LOG_CRIT, "Unable to create response for redirect");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_LOCATION,
			"http://stuph.net/")) {
		syslog(LOG_CRIT, "Unable to add header for redirect");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"text/html")) {
		syslog(LOG_CRIT, "Unable to add content type for redirect");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_TEMPORARY_REDIRECT,
			response)) {
		syslog(LOG_CRIT, "Unable to queue response for redirect");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_footer(
			response,
			MHD_HTTP_HEADER_CONNECTION,
			"close")) {
		syslog(LOG_CRIT, "Unable to close connection for redirect");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}
*/

static int respond_options(struct MHD_Connection* connection)
{
	struct MHD_Response* response = MHD_create_response_from_buffer(
			0,
			NULL,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		syslog(LOG_CRIT, "Unable to create response for options");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_ALLOW,
			MHD_HTTP_METHOD_GET","
			MHD_HTTP_METHOD_HEAD","
			MHD_HTTP_METHOD_OPTIONS)) {
		syslog(LOG_CRIT, "Unable to add header for options");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_OK,
			response)) {
		syslog(LOG_CRIT, "Unable to queue response for options");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_footer(
			response,
			MHD_HTTP_HEADER_CONNECTION,
			"close")) {
		syslog(LOG_CRIT, "Unable to close connection for options");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int respond_not_allowed(struct MHD_Connection* connection)
{
	char* page = START2SUBTITLE " (Method Not Allowed)"
		SUBTITLE2IMGDOMAIN IMGDOMAIN2H1 "Method Not Allowed"
		H12P "The request method that was sent to the server is not "
		"allowed for this particular URL. Please use the HTTP GET, "
		"HEAD, or OPTIONS methods to attempt to access this URL."
		P2JS JS JS2END;

	struct MHD_Response* response = MHD_create_response_from_buffer(
			strlen(page),
			(void*)page,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		syslog(LOG_CRIT, "Unable to create response for not allowed");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_ALLOW,
			MHD_HTTP_METHOD_GET","
			MHD_HTTP_METHOD_HEAD","
			MHD_HTTP_METHOD_OPTIONS)) {
		syslog(LOG_CRIT, "Unable to add header for not allowed");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"text/html")) {
		syslog(LOG_CRIT, "Unable to add content type for not allowed");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_METHOD_NOT_ALLOWED,
			response)) {
		syslog(LOG_CRIT, "Unable to queue response for not allowed");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_footer(
			response,
			MHD_HTTP_HEADER_CONNECTION,
			"close")) {
		syslog(LOG_CRIT, "Unable to close connection for not allowed");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int respond_not_found(struct MHD_Connection* connection)
{
	char* page = START2SUBTITLE " (Page Not Found)"
		SUBTITLE2IMGDOMAIN IMGDOMAIN2H1 "Page Not Found"
		H12P "To go to the main Stuph Labs website, <a "
		"href=\"http://stuph.net/\">click here</a>." P2JS JS JS2END;

	struct MHD_Response* response = MHD_create_response_from_buffer(
			strlen(page),
			(void*)page,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		syslog(LOG_CRIT, "Unable to create response for not found");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"text/html")) {
		syslog(LOG_CRIT, "Unable to add content type for not found");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_NOT_FOUND,
			response)) {
		syslog(LOG_CRIT, "Unable to queue response for not found");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_footer(
			response,
			MHD_HTTP_HEADER_CONNECTION,
			"close")) {
		syslog(LOG_CRIT, "Unable to close connection for not found");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int respond_index(struct MHD_Connection* connection)
{
	char* page = START2SUBTITLE SUBTITLE2IMGDOMAIN IMGDOMAIN2H1
		"Stuph Labs' New Website" H12P "New websites for Stuph Labs "
		"and several of its projects are currently being planned,  "
		"Stay tuned..." P2JS JS JS2END;

	struct MHD_Response* response = MHD_create_response_from_buffer(
			strlen(page),
			(void*)page,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		syslog(LOG_CRIT, "Unable to create response for index");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"text/html")) {
		syslog(LOG_CRIT, "Unable to add content type for index");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_OK,
			response)) {
		syslog(LOG_CRIT, "Unable to queue response for index");
		return MHD_NO;
	} else {
		MHD_destroy_response(response);
		return MHD_YES;
	}
}

static int respond_logo(struct MHD_Connection* connection)
{
	unsigned char* logo = &_binary_stuph_logo_png_start;
	size_t logo_size = &_binary_stuph_logo_png_end 
		- &_binary_stuph_logo_png_start;

	struct MHD_Response* response = MHD_create_response_from_buffer(
			logo_size,
			(void*)logo,
			MHD_RESPMEM_PERSISTENT);

	if (response == NULL) {
		syslog(LOG_CRIT, "Unable to create response for logo");
		return MHD_NO;
	} else if (MHD_NO == MHD_add_response_header(
			response,
			MHD_HTTP_HEADER_CONTENT_TYPE,
			"image/png")) {
		syslog(LOG_CRIT, "Unable to add content type for logo");
		return MHD_NO;
	} else if (MHD_NO == MHD_queue_response(
			connection,
			MHD_HTTP_OK,
			response)) {
		syslog(LOG_CRIT, "Unable to queue response for logo");
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
		if (!(SMATCH(
				MHD_lookup_connection_value(
					connection,
					MHD_HEADER_KIND,
					"Host"),
				"stuph.net"))) {
			/*return respond_redirect(connection);*/
			syslog(LOG_INFO, "wrong host");
		} else if (version == NULL
				&& upload_data_size == NULL
				&& upload_data == NULL
				&& cls == NULL) {
			syslog(LOG_DEBUG, "lotsa stuff is null");
		}
	}

	if (SMATCH(url, "/")
			|| SMATCH(url, "/index.htm")
			|| SMATCH(url, "/index.html")) {
		/* main page */
		if (SMATCH(method, MHD_HTTP_METHOD_OPTIONS)) {
			return respond_options(connection);
		} else if (!(SMATCH(method, MHD_HTTP_METHOD_GET)
				|| SMATCH(method, MHD_HTTP_METHOD_HEAD))) {
			return respond_not_allowed(connection);
		} else if (*con_cls == NULL) {
			*con_cls = connection;
			return MHD_YES;
		} else {
			return respond_index(connection);
		}
	} else if (SMATCH(url, "/stuph_logo.png")) {
		/* logo */
		if (SMATCH(method, MHD_HTTP_METHOD_OPTIONS)) {
			return respond_options(connection);
		} else if (!(SMATCH(method, MHD_HTTP_METHOD_GET)
				|| SMATCH(method, MHD_HTTP_METHOD_HEAD))) {
			return respond_not_allowed(connection);
		} else if (*con_cls == NULL) {
			*con_cls = connection;
			return MHD_YES;
		} else {
			return respond_logo(connection);
		}
	} else if (SMATCH(url, "*")
			&& SMATCH(method, MHD_HTTP_METHOD_OPTIONS)) {
		return respond_options(connection);
	} else {
		return respond_not_found(connection);
	}
}


