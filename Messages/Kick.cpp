#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

bool	MessageServ::handleKickCommand(std::string & command, User & user) {
	std::cout << "Handling KICK command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, nickname, message;

    iss >> cmd >> channel >> nickname >> std::ws;
	std::getline(iss, message);
	if (channel.empty() || channel[0] != '#' || nickname.empty())
		throw (NeedMoreParamsException(user.getNickname(), cmd));
	channel = channel.substr(1);
	if (_channelServ.DoesChannelExist(channel) == false)
		throw (NoSuchChannelException(user.getNickname(), channel));
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(user.getNickname(), channel));
	Channel	*channelObj = _channelServ.getChannel(channel);
	if (channelObj->isOperator(user.getNickname()) == false)
		throw (ChanOPrivsNeededException(user.getNickname(), channel));
	std::vector<std::string>	nicksToKick;
	getList(nickname, nicksToKick, 0, user);
	for (size_t i = 0; i < nicksToKick.size(); i++) {
		if (_userServ.isNicknameInUse(nicksToKick[i]) == false)
			throw (NoSuchNickException(user.getNickname(), nicksToKick[i]));
		User	*troublemaker = _userServ.getUserByNickname(nicksToKick[i]);
		if (_channelServ.isUserOnChannel(channel, *troublemaker) == false)
			throw (UserNotInChannelException(user.getNickname(), (*troublemaker).getNickname(), channel));
		_channelServ.leaveChannel(channel, *troublemaker);
		std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost KICK #" + channel + " " + troublemaker->getNickname();
		if (!message.empty() || message == ":")
			response += " " + message;
		response += "\r\n";
		std::cout << response;
		user.broadcastMessageToHimself(response);
		_channelServ.getChannel(channel)->broadcastMessageOnChannel(response, user);
        troublemaker->broadcastMessageToHimself(message);
	}
	return true;
}
