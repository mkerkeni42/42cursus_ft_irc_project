#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

static void	sendResponse(User &user, std::vector<std::string> &channels, ChannelServ &_channelServ, size_t i) {
	std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost JOIN #" + channels[i] + "\r\n";
	user.broadcastMessageToHimself(response);
	_channelServ.getChannel(channels[i])->broadcastMessageOnChannel(response);
	if (_channelServ.getChannel(channels[i])->getTopic() != "") {
		std::ostringstream	topicMsg;
		topicMsg << ":irc.myyamin.chat " << RPL_TOPIC << " " << user.getNickname() << " #" << channels[i] << " :" << _channelServ.getChannel(channels[i])->getTopic() << "\r\n";
		response = topicMsg.str();
		user.broadcastMessageToHimself(response);
	}
}

static void	getList(std::string const &	arg, std::vector<std::string> &list, int x) {
	if (arg.find(",") != std::string::npos) {
		std::stringstream ss(arg);
    	std::string item;
    	while (std::getline(ss, item, ',')) {
			if (!item.empty() && item[0] == '#')
            	item.erase(0, 1);
			else if (x == 1)
				throw (NoSuchChannelException(item));
        	list.push_back(item);
   		}
	}
	else {
		if (!arg.empty() && arg[0] == '#')
			list.push_back(arg.substr(1));
		else if (x == 0)
			list.push_back(arg);
		else
			throw (NoSuchChannelException(arg));
	}
}

void	MessageServ::handleJoinCommand(std::string & command, User & user) {
	std::cout << "Handling JOIN command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, key;

    iss >> cmd >> channel >> key >> std::ws;
	if (channel.empty())
		throw (NeedMoreParamsException(cmd));
	std::vector<std::string>	channels;
	getList(channel, channels, 0);
	std::vector<std::string>	keys;
	if (!key.empty())
		getList(key, keys, 0);
	for (size_t i = 0; i < channels.size(); i++) {
		if (user.getJoinedChanNb() == MAX_CHANNELS_PER_USER)
			throw (TooManyChannelsException(channels[i]));
		if (_channelServ.DoesChannelExist(channels[i]) == false) {
			_channelServ.createChannel(channels[i], user);
			user.incJoinedChanNb();
			return;
		}
		if (_channelServ.isUserOnChannel(channels[i], user) == true) {
			return;
		}
		if (_channelServ.isChannelFull(channels[i]) == true)
			throw (ChannelIsFullException(channels[i]));
		if (_channelServ.getChannel(channels[i])->getMode() == INVITE_ONLY \
			&& _channelServ.isUserInvited(channels[i], user) == false)
				throw (InviteOnlyChanException(channels[i]));
		if (_channelServ.getChannel(channels[i])->getMode() == PROTECTED) {
			if (keys.size() <= i || _channelServ.getChannel(channels[i])->getPassword() != keys[i])
                throw BadChannelKeyException(channels[i]);
		}
		_channelServ.joinChannel(channels[i], user);
		user.incJoinedChanNb();
		sendResponse(user, channels, _channelServ, i);
	}
}

void	MessageServ::handlePartCommand(std::string & command, User & user) {
	std::cout << "Handling PART command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel;

    iss >> cmd >> channel >> std::ws;
	if (channel.empty())
		throw (NeedMoreParamsException(cmd));
	std::vector<std::string>	channels;
	getList(channel, channels, 1);
	for (size_t i = 0; i < channels.size(); i++) {
		if (_channelServ.isUserOnChannel(channels[i], user) == false)
			throw (NotOnChannelException(channels[i]));
		_channelServ.leaveChannel(channels[i], user);
		user.decJoinedChanNb();
		std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost PART #" + channels[i] + "\r\n";
		user.broadcastMessageToHimself(response);
		_channelServ.getChannel(channels[i])->broadcastMessageOnChannel(response);
	}
	// need to find how to close channel window when part
}
