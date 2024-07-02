#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

void	MessageServ::handleModeCommand(std::string & command, User & user) {
	std::cout << "Handling MODE command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, target, mode, arg;
    iss >> cmd >> target >> mode;
	getline(iss, arg);
	if (target == "*")
		return;
	if (target.empty())
		throw (NeedMoreParamsException(user.getNickname(), cmd));
	std::string	channel;
	if (target[0] == '#')
		channel = target.substr(1);
	else
		channel = target;
	if (_channelServ.DoesChannelExist(channel) == false)
		throw (NoSuchChannelException(user.getNickname(), channel));
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(user.getNickname(), channel));
	Channel	*channelObj = _channelServ.getChannel(channel);
	if (mode.empty()) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << RPL_CHANNELMODEIS << " " << user.getNickname() << " " << target << " " << channelObj->getModes() << "\r\n";
		std::string	response = message.str();
		std::cout << response;
		user.broadcastMessageToHimself(response);
		return;
	}
	if (channelObj->isOperator(user.getNickname()) == false)
		throw (ChanOPrivsNeededException(user.getNickname(), target));
	for (size_t i = 0; i < mode.length(); i++) {
		if (mode[i] != '+' && mode[i] != '-' && mode[i] != 'i' && mode[i] != 't' \
		&& mode[i] != 'k' && mode[i] != 'o' && mode[i] != 'l')
			throw (UnknownModeException(user.getNickname(), mode));
	}
	std::istringstream	args(arg);
	int	ret;
	for (size_t i = 0; i < mode.length(); i++) {
		if (mode[i] == '+') {
			while (mode[i] && mode[i] != '-') {
				ret = handleSetMode(channelObj, mode[i], args, user);
				if (ret != 1)
					channelObj->addMode(mode[i]);
				i++;
			}
		}
		else if (mode[i] == '-') {
			while (mode[i] && mode[i] != '+') {
				handleRemoveMode(channelObj, mode[i], args, user);
				channelObj->deleteMode(mode[i]);
				i++;
			}
		}
	}
	std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost MODE #" + channel + " " + mode;
	if (!arg.empty())
		response += " " + arg;
	response += "\r\n";
	if (ret != 1) {
		channelObj->broadcastMessageOnChannel(response, user);
		user.broadcastMessageToHimself(response);
	}
}

int	MessageServ::handleSetMode(Channel *channel, char const & mode, std::istringstream &args, User &user) {
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
				throw (NeedMoreParamsException(user.getNickname(), "MODE"));
			if (key.empty())
				throw (NeedMoreParamsException(user.getNickname(), "MODE"));
			channel->setPasswordMode(ENABLED);
			channel->setPassword(key);
			break;
		}
		case 'o': {
			std::string	nick;
			args >> nick;
			if (nick.empty())
				throw (NeedMoreParamsException(user.getNickname(), "MODE"));
			User *newOp = _userServ.getUserByNickname(nick);
			if (newOp == NULL)
				throw (NoSuchNickException(user.getNickname(), nick));
			if (_channelServ.isUserOnChannel(channel->getName(), *newOp) == false)
				throw (UserNotInChannelException(user.getNickname(), nick, channel->getName()));
			channel->addOperator(nick);
			break;
		}
		case 'l': {
			std::string	limit;
			args >> limit;
			if (limit.empty())
				throw (NeedMoreParamsException(user.getNickname(), "MODE"));
			for (size_t i = 0; i < limit.length(); i++) {
				if (std::isdigit(limit[i]) == false) {
					return (1);
				}
			}
			channel->setMaxUsersPerChannel(std::atoi(limit.c_str()));
			break;
		}
	}
	return (0);
}

void	MessageServ::handleRemoveMode(Channel *channel, char const & mode, std::istringstream &args, User& user) {
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
				throw (NeedMoreParamsException(user.getNickname(), "MODE"));
			User *op = _userServ.getUserByNickname(nick);
			if (_channelServ.isUserOnChannel(channel->getName(), *op) == false)
				throw (UserNotInChannelException(user.getNickname(), nick, channel->getName()));
			channel->removeOperator(nick);
			break;
		}
		case 'l':
			channel->setMaxUsersPerChannel(0);
			break;
	}
}
