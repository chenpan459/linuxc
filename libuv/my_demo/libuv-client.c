#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define DEFAULT_PORT 7000
#define DEFAULT_HOST "127.0.0.1"
#define BUFFER_SIZE (1024*100)
#define NUM_SENDS 10  // Number of times to send the data

uv_loop_t *loop;
uv_tcp_t client;
uv_connect_t connect_req;
int send_count = 0;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;



void get_timestamp(char* timestamp, size_t max_len) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(timestamp, max_len, "%Y-%m-%d %H:%M:%S", t);
}
// Function declarations
void send_data(uv_timer_t* handle);
void on_write(uv_write_t *req, int status);

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
	char timestamp[26];
	get_timestamp(timestamp, sizeof(timestamp));

    if (nread > 0) {
        printf("[%s]Client received: %d\n",timestamp, (int)nread);
    } else if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*)stream, NULL);
    }

    free(buf->base);
}

void on_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free_write_req(req);
    send_count++;

   // if (send_count < NUM_SENDS) {
        // Schedule the next send
        uv_timer_t *timer = malloc(sizeof(uv_timer_t));
        uv_timer_init(loop, timer);
        uv_timer_start(timer, (uv_timer_cb)send_data, 1000, 0);  // Send every 1 second
    /*} else {
        printf("Finished sending data %d times\n", NUM_SENDS);
        uv_close((uv_handle_t*)&client, NULL);
    }*/
}

void send_data(uv_timer_t* handle) {
	char timestamp[26];
	get_timestamp(timestamp, sizeof(timestamp));

    write_req_t *wr = (write_req_t*) malloc(sizeof(write_req_t));
    wr->buf.base = malloc(BUFFER_SIZE);
    wr->buf.len = BUFFER_SIZE;
    memset(wr->buf.base, 'C', BUFFER_SIZE);  // Fill buffer with 'A's

    uv_write((uv_write_t*)wr, (uv_stream_t*)&client, &wr->buf, 1, on_write);
    
    printf("[%s]Sent %d bytes of data (send #%d)\n", timestamp,BUFFER_SIZE,send_count + 1);
	
    if (handle != NULL) {
        uv_close((uv_handle_t*)handle, (uv_close_cb)free);
    }
}

void on_connect(uv_connect_t *req, int status) {
    if (status < 0) {
        fprintf(stderr, "Connection error %s\n", uv_strerror(status));
        return;
    }

    printf("Connected to server\n");

    // Start reading from the server
    uv_read_start((uv_stream_t*)&client, alloc_buffer, on_read);

    // Start sending data
    send_data(NULL);
}

int main() {
    loop = uv_default_loop();

    uv_tcp_init(loop, &client);

    struct sockaddr_in dest;
    uv_ip4_addr(DEFAULT_HOST, DEFAULT_PORT, &dest);

    uv_tcp_connect(&connect_req, &client, (const struct sockaddr*)&dest, on_connect);

    return uv_run(loop, UV_RUN_DEFAULT);
}
