#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using std::cin;
using std::cerr;
using std::cout;
using std::endl;
using std::flush;

const char DEFAULT_PORT[] = "4000";
const int SEND_BUF_SIZE = 256;
const int BUFFER_SIZE = 1024;

//客户端
int main() {
	WSADATA wsa_data; //WSADATA变量,包含windows socket执行的信息
	int i_result = 0; //接收返回值
	SOCKET sock_client = INVALID_SOCKET;
	addrinfo *result = nullptr, hints;
	//初始化winsock动态库(ws2_32.dll),MAKEWORD(2, 2)用于请求使用winsock2.2版本
	i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (i_result != 0) {
		cerr << "WSAStartup() function failed: " << i_result << "\n";
		system("pause");
		return 1;
	}
	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//
	i_result = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (i_result != 0) {
		cerr << "getaddrinfo() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	//创建套接字
	sock_client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock_client == INVALID_SOCKET) {
		cerr << "socket() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	//连接服务器
	i_result = connect(sock_client, result->ai_addr, result->ai_addrlen);
	if (i_result == SOCKET_ERROR) {
		cerr << "connect() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	cout << "connect server successfully..." << endl;
	//
	freeaddrinfo(result);
	//
	char send_buf[SEND_BUF_SIZE];
	int recv_result = 0;
	//SecureZeroMemory(send_buf, SEND_BUF_SIZE);

	//具体的收发操作
	char buffer[BUFFER_SIZE];
	ZeroMemory(buffer, BUFFER_SIZE);
	FILE *fp = fopen("D://test/picture1.jpg", "rb");
	if (fp == NULL) {
		cout << "File open failed" << endl;
	}
	else {
		cout << "File open succeed" << endl;
		int length = 0;

		recv(sock_client, buffer, BUFFER_SIZE, 0);
		cout << "data from server: " << buffer << endl;
		ZeroMemory(buffer, BUFFER_SIZE);
		Sleep(3000);	//收到后延时3s发送
		while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
			if (send(sock_client, buffer, BUFFER_SIZE, 0) < 0) {
				cout << "File read failed" << endl;
			}
			cout << "sending file....." << endl;
			ZeroMemory(buffer, BUFFER_SIZE);
		}
		cout << "FIle send succeed" << endl;
	}
	fclose(fp);


	//do {
	//	cout << "enter the message that you want to send: " << flush;
	//	cin.getline(send_buf, SEND_BUF_SIZE);
	//	i_result = send(sock_client, send_buf, static_cast<int>(strlen(send_buf)), 0);
	//	if (i_result == SOCKET_ERROR) {
	//		cerr << "send() function failed with error: " << WSAGetLastError() << "\n";
	//		closesocket(sock_client);
	//		WSACleanup();
	//		system("pause");
	//		return 1;
	//	}
	//	cout << "Bytes sent: " << i_result << endl;
	//	//接收服务器返回的数据
	//	recv_result = recv(sock_client, send_buf, SEND_BUF_SIZE, 0);
	//	if (recv_result > 0) {
	//		cout << "feedback from server: " << send_buf << endl;
	//	}
	//	else if (recv_result == 0) {
	//		cout << "connection closed..." << endl;
	//	}
	//	else {
	//		cerr << "recv() function failed with error: " << WSAGetLastError() << "\n";
	//		closesocket(sock_client);
	//		WSACleanup();
	//		system("pause");
	//		return 1;
	//	}
	//} while (recv_result > 0);

	/*cout << "before recv" << endl;*/
	
	//i_result = send(sock_client, "jjjjjjjjj", SEND_BUF_SIZE, 0);

	/*do {
		recv_result = recv(sock_client, buffer, BUFFER_SIZE, 0);
		cout << "recved data:" << buffer << endl;
	} while (recv_result > 0);*/

	
	


	//
	i_result = shutdown(sock_client, SD_SEND);
	if (i_result == SOCKET_ERROR) {
		cerr << "shutdown() function failed with error: " << WSAGetLastError() << "\n";
		closesocket(sock_client);
		WSACleanup();
		system("pause");
		return 1;
	}
	closesocket(sock_client);
	WSACleanup();
	cout << "socket closed..." << endl;
	system("pause");
	return 0;
}