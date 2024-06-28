#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

void	MessageServ::handlePrivmsgCommand(std::string & command, User & user) {
	std::cout << "Handling PRIVMSG command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, recipient, message;
    iss >> cmd >> recipient >> std::ws;
	std::getline(iss, message);
	if (recipient.empty())
		throw (NeedMoreParamsException(cmd));
	if (message.empty())
		throw (NoTextToSendException());
	if (recipient[0] == '#')
		recipient = recipient.substr(1);
	if (_channelServ.DoesChannelExist(recipient) == true) {
		if (_channelServ.isUserOnChannel(recipient, user) == false)
			throw (NotOnChannelException(recipient));
		message = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost PRIVMSG #" + recipient + " " + message + "\r\n";
		_channelServ.getChannel(recipient)->broadcastMessageOnChannel(message, user);
	}
	else if (_userServ.isNicknameInUse(recipient) == true) {
		message = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost PRIVMSG " + recipient + " " + message + "\r\n"; 
		_userServ.broadcastPrivateMessage(message, recipient);
	}
	else
		throw (NoSuchNickException(recipient));
	// need to handle multiple recipients (channels or nicknames or both)
}
