#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 50
const char key = 'K'; // XOR key used for encryption/decryption

// Function to decrypt a string using XOR cipher
void decrypt(const char *input, char *output) {
    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ key;
    }
    output[len] = '\0'; // Null-terminate the decrypted string
}

// Function to read and decrypt user credentials from a file
void readAndDecryptUsers(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        return;
    }

    char encUser[MAX_LEN], encPass[MAX_LEN];
    char decUser[MAX_LEN], decPass[MAX_LEN];

    printf("Decrypted User Credentials:\n");
    printf("----------------------------\n");

    while (fscanf(fp, "%s %s", encUser, encPass) != EOF) {
        decrypt(encUser, decUser);
        decrypt(encPass, decPass);
        printf("Username: %s\tPassword: %s\n", decUser, decPass);
    }

    fclose(fp);
}

int main() {
    const char *filename = "users.txt";
    readAndDecryptUsers(filename);
    return 0;
}
