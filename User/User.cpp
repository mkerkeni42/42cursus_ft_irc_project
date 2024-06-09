/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:39 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/09 23:32:16 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "UserServ.hpp"

User::User(void) {}

User::User(int fd) : _fd(fd) {}

User::~User(void) {}

std::string	User::getPassword(void) const { return (this->_password); }

int	User::getFD(void) const { return (this->_fd); }

std::string	User::getUsername(void) const { return (this->_username); }

std::string	User::getNickname(void) const { return (this->_nickname); }

std::string	User::getRole(void) const { return (this->_role); }

void	User::setPassword(std::string const & password) { this->_password = password; }

void	User::setUsername(std::string const & username) { this->_username = username; }

void	User::setNickname(std::string const & nickname, UserServ & userServ) {
    if (getNickname().empty()) {
       this->_nickname = nickname;
       return;
    }
    std::string    oldNickname = this->_nickname;
    this->_nickname = nickname;
    userServ.updateUserNicknameMap(oldNickname, nickname, this);
    
}

void	User::setRole(std::string const & role) { this->_role = role; }

int User::receiveData() {
    char buf[512];
    int bytesRead = recv(_fd, buf, sizeof(buf), 0);
    if (bytesRead <= 0) {
        return -1;
    }
    _buffer.append(buf, bytesRead);
    return 0;
}

bool User::hasBufferedCommand() const {
    return _buffer.find("\r\n") != std::string::npos;
}

std::string User::getBufferedCommand() {
    size_t pos = _buffer.find("\r\n");
    std::string command = _buffer.substr(0, pos);
    _buffer.erase(0, pos + 2);
    return command;
}
