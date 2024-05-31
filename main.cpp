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

bool	check_port(char *arg) {
	std::string	value = static_cast<std::string>(arg);
	if (value.length() > 5) {
		cerr << RED << "ERROR: wrong port number !" << endl;
		return (false);
	}
	for (size_t i = 0; i < value.length(); i++) {
		if (!isdigit(value[i])) {
			cerr << RED << "ERROR: wrong port number !" << endl;
			return (false);
		}
	}
	int	nb = std::atoi(value.c_str());
	if (nb > 65535) {
		cerr << RED << "ERROR: wrong port number !" << endl;
		return (false);
	}
	//maybe try to open port here 
	return (true);
}
	
bool	check_password(char* arg) {
	std::string entered_password = static_cast<std::string>(arg);
	std::string correct_password = "random password"; // Only for testing purposes

    // Check if entered characters are allowed (e.g., only alphanumeric characters)
    std::regex allowed_chars("^[a-zA-Z0-9]*$");
    if (!std::regex_match(entered_password, allowed_chars)) {
        std::cout << YELLOW << "Password contains invalid characters." << END << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for incorrect attempt
        return (false);
    }

    // Compare the entered password with the true one
    if (entered_password == correct_password) {
        return (true);
    } else {
        std::cout << RED << "Incorrect password." << END << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay for incorrect attempt
        return (false);
    }
}

int	main(int ac, char **av) {
	if (ac != 3) {
		cerr << RED << "ERROR: wrong number of arguments !" << endl;
		return (1);
	}
	if (check_port(av[1]) == false)
		return (1);
	if (check_password(av[2]) == false)
		return (1); 
	return (0);
}
