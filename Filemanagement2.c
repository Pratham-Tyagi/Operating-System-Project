/*  Multi-User File Management System
 *  ---------------------------------
 *  - XOR-encrypted credentials stored in users.txt
 *  - XOR-encrypted admin password stored in admin.txt
 *  - Admin password never appears in the source code
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/* ---------- GLOBALS ---------- */
char  loggedInUser[20];
int   isAdmin = 0;          /* 1 = admin, 0 = normal user               */
const char key = 'K';       /* XOR key for encryption / decryption      */

/* ---------- UTILITY ---------- */
void encryptDecrypt(const char *input, char *output)
{
    size_t len = strlen(input);
    for (size_t i = 0; i < len; ++i)
        output[i] = input[i] ^ key;
    output[len] = '\0';
}

/* ---------- ADMIN PASSWORD  ---------- */

/* Called once at start-up.  If admin.txt does not exist, create it.    */
void initializeAdminPassword(void)
{
    if (access("admin.txt", F_OK) == 0)      /* already exists */
        return;

    char plain[100], enc[100];

    printf("\n*** Initial setup ***\n");
    printf("Set ADMIN password (will be stored encrypted in admin.txt): ");
    scanf("%99s", plain);

    encryptDecrypt(plain, enc);

    FILE *fp = fopen("admin.txt", "w");
    if (!fp) {
        perror("Could not create admin.txt");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%s\n", enc);
    fclose(fp);

    printf("Admin password saved successfully.\n\n");
}

/* Prompt user and verify against the encrypted value in admin.txt.     */
int checkAdminPassword(void)
{
    char enc[200], stored[200], input[200];

    FILE *fp = fopen("admin.txt", "r");
    if (!fp) {
        perror("Error opening admin.txt");
        return 0;
    }
    if (fscanf(fp, "%199s", enc) != 1) {     /* read encrypted pwd   */
        fclose(fp);
        printf("admin.txt is corrupt.\n");
        return 0;
    }
    fclose(fp);

    encryptDecrypt(enc, stored);             /* decrypt stored value */

    printf("Enter admin password: ");
    scanf("%199s", input);

    return strcmp(input, stored) == 0;
}

/* ---------- USER MANAGEMENT ---------- */
void addUser(void)
{
    char username[20], password[20];
    char encUser[50], encPass[50];

    printf("Enter new username: ");
    scanf("%19s", username);
    printf("Enter new password: ");
    scanf("%19s", password);

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

/* Return 1 on success, 0 on failure */
int authenticate(void)
{
    char username[20], password[20];
    char fEncUser[50], fEncPass[50];
    char fUser[50], fPass[50];

    printf("Enter username: ");
    scanf("%19s", username);
    printf("Enter password: ");
    scanf("%19s", password);

    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        perror("Error opening users file");
        return 0;
    }

    while (fscanf(fp, "%49s %49s", fEncUser, fEncPass) == 2) {
        encryptDecrypt(fEncUser, fUser);
        encryptDecrypt(fEncPass, fPass);

        if (!strcmp(username, fUser) && !strcmp(password, fPass)) {
            strcpy(loggedInUser, username);
            fclose(fp);

            if (!strcmp(username, "admin")) {
                printf("\n--- Admin verification ---\n");
                if (checkAdminPassword()) {
                    isAdmin = 1;
                    printf("Admin verification successful.\n");
                } else {
                    isAdmin = 0;
                    printf("Admin verification failed. Logged in as normal user.\n");
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

/* ---------- ADMIN RE-VERIFICATION BEFORE SENSITIVE OPS ---------- */
int verifyAdmin(void)
{
    if (!isAdmin)           /* non-admin users bypass */
        return 1;

    printf("\n--- Admin re-verification required ---\n");
    if (checkAdminPassword())
        return 1;

    printf("Admin password incorrect. Access denied.\n");
    return 0;
}

/* ---------- FILE PATH HELPER ---------- */
void getFullPath(const char *filename, char *fullPath)
{
    char owner[20];
    if (isAdmin) {                           /* admin may act on anyone’s files */
        printf("Enter owner username: ");
        scanf("%19s", owner);
    } else {
        strcpy(owner, loggedInUser);
    }
    sprintf(fullPath, "%s_%s", owner, filename);
}

/* ---------- FILE OPERATIONS ---------- */
void createFile(void)
{
    if (!verifyAdmin())
        return;

    char filename[50], fullPath[100];
    printf("Enter file name: ");
    scanf("%49s", filename);
    getFullPath(filename, fullPath);

    int fd = open(fullPath, O_WRONLY | O_CREAT, 0666);
    if (fd == -1) { perror("Error creating file"); return; }

    printf("File '%s' created successfully.\n", fullPath);
    close(fd);
}

void writeToFile(void)
{
    char filename[50], fullPath[100];
    printf("Enter file name: ");
    scanf("%49s", filename);
    getFullPath(filename, fullPath);

    int fd = open(fullPath, O_WRONLY | O_APPEND);
    if (fd == -1) { perror("Error opening file"); return; }

    char content[256];
    printf("Enter content: ");
    getchar();                              /* clear newline */
    fgets(content, sizeof(content), stdin);

    write(fd, content, strlen(content));
    printf("Content written successfully.\n");
    close(fd);
}

void modifyFile(void)
{
    char filename[50], fullPath[100];
    printf("Enter file name to modify: ");
    scanf("%49s", filename);
    getFullPath(filename, fullPath);

    int fd = open(fullPath, O_WRONLY | O_TRUNC);
    if (fd == -1) { perror("Error opening file"); return; }

    char content[256];
    printf("Enter new content: ");
    getchar();
    fgets(content, sizeof(content), stdin);

    write(fd, content, strlen(content));
    printf("File modified successfully.\n");
    close(fd);
}

void searchContent(void)
{
    char filename[50], keyword[50], fullPath[100];
    printf("Enter file name: ");
    scanf("%49s", filename);
    printf("Enter keyword to search: ");
    scanf("%49s", keyword);
    getFullPath(filename, fullPath);

    FILE *fp = fopen(fullPath, "r");
    if (!fp) { perror("Error opening file"); return; }

    char line[512];
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

void readFromFile(void)
{
    char filename[50], fullPath[100];
    printf("Enter file name: ");
    scanf("%49s", filename);
    getFullPath(filename, fullPath);

    int fd = open(fullPath, O_RDONLY);
    if (fd == -1) { perror("Error opening file"); return; }

    char buf[1024];
    ssize_t n;
    printf("\n----- File Contents (%s) -----\n", fullPath);
    while ((n = read(fd, buf, sizeof(buf))) > 0)
        write(STDOUT_FILENO, buf, n);

    close(fd);
    printf("\n----- End of File -----\n");
}

void deleteFile(void)
{
    if (!verifyAdmin())
        return;

    char filename[50], fullPath[100];
    printf("Enter file name to delete: ");
    scanf("%49s", filename);
    getFullPath(filename, fullPath);

    if (unlink(fullPath) == 0)
        printf("File '%s' deleted successfully.\n", fullPath);
    else
        perror("Error deleting file");
}

/* ---------- MAIN ---------- */
int main(void)
{
    initializeAdminPassword();              /* one-time setup if needed */

    int choice;

    printf("Do you want to (1) Login or (2) Register new user? Enter choice: ");
    scanf("%d", &choice);

    if (choice == 2)
        addUser();

    if (!authenticate())
        return 0;                           /* stop on failed login */

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
            case 1: createFile();    break;
            case 2: writeToFile();   break;
            case 3: modifyFile();    break;
            case 4: searchContent(); break;
            case 5: readFromFile();  break;
            case 6: deleteFile();    break;
            case 7: puts("Exiting...");  exit(0);
            default: puts("Invalid choice! Try again.");
        }
    }
    return 0;
}
