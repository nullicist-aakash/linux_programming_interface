// Implement getpwnam() using setpwent(), getpwent(), and endpwent().

#include <cstring>
#include <pwd.h>


passwd* my_getpwnam(const char* name) {
    passwd *pwd;

    while ((pwd = getpwent()) != nullptr) {
        if (strcmp(pwd->pw_name, name) == 0) {
            endpwent();
            return pwd;
        }
    }

    endpwent();
    return nullptr;
}
