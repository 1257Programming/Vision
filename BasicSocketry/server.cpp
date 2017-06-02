#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Xinput.h>

#pragma comment(lib, "Ws2_32.lib") // Linking to socket lib
#pragma comment(lib, "XInput.lib") // For Xbox input used in this test

using namespace std;

int main()
{
	// Setting up Xbox controller's XINPUT
	// Poll the controller
	XINPUT_STATE controller_status;
	XInputGetState(0, &controller_status);

	// Windows-exclusive Winsock startup
	// WSA = Windows Socket API
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	
	// Create socket used to listen on port
	// socket (addressFamily, typeOfSocket, protocol)
	// If protocol == 0, we don't care about the protocol
	// AF_INET stands for the IPv4 address family
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	// Address structures (sockaddr_in can be casted to the sockaddr struct requested by most functions)
	sockaddr_in serv_addr = {};
	sockaddr_in cli_addr;

	// sin = socket in
	serv_addr.sin_family = AF_INET; // IPv4
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(5800);

	cout<<bind(ListenSocket, (sockaddr*)&serv_addr, sizeof(serv_addr)); // Bind to chosen port
	listen(ListenSocket, 5);

	// Lenght of client address must be specified by address (not value), hence this new variable
	int clilen = sizeof(cli_addr);
	SOCKET ConnectedSocket = accept(ListenSocket, (sockaddr*)&cli_addr, &clilen); // Make new socket for connection with client
	cout << "accepted" << endl;
	char buffer[255] = { 0 };
	while (!controller_status.Gamepad.wButtons)
	{
		XInputGetState(0, &controller_status);

		ZeroMemory(buffer, strlen(buffer)); // Equivalent to bzero on Linux
		CopyMemory(buffer, &controller_status.Gamepad.sThumbRY, 2); // Equivalent to bcopy on Linux
		
		send(ConnectedSocket, buffer, strlen(buffer), 0);

		cout << controller_status.Gamepad.sThumbRY << endl;
	}
	// Cleanup (note: unlike Linux, a call to setsockopt is not needed to allow 
	// similar sockets to be used by later program runs ("reuse" option) 
	closesocket(ConnectedSocket);
	closesocket(ListenSocket);
	system("pause");
	
	return 0;
}