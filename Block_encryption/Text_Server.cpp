#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#define BUFSIZE 4096

int end_index = 0;
unsigned char aes_key[128];

void des_encrypto(FILE* rfp, FILE* wfp) {

	unsigned char buf_in[BUFSIZE], buf_out[BUFSIZE];

	int n;

	DES_cblock key = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
	DES_key_schedule keysched;

	DES_set_key((DES_cblock*)key, &keysched);

	while ((n = fread(buf_in, sizeof(char), 8, rfp)) > 0) {
		int p;
		end_index += n;
		if (n < 8) {
			for (p = 8 - n; p <= 7; p++) {
				buf_in[p] = '0';
			}
			buf_in[p] = '\0';
		}
		else {
			p = 8;
			buf_in[n] = '\0';
		}

		DES_ecb_encrypt((DES_cblock*)buf_in, (DES_cblock*)buf_out, &keysched, DES_ENCRYPT);
		fwrite(buf_out, sizeof(char), p, wfp);
	}
}

void des3_encryto(FILE * rfp, FILE * wfp) {
	int n;

	unsigned char buf_in[BUFSIZE], buf_out[BUFSIZE];

	memset(buf_in, 0, sizeof(buf_in));
	memset(buf_out, 0, sizeof(buf_out));

	DES_cblock key1 = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
	DES_cblock key2 = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
	DES_cblock key3 = { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33 };

	DES_key_schedule keysched1, keysched2, keysched3;

	DES_set_key((DES_cblock*)key1, &keysched1);
	DES_set_key((DES_cblock*)key2, &keysched2);
	DES_set_key((DES_cblock*)key3, &keysched3);

	while ((n = fread(buf_in, sizeof(char), 8, rfp)) > 0) {
		int p;
		end_index += n;
		if (n < 8) {
			for (p = 8 - n; p <= 7; p++) {
				buf_in[p] = '0';
			}
			buf_in[p] = '\0';
		}
		else {
			p = 8;
			buf_in[n] = '\0';
		}
		DES_ecb3_encrypt((DES_cblock*)buf_in, (DES_cblock*)buf_out, &keysched1, &keysched2, &keysched3, DES_ENCRYPT);
		fwrite(buf_out, sizeof(char), p, wfp);
	}
}

void aes_encrypto(FILE* rfp, FILE* wfp) {
	int n;
	int keylength = 128;
	memset(aes_key, 0, sizeof(aes_key));
	if (!RAND_bytes(aes_key, keylength))
		exit(-1);
	aes_key[keylength - 1] = '\0';

	unsigned char buf_in[BUFSIZE];
	unsigned char buf_out[BUFSIZE];

	memset(buf_in, 0, sizeof(buf_in));
	memset(buf_out, 0, sizeof(buf_out));

	AES_KEY enc_key;

	AES_set_encrypt_key(aes_key, 128, &enc_key);

	while ((n = fread(buf_in, sizeof(char), 16, rfp)) > 0) {
		int p = 16;
		end_index += n;
		if (n < 16) {
			int i;
			for (i = 16 - n; i <= 15; i++)
			{
				buf_in[i] = '0';
			}
			buf_in[i] = '\0';
		}
		else {
			buf_in[n] = '\0';
		}
		AES_ecb_encrypt(buf_in, buf_out, &enc_key, AES_ENCRYPT);
		buf_out[p] = '\0';
		fwrite(buf_out, sizeof(char), p, wfp);
	}

}

int main(int argc, char* argv[])
{
	FILE* rfp, *wfp;
	int select_mode;

	if ((rfp = fopen("test.txt", "r")) == NULL) {
		perror("fopen : test.txt");
		exit(1);
	}

	if ((wfp = fopen("encrypt.txt", "wb+")) == NULL) {
		perror("fopen : encrypt.txt");
		exit(1);
	}


	/*Socket 통신 Server*/
	int port = 10000;

	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;


	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		perror("WSA Start error");
		exit(1);
	}

	if ((hServSock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		perror("INVALID SOCKET");
		exit(1);
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		perror("SOCKET ERROR");
		exit(1);
	}

	printf("클라이언트 응답대기중...\n");
	listen(hServSock, 10);

	int size_clntAddr = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &size_clntAddr);

	printf("---전송파일 암호화 모드 선택---\n");
	printf("0. 종료\n1. DES\n2. 3-DES\n3. AES\n");
	printf("입력 : ");
	int tmp = scanf("%d", &select_mode);

	switch (select_mode) {
	case 0:
		break;
	case 1:
		des_encrypto(rfp, wfp);
		break;
	case 2:
		des3_encryto(rfp, wfp);
		break;
	case 3:
		aes_encrypto(rfp, wfp);
		break;
	}

	char select_num[1];
	sprintf(select_num, "%d", select_mode);

	/*전송*/
	// 1. 파일크기계산

	fseek(wfp, 0, SEEK_END);
	size_t fsize = ftell(wfp);
	fseek(wfp, 0, SEEK_SET);

	size_t nsize = 0;

	// 2. 파일전송
	char buf[BUFSIZ];
	char end[10];
	sprintf(end, "%d", end_index);

	switch (select_mode) {
	case 0 :
		send(hClntSock, select_num, sizeof(select_num), 0);
		break;
	case 1 :
		send(hClntSock, select_num, sizeof(select_num), 0);
		printf("DES 암호문 전송 Start\n");
		printf("...END_INDEX 전송...\n");
		send(hClntSock, end, sizeof(end), 0);
		printf("END_INDEX 전송완료.\n");
		printf("...파일전송시작...\n");
		while (nsize != fsize) {
			int fpsize = fread(buf, 1, 256, wfp);
			printf(" %d(Bytes) 전송\n", fpsize);
			nsize += fpsize;
			send(hClntSock, buf, fpsize, 0);
		}
		printf("파일전송완료.");
		break;
	case 2 :
		send(hClntSock, select_num, sizeof(select_num), 0);
		printf("3-DES 암호문 전송 Start\n");
		printf("...END_INDEX 전송...\n");
		send(hClntSock, end, sizeof(end), 0);
		printf("END_INDEX 전송완료.\n");
		printf("...파일전송시작...\n");
		while (nsize != fsize) {
			int fpsize = fread(buf, 1, 256, wfp);
			printf(" %d(Bytes) 전송\n", fpsize);
			nsize += fpsize;
			send(hClntSock, buf, fpsize, 0);
		}
		printf("파일전송완료.");
		break;
	case 3 :
		send(hClntSock, select_num, sizeof(select_num), 0);
		printf("AES 암호문 전송 Start\n");
		printf("...aes_key 전송...\n");
		send(hClntSock, (char*)aes_key, sizeof(aes_key), 0);
		printf("key : %s\n", aes_key);
		printf("aes_key 전송 완료\n");
		printf("...END_INDEX 전송...\n");
		send(hClntSock, end, sizeof(end), 0);
		printf("END_INDEX 전송완료.\n");
		printf("...파일전송시작...\n");
		while (nsize != fsize) {
			int fpsize = fread(buf, 1, 256, wfp);
			printf(" %d(Bytes) 전송\n", fpsize);
			nsize += fpsize;
			send(hClntSock, buf, fpsize, 0);
		}
		printf("파일전송완료.");
		break;
	}

	closesocket(hClntSock);
	closesocket(hServSock);

	WSACleanup();

	fclose(rfp);
	fclose(wfp);

	return 0;

}
