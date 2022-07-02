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
	char ip[] = "127.0.0.1"; // ip ����
	int port = 10000; // ��Ʈ��ȣ ����

	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAddr;

	// ���̺귯�� ��� Ȯ��
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		perror("WSAStartup");
		exit(1);
	}

	//���� ����
	if ((hSock = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		perror("Invalid socket");
		exit(1);
	}

	// ��� ���� ���Ͽ� ���� �ּ� ����ü ����
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(ip);
	servAddr.sin_port = htons(port);

	printf("Server Connect �õ�...\n");

	// ��� ���� ���Ͽ� �ּ� �Ҵ�
	if (connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) != 0) {
		perror("Socket error");
		exit(1);
	}

	printf("Server Connect Success!\n");

	//�޼��� ����
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
		printf("���� ����!\n");
		printf("������ �� : ");
		tmp = scanf("%d", &value);
		char s_value[20];
		itoa(value, s_value, 10);
		send(hSock, s_value, strlen(s_value) + 1, 0);

		recv(hSock, s_value, sizeof(s_value), 0);

		printf("Recieve return : %s\n", s_value);
	}
	else {
		printf("���� ����.\n");
	}

	printf("Client ����\n");

	// ���� �ݱ�
	closesocket(hSock);

	//���̺귯�� ��� �� ��ȯ
	WSACleanup();

	return 0;
}