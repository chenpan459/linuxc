#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BUFFER_SIZE 1024

// MD5 Constants
#define F(x, y, z) ((x & y) | (~x & z))
#define G(x, y, z) ((x & z) | (y & ~z))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | ~z))
#define ROTATE_LEFT(x, n) ((x << n) | (x >> (32 - n)))

// MD5 Transformation Functions
#define FF(a, b, c, d, x, s, ac) { \
    a += F(b, c, d) + x + ac; \
    a = ROTATE_LEFT(a, s); \
    a += b; \
}
#define GG(a, b, c, d, x, s, ac) { \
    a += G(b, c, d) + x + ac; \
    a = ROTATE_LEFT(a, s); \
    a += b; \
}
#define HH(a, b, c, d, x, s, ac) { \
    a += H(b, c, d) + x + ac; \
    a = ROTATE_LEFT(a, s); \
    a += b; \
}
#define II(a, b, c, d, x, s, ac) { \
    a += I(b, c, d) + x + ac; \
    a = ROTATE_LEFT(a, s); \
    a += b; \
}

typedef struct {
    uint32_t state[4];  // A, B, C, D
    uint32_t count[2];  // Number of bits, modulo 2^64 (low-order word first)
    uint8_t buffer[64]; // Input buffer
} MD5_CTX;

void MD5_Init(MD5_CTX *context) {
    context->count[0] = context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xefcdab89;
    context->state[2] = 0x98badcfe;
    context->state[3] = 0x10325476;
}

void MD5_Transform(uint32_t state[4], const uint8_t block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    for (int i = 0, j = 0; i < 16; ++i, j += 4)
        x[i] = (uint32_t)(block[j]) | ((uint32_t)(block[j+1]) << 8) |
               ((uint32_t)(block[j+2]) << 16) | ((uint32_t)(block[j+3]) << 24);

    // Round 1
    FF(a, b, c, d, x[ 0],  7, 0xd76aa478);
    FF(d, a, b, c, x[ 1], 12, 0xe8c7b756);
    // ... [Additional FF transformations omitted for brevity]

    // Round 2
    GG(a, b, c, d, x[ 1],  5, 0xf61e2562);
    GG(d, a, b, c, x[ 6],  9, 0xc040b340);
    // ... [Additional GG transformations omitted for brevity]

    // Round 3
    HH(a, b, c, d, x[ 5],  4, 0xfffa3942);
    HH(d, a, b, c, x[ 8], 11, 0x8771f681);
    // ... [Additional HH transformations omitted for brevity]

    // Round 4
    II(a, b, c, d, x[ 0],  6, 0xf4292244);
    II(d, a, b, c, x[ 7], 10, 0x432aff97);
    // ... [Additional II transformations omitted for brevity]

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}

void MD5_Update(MD5_CTX *context, const uint8_t *input, size_t inputLen) {
    size_t i, index, partLen;
    index = (context->count[0] >> 3) & 0x3F;
    if ((context->count[0] += ((uint32_t)inputLen << 3)) < ((uint32_t)inputLen << 3))
        context->count[1]++;
    context->count[1] += ((uint32_t)inputLen >> 29);

    partLen = 64 - index;

    if (inputLen >= partLen) {
        memcpy(&context->buffer[index], input, partLen);
        MD5_Transform(context->state, context->buffer);
        for (i = partLen; i + 63 < inputLen; i += 64)
            MD5_Transform(context->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }

    memcpy(&context->buffer[index], &input[i], inputLen - i);
}

void MD5_Final(uint8_t digest[16], MD5_CTX *context) {
    uint8_t bits[8];
    uint32_t index, padLen;
    static uint8_t PADDING[64] = {0x80, 0, };

    for (index = 0; index < 8; index++)
        bits[index] = (context->count[(index >= 4 ? 0 : 1)] >> ((index % 4) * 8)) & 0xFF;
    
    index = (context->count[0] >> 3) & 0x3f;
    padLen = (index < 56) ? (56 - index) : (120 - index);
    MD5_Update(context, PADDING, padLen);
    MD5_Update(context, bits, 8);

    for (index = 0; index < 16; index++)
        digest[index] = (context->state[index >> 2] >> ((index & 0x03) * 8)) & 0xFF;
}

void calculate_md5(const char *filename, uint8_t digest[16]) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Cannot open file %s\n", filename);
        exit(1);
    }

    MD5_CTX context;
    MD5_Init(&context);

    uint8_t buffer[BUFFER_SIZE];
    size_t bytes;

    while ((bytes = fread(buffer, 1, BUFFER_SIZE, file)) != 0) {
        MD5_Update(&context, buffer, bytes);
    }

    MD5_Final(digest, &context);

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    uint8_t digest[16];
    calculate_md5(argv[1], digest);

    printf("MD5 (%s) = ", argv[1]);
    for (int i = 0; i < 16; i++) {
        printf("%02x", digest[i]);
    }
    printf("\n");

    return 0;
}
