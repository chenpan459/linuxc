#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>

void send_file(struct evhttp_request *req, void *arg) {
    const char *file_path = (const char *)arg;
    FILE *file = fopen(file_path, "rb");
    printf("file_path:%s\n",file_path);
    if (!file) {
        perror("fopen");
        evhttp_send_reply(req, HTTP_NOTFOUND, "Not Found", NULL);
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    struct evbuffer *buf = evbuffer_new();
    if (!buf) {
        fclose(file);
        evhttp_send_reply(req, HTTP_INTERNAL, "Internal Server Error", NULL);
        return;
    }

    evbuffer_add_file(buf, fileno(file), 0, file_size);

    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "application/octet-stream");
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Disposition", "attachment; filename=\"downloaded_file\"");
    
    evhttp_send_reply(req, HTTP_OK, "OK", buf);

    evbuffer_free(buf);
    fclose(file);
}

int main() {
    struct event_base *base = event_base_new();
    if (!base) {
        fprintf(stderr, "event_base_new() failed\n");
        return 1;
    }

    struct evhttp *http = evhttp_new(base);
    if (!http) {
        fprintf(stderr, "evhttp_new() failed\n");
        event_base_free(base);
        return 1;
    }

    if (evhttp_bind_socket(http, "0.0.0.0", 8080) != 0) {
        fprintf(stderr, "evhttp_bind_socket_with_port() failed\n");
        evhttp_free(http);
        event_base_free(base);
        return 1;
    }

    const char *file_path = "/home/cp/work/libevent/new.tar.gz"; // 替换为你要提供的文件路径
    evhttp_set_cb(http, "/download", send_file, (void *)file_path);

    printf("HTTP server is running on port 8080\n");
    event_base_dispatch(base);

    evhttp_free(http);
    event_base_free(base);

    return 0;
}
