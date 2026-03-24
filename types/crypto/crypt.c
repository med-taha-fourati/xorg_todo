#include "crypt.h"

#include <stdint.h>

#define MAX_BYTES 256

#define AES_BLOCK_SIZE 16
#define AES_256_KEY_SIZE 32
#define AES_256_ROUNDS 14
#define AES_256_EXPANDED_KEY_SIZE 240
static Crypt* g_crypt_ctx = NULL;

unsigned const char sbox[MAX_BYTES] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

static const unsigned char inv_sbox[256] = {
    0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb,
    0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb,
    0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e,
    0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25,
    0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92,
    0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84,
    0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06,
    0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b,
    0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73,
    0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e,
    0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b,
    0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4,
    0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f,
    0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef,
    0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61,
    0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d
};

static const unsigned char rcon[15] = {
    0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1B,0x36,0x6C,0xD8,0xAB,0x4D
};

unsigned char split[MAX_BYTES] = {0x00};

static void padder(unsigned char* pad_in, unsigned char* pad_out) {
    size_t i = 0;
    while (pad_in[i] != '\0' && i < MAX_BYTES) {
        pad_out[i] = pad_in[i];
        i++;
    }
    unsigned char padding_value = (unsigned char)(MAX_BYTES - i);

    while (i < MAX_BYTES) {
        pad_out[i] = padding_value;
        i++;
    }
}

static unsigned char** splitter(unsigned char* split_in, int* num_chunks) {
    if (split_in == NULL) return NULL;

    size_t in_len = strlen((char*)split_in);
    *num_chunks = (in_len / MAX_BYTES) + 1;

    unsigned char** chunks = malloc((*num_chunks) * sizeof(unsigned char*));

    for (int i = 0; i < *num_chunks; i++) {
        chunks[i] = malloc(MAX_BYTES);

        size_t start_offset = i * MAX_BYTES;

        if (i < (*num_chunks - 1)) {
            memcpy(chunks[i], split_in + start_offset, MAX_BYTES);
        }
        else {
            padder(split_in + start_offset, chunks[i]);
        }
    }

    return chunks;
}

static int pkcs7_unpad_16(unsigned char* in_out, size_t* in_out_len) {
    if (!in_out || !in_out_len || *in_out_len == 0) return -1;
    if ((*in_out_len % AES_BLOCK_SIZE) != 0) return -2;

    unsigned char pad = in_out[*in_out_len - 1];
    if (pad == 0 || pad > AES_BLOCK_SIZE) return -3;
    for (unsigned char i = 0; i < pad; i++) {
        if (in_out[*in_out_len - 1 - i] != pad) return -4;
    }
    *in_out_len -= pad;
    return 0;
}

static void bytes_to_hex(const unsigned char* in, size_t len, char* out_hex) {
    static const char* hex = "0123456789abcdef";
    for (size_t i = 0; i < len; i++) {
        out_hex[i * 2 + 0] = hex[(in[i] >> 4) & 0x0F];
        out_hex[i * 2 + 1] = hex[in[i] & 0x0F];
    }
    out_hex[len * 2] = '\0';
}

static int hex_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a');
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    return -1;
}

static unsigned char* hex_to_bytes(const char* hex_str, size_t* out_len) {
    if (!hex_str) return NULL;
    size_t n = strlen(hex_str);
    if ((n % 2) != 0) return NULL;

    size_t bytes_len = n / 2;
    unsigned char* out = (unsigned char*)malloc(bytes_len);
    if (!out) return NULL;

    for (size_t i = 0; i < bytes_len; i++) {
        int hi = hex_val(hex_str[i * 2]);
        int lo = hex_val(hex_str[i * 2 + 1]);
        if (hi < 0 || lo < 0) {
            free(out);
            return NULL;
        }
        out[i] = (unsigned char)((hi << 4) | lo);
    }
    if (out_len) *out_len = bytes_len;
    return out;
}


/*
 * @brief So basically like a fucking miltpltlier that is like super fast or sum shi-
 * shift everything to the left by 1 bit, and if the high bit is set, XOR with 0x1b (prevents overflow)
 */
static unsigned char xtime(unsigned char x) {
    return (unsigned char)((x << 1) ^ ((x & 0x80) ? 0x1b : 0x00));
}

/*
 * @brief Galois field multiplication of two bytes (super fast)
 * @param we take a = uhhhh 0b10000000 and b = uhhhh 0b00000010
 */
static unsigned char gf_mul(unsigned char a, unsigned char b) {
    unsigned char res = 0;
    while (b) { // b is not all 0, we ball
        if (b & 1) res ^= a; // b is not rightmost 1, we dont add a (XOR) it to res
        a = xtime(a); // apply the very HYPER fast xtime multiplication
        b >>= 1;
    }
    return res;
}

static void add_round_key(unsigned char state[16], const unsigned char* round_key) {
    for (int i = 0; i < 16; i++) state[i] ^= round_key[i];
}

static void sub_bytes_state(unsigned char state[16]) {
    for (int i = 0; i < 16; i++) state[i] = sbox[state[i]];
}

static void inv_sub_bytes_state(unsigned char state[16]) {
    for (int i = 0; i < 16; i++) state[i] = inv_sbox[state[i]];
}

static unsigned char* pkcs7_pad_16(const unsigned char* in, size_t len, size_t* out_len) {
    size_t pad = AES_BLOCK_SIZE - (len % AES_BLOCK_SIZE);
    if (pad == 0) pad = AES_BLOCK_SIZE;
    size_t total = len + pad;

    unsigned char* out = (unsigned char*)malloc(total);
    if (!out) return NULL;
    memcpy(out, in, len);
    memset(out + len, (unsigned char)pad, pad);
    if (out_len) *out_len = total;
    return out;
}

static void shift_rows(unsigned char state[16]) {
    unsigned char tmp;

    // Row 1: shift left by 1
    // { - 1 - - } => { - 13 - - }
    // { - 5 - - } => { - 1 - - }
    // { - 9 - - } => { - 5 - - }
    // { - 13 - - } => { - 9 - - }
    tmp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = tmp;

    // Row 2: shift left by 2
    // { - - 2 - } => { - - 10 - }
    // { - - 6 - } => { - - 14 - }
    // { - - 10 - } => { - - 2 - }
    // { - - 14 - } => { - - 6 - }
    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[6];
    state[6] = state[14];
    state[14] = tmp;

    // Row 3: shift left by 3 (or right by 1)
    // { - - - 3 } => { - - - 7 }
    // { - - - 7 } => { - - - 11 }
    // { - - - 11 } => { - - - 15 }
    // { - - - 15 } => { - - - 3 }
    tmp = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = state[3];
    state[3] = tmp;
}

static void inv_shift_rows(unsigned char state[16]) {
    unsigned char tmp;

    // Row 1: shift right by 1
    tmp = state[13];
    state[13] = state[9];
    state[9] = state[5];
    state[5] = state[1];
    state[1] = tmp;

    // Row 2: shift right by 2
    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[6];
    state[6] = state[14];
    state[14] = tmp;

    // Row 3: shift right by 3 (or left by 1)
    tmp = state[3];
    state[3] = state[7];
    state[7] = state[11];
    state[11] = state[15];
    state[15] = tmp;
}

static void mix_columns(unsigned char state[16]) {
    for (int c = 0; c<16; c+=4) {
        //int i = c * 4;
        unsigned char a0 = state[c + 0];
        unsigned char a1 = state[c + 1];
        unsigned char a2 = state[c + 2];
        unsigned char a3 = state[c + 3];

        state[c + 0] = (unsigned char)(gf_mul(a0,2) ^ gf_mul(a1,3) ^ a2 ^ a3);
        state[c + 1] = (unsigned char)(a0 ^ gf_mul(a1,2) ^ gf_mul(a2,3) ^ a3);
        state[c + 2] = (unsigned char)(a0 ^ a1 ^ gf_mul(a2,2) ^ gf_mul(a3,3));
        state[c + 3] = (unsigned char)(gf_mul(a0,3) ^ a1 ^ a2 ^ gf_mul(a3,2));
    }
}

static void inv_mix_columns(unsigned char state[16]) {
    for (int c = 0; c<16; c+=4) {
        unsigned char a0 = state[c + 0];
        unsigned char a1 = state[c + 1];
        unsigned char a2 = state[c + 2];
        unsigned char a3 = state[c + 3];

        state[c + 0] = (unsigned char)(gf_mul(a0,14) ^ gf_mul(a1,11) ^ gf_mul(a2,13) ^ gf_mul(a3,9));
        state[c + 1] = (unsigned char)(gf_mul(a0,9) ^ gf_mul(a1,14) ^ gf_mul(a2,11) ^ gf_mul(a3,13));
        state[c + 2] = (unsigned char)(gf_mul(a0,13) ^ gf_mul(a1,9) ^ gf_mul(a2,14) ^ gf_mul(a3,11));
        state[c + 3] = (unsigned char)(gf_mul(a0,11) ^ gf_mul(a1,13) ^ gf_mul(a2,9) ^ gf_mul(a3,14));
    }
}

static void key_expansion_aes256(const unsigned char* key32, unsigned char* expanded_key240) {
    // Expanded key is 60 words (4 bytes) = 240 bytes for AES-256
    memcpy(expanded_key240, key32, AES_256_KEY_SIZE);

    int bytes_generated = AES_256_KEY_SIZE;
    int rcon_iter = 1;
    unsigned char temp[4];

    while (bytes_generated < AES_256_EXPANDED_KEY_SIZE) {
        for (int i = 0; i < 4; i++) temp[i] = expanded_key240[bytes_generated - 4 + i]; // last 4 bytes of round

        if ((bytes_generated % AES_256_KEY_SIZE) == 0) {
            // rotate word
            unsigned char t = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = t;

            // substitue with the table above
            temp[0] = sbox[temp[0]];
            temp[1] = sbox[temp[1]];
            temp[2] = sbox[temp[2]];
            temp[3] = sbox[temp[3]];

            // XOR with Rcon
            temp[0] ^= rcon[rcon_iter++];
        } else if ((bytes_generated % AES_256_KEY_SIZE) == 16) {
            // we need this cuz uhhh thats how AES-256 works (according to the spec)
            temp[0] = sbox[temp[0]];
            temp[1] = sbox[temp[1]];
            temp[2] = sbox[temp[2]];
            temp[3] = sbox[temp[3]];
        }

        // return everything
        for (int i = 0; i < 4; i++) {
            expanded_key240[bytes_generated] = (unsigned char)(expanded_key240[bytes_generated - AES_256_KEY_SIZE] ^ temp[i]);
            bytes_generated++;
        }
    }
}

/*
 * proceed with encrypting a chunk/block
 */
static void aes256_encrypt_block(const unsigned char* in16, unsigned char* out16, const unsigned char* expanded_key240) {
    unsigned char state[16];
    memcpy(state, in16, 16);

    add_round_key(state, expanded_key240);

    for (int round = 1; round < AES_256_ROUNDS; round++) {
        sub_bytes_state(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, expanded_key240 + (round * 16));
    }

    sub_bytes_state(state);
    shift_rows(state);
    add_round_key(state, expanded_key240 + (AES_256_ROUNDS * 16));

    memcpy(out16, state, 16);
}

static void aes256_decrypt_block(const unsigned char* in16, unsigned char* out16, const unsigned char* expanded_key240) {
    unsigned char state[16];
    memcpy(state, in16, 16);

    add_round_key(state, expanded_key240 + (AES_256_ROUNDS * 16));

    for (int round = AES_256_ROUNDS - 1; round >= 1; round--) {
        inv_shift_rows(state);
        inv_sub_bytes_state(state);
        add_round_key(state, expanded_key240 + (round * 16));
        inv_mix_columns(state);
    }

    inv_shift_rows(state);
    inv_sub_bytes_state(state);
    add_round_key(state, expanded_key240);

    memcpy(out16, state, 16);
}

static int aes256_cbc_encrypt_raw(const unsigned char* plaintext, size_t len,
                                 const unsigned char* key32,
                                 const unsigned char iv16[16],
                                 unsigned char** out_ciphertext) {
    if (!plaintext || !key32 || !iv16 || !out_ciphertext) return -1;
    if ((len % AES_BLOCK_SIZE) != 0) return -2;

    unsigned char expanded[AES_256_EXPANDED_KEY_SIZE];
    key_expansion_aes256(key32, expanded);

    unsigned char prev[AES_BLOCK_SIZE];
    memcpy(prev, iv16, AES_BLOCK_SIZE);

    for (size_t off = 0; off < len; off += AES_BLOCK_SIZE) {
        unsigned char block[AES_BLOCK_SIZE];
        for (int i = 0; i < AES_BLOCK_SIZE; i++) block[i] = (unsigned char)(plaintext[off + i] ^ prev[i]);
        aes256_encrypt_block(block, (*out_ciphertext + off), expanded);
        memcpy(prev, (*out_ciphertext + off), AES_BLOCK_SIZE);
    }
    return 0;
}

static int aes256_cbc_decrypt_raw(const unsigned char* ciphertext, size_t len,
                                 const unsigned char* key32,
                                 const unsigned char iv16[16],
                                 unsigned char** out_plaintext) {
    if (!ciphertext || !key32 || !iv16 || !out_plaintext) return -1;
    if ((len % AES_BLOCK_SIZE) != 0) return -2;

    unsigned char expanded[AES_256_EXPANDED_KEY_SIZE];
    key_expansion_aes256(key32, expanded);

    unsigned char prev[AES_BLOCK_SIZE];
    memcpy(prev, iv16, AES_BLOCK_SIZE);

    for (size_t off = 0; off < len; off += AES_BLOCK_SIZE) {
        unsigned char block[AES_BLOCK_SIZE];
        aes256_decrypt_block(ciphertext + off, block, expanded);
        /* Fixed: correctly write into the caller-provided output buffer.
         * Previous code used *out_plaintext[off + i] which is incorrect due to operator precedence.
         * We must index into the buffer pointed to by *out_plaintext.
         *
         * Oh fuck you gpt :sob:
         */
        for (int i = 0; i < AES_BLOCK_SIZE; i++) {
            (*out_plaintext)[off + i] = (unsigned char)(block[i] ^ prev[i]);
        }
        memcpy(prev, ciphertext + off, AES_BLOCK_SIZE);
    }
    return 0;
}

static int crypt_encrypt_aes256_cbc(const unsigned char* plaintext, size_t len,
                                   const unsigned char* key, unsigned char** out) {
    // Low-level: caller must provide `len` that is already a multiple of 16.
    // Uses the IV stored in the last `crypt_init()`-initialized context.
    if (!g_crypt_ctx || !g_crypt_ctx->iv || g_crypt_ctx->iv_len != AES_BLOCK_SIZE) return -1;
    if (!plaintext || !key || !out) return -2;
    if ((len % AES_BLOCK_SIZE) != 0) return -3;

    return aes256_cbc_encrypt_raw(plaintext, len, key, (const unsigned char*)g_crypt_ctx->iv,out);
}

int crypt_encrypt_string_aes256_cbc_hex(Crypt* crypt,
                                       const char* plaintext,
                                       const unsigned char* key,
                                       char** out_hex_ciphertext) {
    if (!crypt || !plaintext || !key || !out_hex_ciphertext) return -1;
    if (crypt->iv_len != AES_BLOCK_SIZE) return -2;

    size_t plain_len = strlen(plaintext);
    size_t padded_len = 0;
    unsigned char* padded = pkcs7_pad_16((const unsigned char*)plaintext, plain_len, &padded_len);
    if (!padded) return -3;

    unsigned char* ciphertext = (unsigned char*)malloc(padded_len);
    if (!ciphertext) {
        free(padded);
        return -4;
    }

    int rc = aes256_cbc_encrypt_raw(padded, padded_len, key, (const unsigned char*)crypt->iv, &ciphertext);
    free(padded);
    if (rc != 0) {
        free(ciphertext);
        return -5;
    }

    char* hex = (char*)malloc(padded_len * 2 + 1);
    if (!hex) {
        free(ciphertext);
        return -6;
    }
    bytes_to_hex(ciphertext, padded_len, hex);
    free(ciphertext);

    *out_hex_ciphertext = hex;
    return 0;
}

int crypt_decrypt_string_aes256_cbc_hex(Crypt* crypt,
                                       const char* hex_ciphertext,
                                       const unsigned char* key,
                                       char** out_plaintext) {
    if (!crypt || !hex_ciphertext || !key || !out_plaintext) return -1;
    if (crypt->iv_len != AES_BLOCK_SIZE) return -2;

    size_t cipher_len = 0;
    unsigned char* ciphertext = hex_to_bytes(hex_ciphertext, &cipher_len);
    if (!ciphertext) return -3;
    if ((cipher_len % AES_BLOCK_SIZE) != 0) {
        free(ciphertext);
        return -4;
    }

    unsigned char* padded_plain = (unsigned char*)malloc(cipher_len);
    if (!padded_plain) {
        free(ciphertext);
        return -5;
    }

    int rc = aes256_cbc_decrypt_raw(ciphertext, cipher_len, key, (const unsigned char*)crypt->iv, &padded_plain);
    free(ciphertext);
    if (rc != 0) {
        free(padded_plain);
        return -6;
    }

    size_t plain_len = cipher_len;
    rc = pkcs7_unpad_16(padded_plain, &plain_len);
    if (rc != 0) {
        free(padded_plain);
        return -7;
    }

    char* out = (char*)malloc(plain_len + 1);
    if (!out) {
        free(padded_plain);
        return -8;
    }
    memcpy(out, padded_plain, plain_len);
    out[plain_len] = '\0';
    free(padded_plain);

    *out_plaintext = out;
    return 0;
}





// ---------------------------
// Crypt interface wiring
// ---------------------------

// The function-pointer interface in `Crypt` does not pass `Crypt*`.
// To still support the struct API as you designed it, we keep a tiny
// internal "current context".
//
// This is NOT thread-safe, but it keeps your API unchanged.


static int crypt_decrypt_aes256_cbc(const unsigned char* ciphertext, size_t len,
                                   const unsigned char* key, unsigned char** out) {
    if (!g_crypt_ctx || !g_crypt_ctx->iv || g_crypt_ctx->iv_len != AES_BLOCK_SIZE) return -1;
    if (!ciphertext || !key || !out) return -2;
    if ((len % AES_BLOCK_SIZE) != 0) return -3;

    return aes256_cbc_decrypt_raw(ciphertext, len, key, (const unsigned char*)g_crypt_ctx->iv, out);
}

// Since `Crypt->encrypt/decrypt` signatures do not include `Crypt*`,
// we implement the real functionality in the public string wrappers below.

Crypt* crypt_init(const unsigned char* iv, size_t iv_len) {
    Crypt* crypt = malloc(sizeof(Crypt));
    if (!crypt) return NULL;

    crypt->iv = malloc(iv_len);
    if (!crypt->iv) {
        free(crypt);
        return NULL;
    }

    memcpy(crypt->iv, iv, iv_len);
    crypt->iv_len = iv_len;

    g_crypt_ctx = crypt;

    crypt->encrypt = crypt_encrypt_aes256_cbc;
    crypt->decrypt = crypt_decrypt_aes256_cbc;

    return crypt;
}

int crypt_destroy(Crypt* crypt) {
    if (!crypt) return -1;

    if (g_crypt_ctx == crypt) g_crypt_ctx = NULL;

    free(crypt->iv);
    free(crypt);

    return 0;
}
