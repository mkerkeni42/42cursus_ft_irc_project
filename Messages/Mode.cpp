#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

static bool	isValidMode(std::string mode) {
	for (size_t i = 0; i < mode.length(); i++) {
		if (mode[i] != '+' && mode[i] != '-' && mode[i] != 'i' && mode[i] != 't' \
		&& mode[i] != 'k' && mode[i] != 'o' && mode[i] != 'l')
			return false;
	}
	return true;
}

bool	MessageServ::handleModeCommand(std::string & command, User *user) {
	std::istringstream iss(command);
    std::string cmd, target, mode, arg;
    iss >> cmd >> target >> mode >> std::ws;
	getline(iss, arg);

	if (target.empty())
		throw NeedMoreParamsException(user->getNickname(), cmd);
	if (target[0] != '#')
		return true;
	std::string	channel = target.substr(1);
	if (!_channelServ.DoesChannelExist(channel))
		throw NoSuchChannelException(user->getNickname(), target);
	if (!_channelServ.isUserOnChannel(channel, user))
		throw NotOnChannelException(user->getNickname(), target);

	Channel	*channelObj = _channelServ.getChannel(channel);
	if (mode.empty()) {
		user->broadcastMessageToHimself(getRPL(user, RPL_CHANNELMODEIS, target + " " + channelObj->getModes()));
		return true;
	}
	if (!channelObj->isOperator(user->getNickname()))
		throw ChanOPrivsNeededException(user->getNickname(), target);
	if (!isValidMode(mode))
		throw UnknownModeException(user->getNickname(), mode);

	int ret = handleMode(channelObj, mode, arg, user);

	std::string response = target + " " + mode;
	if (!arg.empty())
		response += " " + arg;
	if (ret != 1) {
		channelObj->broadcastMessageOnChannel(getNotif(user, cmd, CLIENT, response), user, 0);
		user->broadcastMessageToHimself(getNotif(user, cmd, CLIENT, response));
	}
	
	return true;
}

int	MessageServ::handleMode(Channel *channel, std::string const & mode, std::string &arg, User *user) {
	std::istringstream	args(arg);
	int	ret = 0;
	bool	addMode = true;

	for (size_t i = 0; i < mode.length(); i++) {
		char	currentMode = mode[i];
		if (currentMode == '+')
			addMode = true;
		else if (currentMode == '-')
			addMode = false;
		else {
			if (addMode) {
				ret = handleSetMode(channel, currentMode, args, user);
				if (ret != 2)
					channel->addMode(currentMode);
			} else {
				handleRemoveMode(channel, currentMode, args, user);
				channel->deleteMode(currentMode);
			}
		}
	}
	return ret;
}

int	MessageServ::handleSetMode(Channel *channel, char const & mode, std::istringstream &args, User *user) {
	switch (mode) {
		case 'i':
			channel->setMode(INVITE_ONLY);
			break;
		case 't':
			channel->setTopicMode(CHANOP_ONLY);
			break;
		case 'k': {
			std::string	key;
			args >> key;
			if (key.empty())
				throw NeedMoreParamsException(user->getNickname(), "MODE");
			channel->setPasswordMode(ENABLED);
			channel->setPassword(key);
			break;
		}
		case 'o': {
			std::string	nick;
			args >> nick;
			if (nick.empty())
				throw NeedMoreParamsException(user->getNickname(), "MODE");
			User *newOp = _userServ.getUserByNickname(nick);
			if (newOp == NULL)
				throw NoSuchNickException(user->getNickname(), nick);
			if (!_channelServ.isUserOnChannel(channel->getName(), newOp))
				throw UserNotInChannelException(user->getNickname(), nick, channel->getName());
			channel->addOperator(nick);
			return 2;
		}
		case 'l': {
			std::string	limit;
			args >> limit;
			if (limit.empty())
				throw NeedMoreParamsException(user->getNickname(), "MODE");
			for (size_t i = 0; i < limit.length(); i++) {
				if (limit.length() > 9 || !std::isdigit(limit[i])) {
					return 1;
				}
			}
			channel->setMaxUsersPerChannel(std::atoi(limit.c_str()));
			break;
		}
	}
	return 0;
}

void	MessageServ::handleRemoveMode(Channel *channel, char const & mode, std::istringstream &args, User *user) {
	switch (mode) {
		case 'i':
			channel->setMode(PUBLIC);
			break;
		case 't':
			channel->setTopicMode(PUBLIC);
			break;
		case 'k': {
			channel->setPasswordMode(DISABLED);
			std::string	emptykey = "*";
			channel->setPassword(emptykey);
			break;
		}
		case 'o': {
			std::string	nick;
			args >> nick;
			if (nick.empty())
				throw NeedMoreParamsException(user->getNickname(), "MODE");
			User *op = _userServ.getUserByNickname(nick);
			if (!_channelServ.isUserOnChannel(channel->getName(), op))
				throw UserNotInChannelException(user->getNickname(), nick, channel->getName());
			channel->removeOperator(nick);
			break;
		}
		case 'l':
			channel->setMaxUsersPerChannel(0);
			break;
	}
}
