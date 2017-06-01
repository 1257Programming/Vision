#include <WPILib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>

/**
 * This is a demo program showing the use of the RobotDrive class.
 * The SampleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're inexperienced,
 * don't. Unless you know what you are doing, complex code will be much more difficult under
 * this system. Use IterativeRobot or Command-Based instead if you're new.
 */
class Robot: public frc::IterativeRobot {
	frc::RobotDrive myRobot { 0, 1 };  // robot drive system
	frc::Joystick stick { 0 };  // only joystick
	int clientSocket = 0;
	sockaddr_in serverAddress;
	hostent* Server;
	int port = 5800;
	int errorChecking = 0;
	char data [100];


public:
	Robot() {
		myRobot.SetExpiration(0.1);
	}

	/**
	 * Runs the motors with arcade steering.
	 */

	void AutonomousInit() {
		clientSocket = socket (AF_INET, SOCK_STREAM, 0);
		bzero((char*) &serverAddress, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port = htons(port);
		Server = gethostbyname("10.12.57.5");
		bcopy((char*) Server->h_addr, (char*) &serverAddress.sin_addr.s_addr, Server->h_length);
		connect(clientSocket, (sockaddr*) &serverAddress, sizeof(serverAddress));


	}

};

START_ROBOT_CLASS(Robot)
