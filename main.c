#include <stdio.h>
#include <stdlib.h>
#include "user_auth.h"
#include "file_operations.h"

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
