/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:31 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/08 00:06:05 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserServ.hpp"

UserServ::UserServ(std::string const & password) : _password(password) {}

UserServ::~UserServ(void) {}

void	UserServ::addUser(int fd) {
	User	newUser(fd);
	this->_users[fd] = newUser;
}

int		UserServ::handleUserActivity(int fd) {
	User	&user = _users[fd];
	if (user.receiveData() == -1)
		return (-1);
	while (user.hasBufferedCommand()) {
		std::string	command = user.getBufferedCommand();
		//messageServ.handleCommand(command, user);
	}
	return (0);
}

void	UserServ::removeUser(int fd) {
	User	&user = this->_users[fd];
	_nicknameMap.erase(user.getNickname());
	this->_users.erase(fd);
}

User	*UserServ::getUserByNickname(std::string const & nickname) {
	std::map<std::string, User*>::iterator it = _nicknameMap.find(nickname);
    if (it != _nicknameMap.end()) {
        return it->second;
    }
    return NULL;
}

void	User::setNickname(std::string const & nickname) {
	_nicknameMap.erase(nickname);
    nickname = nick;
    _nicknameMap[nick] = this;
}
