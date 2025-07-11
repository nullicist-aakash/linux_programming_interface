/*
 * Implement setenv() and unsetenv() using getenv(), putenv(), and, where necessary,
 * code that directly modifies environ. Your version of unsetenv() should check to see
 * whether there are multiple definitions of an environment variable, and remove
 * them all (which is what the glibc version of unsetenv() does).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int exercise_6_3_setenv(const char *name, const char *value, int overwrite) {
    if (getenv(name) != NULL && !overwrite) {
        return 0;
    }

    char* env = static_cast<char*>(malloc(strlen(name) + strlen(value) + 2));
    if (env == nullptr) {
        perror("malloc");
        return 1;
    }
    memcpy(env, name, strlen(name));
    env[strlen(name)] = '=';
    memcpy(env + strlen(name) + 1, value, strlen(value) + 1);
    env[strlen(name) + strlen(value) + 1] = '\0';
    return putenv(env);
}

int exercise_6_4_unsetenv(const char *name) {
    const size_t name_len = strlen(name);

    int count = 0;
    for (char** p = environ; *p != NULL; p++)
        ++count;

    for (int i = 0; i < count; i++) {
        if (strncmp(name, environ[i], name_len) != 0 || environ[i][name_len] != '=')
            continue;

        environ[i] = environ[--count];
        environ[i] = NULL;
    }

    return 0;
}
