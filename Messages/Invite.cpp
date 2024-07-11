#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

bool	MessageServ::handleInviteCommand(std::string & command, User *user) {
	std::istringstream iss(command);
    std::string cmd, nickname, channel;
    iss >> cmd >> nickname >> channel >> std::ws;

	if (nickname.empty() || channel.empty())
		throw NeedMoreParamsException(user->getNickname(), cmd);
	if (channel[0] == '#')
		channel = channel.substr(1);
	if (!_channelServ.DoesChannelExist(channel))
		throw NoSuchChannelException(user->getNickname(), "#" + channel);
	if (!_channelServ.isUserOnChannel(channel, user))
		throw NotOnChannelException(user->getNickname(), "#" + channel);

	Channel	*channelObj = _channelServ.getChannel(channel);
	if (channelObj->getMode() == INVITE_ONLY && !channelObj->isOperator(user->getNickname()))
		throw ChanOPrivsNeededException(user->getNickname(), "#" + channel);
	if (!_userServ.isNicknameInUse(nickname))
		throw NoSuchNickException(user->getNickname(), nickname);
	User	*guest = _userServ.getUserByNickname(nickname);
	if (_channelServ.isUserOnChannel(channel, guest))
		throw UserOnChannelException(user->getNickname(), guest->getNickname(), "#" + channel);

	channelObj->addInvitedUser(guest->getNickname());
	user->broadcastMessageToHimself(getRPL(user, RPL_INVITING, nickname + " #" + channel));
	_userServ.broadcastPrivateMessage(getNotif(user, cmd, CLIENT, nickname + " :#" + channel), nickname);

	return true;
}
