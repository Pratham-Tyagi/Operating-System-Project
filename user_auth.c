#include <stdio.h>
#include <string.h>
#include "user_auth.h"
#include "encryption.h"

char loggedInUser[20];
int isAdmin = 0;
const char adminPassword[] = "admin123";

void addUser() {
    char username[20], password[20];
    char encUser[50], encPass[50];

    printf("Enter new username: ");
    scanf("%s", username);
    printf("Enter new password: ");
    scanf("%s", password);

    encryptDecrypt(username, encUser);
    encryptDecrypt(password, encPass);

    FILE *fp = fopen("users.txt", "a");
    if (!fp) {
        perror("Error opening users file");
        return;
    }
    fprintf(fp, "%s %s\n", encUser, encPass);
    fclose(fp);
    printf("User added successfully.\n");
}

int authenticate() {
    char username[20], password[20];
    char fileUserEncrypted[50], filePassEncrypted[50];
    char fileUser[50], filePass[50];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        perror("Error opening users file");
        return 0;
    }

    while (fscanf(fp, "%s %s", fileUserEncrypted, filePassEncrypted) != EOF) {
        encryptDecrypt(fileUserEncrypted, fileUser);
        encryptDecrypt(filePassEncrypted, filePass);

        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            strcpy(loggedInUser, username);
            fclose(fp);

            if (strcmp(username, "admin") == 0) {
                isAdmin = 0;
                char enteredPassword[20];
                printf("Enter admin password: ");
                scanf("%s", enteredPassword);

                if (strcmp(enteredPassword, adminPassword) == 0) {
                    isAdmin = 1;
                    printf("Admin verification successful.\n");
                } else {
                    printf("Admin verification failed. Limited access granted.\n");
                }
            } else {
                isAdmin = 0;
            }

            printf("Login successful! Welcome %s\n", loggedInUser);
            return 1;
        }
    }

    fclose(fp);
    printf("Login failed! Invalid credentials.\n");
    return 0;
}
