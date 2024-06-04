/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:39 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/03 11:30:00 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(void) {}

User::User(User const &copy) {
	if (this != &copy)
		*this = copy;
}

User::~User(void) {}

User	&User::operator=(User const & obj) {
	return (*this);
}

std::string	User::getUsername(void) const { return (this->_username); }

std::string	User::getNickname(void) const { return (this->_nickname); }

std::string	User::getStatus(void) const { return (this->_status); }

std::string	User::getRole(void) const { return (this->_role); }

void	User::setUsername(std::string username) { this->_username = username; }

void	User::setNickname(std::string nickname) { this->_nickname = nickname; }

void	User::setStatus(std::string status) { this->_status = status; }

void	User::setRole(std::string role) { this->_role = role; }
