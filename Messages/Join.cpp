#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

void	MessageServ::handleJoinCommand(std::string & command, User *user) {
	std::cout << "Handling JOIN command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, key;

    iss >> cmd >> channel >> key >> std::ws;
	if (channel.empty())
		throw NeedMoreParamsException(user->getNickname(), cmd);
	std::vector<std::string>	channels;
	getList(channel, channels, 0, user);
	std::vector<std::string>	keys;
	if (!key.empty())
		getList(key, keys, 0, user);

	for (size_t i = 0; i < channels.size(); i++) {
		if (user->getJoinedChanNb() == MAX_CHANNELS_PER_USER)
			throw TooManyChannelsException(user->getNickname(), channels[i]);
		
		Channel	*currentChan = NULL;
		if (!_channelServ.DoesChannelExist(channels[i])) {
			_channelServ.createChannel(channels[i], user);
			currentChan = _channelServ.getChannel(channels[i]);
			user->incJoinedChanNb();
		}
		else {
			currentChan = _channelServ.getChannel(channels[i]);
			if (!currentChan)
				return;
			if (_channelServ.isChannelFull(channels[i]))
				throw ChannelIsFullException(user->getNickname(), channels[i]);

			if (currentChan->getPasswordMode() == ENABLED) {
				if (keys.size() <= i || currentChan->getPassword() != keys[i])
					throw BadChannelKeyException(user->getNickname(), channels[i]);
			}
			if (currentChan->getMode() == INVITE_ONLY \
				&& !currentChan->isInvited(user->getNickname()))
					throw InviteOnlyChanException(user->getNickname(), channels[i]);
			_channelServ.joinChannel(channels[i], user);
			user->incJoinedChanNb();
		}
		user->broadcastMessageToHimself(getNotif(user, "JOIN", CLIENT, "#" + channels[i]));
		currentChan->broadcastMessageOnChannel(getNotif(user, "JOIN", CLIENT, "#" + channels[i]), user, 0);
		if (currentChan->getTopic() != "")
			user->broadcastMessageToHimself(getRPL(user, RPL_TOPIC, "#" + channels[i] + " " + currentChan->getTopic()));
	}
}
	
void	MessageServ::handlePartCommand(std::string & command, User *user) {
	std::cout << "Handling PART command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, reason;

    iss >> cmd >> channel;
	std::getline(iss >> std::ws, reason);
	if (channel.empty())
		throw NeedMoreParamsException(user->getNickname(), cmd);

	std::vector<std::string>	channels;
	getList(channel, channels, 1, user);
	for (size_t i = 0; i < channels.size(); i++) {
		if (!_channelServ.isUserOnChannel(channels[i], user))
			throw NotOnChannelException(user->getNickname(), channels[i]);
		_channelServ.leaveChannel(channels[i], user);
		user->decJoinedChanNb();

		std::string response = "#" + channels[i];
		if (!reason.empty())
			response += " " + reason;
		user->broadcastMessageToHimself(getNotif(user, cmd, CLIENT, response));
		if (_channelServ.DoesChannelExist(channels[i]))
			_channelServ.getChannel(channels[i])->broadcastMessageOnChannel(getNotif(user, cmd, CLIENT, response), user, 0);
	}
}
