/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:31 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/04 22:43:17 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserServ.hpp"

UserServ::UserServ(std::string const & password) : _password(password) _messageServ(*this) {}

UserServ::~UserServ(void) {}

void	UserServ::addUser(int fd) {
	User	newUser(fd);
	this->_users[fd] = newUser;
}

int		UserServ::handleUserActivity(int fd) {
	User	&user = users[fd];
	if (user.receiveData() == -1)
		return (-1);
	while (user.hasBufferedCommand()) {
		std::string	command = user.getBufferedCommand();
		messageServ.handleCommand(command, user);
	}
	return (0);
}

void	UserServ::removeUser(int fd) {
	User	&user = this->_users[fd];
	nicknameMap.erase(user.getNickname());
	this->_users.erase(fd);
}

User	*UserServ::getUserByNickname(std::string const & nickname) {
	std::map<std::string, User*>::iterator it = nicknameMap.find(nickname);
    if (it != nicknameMap.end()) {
        return it->second;
    }
    return nullptr;
}

void	User::setNickname(std::string const & nickname) {
	nicknameMap.erase(nickname);
    nickname = nick;
    nicknameMap[nick] = this;
}
