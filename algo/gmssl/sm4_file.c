#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmssl/sm4.h>
#include <gmssl/rand.h>
void sm4_encrypt_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "rb");
    if (!in) {
        perror("无法打开输入文件");
        exit(1);
    }

    FILE *out = fopen(output_file, "wb");
    if (!out) {
        perror("无法打开输出文件");
        exit(1);
    }

   	SM4_CBC_CTX cbc_ctx;
	unsigned char key[16] = {
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
	};
	unsigned char iv[16] = {
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
	};
	//unsigned char inbuf[1024];
	//unsigned char outbuf[1024 + 32];
	size_t inlen;
	size_t outlen;

    if (sm4_cbc_encrypt_init(&cbc_ctx, key, iv) != 1) {
		fprintf(stderr, "%s %d: error\n", __FILE__, __LINE__);
		return 1;
	}

    unsigned char buffer[1024];
    unsigned char encrypted_buffer[1024+32];
    int bytes_read;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        //sm4_crypt_ecb(&sm4_key, SM4_ENCRYPT, bytes_read, buffer, encrypted_buffer);
        if (sm4_cbc_encrypt_update(&cbc_ctx, buffer, bytes_read, encrypted_buffer, &outlen) != 1) {
			fprintf(stderr, "%s %d: error\n", __FILE__, __LINE__);
			return 1;
		}
        fwrite(encrypted_buffer, 1, outlen, out);
    }


    if (sm4_cbc_encrypt_finish(&cbc_ctx, encrypted_buffer, &outlen) != 1) {
		fprintf(stderr, "%s %d: error\n", __FILE__, __LINE__);
		return 1;
	}
    fwrite(encrypted_buffer, 1, outlen, out);

    fclose(in);
    fclose(out);
}

void sm4_decrypt_file(const char *input_file, const char *output_file) {
    FILE *in = fopen(input_file, "rb");
    if (!in) {
        perror("无法打开输入文件");
        exit(1);
    }

    FILE *out = fopen(output_file, "wb");
    if (!out) {
        perror("无法打开输出文件");
        exit(1);
    }

   
	SM4_CBC_CTX cbc_ctx;
	unsigned char key[16] = {
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
	};
	unsigned char iv[16] = {
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
	};
	//unsigned char inbuf[1024];
	//unsigned char outbuf[1024 + 32];
	size_t inlen;
	size_t outlen;


    unsigned char buffer[1024];
    unsigned char decrypted_buffer[1024+32];
    int bytes_read;


    if (sm4_cbc_decrypt_init(&cbc_ctx, key, iv) != 1) {
		fprintf(stderr, "%s %d: error\n", __FILE__, __LINE__);
		return 1;
	}

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        //sm4_crypt_ecb(&ctx, SM4_DECRYPT, bytes_read, buffer, decrypted_buffer);
        if (sm4_cbc_decrypt_update(&cbc_ctx, buffer, bytes_read, decrypted_buffer, &outlen) != 1) {
			fprintf(stderr, "%s %d: error\n", __FILE__, __LINE__);
			return 1;
		}
        fwrite(decrypted_buffer, 1, outlen, out);
    }

    if (sm4_cbc_decrypt_finish(&cbc_ctx, decrypted_buffer, &outlen) != 1) {
		fprintf(stderr, "%s %d: error\n", __FILE__, __LINE__);
		return 1;
	}
    fwrite(decrypted_buffer, 1, outlen, out);
	
    fclose(in);
    fclose(out);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "用法: %s [encrypt|decrypt] [输入文件] [输出文件] \n", argv[0]);
        return 1;
    }
   // char *key=NULL;
    const char *mode = argv[1];
    const char *input_file = argv[2];
    const char *output_file = argv[3];
    const char *key_str = argv[4];
   

    if (strcmp(mode, "encrypt") == 0) {
        sm4_encrypt_file(input_file, output_file );
    } else if (strcmp(mode, "decrypt") == 0) {
        sm4_decrypt_file(input_file, output_file);
    } else {
        fprintf(stderr, "无效的模式: %s\n", mode);
        return 1;
    }

    printf("操作完成\n");
    return 0;
}

