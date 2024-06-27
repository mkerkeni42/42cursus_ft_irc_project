#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

static void	sendResponse(User &user, std::vector<std::string> &channels, ChannelServ &_channelServ, size_t i) {
	std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost JOIN #" + channels[i] + "\r\n";
	user.broadcastMessageToHimself(response);
	_channelServ.getChannel(channels[i])->broadcastMessageOnChannel(response, user);
	if (_channelServ.getChannel(channels[i])->getTopic() != "") {
		std::ostringstream	topicMsg;
		topicMsg << ":irc.myyamin.chat " << RPL_TOPIC << " " << user.getNickname() << " #" << channels[i] << " :" << _channelServ.getChannel(channels[i])->getTopic() << "\r\n";
		response = topicMsg.str();
		user.broadcastMessageToHimself(response);
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

		}
		else {
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
		}
		sendResponse(user, channels, _channelServ, i);
	}
}

void	MessageServ::handlePartCommand(std::string & command, User & user) {
	std::cout << "Handling PART command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, reason;

    iss >> cmd >> channel >> std::ws;
	std::getline(iss, reason);
	if (channel.empty())
		throw (NeedMoreParamsException(cmd));
	std::vector<std::string>	channels;
	getList(channel, channels, 1);
	for (size_t i = 0; i < channels.size(); i++) {
		if (_channelServ.isUserOnChannel(channels[i], user) == false)
			throw (NotOnChannelException(channels[i]));
		_channelServ.leaveChannel(channels[i], user);
		user.decJoinedChanNb();
		std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost PART #" + channels[i];
		if (!reason.empty())
			response += " " + reason;
		response += "\r\n";
		_channelServ.getChannel(channels[i])->broadcastMessageOnChannel(response, user);
		user.broadcastMessageToHimself(response);
	}
}
