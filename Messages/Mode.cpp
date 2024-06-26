#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

void	MessageServ::handleModeCommand(std::string & command, User & user) {
	std::cout << "Handling MODE command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, target, mode, arg;
    iss >> cmd >> target >> mode >> arg >> std::ws;
	if (target.empty() || target[0] != '#') {
		return;
	}
	std::string	channel = target.substr(1);
	if (mode.empty())
		throw (NeedMoreParamsException(cmd));
	if (_channelServ.DoesChannelExist(channel) == false)
		throw (NoSuchChannelException(channel));
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(channel));
	Channel	*channelObj = _channelServ.getChannel(channel);
	if (channelObj->isOperator(user.getUsername()) == false)
		throw (ChanOPrivsNeededException(channel));
	if (mode.length() != 2 || (mode[0] != '+' && mode[0] != '-') || (mode[1] != 'i' && mode[1] != 't' \
		&& mode[1] != 'k' && mode[1] != 'o' && mode[1] != 'l'))
			throw (UnknownModeException(mode));
	if (mode[0] == '+')
		handleSetMode(channelObj, mode[1], arg);
	else if (mode[0] == '-')
		handleRemoveMode(channelObj, mode[1], arg);
	std::string response = ":irc.myyamin.chat MODE " + user.getNickname() + " " + channel + " " + mode;
	if (!arg.empty())
		response += " " + arg;
	response += "\r\n";
	std::cout << response << std::endl;
    user.broadcastMessageToHimself(response);
	// need to handle multiple options for Mode command like "MODE +iot"
}

void	MessageServ::handleSetMode(Channel *channel, char const & mode, std::string arg) {
	switch (mode) {
		case 'i':
			channel->setMode(INVITE_ONLY);
			break;
		case 't':
			channel->setTopicMode(CHANOP_ONLY);
			break;
		case 'k':
			if (channel->getPasswordMode() == ENABLED)
				throw (KeySetException(channel->getName()));
			if (arg.empty())
				throw (NeedMoreParamsException("MODE"));
			if (arg.length() < 8 || arg.length() > 24) {
				// throw password exception
				return;
			}
			channel->setPasswordMode(ENABLED);
			channel->setPassword(arg);
			channel->setMode(PROTECTED);
			break;
		case 'o': {
			User *user = _userServ.getUserByNickname(arg);
			if (_channelServ.isUserOnChannel(channel->getName(), *user) == false)
				throw (UserNotInChannelException(arg, channel->getName()));
			channel->addOperator(arg);
			break;
		}
		case 'l': {
			for (size_t i = 0; i < arg.length(); i++) {
				if (std::isdigit(arg[i]) == false) {
					// throw exception for bad characters
					return;
				}
			}
			channel->setMaxUsersPerChannel(std::atoi(arg.c_str()));
			break;
		}
	}
}

void	MessageServ::handleRemoveMode(Channel *channel, char const & mode, std::string arg) {
	switch (mode) {
		case 'i':
			channel->setMode(PUBLIC);
			break;
		case 't':
			channel->setTopicMode(PUBLIC);
			break;
		case 'k': {
			channel->setPasswordMode(DISABLED);
			channel->setMode(PUBLIC);
			std::string	emptykey = "";
			channel->setPassword(emptykey);
			break;
		}
		case 'o': {
			User *user = _userServ.getUserByNickname(arg);
			if (_channelServ.isUserOnChannel(channel->getName(), *user) == false)
				throw (UserNotInChannelException(arg, channel->getName()));
			channel->removeOperator(arg);
			break;
		}
		case 'l':
			channel->setMaxUsersPerChannel(0);
			break;
	}
}
