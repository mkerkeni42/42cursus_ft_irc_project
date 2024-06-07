/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykifadji <ykifadji@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:12:23 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/04 14:47:32 by ykifadji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

void	Channel::addUser(User& user) {
	_users.push_back(&user);
	_userMap[user.getNickName()] = &user;
}

void	Channel::removeUser(User& user) {
	_users.erase(std::remove(_users.begin(), _users.end(), &user), _users.end());
	_userMap.erase(user.getNickName());
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

void	Channel::setTopic(const std::string& newTopic) {_topic = newTopic;}

void	Channel::setMode(const std::string& newMode) {_mode = newMode;}

const std::vector<User*>&	Channel::getUsers() const {return _users;}