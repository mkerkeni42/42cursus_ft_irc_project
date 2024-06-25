#include "config.hpp"
#include "Network/NetworkServ.hpp"
#include "User/User.hpp"
#include "User/UserServ.hpp"
#include "Messages/MessageServ.hpp"

bool	check_port(char *arg) {
	std::string	value = static_cast<std::string>(arg);
	if (value.length() > 5) {
		std::cerr << RED << "ERROR: wrong port number !" << std::endl;
		return (false);
	}
	for (size_t i = 0; i < value.length(); i++) {
		if (!isdigit(value[i])) {
			std::cerr << RED << "ERROR: wrong port number !" << std::endl;
			return (false);
		}
	}
	int	nb = std::atoi(value.c_str());
	if (nb > 65535) {
		std::cerr << RED << "ERROR: wrong port number !" << std::endl;
		return (false);
	}
	//maybe try to open port here
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

int	main(int ac, char **av) {
	if (ac != 3) {
		std::cerr << RED << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return (1);
	}
	if (check_port(av[1]) == false)
		return (1);
	if (check_password(av[2]) == false)
		return (1);
	std::string	port = av[1];
	int portNb = std::atoi(port.c_str());
    std::string password = av[2];
	NetworkServ server(portNb, password);
	server.run();
	return (0);
}
