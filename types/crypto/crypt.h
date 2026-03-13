#ifndef XORG_TODO_TYPES_CRYPTO_CRYPT_H
#define XORG_TODO_TYPES_CRYPTO_CRYPT_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct Crypt {
    unsigned char *iv;           // Binary buffer, not a null-terminated string
    size_t iv_len;               // AES usually needs 16 bytes

    int (*encrypt)(const unsigned char* plaintext, size_t len,
                   const unsigned char* key, unsigned char* out);

    int (*decrypt)(const unsigned char* ciphertext, size_t len,
                   const unsigned char* key, unsigned char* out);
} Crypt;

Crypt* crypt_init(const unsigned char* iv, size_t iv_len);
int crypt_destroy(Crypt* crypt);



#endif
