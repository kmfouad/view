#include <stdio.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <xxhash.h>
#include "estd.h"
#include "db.h"

#define HASH_LEN 16
#define HASH_BASE 16
#define PROG_LEN 11
#define PROG_BASE 10

static XXH64_hash_t 
get_file_hash(const char *path) {
    char *buffer;
    size_t size;
    XXH64_hash_t hash;
    FILE *f;

    f = efopen(path, "r");
    fseek(f, 0, SEEK_END);
    size = (size_t)ftello(f);
    fseek(f, 0, SEEK_SET);
    buffer = (char*)emalloc(size);
    fread(buffer, size, 1, f);
    hash = XXH64(buffer, size, 0);
    fclose(f);
    return hash;
}

DB *
db_init(const char *dir, const char *file) {
    char *sdir, *share = "/.local/share/";
    const char *home;
    size_t dir_len, path_len;
    DB *db;

    db = emalloc(sizeof(DB));
    home = egetenv("HOME");
    dir_len = strlen(home) + strlen(share) + strlen(dir);
    path_len = dir_len+1+strlen(file);
    sdir = emalloc(path_len+1);
    sprintf(sdir, "%s%s%s/%s", home, share, dir, file);
    if(access(sdir, F_OK)) {
        sdir[dir_len] = '\0';
        mkdir(sdir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        sdir[dir_len] = '/';
        creat(sdir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    db->path = sdir;
    return db;
}

int
db_add(DB *db, const char *path, int prog) {
    char buf[PATH_MAX+8];
    char hbuf[17];
    XXH64_hash_t check_hash;
    XXH64_hash_t hash;
    FILE *f;

    hash = get_file_hash(path);
    f = efopen(db->path, "r+");
    for(;;) {
        if(fread(hbuf, 16, 1, f) == 0)
            break;
        hbuf[16] = '\0';
        check_hash = strtoul(hbuf, NULL, 16);
        if(check_hash == hash) {
            fflush(f);
            fprintf(f, " %10d", prog);
            fclose(f);
            return 0;
        }
        fgets(buf, sizeof(buf), f);
    }
    fseek(f, 0, SEEK_END);
    fprintf(f, "%016lx ", hash);
    fprintf(f, "%10d ", prog);
    fprintf(f, "%s\n", path);
    fclose(f);
    return 0;
}

int
db_read(DB *db, const char *path) {
    int prog;
    XXH64_hash_t check_hash, hash;
    char buf[PATH_MAX+8], hbuf[HASH_LEN+1];
    FILE *f;

    hash = get_file_hash(path);
    f = fopen(db->path, "r");
    if(!f)
        return 0;
    while(fread(hbuf, HASH_LEN, 1, f)) {
        hbuf[HASH_LEN] = '\0';
        check_hash = strtoul(hbuf, NULL, HASH_BASE);
        if(check_hash == hash) {
            fread(hbuf, PROG_LEN, 1, f);
            hbuf[PROG_LEN] = '\0';
            prog = (int)strtol(hbuf+1, NULL, PROG_BASE);
            fclose(f);
            return prog;
        }
        fgets(buf, sizeof(buf), f);
    }
    fclose(f);
    return 0;
}

void
db_free(DB *db) {
    free(db->path);
    free(db);
}
