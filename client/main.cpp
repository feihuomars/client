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

//�ͻ���
int main() {
	WSADATA wsa_data; //WSADATA����,����windows socketִ�е���Ϣ
	int i_result = 0; //���շ���ֵ
	SOCKET sock_client = INVALID_SOCKET;
	addrinfo *result = nullptr, hints;
	//��ʼ��winsock��̬��(ws2_32.dll),MAKEWORD(2, 2)��������ʹ��winsock2.2�汾
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
	//�����׽���
	sock_client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock_client == INVALID_SOCKET) {
		cerr << "socket() function failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		system("pause");
		return 1;
	}
	//���ӷ�����
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
	do {
		cout << "enter the message that you want to send: " << flush;
		cin.getline(send_buf, SEND_BUF_SIZE);
		i_result = send(sock_client, send_buf, static_cast<int>(strlen(send_buf)), 0);
		if (i_result == SOCKET_ERROR) {
			cerr << "send() function failed with error: " << WSAGetLastError() << "\n";
			closesocket(sock_client);
			WSACleanup();
			system("pause");
			return 1;
		}
		cout << "Bytes sent: " << i_result << endl;
		//���շ��������ص�����
		recv_result = recv(sock_client, send_buf, SEND_BUF_SIZE, 0);
		if (recv_result > 0) {
			cout << "feedback from server: " << send_buf << endl;
		}
		else if (recv_result == 0) {
			cout << "connection closed..." << endl;
		}
		else {
			cerr << "recv() function failed with error: " << WSAGetLastError() << "\n";
			closesocket(sock_client);
			WSACleanup();
			system("pause");
			return 1;
		}
	} while (recv_result > 0);
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