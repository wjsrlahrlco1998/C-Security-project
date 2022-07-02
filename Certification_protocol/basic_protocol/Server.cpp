#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 4096

struct userinfo {
	char id[20];
	char pw[20];
};

int main(int argc, char* argv[]) {
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

	printf("Waiting Client...\n");
	listen(hServSock, 10);

	int size_clntAddr = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &size_clntAddr);
	printf("Connect Client!\n");
	printf("사용자인증 확인중...\n");

	/* 파일 처리 */
	FILE* fp;

	char id[20];
	char pw[20];
	char buf[BUFSIZE];
	char check[BUFSIZE];
	struct userinfo info[20];
	int i = 0;
	int certification_flag;

	if ((fp = fopen("pw.txt", "r+")) == NULL) {
		perror("fopen : pw.txt");
		exit(1);
	}

	//파일을 읽어서 userinfo의 형태에 맞게 저장
	memset(info, '\0', sizeof(info));

	while (1) {
		int t = fscanf(fp, "%s %s\n", info[i].id, info[i].pw);
		if (t == EOF) break;
		i++;
	}

	// Client로부터 ID와 PW 받기
	recv(hClntSock, id, sizeof(id), 0);
	recv(hClntSock, pw, sizeof(pw), 0);

	// Client로부터 받은 id, pw와 userinfo의 id에 매칭되는 pw와 비교
	for (int q = 0; q <= i; q++) {
		if (strcmp(id, info[q].id) == 0 && strcmp(pw, info[q].pw) == 0) {
			printf("인증에 성공했습니다!\n");
			certification_flag = 1;
			break;
		}
		else {
			certification_flag = 0;
		}
	}

	if (certification_flag == 1) {
		strcpy(check, "Success");
		send(hClntSock, check, strlen(check) + 1, 0);
		recv(hClntSock, buf, sizeof(buf), 0);
		int value = atoi(buf);
		printf("Client로부터 받은 값 : %d\n", value);
		printf("Client에 전송할 값 : %d\n", value * value);
		itoa(value * value, buf, 10);
		send(hClntSock, buf, strlen(buf) + 1, 0);
		printf("전송완료.\n");
		printf("서버를 종료합니다.\n");
	}
	else {
		strcpy(check, "Fail");
		send(hClntSock, check, strlen(check) + 1, 0);
		printf("인증에 실패했습니다.\n 서버를 종료합니다.\n");
	}

	fclose(fp);

	closesocket(hClntSock);
	closesocket(hServSock);

	WSACleanup();

	return 0;
}