#include "ChannelServ.hpp"
#include "../User/User.hpp"

ChannelServ::ChannelServ() {}

ChannelServ::~ChannelServ() {}

void	ChannelServ::joinChannel(const std::string& channelName, User & user) {
	_channels[channelName].addUser(user);
}

void	ChannelServ::leaveChannel(const std::string& channelName, User & user) {
	_channels[channelName].removeUser(user);
	_channels[channelName].removeOperator(user.getNickname());
	_channels[channelName].removeInvitedUser(user.getNickname());
	if (_channels[channelName].empty()) // find a way to check if channel is empty
		deleteChannel(channelName);
}

void	ChannelServ::createChannel(const std::string & channelName, User & user) {
	Channel	newChannel;
	
	newChannel.setName(channelName);
	newChannel.addUser(user);
	newChannel.addOperator(user.getNickname());
	_channels[channelName] = newChannel;
}

void		ChannelServ::deleteChannel(const std::string & channelName) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	if (it != _channels.end())
		_channels.erase(it);
}

void		ChannelServ::removeUserFromAllChannels(User & user) {
   std::map<std::string, Channel>::iterator it;
   
	for (it = _channels.begin(); it != _channels.end(); ++it) {
        it->second.removeUser(user);
    }
}

void	ChannelServ::broadcastMessageToChannels(const std::string& message, User& sender) {
	std::map<std::string, Channel>::iterator	it;
	for (it = _channels.begin(); it != _channels.end(); ++it) {
		if (this->isUserOnChannel(it->first, sender) == true) {
			it->second.broadcastMessageOnChannel(message, sender);
		}
	}
}

Channel*	ChannelServ::getChannel(const std::string& channelName) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	if (it != _channels.end())
		return &(it->second);
	return NULL;
}


std::map<std::string, Channel>	&ChannelServ::getChannelsList() { return (_channels);}

bool	ChannelServ::DoesChannelExist(const std::string & channelName) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	if (it == _channels.end())
		return (false);
	return (true);
}

bool	ChannelServ::isUserOnChannel(std::string const & channelName, User & user) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	if (it != _channels.end()) {
		Channel channel = it->second;
		if (channel.isUserOnChannel(user.getNickname()) == true)
			return (true);
	}
	return (false);
}

bool	ChannelServ::isChannelFull(std::string const & channelName) {
	std::map<std::string, Channel>::iterator	it = _channels.find(channelName);
	
	Channel channel = it->second;
	std::vector<User*> users = channel.getUsers();
	if (channel.getMaxUsersPerChannel() != 0 && users.size() == channel.getMaxUsersPerChannel())
		return (true);
	return (false);	
}
