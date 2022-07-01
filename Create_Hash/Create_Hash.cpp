#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#define BUFSIZE 1024

void mode_MD5(FILE* rfp) {
	unsigned char buf_in[BUFSIZE];
	unsigned char digest[MD5_DIGEST_LENGTH];
	char md5Hash[50] = { 0, };
	int n;
	fseek(rfp, 0, SEEK_SET);
	n = fread(buf_in, sizeof(char), BUFSIZE, rfp);
	buf_in[n] = '\0';

	MD5_CTX context;
	MD5_Init(&context);
	MD5_Update(&context, buf_in, strlen((char*)buf_in));
	MD5_Final(digest, &context);

	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
		sprintf(md5Hash + (i * 2), "%02x", digest[i]);

	printf("<입력 텍스트>\n%s\n", buf_in);
	printf("<생성된 Hash>\n%s\n", md5Hash);
}

void mode_SHA(FILE* rfp) {
	unsigned char buf_in[BUFSIZE];
	unsigned char digest[SHA256_DIGEST_LENGTH];
	char shaHash[SHA256_DIGEST_LENGTH * 2 + 1];
	int n;
	fseek(rfp, 0, SEEK_SET);
	n = fread(buf_in, sizeof(char), BUFSIZE, rfp);
	buf_in[n] = '\0';

	SHA256_CTX context;
	SHA256_Init(&context);
	SHA256_Update(&context, buf_in, strlen((char*)buf_in));
	SHA256_Final(digest, &context);

	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		sprintf(&shaHash[i * 2], "%02x", (unsigned int)digest[i]);

	printf("<입력 텍스트>\n%s\n", buf_in);
	printf("<생성된 Hash>\n%s\n", shaHash);

}

int main() {
	FILE* rfp;

	int end = 1;

	if ((rfp = fopen("test.txt", "r")) == NULL) {
		perror("fopen : test.txt");
		exit(1);
	}

	int select_mode;
	while (end != 0) {
		printf("----HASH모드선택---\n");
		printf("0. 종료\n1. MD5\n2. SHA\n");
		printf("입력 : ");
		int tmp = scanf("%d", &select_mode);
		printf("-------------------\n");

		switch (select_mode) {
		case 0:
			end = 0;
			break;
		case 1:
			mode_MD5(rfp);
			break;
		case 2:
			mode_SHA(rfp);
			break;
		}
	}

	fclose(rfp);

	return 0;
}