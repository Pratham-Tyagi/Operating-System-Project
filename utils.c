#include <stdio.h>
#include <string.h>
#include "user_auth.h"

void getFullPath(char *filename, char *fullPath) {
    char owner[20];
    if (isAdmin) {
        printf("Enter owner username: ");
        scanf("%s", owner);
    } else {
        strcpy(owner, loggedInUser);
    }
    sprintf(fullPath, "%s_%s", owner, filename);
}
