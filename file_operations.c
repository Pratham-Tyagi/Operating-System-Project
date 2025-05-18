#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "utils.h"
#include "admin.h"

void createFile() {
    if (!verifyAdmin()) return;

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
    getchar();
    fgets(content, sizeof(content), stdin);

    write(fd, content, strlen(content));
    printf("Content written successfully.\n");
    close(fd);
}

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
    getchar();
    fgets(content, sizeof(content), stdin);

    write(fd, content, strlen(content));
    printf("File modified successfully.\n");
    close(fd);
}

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

void deleteFile() {
    if (!verifyAdmin()) return;

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
