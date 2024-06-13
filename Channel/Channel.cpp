/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:12:23 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/13 14:55:40 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "../User/User.hpp"

Channel::Channel() : _maxUsersPerChannel(1) {}

Channel::~Channel() {}

void	Channel::addUser(User & user) {
	_users.push_back(&user);
	_userMap[user.getNickname()] = &user;
}

void	Channel::removeUser(User& user) {
	_users.erase(std::remove(_users.begin(), _users.end(), &user), _users.end());
	_userMap.erase(user.getNickname());
}

void	Channel::broadcastMessage(const std::string& message, User& sender) {
	for (std::vector<User*>::iterator	it = _users.begin(); it != _users.end(); ++it) {
		User*	user = *it;
		if (user != &sender)
			send(user->getFD(), message.c_str(), message.size(), 0);
	}
}

User*	Channel::getUserByNickname(const std::string& nickname) {
	std::map<std::string, User*>::iterator	it = _userMap.find(nickname);
	if (it != _userMap.end())
		return it->second;
	return NULL;
}

void	Channel::setTopic(const std::string& newTopic) { _topic = newTopic; }

void	Channel::setMode(const int & newMode) { _mode = newMode; }

void	Channel::setPassword(std::string & newPassword) { _password = newPassword; }

void	Channel::setMaxUsersPerChannel(size_t nb) { _maxUsersPerChannel = nb; }

void	Channel::setName(const std::string& name) { _name = name; }

void	Channel::setOperator(const std::string& username) { _operator = username; }

void	Channel::setBannedUsers(const std::string& username) { _bannedUsers.push_back(username); }

void	Channel::setInvitedUsers(const std::string& username) { _invitedUsers.push_back(username); }

const std::vector<User*>&	Channel::getUsers() const { return _users; }

size_t	Channel::getMaxUsersPerChannel(void) const { return (_maxUsersPerChannel); }

int	Channel::getMode(void) const { return (_mode); }

std::string	Channel::getPassword(void) const { return (_password); }

std::string	Channel::getTopic(void) const { return (_topic); }

std::string	Channel::getOperator(void) const { return (_operator); }

std::vector<std::string>	Channel::getBannedUsers(void) const { return (_bannedUsers); }

std::vector<std::string>	Channel::getInvitedUsers(void) const { return (_invitedUsers); }	
