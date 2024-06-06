#ifndef DATABASE_H
#define DATABASE_H

#include <stdbool.h>

typedef struct _database_state_ {
    void *context;
    void *ssl;
} database_state;

database_state *database_init_ssl(const char *hostname, const char *password, int port);
database_state *database_init(const char *hostname, const char *password, int port);
database_state *database_init_no_pw(const char *hostname, int port);
void database_free(database_state *state);

bool database_set(database_state *state, const char *key, const char *value);
char *database_get(database_state *state, const char *key);
bool database_append(database_state *state, const char *key, const char *value);

#endif // !DATABASE_H
