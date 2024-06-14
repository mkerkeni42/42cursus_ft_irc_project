/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:12:23 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/14 11:39:26 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "../User/User.hpp"

Channel::Channel() : _mode(PUBLIC), _passwordMode(DISABLED), _topicMode(PUBLIC), _maxUsersPerChannel(0) {}

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

void	Channel::setPasswordMode(const int & newMode) { _passwordMode = newMode; }

void	Channel::setTopicMode(const int & newMode) { _topicMode = newMode; }

void	Channel::setPassword(std::string & newPassword) { _password = newPassword; }

void	Channel::setMaxUsersPerChannel(size_t nb) { _maxUsersPerChannel = nb; }

void	Channel::setName(const std::string& name) { _name = name; }

void	Channel::addOperator(const std::string& username) { _operators.push_back(username); }

void	Channel::removeOperator(const std::string& username) {
	std::vector<std::string>::iterator it = std::find(_operators.begin(), _operators.end(), username);
	if (it != _operators.end())
		_operators.erase(it);
}

void	Channel::setBannedUsers(const std::string& username) { _bannedUsers.push_back(username); }

void	Channel::setInvitedUsers(const std::string& username) { _invitedUsers.push_back(username); }

const std::vector<User*>&	Channel::getUsers() const { return _users; }

size_t	Channel::getMaxUsersPerChannel(void) const { return (_maxUsersPerChannel); }

int	Channel::getMode(void) const { return (_mode); }

int	Channel::getPasswordMode(void) const { return (_passwordMode); }

int	Channel::getTopicMode(void) const { return (_topicMode); }

std::string	Channel::getPassword(void) const { return (_password); }

std::string	Channel::getTopic(void) const { return (_topic); }

std::string	Channel::getName(void) const { return (_name); }

bool	Channel::isOperator(std::string const & username) const {
	std::vector<std::string>::const_iterator it = std::find(_operators.begin(), _operators.end(), username);
	if (it != _operators.end())
		return (true);
	return (false);
}

std::vector<std::string>	Channel::getBannedUsers(void) const { return (_bannedUsers); }

std::vector<std::string>	Channel::getInvitedUsers(void) const { return (_invitedUsers); }	
