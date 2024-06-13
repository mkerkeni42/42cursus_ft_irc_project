/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:31 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/13 15:03:25 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserServ.hpp"
#include "User.hpp"

UserServ::UserServ(std::string const & password) : _password(password), _messageServ(*this, _channelServ), _channelServ() {}

UserServ::~UserServ(void) {}

void	UserServ::addUser(int fd) {
	User	newUser(fd);
	this->_users[fd] = newUser;
}

int		UserServ::handleUserActivity(int fd) {
	User	&user = _users[fd];
	//if (user.receiveData() == -1)
	//	return (-1);
	//test here without socket
	user.setBuffer("TOPIC #chan hello world\r\n"); 
	while (user.hasBufferedCommand()) {
		std::string	command = user.getBufferedCommand();
		_messageServ.handleCommand(command, user);
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

void	UserServ::updateUserNicknameMap(std::string const & oldNickname, std::string const & newNickname, User* user) {
	_nicknameMap.erase(oldNickname);
	_nicknameMap[newNickname] = user;
}

void	UserServ::addUserByNickname(std::string const & nickname, User* user) {
	_nicknameMap[nickname] = user;
}

bool UserServ::isUserRegistered(const std::string & username) {
	std::map<int, User>::const_iterator it;
	for ( it = _users.begin(); it != _users.end(); ++it) {
    	if (it->second.getUsername() == username)
        	return true;
    }
    return false;
}

bool 	UserServ::isNicknameInUse(const std::string & nickname) {
	std::map<std::string, User*>::iterator it;
	for ( it = _nicknameMap.begin(); it != _nicknameMap.end(); ++it) {
    	if (it->first == nickname)
        	return true;
    }
    return false;
}
