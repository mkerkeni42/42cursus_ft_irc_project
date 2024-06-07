/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 10:16:49 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/05/31 10:16:49 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "User/User.hpp"
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
	std::string	correct_password = "randompassword"; // Only for testing purposes

	for (size_t i = 0; i < entered_password.length(); ++i) {
		if (!std::isalnum(entered_password[i])) {
			std::cout << YELLOW << "Password contains invalid characters." << END << std::endl;
			sleep(1); // Delay for incorrect attempt
			return (false);
		}
	}

	if (entered_password == correct_password) {
		return (true);
	} else {
		std::cout << RED << "Incorrect password." << END << std::endl;
		sleep(1); // Delay for incorrect attempt
		return (false);
	}
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
	try {
		UserServ		newUserServ("1234");
		MessageServ	message(newUserServ);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
