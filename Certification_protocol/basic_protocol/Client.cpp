#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUFSIZE 4096

int main(int argc, char* argv[]) {
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

	printf("Server Connect 시도...\n");

	// 대상 서버 소켓에 주소 할당
	if (connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) != 0) {
		perror("Socket error");
		exit(1);
	}

	printf("Server Connect Success!\n");

	//메세지 전송
	char id[20];
	char pw[20];
	char check[BUFSIZE];

	int value, tmp;

	memset(id, '\0', sizeof(id));
	memset(pw, '\0', sizeof(pw));

	printf("ID : ");
	tmp = scanf("%s", id);
	printf("Password : ");
	tmp = scanf("%s", pw);

	send(hSock, id, sizeof(id), 0);
	send(hSock, pw, sizeof(pw), 0);
	recv(hSock, check, sizeof(check), 0);


	if (strcmp(check, "Success") == 0) {
		printf("인증 성공!\n");
		printf("전송할 값 : ");
		tmp = scanf("%d", &value);
		char s_value[20];
		itoa(value, s_value, 10);
		send(hSock, s_value, strlen(s_value) + 1, 0);

		recv(hSock, s_value, sizeof(s_value), 0);

		printf("Recieve return : %s\n", s_value);
	}
	else {
		printf("인증 실패.\n");
	}

	printf("Client 종료\n");

	// 소켓 닫기
	closesocket(hSock);

	//라이브러리 사용 후 반환
	WSACleanup();

	return 0;
}