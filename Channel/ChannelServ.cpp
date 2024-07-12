#include "ChannelServ.hpp"
#include "../User/User.hpp"

ChannelServ::ChannelServ() {}

ChannelServ::~ChannelServ() {
	std::map<std::string, Channel*>::iterator	it;
	for (it = _channels.begin(); it != _channels.end(); ++it)
		delete it->second;
	_channels.clear();
}

void	ChannelServ::joinChannel(const std::string& channelName, User *user) {
	_channels[channelName]->addUser(user);
}

void	ChannelServ::leaveChannel(const std::string& channelName, User *user) {
	Channel*	channel = _channels[channelName];
	channel->removeUser(user);
	channel->removeOperator(user->getNickname());
	channel->removeInvitedUser(user->getNickname());
	if (channel->getUsers().empty())
		deleteChannel(channelName);
}

void	ChannelServ::createChannel(const std::string & channelName, User *user) {
	Channel	*newChannel = new Channel(channelName);
	
	newChannel->addUser(user);
	newChannel->addOperator(user->getNickname());
	_channels[channelName] = newChannel;
}

void		ChannelServ::deleteChannel(const std::string & channelName) {
	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	if (it != _channels.end()) {
		delete it->second;
		_channels.erase(it);
	}
}

void		ChannelServ::removeUserFromAllChannels(User *user) {
   std::map<std::string, Channel*>::iterator it;
   
	for (it = _channels.begin(); it != _channels.end(); ++it) {
        it->second->removeUser(user);
    }
}

void	ChannelServ::updateNicknameInChannels(const std::string &newNickname, User *user) {
	std::map<std::string, Channel*>::iterator	it;
	for (it = _channels.begin(); it != _channels.end(); ++it) {
		Channel	*channel = it->second;
		if (channel->isUserOnChannel(user->getNickname())) {
			channel->updateNicknameMap(user, newNickname);
		}
		if (channel->isInvited(user->getNickname()) == true) {
			channel->removeInvitedUser(user->getNickname());
			channel->addInvitedUser(newNickname);
		}
	}
}

void	ChannelServ::broadcastMessageToChannels(const std::string& message, User *sender) {
	std::map<std::string, Channel*>::iterator	it;
	for (it = _channels.begin(); it != _channels.end(); ++it) {
		Channel	*channel = it->second;
		if (this->isUserOnChannel(it->first, sender)) {
			channel->broadcastMessageOnChannel(message, sender, 1);
		}
	}
	resetUsersNotif();
}

void	ChannelServ::resetUsersNotif(void) {
	std::map<std::string, Channel*>::iterator	it;
	for (it = _channels.begin(); it != _channels.end(); ++it) {
		Channel	*channel = it->second;
		std::vector<User*>	&users = channel->getUsers();
		for (std::vector<User*>::iterator	it = users.begin(); it != users.end(); ++it) {
			User*	user = *it;
			user->setNotified(false);
		}
	}	
}

Channel*	ChannelServ::getChannel(const std::string& channelName) {
	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

bool	ChannelServ::DoesChannelExist(const std::string & channelName) {
	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	if (it == _channels.end())
		return (false);
	return (true);
}

bool	ChannelServ::isUserOnChannel(std::string const & channelName, User *user) {
	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	if (it != _channels.end()) {
		Channel *channel = it->second;
		if (channel->isUserOnChannel(user->getNickname()) == true)
			return (true);
	}
	return (false);
}

bool	ChannelServ::isChannelFull(std::string const & channelName) {
	std::map<std::string, Channel*>::iterator	it = _channels.find(channelName);
	
	Channel *channel = it->second;
	std::vector<User*> users = channel->getUsers();
	if (channel->getMaxUsersPerChannel() != 0 && users.size() == channel->getMaxUsersPerChannel())
		return (true);
	return (false);	
}
