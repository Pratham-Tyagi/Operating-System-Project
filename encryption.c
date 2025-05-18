#include <string.h>
#include "encryption.h"

const char key = 'K'; // XOR key

void encryptDecrypt(char *input, char *output) {
    int len = strlen(input);
    for (int i = 0; i < len; i++) {
        output[i] = input[i] ^ key;
    }
    output[len] = '\0';
}
