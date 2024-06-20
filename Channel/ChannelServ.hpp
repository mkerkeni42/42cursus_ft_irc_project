/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelServ.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykifadji <ykifadji@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:08:58 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/20 07:57:45 by ykifadji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"
#include "Channel.hpp"

class ChannelServ {

public:
	ChannelServ();
	~ChannelServ();

	void	joinChannel(const std::string& channelName, User* user);
	void	leaveChannel(const std::string& channelName, User* user);
	Channel*	getChannel(const std::string& channelName);

private:
	std::map<std::string, Channel> _channels;
};
