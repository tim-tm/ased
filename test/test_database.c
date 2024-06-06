#include <stdio.h>
#include <time.h>
#define LOGGER_IMPLEMENTATION
#include "logger.h"

#include "database/database.h"

int main(void) {
    database_state *db = database_init_no_pw(NULL, 6379);
    LOGGER_ASSERT(db == NULL);

    db = database_init_ssl("golden-ringtail-54122.upstash.io", "AdNqAAIncDE0ZTgyYzQ3ZjEwYjA0ZjU4ODZhMDc5ZDgxMjY5MzYwY3AxNTQxMjI", 6379);
    LOGGER_ASSERT(db != NULL);

    srand(time(NULL));
    
    char str[11];
    sprintf(str, "%i", rand());
    
    database_append(db, "test_database", str);
    LOGGER_ASSERT(db != false);

    database_free(db);
    return 0;
}
