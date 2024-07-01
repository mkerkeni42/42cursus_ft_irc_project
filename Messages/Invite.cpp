#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

void	MessageServ::handleInviteCommand(std::string & command, User & user) {
	std::cout << "Handling INVITE command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, nickname, channel;

    iss >> cmd >> nickname >> channel >> std::ws;
	if (nickname.empty() || channel.empty())
		throw (NeedMoreParamsException(user.getNickname(), cmd));
	if (channel[0] == '#')
		channel = channel.substr(1);
	if (_channelServ.DoesChannelExist(channel) == false)
		throw (NoSuchChannelException(user.getNickname(), channel));
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(user.getNickname(), channel));
	Channel	*channelObj = _channelServ.getChannel(channel);
	if (channelObj->getMode() == INVITE_ONLY && channelObj->isOperator(user.getNickname()) == false)
		throw (ChanOPrivsNeededException(user.getNickname(), channel));
	if (_userServ.isNicknameInUse(nickname) == false)
		throw (NoSuchNickException(user.getNickname(), nickname));
	User	*guest = _userServ.getUserByNickname(nickname);
	if (_channelServ.isUserOnChannel(channel, *guest) == true)
		throw (UserOnChannelException(user.getNickname(), (*guest).getNickname(), channel));
	channelObj->addInvitedUser((*guest).getNickname());
	std::ostringstream	invitation;
	invitation << ":irc.myyamin.chat " << RPL_INVITING << " " << user.getNickname() << " " << guest->getNickname() << " #" << channel << "\r\n";
	std::string	response = invitation.str();
	user.broadcastMessageToHimself(response);
	std::ostringstream message;
	message << ":" << user.getNickname() << "!" << user.getUsername() << "@localhost INVITE " << nickname << " :#" << channel << "\r\n";
	response = message.str();
	_userServ.broadcastPrivateMessage(response, nickname);
}
