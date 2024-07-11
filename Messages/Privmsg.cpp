#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

bool	MessageServ::handlePrivmsgCommand(std::string & command, User *user) {
	std::istringstream iss(command);
    std::string cmd, recipient, message;
    iss >> cmd >> recipient;
	std::getline(iss >> std::ws, message);

	if (recipient.empty())
		throw NeedMoreParamsException(user->getNickname(), cmd);
	if (message.empty())
		throw NoTextToSendException(user->getNickname());
	if (recipient[0] == '#')
		recipient = recipient.substr(1);
	message = recipient + " " + message;
	if (_channelServ.DoesChannelExist(recipient)) {
		if (!_channelServ.isUserOnChannel(recipient, user))
			throw NotOnChannelException(user->getNickname(), "#" + recipient);
		_channelServ.getChannel(recipient)->broadcastMessageOnChannel(getNotif(user, cmd, CLIENT, "#" + message), user, 0);
	}
	else if (_userServ.isNicknameInUse(recipient)) 
		_userServ.broadcastPrivateMessage(getNotif(user, cmd, CLIENT, message), recipient);
	else
		throw NoSuchNickException(user->getNickname(), recipient);
		
	return true;
}
