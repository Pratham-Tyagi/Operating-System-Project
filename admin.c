#include <stdio.h>
#include <string.h>
#include "user_auth.h"

int verifyAdmin() {
    if (!isAdmin) {
        return 1;
    }

    char response[10];
    printf("Are you genuine? (yes/no): ");
    scanf("%s", response);
    if (strcmp(response, "yes") == 0) {
        return 1;
    } else {
        printf("Admin verification failed. Access denied.\n");
        return 0;
    }
}
