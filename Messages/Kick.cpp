#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

void	MessageServ::handleKickCommand(std::string & command, User & user) {
	std::cout << "Handling KICK command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, nickname, message;

    iss >> cmd >> channel >> nickname >> std::ws;
	std::getline(iss, message); // need to check how it works
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
	User	*troublemaker = _userServ.getUserByNickname(nickname);
	if (_channelServ.isUserOnChannel(channel, *troublemaker) == false)
		throw (UserNotInChannelException((*troublemaker).getUsername(), channel));
	_channelServ.leaveChannel(channel, *troublemaker);
	// need to handle multiple channels in one KICK comman
}
