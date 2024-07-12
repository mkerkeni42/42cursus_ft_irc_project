#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

bool	MessageServ::handlePartCommand(std::string & command, User *user) {
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
			throw NotOnChannelException(user->getNickname(), "#" + channels[i]);
		_channelServ.leaveChannel(channels[i], user);
		user->decJoinedChanNb();

		std::string response = "#" + channels[i];
		if (!reason.empty())
			response += " " + reason;
		user->broadcastMessageToHimself(getNotif(user, cmd, CLIENT, response));
		if (_channelServ.DoesChannelExist(channels[i]))
			_channelServ.getChannel(channels[i])->broadcastMessageOnChannel(getNotif(user, cmd, CLIENT, response), user, 0);
	}
	
	return true;
}
