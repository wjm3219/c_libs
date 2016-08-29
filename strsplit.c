/*
 * Standard c doesn't support strsplit function
 * usually, we can use 'strtok' to make it, while, 
 * strtok has strict requrement on src str. 
 * Then, you can try this one.
 * @author: Terry.Wei
 * @Email: wjm.terry@gmail.com
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct substrs_s substrs_t;

struct substrs_s {
    char **substrs;
    int substrs_nb;
};

static char* get_substr(char *src, int idx_s, int idx_t)
{
    char *dst;
    int len = idx_t - idx_s + 1;
    int i;
    if (len < 0) {
        printf("len invalid. %d\n", len);
        return NULL;
    }

    dst = (char*) malloc((len + 1) * sizeof(char));
    memset(dst, 0, (len + 1)*sizeof(char));
    for (i = 0; i < len; i++)
        dst[i] = src[idx_s + i];

    return dst;
}

static int get_substrs_nb(char *src, char sep)
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

void free_substrs(substrs_t *sub_strs)
{
    if (!sub_strs)
        return;

    for (int idx = 0; idx < sub_strs->substrs_nb; idx++) {
        free(sub_strs->substrs[idx]);
        sub_strs->substrs[idx] = NULL;
    }

    sub_strs->substrs_nb = 0;
    free(sub_strs);
    sub_strs = NULL;
}

int strsplit(char *src, char sep, substrs_t *sub_strs)
{
    int len = strlen(src);
    int idx_s = 0, idx_t = 0;
    int index;

    if (!sub_strs)
        return -1;

    sub_strs->substrs_nb = get_substrs_nb(src, sep);
    sub_strs->substrs = (char**) malloc(sub_strs->substrs_nb * sizeof(char*));
    memset(sub_strs->substrs, 0, sub_strs->substrs_nb * sizeof(char*));

    index = 0;
    for (int i = 0; i < len; i++) {
        if (src[i] != sep)
            continue;
        if (i != 0) {
            idx_t = i - 1;
            sub_strs->substrs[index] = get_substr(src, idx_s, idx_t);
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
        sub_strs->substrs[index] = get_substr(src, idx_s, idx_t);
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    char *names = "xiao7:xiao8:xiao9";
    char sep = ':';

    substrs_t *sub_strs = (substrs_t*) malloc(sizeof(substrs_t));
    if (!sub_strs) {
        printf("failed to malloc. \n");
        exit(0);
    }

    sub_strs->substrs = NULL;
    sub_strs->substrs_nb = 0;

    if (strsplit(names, sep, sub_strs) == 0) {
        printf("sub strs' number is %d\n", sub_strs->substrs_nb);
        for (int i = 0; i < sub_strs->substrs_nb; i++)
            printf("the %dth sub str is %s\n", i, sub_strs->substrs[i]);
    }

    free_substrs(sub_strs);

    printf("done.\n");

    return 0;
}