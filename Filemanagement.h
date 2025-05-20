#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

char loggedInUser[20];
int isAdmin = 0; // 1 if admin, 0 otherwise
const char key = 'K'; // XOR key for encryption/decryption
const char adminPassword[] = "admin123"; // Admin password for verification

// Encrypt/Decrypt function using XOR cipher
void encryptDecrypt(char *input, char *output) {
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        output[i] = input[i] ^ key;
    }
    output[len] = '\0';
}

// Add a new user with encrypted username and password to users.txt
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

// Authentication Function (reads encrypted credentials from users.txt)
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

// Verify admin authenticity before admin actions
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

// Get full file path based on user and filename
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

// Create File
void createFile() {
    if (!verifyAdmin()) {
        return;
    }

    char filename[50], fullPath[100];
    printf("Enter file name: ");
    scanf("%s", filename);
    getFullPath(filename, fullPath);

    int fd = open(fullPath, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    printf("File '%s' created successfully.\n", fullPath);
    close(fd);
}

// Write to File
void writeToFile() {
    char filename[50], fullPath[100];
    printf("Enter file name: ");
    scanf("%s", filename);
    getFullPath(filename, fullPath);

    int fd = open(fullPath, O_WRONLY | O_APPEND);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char content[100];
    printf("Enter content: ");
    getchar(); // Clear newline
    fgets(content, sizeof(content), stdin);

    write(fd, content, strlen(content));
    printf("Content written successfully.\n");

    close(fd);
}

// Modify File
void modifyFile() {
    char filename[50], fullPath[100];
    printf("Enter file name to modify: ");
    scanf("%s", filename);
    getFullPath(filename, fullPath);

    int fd = open(fullPath, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char content[100];
    printf("Enter new content: ");
    getchar(); // Clear newline
    fgets(content, sizeof(content), stdin);

    write(fd, content, strlen(content));
    printf("File modified successfully.\n");

    close(fd);
}

// Search Content in File
void searchContent() {
    char filename[50], keyword[50], fullPath[100];
    printf("Enter file name: ");
    scanf("%s", filename);
    printf("Enter keyword to search: ");
    scanf("%s", keyword);
    getFullPath(filename, fullPath);

    FILE *fp = fopen(fullPath, "r");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword)) {
            printf("Keyword found: %s", line);
            found = 1;
        }
    }

    if (!found) printf("Keyword not found in file.\n");
    fclose(fp);
}

// Read from File
void readFromFile() {
    char filename[50], fullPath[100];
    printf("Enter file name: ");
    scanf("%s", filename);
    getFullPath(filename, fullPath);

    int fd = open(fullPath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    printf("File Contents:\n");
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
    }

    close(fd);
}

// Delete File
void deleteFile() {
    if (!verifyAdmin()) {
        return;
    }

    char filename[50], fullPath[100];
    printf("Enter file name to delete: ");
    scanf("%s", filename);
    getFullPath(filename, fullPath);

    if (unlink(fullPath) == 0) {
        printf("File '%s' deleted successfully.\n", fullPath);
    } else {
        perror("Error deleting file");
    }
}

int main() {
    int choice;

    printf("Do you want to (1) Login or (2) Register new user? Enter choice: ");
    scanf("%d", &choice);

    if (choice == 2) {
        addUser();
    }

    if (!authenticate()) {
        return 0;
    }

    while (1) {
        printf("\n==== File Management System ====\n");
        printf("1. Create File\n");
        printf("2. Write to File\n");
        printf("3. Modify File\n");
        printf("4. Search Content in File\n");
        printf("5. Read from File\n");
        printf("6. Delete File\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createFile(); break;
            case 2: writeToFile(); break;
            case 3: modifyFile(); break;
            case 4: searchContent(); break;
            case 5: readFromFile(); break;
            case 6: deleteFile(); break;
            case 7: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
