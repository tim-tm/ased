#ifdef DATABASE_REDIS

#include "database.h"
#include <string.h>
#include <stdlib.h>
#include <hiredis/hiredis.h>
#include <hiredis/hiredis_ssl.h>

database_state *database_init_ssl(const char *hostname, const char *password, int port) {
    if (hostname == NULL || password == NULL) return NULL;

    database_state *state = malloc(sizeof(database_state));
    if (state == NULL) return NULL;

    redisSSLContextError ssl_error = REDIS_SSL_CTX_NONE;
    redisInitOpenSSL();
    redisSSLContext *ssl_context = redisCreateSSLContext(NULL, NULL, NULL, NULL, NULL, &ssl_error);
    if (!ssl_context || ssl_error != REDIS_SSL_CTX_NONE) {
        printf("Failed to create SSLContext: %s\n", redisSSLContextGetError(ssl_error));
        free(state);
        return NULL;
    }

    struct timeval tv = { 1, 500000 };
    redisOptions opts = {0};
    REDIS_OPTIONS_SET_TCP(&opts, hostname, port);
    opts.connect_timeout = &tv;

    redisContext *context = redisConnectWithOptions(&opts);
    if (context == NULL || context->err) {
        if (context) {
            printf("Connection error: %s\n", context->errstr);
            redisFree(context);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        free(state);
        return NULL;
    }

    if (redisInitiateSSLWithContext(context, ssl_context) != REDIS_OK) {
        printf("Couldn't init ssl: %s\n", context->errstr);
        redisFree(context);
        free(state);
        return NULL;
    }

    redisReply *reply = redisCommand(context, "AUTH %s", password);
    if (strcmp(reply->str, "OK") != 0) {
        freeReplyObject(reply);
        free(state);
        return NULL;
    }
    freeReplyObject(reply);

    state->context = context;
    state->ssl = ssl_context;
    return state;
}

database_state *database_init(const char *hostname, const char *password, int port) {
    if (hostname == NULL || password == NULL) return NULL;

    database_state *state = malloc(sizeof(database_state));
    if (state == NULL) return NULL;

    redisContext *context = redisConnect(hostname, port);
    if (context == NULL || context->err) {
        if (context) {
            printf("Connection error: %s\n", context->errstr);
            redisFree(context);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        free(state);
        return NULL;
    }

    redisReply *reply = redisCommand(context, "AUTH %s", password);
    if (strcmp(reply->str, "OK") != 0) {
        freeReplyObject(reply);
        free(state);
        return NULL;
    }
    freeReplyObject(reply);

    state->context = context;
    state->ssl = NULL;
    return state;
}

database_state *database_init_no_pw(const char *hostname, int port) {
    if (hostname == NULL) return NULL;

    database_state *state = malloc(sizeof(database_state));
    if (state == NULL) return NULL;

    redisContext *context = redisConnect(hostname, port);
    if (context == NULL || context->err) {
        if (context) {
            printf("Connection error: %s\n", context->errstr);
            redisFree(context);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        free(state);
        return NULL;
    }

    state->context = context;
    state->ssl = NULL;
    return state;
}

void database_free(database_state *state) {
    if (state) {
        if (state->context) {
            redisFree(state->context);
        }
        if (state->ssl) {
            redisFreeSSLContext(state->ssl);
        }
        free(state);
    }
}

bool database_set(database_state *state, const char *key, const char *value) {
    if (state == NULL || state->context == NULL || key == NULL || value == NULL) return false;

    redisReply *reply = redisCommand(state->context, "SET %s %s", key, value);
    if (strcmp(reply->str, "OK") != 0) {
        freeReplyObject(reply);
        return false;
    }
    freeReplyObject(reply);
    return true;
}

char *database_get(database_state *state, const char *key) {
    if (state == NULL || state->context == NULL || key == NULL) return NULL;

    redisReply *reply = redisCommand(state->context, "GET %s", key);
    if (reply->len <= 0) {
        freeReplyObject(reply);
        return NULL;
    }

    char *response = malloc(reply->len);
    if (response == NULL) return NULL;
    strncpy(response, reply->str, reply->len);

    freeReplyObject(reply);
    return response;
}

bool database_append(database_state *state, const char *key, const char *value) {
    if (state == NULL || state->context == NULL || key == NULL || value == NULL) return false;

    redisReply *reply = redisCommand(state->context, "APPEND %s %s", key, value);
    freeReplyObject(reply);
    return true;
}

#endif /* ifdef DATABASE_REDIS */
