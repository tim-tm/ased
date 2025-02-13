#ifdef DATABASE_SQLITE

#include "database.h"
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <assert.h>
#include <stdio.h>

static int callback(void *unused, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
	printf("%s=%s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

database_state *database_init_ssl(const char *hostname, const char *password, int port) {
    if (hostname == NULL || password == NULL) return NULL;

    sqlite3 *db;
    if (sqlite3_open("data.db", &db)) {
    	printf("Can't open database: %s\n", sqlite3_errmsg(db));
    	return NULL;
    }

    database_state *state = malloc(sizeof(database_state));
    if (state == NULL) return NULL;

    state->context = db;
    state->ssl = NULL;
    return state;
}

database_state *database_init(const char *hostname, const char *password, int port) {
    if (hostname == NULL || password == NULL) return NULL;

    sqlite3 *db;
    if (sqlite3_open("data.db", &db)) {
    	printf("Can't open database: %s\n", sqlite3_errmsg(db));
    	return NULL;
    }

    database_state *state = malloc(sizeof(database_state));
    if (state == NULL) return NULL;

    state->context = db;
    state->ssl = NULL;
    return state;
}

database_state *database_init_no_pw(const char *hostname, int port) {
    if (hostname == NULL) return NULL;

    sqlite3 *db;
    if (sqlite3_open("data.db", &db)) {
    	printf("Can't open database: %s\n", sqlite3_errmsg(db));
    	return NULL;
    }

    database_state *state = malloc(sizeof(database_state));
    if (state == NULL) return NULL;

    state->context = db;
    state->ssl = NULL;
    return state;
}

void database_free(database_state *state) {
    if (state) {
        if (state->context) {
            sqlite3_close(state->context);
        }
        free(state);
    }
}

bool database_set(database_state *state, const char *key, const char *value) {
    assert(0 && "Unimplemented");
}

char *database_get(database_state *state, const char *key) {
    assert(0 && "Unimplemented");
}

bool database_append(database_state *state, const char *key, const char *value) {
    assert(0 && "Unimplemented");
}

bool database_query(database_state *state, const char *query) {
    if (state == NULL || query == NULL) return false;
    
    char *err = 0;
    if (sqlite3_exec(state->context, query, callback, 0, &err) != SQLITE_OK) {
        printf("SQL error: %s\n", err);
	sqlite3_free(err);
    	return false;
    }
    return true;
}

#endif /* ifdef DATABASE_SQLITE */
