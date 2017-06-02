#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Winsock linking

using namespace std;

int main()
{
	// Windows-only Winsock startup
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	SOCKET cli_sock = socket(AF_INET, SOCK_STREAM, 0); // Create the one socket for the whole client program

	// Only serv_addr needed, not the client_addr
	sockaddr_in serv_addr;
	ZeroMemory(&serv_addr, sizeof(serv_addr)); // Use bzero on Linux

	serv_addr.sin_family = AF_INET; // IPv4
	serv_addr.sin_port = htons(5800);

	hostent* host = gethostbyname("localhost"); // Use "10.12.57.2" on the RoboRIO, localhost is for two programs on one machine
	//serv_addr.sin_addr.s_addr = host->h_addr;
	CopyMemory(&serv_addr.sin_addr.s_addr, host->h_addr, host->h_length); // Transfer server address from hostent object to sockaddr_in object

	connect(cli_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)); // Unlike accept(), this will NOT hang and wait; may just fail

	// Timeout so recv() does not block program
	int timeout_ms = 500;
	setsockopt(cli_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout_ms, sizeof(timeout_ms));

	char buffer[255] = { 0 };
	short axis = 0;
	while (axis != 32767)
	{
		ZeroMemory(buffer, 255);
		
		recv(cli_sock, buffer, 255, 0);

		CopyMemory((char*)&axis, buffer, 2); // use bcopy on Linux

		cout << axis << endl; // Note that this program does not have any XInput use, but gets the value from the server
	}
	closesocket(cli_sock); // Use close() on Linux

	system("pause");

	return 0;
}
