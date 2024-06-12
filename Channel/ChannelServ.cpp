/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelServ.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:39:32 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/12 22:21:49 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelServ.hpp"
#include "../User/User.hpp"

ChannelServ::ChannelServ() {}

ChannelServ::~ChannelServ() {}

void	ChannelServ::joinChannel(const std::string& channelName, User & user) {
	_channels[channelName].addUser(user);
}

void	ChannelServ::leaveChannel(const std::string& channelName, User & user) {
	_channels[channelName].removeUser(user);
}

Channel*	ChannelServ::getChannel(const std::string& channelName) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	if (it != _channels.end())
		return &(it->second);
	return NULL;
}

bool	ChannelServ::DoesChannelExist(const std::string & channelName) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	if (it == _channels.end())
		return (false);
	return (true);
}

bool	ChannelServ::isUserOnChannel(std::string const & channelName, User & user) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	std::vector<User*> users = it->second.getUsers();
	std::vector<User*>::iterator userIt;
	for (userIt = users.begin(); userIt != users.end(); ++userIt) {
        if (*userIt == &user) {
            return true;
        }
    }
	return (false);
}

bool		ChannelServ::isChannelFull(std::string const & channelName) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	
	Channel channel = it->second;
	std::vector<User*> users = channel.getUsers();
	if (channel.getMaxUsersPerChannel() != 0 && users.size() == channel.getMaxUsersPerChannel())
		return (true);
	return (false);	
}
