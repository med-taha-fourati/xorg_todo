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

// AES-256-CBC convenience wrappers for null-terminated strings.
//
// Notes:
// - `key` must point to 32 bytes.
// - Uses `crypt->iv` as the CBC IV (must be 16 bytes).
// - Output is HEX text so it is safe as a `char*`.
// - The function allocates `*out_*` with `malloc`; caller must `free`.
int crypt_encrypt_string_aes256_cbc_hex(Crypt* crypt,
                                       const char* plaintext,
                                       const unsigned char* key,
                                       char** out_hex_ciphertext);

int crypt_decrypt_string_aes256_cbc_hex(Crypt* crypt,
                                       const char* hex_ciphertext,
                                       const unsigned char* key,
                                       char** out_plaintext);



#endif
