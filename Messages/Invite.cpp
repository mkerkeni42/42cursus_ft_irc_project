#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

void	MessageServ::handleInviteCommand(std::string & command, User & user) {
	std::cout << "Handling INVITE command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, nickname;

    iss >> cmd >> channel >> nickname >> std::ws;
	if (channel.empty() || channel[0] != '#' || nickname.empty())
		throw (NeedMoreParamsException(cmd));
	channel = channel.substr(1);
	if (_channelServ.DoesChannelExist(channel) == false)
		throw (NoSuchChannelException(channel));
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(channel));
	Channel	*channelObj = _channelServ.getChannel(channel);
	if (channelObj->isOperator(user.getUsername()) == false)
		throw (ChanOPrivsNeededException(channel));
	if (_userServ.isNicknameInUse(nickname) == false)
		throw (NoSuchNickException(nickname));
	User	*guest = _userServ.getUserByNickname(nickname);
	if (_channelServ.isUserOnChannel(channel, *guest) == true)
		throw (UserOnChannelException((*guest).getUsername(), channel));
	channelObj->setInvitedUsers((*guest).getUsername());
	//std::string	message = user.getNickname() + " invites you to join channel #" + channel; // check server format for this message
	std::ostringstream	message;
	message << ":irc.myyamin.chat " << RPL_INVITING << " " << user.getUsername() << " " << guest->getNickname() << " " << channel << "\r\n";
	std::string	response = message.str();
	_userServ.broadcastPrivateMessage(response.c_str(), nickname);
	// need to handle multiple channels in one INVITE command
}
