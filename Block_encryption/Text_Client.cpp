#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#define BUFSIZE 4096

unsigned char out[BUFSIZE];
unsigned char aes_key[128];
int end_index = 0;

void des_decrypto(FILE* wfp, FILE* tmp) {

	int n;

	unsigned char buf_out[BUFSIZ], buf_back[BUFSIZ];

	memset(out, 0, sizeof(out));
	memset(buf_out, 0, sizeof(buf_out));
	memset(buf_back, 0, sizeof(buf_back));

	DES_cblock key = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
	DES_key_schedule keysched;

	DES_set_key((DES_cblock*)key, &keysched);

	fseek(tmp, 0, SEEK_SET);

	while ((n = fread(buf_out, sizeof(char), 8, tmp)) > 0) {
		buf_out[n] = '\0';
		DES_ecb_encrypt((DES_cblock*)buf_out, (DES_cblock*)buf_back, &keysched, DES_DECRYPT);
		buf_back[n] = '\0';
		strcat((char*)out, (char*)buf_back);
	}
	fwrite(out, sizeof(char), end_index, wfp);
}

void des3_decrypto(FILE* wfp, FILE* tmp) {
	int n;

	unsigned char buf_out[BUFSIZ], buf_back[BUFSIZ];

	memset(out, 0, sizeof(out));
	memset(buf_out, 0, sizeof(buf_out));
	memset(buf_back, 0, sizeof(buf_back));
	
	DES_cblock key1 = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
	DES_cblock key2 = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
	DES_cblock key3 = { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33 };

	DES_key_schedule keysched1, keysched2, keysched3;

	DES_set_key((DES_cblock*)key1, &keysched1);
	DES_set_key((DES_cblock*)key2, &keysched2);
	DES_set_key((DES_cblock*)key3, &keysched3);

	fseek(tmp, 0, SEEK_SET);
	while ((n = fread(buf_out, sizeof(char), 8, tmp)) > 0) {
		buf_out[n] = '\0';
		DES_ecb3_encrypt((DES_cblock*)buf_out, (DES_cblock*)buf_back, &keysched1, &keysched2, &keysched3, DES_DECRYPT);
		buf_back[n] = '\0';
		strcat((char*)out, (char*)buf_back);
	}
	fwrite(out, sizeof(char), end_index, wfp);
}

void aes_decrypto(FILE* wfp, FILE* tmp) {
	int n;
	int keylength = 128;

	unsigned char buf_out[BUFSIZE];
	unsigned char buf_back[BUFSIZE];

	AES_KEY dec_key;

	AES_set_decrypt_key(aes_key, 128, &dec_key);
	
	fseek(tmp, 0, SEEK_SET);
	while ((n = fread(buf_out, sizeof(char), 16, tmp)) > 0) {
		buf_out[n] = '\0';
		AES_ecb_encrypt(buf_out, buf_back, &dec_key, AES_DECRYPT);
		buf_back[n] = '\0';
		strcat((char*)out, (char*)buf_back);
	}
	fwrite(out, sizeof(char), end_index, wfp);

}

int main(int argc, char* argv[]) {

	FILE* wfp;
	FILE* tmp;

	if ((wfp = fopen("decrypt.txt", "w")) == NULL) {
		perror("fopen : decryptText.txt");
		exit(1);
	}

	if ((tmp = fopen("encrypt.txt", "w+")) == NULL) {
		perror("write Error");
		exit(1);
	}

	/* Client */
	char ip[] = "127.0.0.1"; // ip 지정
	int port = 10000; // 포트번호 지정

	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAddr;

	// 라이브러리 사용 확인
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		perror("WSAStartup");
		exit(1);
	}

	//소켓 생성
	if ((hSock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		perror("Invalid socket");
		exit(1);
	}

	// 대상 서버 소켓에 넣을 주소 구조체 정의
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(port);

	// 대상 서버 소켓에 주소 할당
	if (connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) != 0) {
		perror("Socket error");
		exit(1);
	}

	// 할당 된 클라리언트에 메세지 보냄.
	size_t filesize = 0, bufsize = 256;
	int nbyte = 256;
	char buf[256];
	char end[10];
	char select_num[1];
	int select_mode;

	recv(hSock, select_num, sizeof(select_num), 0);
	select_mode = atoi(select_num);

	switch (select_mode) {
	case 0 :
		break;
	case 1 :
		printf("END_INDEX 수신\n");
		recv(hSock, end, sizeof(end), 0);
		end_index = atoi(end);
		printf("DES 암호문 file 수신\n");
		while (nbyte != 0) {
			nbyte = recv(hSock, buf, bufsize, 0);
			fwrite(buf, sizeof(char), nbyte, tmp);
		}
		printf("DES 복호화 진행...\n");
		des_decrypto(wfp, tmp);
		printf("DES 복호화 완료.");
		break;
	case 2 :
		printf("END_INDEX 수신\n");
		recv(hSock, end, sizeof(end), 0);
		end_index = atoi(end);
		printf("3-DES 암호문 file 수신\n");
		while (nbyte != 0) {
			nbyte = recv(hSock, buf, bufsize, 0);
			fwrite(buf, sizeof(char), nbyte, tmp);
		}
		printf("3-DES 복호화 진행...\n");
		des3_decrypto(wfp, tmp);
		printf("3-DES 복호화 완료.");
		break;
	case 3 :
		printf("AES-KEY 수신\n");
		recv(hSock, (char*)aes_key, sizeof(aes_key), 0);
		recv(hSock, end, sizeof(end), 0);
		end_index = atoi(end);
		printf("AES 암호문 file 수신\n");
		while (nbyte != 0) {
			nbyte = recv(hSock, buf, bufsize, 0);
			fwrite(buf, sizeof(char), nbyte, tmp);
		}
		printf("AES 복호화 진행...\n");
		aes_decrypto(wfp, tmp);
		printf("AES 복호화 완료.");
		break;
	}


	// 소켓 닫기
	closesocket(hSock);

	//라이브러리 사용 후 반환
	WSACleanup();


	fclose(wfp);
	fclose(tmp);

	return 0;

}
