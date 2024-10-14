#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>

#define PORT 8000

static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    return 0;
}

static int callback_websocket(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            printf("WebSocket connection established\n");
            break;
        case LWS_CALLBACK_RECEIVE:
            printf("Received data: %.*s\n", (int)len, (char *)in);
            // Echo the received message back to the client
            lws_write(wsi, in, len, LWS_WRITE_TEXT);
            break;
        case LWS_CALLBACK_CLOSED:
            printf("WebSocket connection closed\n");
            break;
        default:
            break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
  //  { "http", callback_http, 0, 0 },
    { "websocket", callback_websocket, 0, 0 },
    { NULL, NULL, 0, 0 } /* terminator */
};

int main(void) {
    struct lws_context_creation_info info;
    struct lws_context *context;

    memset(&info, 0, sizeof info);

    info.port = PORT;
    info.iface = NULL;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;

    //lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO | LLL_DEBUG, NULL);
	lws_set_log_level(LLL_ERR | LLL_WARN, NULL);

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "libwebsocket init failed\n");
        return -1;
    }

    printf("Server started on port %d\n", PORT);

    while (1) {
        lws_service(context, 50);
    }

    lws_context_destroy(context);

    return 0;
}
