/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelServ.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:08:58 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/13 14:22:19 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"
#include "Channel.hpp"

class	User;

class	ChannelServ {

public:
	ChannelServ();
	~ChannelServ();

	void		joinChannel(const std::string & channelName, User & user);
	void		leaveChannel(const std::string & channelName, User & user);
	void		createChannel(const std::string & channelName, User & user);
	Channel*	getChannel(const std::string & channelName);
	bool		DoesChannelExist(const std::string & channelName);
	bool		isUserOnChannel(std::string const & channelName, User & user);
	bool		isChannelFull(std::string const & channelName);
	bool		isUserBanned(std::string const & channelName, User & user);
	bool		isUserInvited(std::string const & channelName, User & user);
	
private:
	std::map<std::string, Channel>	_channels;
};
