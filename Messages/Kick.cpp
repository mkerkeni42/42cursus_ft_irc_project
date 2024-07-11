#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

bool	MessageServ::handleKickCommand(std::string & command, User *user) {
	std::istringstream iss(command);
    std::string cmd, channel, nickname, message;
    iss >> cmd >> channel >> nickname;
	std::getline(iss >> std::ws, message);

	if (channel.empty() || channel[0] != '#' || nickname.empty())
		throw (NeedMoreParamsException(user->getNickname(), cmd));
	channel = channel.substr(1);
	if (!_channelServ.DoesChannelExist(channel))
		throw NoSuchChannelException(user->getNickname(), "#" + channel);
	if (!_channelServ.isUserOnChannel(channel, user))
		throw NotOnChannelException(user->getNickname(), "#" + channel);

	Channel	*channelObj = _channelServ.getChannel(channel);
	if (!channelObj->isOperator(user->getNickname()))
		throw ChanOPrivsNeededException(user->getNickname(), "#" + channel);

	std::vector<std::string>	nicksToKick;
	getList(nickname, nicksToKick, 0, user);
	for (size_t i = 0; i < nicksToKick.size(); i++) {
		if (!_userServ.isNicknameInUse(nicksToKick[i]))
			throw NoSuchNickException(user->getNickname(), nicksToKick[i]);
		User	*troublemaker = _userServ.getUserByNickname(nicksToKick[i]);
		if (!_channelServ.isUserOnChannel(channel, troublemaker))
			throw UserNotInChannelException(user->getNickname(), troublemaker->getNickname(), "#" + channel);

		std::string response = "#" + channel + " " + troublemaker->getNickname();
		if (!message.empty() && message != ":")
			response += " " + message;
		else
			response += " :" + nickname;
		user->broadcastMessageToHimself(getNotif(user, cmd, CLIENT, response));
		
		_channelServ.getChannel(channel)->broadcastMessageOnChannel(getNotif(user, cmd, CLIENT, response), user, 0);
		_channelServ.leaveChannel(channel, troublemaker);
	}

	return true;
}
