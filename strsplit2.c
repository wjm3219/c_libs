#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct zonelist_s {
    int zonelist_nb;
    char **zonelists;
};

char *zonelist_s = "aaaaaa:bbbbbb:cccccc";

char*
get_substr(char *src, int idx_s, int idx_t)
{
    char *dst;
    int len = idx_t - idx_s + 1;
    int i;
    if (len < 0) {
        printf("len invalid. %d\n", len);
        return NULL;
    }

    dst = (char *) malloc((len + 1) * sizeof(char));
    if (!dst)
        return NULL;
    memset(dst, 0, (len + 1)*sizeof(char));
    for (i = 0; i < len; i++)
        dst[i] = src[idx_s + i];

    return dst;
}

int
get_substrs_nb(char *src, char sep)
{
    int len = strlen(src);
    int i;
    int nb = 0;
    for (i = 0; i < len; i++)
        if (src[i] == sep)
            nb++;

    if (src[0] == sep)
        nb--;
    if (src[len - 1] == sep)
        nb--;

    return nb + 1;
}

int
strsplit(char *src, char sep, int *substrs_nb, char ***substrs)
{
    int len = strlen(src);
    int idx_s = 0, idx_t = 0;
    int index;
    int count;
    char **subs;

    count = get_substrs_nb(src, sep);
    subs = (char **) malloc( count * sizeof(char*));
    if (!subs)
        return -1;
    memset(subs, 0, count * sizeof(char*));

    index = 0;
    for (int i = 0; i < len; i++) {
        if (src[i] != sep)
            continue;
        if (i != 0) {
            idx_t = i - 1;
            subs[index] = get_substr(src, idx_s, idx_t);
            if (i == len - 1)
                break;
            idx_s = i + 1;
            idx_t = i + 1;
            ++index;
        } else {
            idx_s = 1;
            idx_t = 1;
        }
    }

    if (idx_t == idx_s && idx_t < len) {
        idx_t = len - 1;
        subs[index] = get_substr(src, idx_s, idx_t);
    }

    *substrs_nb = count;
    *substrs = subs;
    return 0;
}

int main(int argc, char const *argv[])
{
    struct zonelist_s *zlists;
    zlists = (struct zonelist_s *) malloc(sizeof(struct zonelist_s));
    if (!zlists) {
        printf("failed to malloc. \n");
        return -1;
    }

    zlists->zonelist_nb = 0;
    zlists->zonelists = NULL;

    char sep = ':';
    if (strsplit(zonelist_s, sep, &zlists->zonelist_nb, &zlists->zonelists) == 0) {
        printf("zonelist_nb is %d\n", zlists->zonelist_nb);
        for (int i = 0; i < zlists->zonelist_nb; i++)
            printf("the %dth zone is %s\n", i, zlists->zonelists[i]);

        for (int i = 0; i < zlists->zonelist_nb; i++) {
            free(zlists->zonelists[i]);
            zlists->zonelists[i] = NULL;
        }

        free(zlists->zonelists);
        zlists->zonelists = NULL;
        zlists->zonelist_nb = 0;

        free(zlists);
        zlists = NULL;
    }

    if (zlists == NULL) 
        printf("null. \n");

    return 0;
}