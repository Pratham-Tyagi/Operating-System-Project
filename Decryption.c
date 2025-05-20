#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char key = 'K'; // XOR key used for encryption/decryption

// Function to decrypt a string using XOR cipher
void decrypt(const char *input, char *output) {
    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++) {
        output[i] = input[i] ^ key;
    }
    output[len] = '\0'; // Null-terminate the decrypted string
}

int main() {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) {
        perror("Error opening users.txt");
        return 1;
    }

    char encUser[50], encPass[50];
    char decUser[50], decPass[50];

    printf("Decrypted User Credentials:\n");
    printf("----------------------------\n");

    while (fscanf(fp, "%s %s", encUser, encPass) != EOF) {
        decrypt(encUser, decUser);
        decrypt(encPass, decPass);
        printf("Username: %s\tPassword: %s\n", decUser, decPass);
    }

    fclose(fp);
    return 0;
}
