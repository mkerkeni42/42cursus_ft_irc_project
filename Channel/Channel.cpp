/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:12:23 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/12 22:38:08 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "../User/User.hpp"

Channel::Channel() : _maxUsersPerChannel(0) {}

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

const std::vector<User*>&	Channel::getUsers() const { return _users; }

size_t	Channel::getMaxUsersPerChannel(void) const { return (_maxUsersPerChannel); }

int	Channel::getMode(void) const { return (_mode); }

std::string	Channel::getPassword(void) const { return (_password); }
