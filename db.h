#ifndef VIEW_DB_H
#define VIEW_DB_H

typedef struct DB {
    char *path;
} DB;

DB *db_init(const char *dir, const char *file);
int db_add(DB *db, const char *path, int prog);
int db_read(DB *db, const char *path);
void db_free(DB *db);

#endif
