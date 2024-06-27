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
	void		deleteChannel(const std::string & channelName);
	void		removeUserFromAllChannels(User & user);
	void		broadcastMessageToChannels(const std::string& message, User& sender);
	
	Channel*	getChannel(const std::string & channelName);
	
	bool		DoesChannelExist(const std::string & channelName);
	bool		isUserOnChannel(std::string const & channelName, User & user);
	bool		isChannelFull(std::string const & channelName);
	bool		isUserInvited(std::string const & channelName, User & user);
	
private:

	std::map<std::string, Channel>	_channels;
};
