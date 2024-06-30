#include "config.hpp"
#include "Network/NetworkServ.hpp"
#include "User/User.hpp"
#include "User/UserServ.hpp"
#include "Messages/MessageServ.hpp"
#include <csignal>

bool	check_port_availability(int port) {
	int	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cerr << RED << "ERROR: socket creation failedq!" << std::endl;
		return (false);
	}
	struct sockaddr_in	addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	int result = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	close(sockfd);

	if (result < 0) {
		std::cerr << RED << "ERROR: port " << port << " is unavailable!" << std::endl;
		return (false);
	}
	return (true);
}

bool	check_port(char *arg) {
	std::string	value = static_cast<std::string>(arg);
	if (value.length() > 5) {
		std::cerr << RED << "ERROR: wrong port number!" << std::endl;
		return (false);
	}
	for (size_t i = 0; i < value.length(); i++) {
		if (!isdigit(value[i])) {
			std::cerr << RED << "ERROR: wrong port number!" << std::endl;
			return (false);
		}
	}
	int	nb = std::atoi(value.c_str());
	if (nb < 1024 || nb > 65535) {
		std::cerr << RED << "ERROR: wrong port number!" << std::endl;
		return (false);
	}
	if (!check_port_availability(nb)) {
		return (false);
	}
	return (true);
}


bool	check_password(char* arg) {
	std::string	entered_password = arg;

	if (entered_password.length() < 4 || entered_password.length() > 24)
		std::cerr << RED << "ERROR: Password must contain between 4 and 24 characters." << END << std::endl;
	for (size_t i = 0; i < entered_password.length(); ++i) {
		if (!std::isalnum(entered_password[i])) {
			std::cout << RED << "ERROR: Password contains invalid characters." << END << std::endl;
			return (false);
		}
	}
	return (true);
}


NetworkServ* server = NULL;

void handleSignal(int signal) {
	if (server) {
		server->shutdown();
	}
	exit(signal);
}

int	main(int ac, char **av) {
	if (ac != 3) {
		std::cerr << RED << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}
	if (check_port(av[1]) == false)
		return (1);
	if (check_password(av[2]) == false)
		return (1);
	std::string	port = av[1];
	int			portNb = std::atoi(port.c_str());
	std::string	password = av[2];

	server = new NetworkServ(portNb, password);

	// Setup signal handlers
	std::signal(SIGINT, handleSignal);
	std::signal(SIGTERM, handleSignal);

	server->run();

	delete (server);
	return (0);
}
