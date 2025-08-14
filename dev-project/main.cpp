#include <microhttpd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define PORT 8888

std::string load_html(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    if (file) {
        buffer << file.rdbuf();
    } else {
        buffer << "<html><body><h1>404 Not Found</h1></body></html>";
    }
    return buffer.str();
}

static int handle_request(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls) {
    std::string html = load_html("index.html");

    struct MHD_Response *response;
    response = MHD_create_response_from_buffer(html.size(),
                                               (void *)html.c_str(),
                                               MHD_RESPMEM_MUST_COPY);

    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int main() {
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &handle_request, NULL, MHD_OPTION_END);

    if (!daemon) {
        std::cerr << "❌ Failed to start server." << std::endl;
        return 1;
    }

    std::cout << "✅ Server running at http://localhost:" << PORT << std::endl;
    std::cin.get(); // Wait for Enter key to stop

    MHD_stop_daemon(daemon);
    return 0;
}