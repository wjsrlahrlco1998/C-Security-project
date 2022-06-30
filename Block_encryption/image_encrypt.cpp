#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#define BUFSIZE 4096



void des_gif(FILE* fp_in, FILE* fp_out, FILE* fp_back) {
	int n;
	unsigned char buf_in[BUFSIZE], buf_out[BUFSIZE], buf_back[BUFSIZE];

	memset(buf_in, 0, sizeof(buf_in));
	memset(buf_out, 0, sizeof(buf_out));
	memset(buf_back, 0, sizeof(buf_back));

	DES_cblock key;
	DES_cblock seed = { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };
	DES_key_schedule keysched;

	RAND_seed(seed, sizeof(DES_cblock));

	DES_random_key(&key);
	DES_set_key((DES_cblock*)key, &keysched);

	/*파일 크기*/
	fseek(fp_in, SEEK_SET, SEEK_END);
	int size = ftell(fp_in);
	fseek(fp_in, SEEK_SET, SEEK_SET);

	/*파일 Header*/

	n = fread(buf_in, sizeof(char), 54, fp_in);
	fwrite(buf_in, sizeof(char), n, fp_out);
	fwrite(buf_in, sizeof(char), n, fp_back);
	
	
	while ((n = fread(buf_in, sizeof(char), 8, fp_in)) > 0) {
		buf_in[n] = '\0';
		DES_ecb_encrypt((DES_cblock*)buf_in, (DES_cblock*)buf_out, &keysched, DES_ENCRYPT);
		fwrite(buf_out, sizeof(char), n, fp_out);

		DES_ecb_encrypt((DES_cblock*)buf_out, (DES_cblock*)buf_back, &keysched, DES_DECRYPT);
		fwrite(buf_back, sizeof(char), n, fp_back);
	}
}

void des3_gif(FILE* fp_in, FILE* fp_out, FILE* fp_back) {
	int n;

	unsigned char buf_in[BUFSIZE], buf_out[BUFSIZE], buf_back[BUFSIZE];

	memset(buf_in, 0, sizeof(buf_in));
	memset(buf_out, 0, sizeof(buf_out));
	memset(buf_back, 0, sizeof(buf_back));

	DES_cblock key1, key2, key3;
	DES_cblock seed = { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };
	DES_key_schedule keysched1, keysched2, keysched3;

	RAND_seed(seed, sizeof(DES_cblock));
	DES_random_key(&key1);
	DES_random_key(&key2);
	DES_random_key(&key3);
	DES_set_key((DES_cblock*)key1, &keysched1);
	DES_set_key((DES_cblock*)key2, &keysched2);
	DES_set_key((DES_cblock*)key3, &keysched3);

	/*파일 크기*/
	fseek(fp_in, SEEK_SET, SEEK_END);
	int size = ftell(fp_in);
	fseek(fp_in, SEEK_SET, SEEK_SET);

	/*파일 Header*/
	n = fread(buf_in, sizeof(char), 54, fp_in);
	fwrite(buf_in, sizeof(char), n, fp_out);
	fwrite(buf_in, sizeof(char), n, fp_back);

	while ((n = fread(buf_in, sizeof(char), 8, fp_in)) > 0) {
		DES_ecb3_encrypt((DES_cblock*)buf_in, (DES_cblock*)buf_out, &keysched1, &keysched2, &keysched3, DES_ENCRYPT);
		fwrite(buf_out, sizeof(char), n, fp_out);

		DES_ecb3_encrypt((DES_cblock*)buf_out, (DES_cblock*)buf_back, &keysched1, &keysched2, &keysched3, DES_DECRYPT);
		fwrite(buf_back, sizeof(char), n, fp_back);
	}
}

void aes_gif(FILE* fp_in, FILE* fp_out, FILE* fp_back) {
	int n;

	unsigned char buf_in[BUFSIZE], buf_out[BUFSIZE], buf_back[BUFSIZE];

	memset(buf_in, 0, sizeof(buf_in));
	memset(buf_out, 0, sizeof(buf_out));
	memset(buf_back, 0, sizeof(buf_back));

	int keylength = 128;
	unsigned char aes_key[128];

	memset(aes_key, 0, sizeof(aes_key));

	if (!RAND_bytes(aes_key, keylength))
		exit(-1);
	aes_key[keylength - 1] = '\0';

	AES_KEY enc_key, dec_key;

	AES_set_encrypt_key(aes_key, 128, &enc_key);
	AES_set_decrypt_key(aes_key, 128, &dec_key);

	/*파일 크기*/
	fseek(fp_in, SEEK_SET, SEEK_END);
	int size = ftell(fp_in);
	fseek(fp_in, SEEK_SET, SEEK_SET);

	/*파일 Header*/
	n = fread(buf_in, sizeof(char), 54, fp_in);
	fwrite(buf_in, sizeof(char), n, fp_out);
	fwrite(buf_in, sizeof(char), n, fp_back);

	while ((n = fread(buf_in, sizeof(char), 16, fp_in)) > 0) {
		AES_ecb_encrypt(buf_in, buf_out, &enc_key, AES_ENCRYPT);
		fwrite(buf_out, sizeof(char), n, fp_out);
		AES_ecb_encrypt(buf_out, buf_back, &dec_key, AES_DECRYPT);
		fwrite(buf_back, sizeof(char), n, fp_back);
	}
}

void des_CBC_gif(FILE* fp_in, FILE* fp_out, FILE* fp_back) {
	int n;

	unsigned char buf_in[BUFSIZE], buf_out[BUFSIZE], buf_back[BUFSIZE];

	memset(buf_in, 0, sizeof(buf_in));
	memset(buf_out, 0, sizeof(buf_out));
	memset(buf_back, 0, sizeof(buf_back));

	DES_cblock key;
	DES_cblock iv = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	DES_set_odd_parity(&iv);
	DES_cblock seed = { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };
	DES_key_schedule keysched;

	RAND_seed(seed, sizeof(DES_cblock));

	DES_random_key(&key);
	DES_set_key((DES_cblock*)key, &keysched);

	/*파일 크기*/
	fseek(fp_in, SEEK_SET, SEEK_END);
	int size = ftell(fp_in);
	fseek(fp_in, SEEK_SET, SEEK_SET);

	/*파일 Header*/
	n = fread(buf_in, sizeof(char), 54, fp_in);
	fwrite(buf_in, sizeof(char), n, fp_out);
	fwrite(buf_in, sizeof(char), n, fp_back);

	while ((n = fread(buf_in, sizeof(char), BUFSIZE, fp_in)) > 0) {
		//memset(iv, 0, sizeof(DES_cblock));
		//DES_set_odd_parity(&iv);
		DES_cbc_encrypt(buf_in, buf_out, sizeof(buf_in), &keysched, &iv, DES_ENCRYPT);
		fwrite(buf_out, sizeof(char), n, fp_out);
		memset(iv, 0, sizeof(DES_cblock));
		DES_set_odd_parity(&iv);
		DES_cbc_encrypt(buf_out, buf_back, sizeof(buf_out), &keysched, &iv, DES_DECRYPT);
		fwrite(buf_back, sizeof(char), n, fp_back);
	}
}

void des3_CBC_gif(FILE* fp_in, FILE* fp_out, FILE* fp_back) {
	int n;
	int pedding_len = 0;

	unsigned char buf_in[BUFSIZE], buf_out[BUFSIZE], buf_back[BUFSIZE];

	memset(buf_in, 0, sizeof(buf_in));
	memset(buf_out, 0, sizeof(buf_out));
	memset(buf_back, 0, sizeof(buf_back));

	DES_cblock key1, key2, key3;
	DES_cblock iv = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	DES_set_odd_parity(&iv);
	DES_cblock seed = { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 };
	DES_key_schedule keysched1, keysched2, keysched3;

	RAND_seed(seed, sizeof(DES_cblock));
	DES_random_key(&key1);
	DES_random_key(&key2);
	DES_random_key(&key3);
	DES_set_key((DES_cblock*)key1, &keysched1);
	DES_set_key((DES_cblock*)key2, &keysched2);
	DES_set_key((DES_cblock*)key3, &keysched3);

	/*파일 크기*/
	fseek(fp_in, SEEK_SET, SEEK_END);
	int size = ftell(fp_in);
	fseek(fp_in, SEEK_SET, SEEK_SET);

	/*파일 Header*/
	n = fread(buf_in, sizeof(char), 54, fp_in);
	fwrite(buf_in, sizeof(char), n, fp_out);
	fwrite(buf_in, sizeof(char), n, fp_back);

	while ((n = fread(buf_in, sizeof(char), BUFSIZE, fp_in)) > 0) {
		DES_ede3_cbc_encrypt(buf_in, buf_out, sizeof(buf_in), &keysched1, &keysched2, &keysched3, &iv, DES_ENCRYPT);
		fwrite(buf_out, sizeof(char), n, fp_out);
		memset(iv, 0, sizeof(DES_cblock));
		DES_set_odd_parity(&iv);
		DES_ede3_cbc_encrypt(buf_out, buf_back, sizeof(buf_out), &keysched1, &keysched2, &keysched3, &iv, DES_DECRYPT);
		fwrite(buf_back, sizeof(char), n, fp_back);
	}
}

void aes_CBC_gif(FILE* fp_in, FILE* fp_out, FILE* fp_back) {
	int n;

	unsigned char buf_in[BUFSIZE], buf_out[BUFSIZE], buf_back[BUFSIZE];

	unsigned char iv_aes[AES_BLOCK_SIZE];

	memset(buf_in, 0, sizeof(buf_in));
	memset(buf_out, 0, sizeof(buf_out));
	memset(buf_back, 0, sizeof(buf_back));

	int keylength = 128;
	unsigned char aes_key[128];

	memset(aes_key, 0, sizeof(aes_key));

	if (!RAND_bytes(aes_key, keylength))
		exit(-1);
	aes_key[keylength - 1] = '\0';

	AES_KEY enc_key, dec_key;

	AES_set_encrypt_key(aes_key, 128, &enc_key);
	AES_set_decrypt_key(aes_key, 128, &dec_key);

	memset(iv_aes, 0, sizeof(iv_aes));

	/*파일 크기*/
	fseek(fp_in, SEEK_SET, SEEK_END);
	int size = ftell(fp_in);
	fseek(fp_in, SEEK_SET, SEEK_SET);

	/*파일 Header*/
	n = fread(buf_in, sizeof(char), 54, fp_in);
	fwrite(buf_in, sizeof(char), n, fp_out);
	fwrite(buf_in, sizeof(char), n, fp_back);


	while ((n = fread(buf_in, sizeof(char), BUFSIZE, fp_in)) > 0) {
		//int encrypt_size = (sizeof(buf_in) + AES_BLOCK_SIZE) / 16 * 16;
		memset(iv_aes, 0, sizeof(iv_aes));
		AES_cbc_encrypt(buf_in, buf_out, sizeof(buf_in), &enc_key, iv_aes, AES_ENCRYPT);
		fwrite(buf_out, sizeof(char), n, fp_out);
		memset(iv_aes, 0, sizeof(iv_aes));
		AES_cbc_encrypt(buf_out, buf_back, sizeof(buf_in), &dec_key, iv_aes, AES_DECRYPT);
		fwrite(buf_back, sizeof(char), n, fp_back);
	}
}

int main() {
	FILE* gif_in, *gif_out, *gif_back;

	if ((gif_in = fopen("test3.bmp", "rb")) == NULL) {
		perror("open error");
		exit(1);
	}

	if ((gif_out = fopen("test_out.bmp", "wb")) == NULL) {
		perror("open error");
		exit(1);
	}

	if ((gif_back = fopen("test_back.bmp", "wb")) == NULL) {
		perror("open error");
		exit(1);
	}

	int select_mode;

	printf("---암호화 모드 선택---\n");
	printf("0. 종료\n1. DES(ecb)\n2. 3-DES(ecb)\n3. AES(ecb)\n4. DES(cbc)\n5. 3-DES(cbc)\n6. AES(cbc)\n");
	printf("입력 : ");
	int tmp = scanf("%d", &select_mode);

	switch (select_mode) {
	case 0 :
		break;
	case 1 :
		des_gif(gif_in, gif_out, gif_back);
		break;
	case 2 :
		des3_gif(gif_in, gif_out, gif_back);
		break;
	case 3 :
		aes_gif(gif_in, gif_out, gif_back);
		break;
	case 4 :
		des_CBC_gif(gif_in, gif_out, gif_back);
		break;
	case 5 :
		des3_CBC_gif(gif_in, gif_out, gif_back);
		break;
	case 6 :
		aes_CBC_gif(gif_in, gif_out, gif_back);
		break;
	}


	fclose(gif_in);
	fclose(gif_out);
	fclose(gif_back);

	return 0;
}