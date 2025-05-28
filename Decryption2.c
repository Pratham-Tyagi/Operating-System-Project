#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define XOR_KEY 'K'  // Same key used in the main project

void decrypt(const char *input, char *output) {
    size_t len = strlen(input);
    for (size_t i = 0; i < len; ++i)
        output[i] = input[i] ^ XOR_KEY;
    output[len] = '\0';
}

int main() {
    FILE *fp = fopen("admin.txt", "r");
    if (!fp) {
        perror("Could not open admin.txt");
        return 1;
    }

    char encrypted[100], decrypted[100];

    if (fscanf(fp, "%99s", encrypted) != 1) {
        printf("admin.txt is empty or corrupt.\n");
        fclose(fp);
        return 1;
    }
    fclose(fp);

    decrypt(encrypted, decrypted);
    printf("Decrypted admin password: %s\n", decrypted);

    return 0;
}
