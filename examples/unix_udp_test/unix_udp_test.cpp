
#include <iostream>

//configuration
#define ASCS_DELAY_CLOSE 1 //this demo not used object pool and doesn't need life cycle management,
						   //so, define this to avoid hooks for async call (and slightly improve efficiency),
						   //any value which is bigger than zero is okay.
//configuration

#include <ascs/ext/udp.h>
using namespace ascs::ext::udp;

#define QUIT_COMMAND		"quit"
#define RESTART_COMMAND		"restart"
#define UNIX_SOCKET_NAME_1	"unix-socket-1"
#define UNIX_SOCKET_NAME_2	"unix-socket-2"

int main(int argc, const char* argv[])
{
	puts("type " QUIT_COMMAND " to end.");

	ascs::service_pump sp;

	unix_single_socket_service uu1(sp);
	unix_single_socket_service uu2(sp);

	unlink(UNIX_SOCKET_NAME_1);
	unlink(UNIX_SOCKET_NAME_2);
	uu1.set_local_addr(UNIX_SOCKET_NAME_1);
	uu1.set_peer_addr(UNIX_SOCKET_NAME_2);
	uu2.set_local_addr(UNIX_SOCKET_NAME_2);
	uu2.set_peer_addr(UNIX_SOCKET_NAME_1);

	sp.start_service();
	while(sp.is_running())
	{
		std::string str;
		std::cin >> str;
		if (QUIT_COMMAND == str)
			sp.stop_service();
		else if (RESTART_COMMAND == str)
		{
			sp.stop_service();
			sp.start_service();
		}
		else
		{
			uu1.send_native_msg("uu1 -> uu2: " + str);
			uu2.send_native_msg("uu2 -> uu1: " + str);
		}
	}
	unlink(UNIX_SOCKET_NAME_1);
	unlink(UNIX_SOCKET_NAME_2);

	return 0;
}
