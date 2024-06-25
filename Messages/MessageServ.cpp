#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

std::string	get_cmd(std::string message) {
	size_t	pos = message.find_first_not_of(" ");
	if (pos == std::string::npos)
		return ("");
	message = message.substr(pos);
	pos = message.find(" ");
	if (pos == std::string::npos)
		return (message);
	return(message.substr(0, pos));
}

MessageServ::MessageServ(UserServ & userServ, ChannelServ & channelServ) : _userServ(userServ),_channelServ(channelServ) {
	_commandMap["USER"] = &MessageServ::handleUserCommand;
	_commandMap["NICK"] = &MessageServ::handleNickCommand;
	_commandMap["PASS"] = &MessageServ::handlePassCommand;
	_commandMap["QUIT"] = &MessageServ::handleQuitCommand;
	_commandMap["JOIN"] = &MessageServ::handleJoinCommand;
	_commandMap["PART"] = &MessageServ::handlePartCommand;
	_commandMap["INVITE"] = &MessageServ::handleInviteCommand;
	_commandMap["KICK"] = &MessageServ::handleKickCommand;
	_commandMap["TOPIC"] = &MessageServ::handleTopicCommand;
	_commandMap["MODE"] = &MessageServ::handleModeCommand;
	_commandMap["PRIVMSG"] = &MessageServ::handlePrivmsgCommand;
	_commandMap["CAP"] = &MessageServ::handleCapCommand;
	_commandMap["PING"] = &MessageServ::handlePingCommand;
}

void	MessageServ::handleCommand(std::string & command, User& user) {
	std::string	cmd = get_cmd(command);
	std::cout << command << std::endl;
	try {
		std::map<std::string, CommandHandler>::iterator it = _commandMap.find(cmd);
		if (it != _commandMap.end())
			(this->*(it->second))(command, user);
		else
			throw (UnknownCommandException(cmd));
	}
	catch (std::exception &e) {
		std::ostringstream message;
		message << e.what() << "\r\n";
		std::string	response = message.str();
		user.broadcastMessageToHimself(response.c_str());
	}
}

MessageServ::~MessageServ(void) {}

void MessageServ::handleCapCommand(std::string & command, User & user) {
    std::cout << "Handling CAP command: " << std::endl;
    std::istringstream iss(command);
    std::string cmd, subCommand, capabilities;

    iss >> cmd >> subCommand >> capabilities >> std::ws;
    if (subCommand == "LS") {
        std::string response = ":irc.myyamin.chat CAP " + user.getNickname() + " LS :multi-prefix\r\n";
       user.broadcastMessageToHimself(response);
    } else if (subCommand == "REQ") {
		if (capabilities == ":multi-prefix") {
            std::string response = ":irc.myyamin.chat CAP " + user.getNickname() + " ACK " + capabilities + "\r\n";
            user.broadcastMessageToHimself(response);
        } else {
            std::string response = ":irc.myyamin.chat CAP " + user.getNickname() + " NAK " + capabilities + "\r\n";
            user.broadcastMessageToHimself(response);
        }
    } else if (subCommand == "END") {
        std::string response = ":irc.myyamin.chat CAP " + user.getNickname() + " END\r\n";
        user.broadcastMessageToHimself(response);
		response = ":irc.myyamin.chat You need to authenticate to connect to the server\r\n";
   		user.broadcastMessageToHimself(response);
    } else {
        std::string response = ":irc.myyamin.chat CAP " + user.getNickname() + " ERR :Unknown CAP subcommand\r\n";
        user.broadcastMessageToHimself(response);
    }
}

void	MessageServ::handleUserCommand(std::string & command, User & user) {
	std::cout << "Handling USER command" << std::endl;
	std::istringstream	iss(command);
	std::string cmd, username, mode, unused, realname;

	iss >> cmd >> username >>  mode >> unused >> std::ws;
	std::getline(iss, realname);
	if (username.empty() || mode.empty() || unused.empty() || realname.empty())
		throw (NeedMoreParamsException(cmd));
	if (user.getUsername() != "*")
		throw (AlreadyRegisteredException());
	user.setRealname(realname);
	user.setUsername(username);
}

void	MessageServ::handleNickCommand(std::string & command, User & user) {
	std::cout << "Handling NICK command" << std::endl;
	std::istringstream	iss(command);
	std::string cmd, nickname;

	iss >> cmd >> nickname >> std::ws;
	std::cout << "old nickname = " << user.getNickname() << std::endl;
	if (nickname.empty()) {
		std::string message = ":irc.myyamin.chat NICK " + user.getUsername() + " :Your nickname is " + user.getNickname() + "\r\n";
		return;
		//throw (NoNicknameGivenException());
	}
	if (_userServ.isUserRegistered(user.getUsername()) == false)
		throw (NotRegisteredException());
	if (nickname.length() > 9) {
		throw (ErroneusNicknameException(nickname));
	}
	for (size_t i = 0; i < nickname.length(); i++) {
		if (!std::isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '-')
			throw (ErroneusNicknameException(nickname));
	}
    if (_userServ.isNicknameInUse(nickname) == true) {
        throw (NicknameInUseException(user.getUsername(), nickname));
	}
	std::string message = ":irc.myyamin.chat NICK " + nickname + " :" + user.getNickname() + " changed his nickname to " + nickname + "\r\n";
	std::cout << message;
	user.setNickname(nickname, _userServ);
	_channelServ.broadcastMessageToChannels(message, user);
}

void	MessageServ::handlePassCommand(std::string & command, User & user) {
	std::cout << "Handling PASS command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, password;
    iss >> cmd >> password >> std::ws;
	if (password.empty())
		throw (NeedMoreParamsException(cmd));
	if (user.getRegistrationStatus() == true)
		throw (AlreadyRegisteredException());
	if (_userServ.getPassword() != password)
		throw (PasswdMismatchException(user.getUsername()));
    user.setRegistrationStatus(true);
	sendWelcomeMessages(user);

}

void	MessageServ::handleQuitCommand(std::string & command, User & user) {
	std::cout << "Handling QUIT command" << std::endl;
	(void)command;
	
	_channelServ.removeUserFromAllChannels(user);
	_userServ.removeUserfromNetwork(user.getFD());
	_userServ.removeUser(user.getFD());
}

void	MessageServ::handleJoinCommand(std::string & command, User & user) {
	std::cout << "Handling JOIN command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, key;

    iss >> cmd >> channel >> key >> std::ws;
	if (channel.empty() || channel[0] != '#')
		throw (NeedMoreParamsException(cmd));
	channel = channel.substr(1);
	if (user.getJoinedChanNb() == MAX_CHANNELS_PER_USER)
		throw (TooManyChannelsException(channel));
	if (_channelServ.DoesChannelExist(channel) == false) {
		_channelServ.createChannel(channel, user);
		user.incJoinedChanNb();
		return;
	}
	if (_channelServ.isUserOnChannel(channel, user) == true) {
		return;
	}
	if (_channelServ.isChannelFull(channel) == true)
		throw (ChannelIsFullException(channel));
	if (_channelServ.getChannel(channel)->getMode() == INVITE_ONLY \
		&& _channelServ.isUserInvited(channel, user) == false)
			throw (InviteOnlyChanException(channel));
	if (_channelServ.getChannel(channel)->getMode() == PROTECTED && (key.empty()
		|| _channelServ.getChannel(channel)->getPassword() != key)) {
			throw (BadChannelKeyException(channel));
	}
	_channelServ.joinChannel(channel, user);
	user.incJoinedChanNb();
	std::string response = ":irc.myyamin.chat JOIN " + user.getNickname() + "\r\n";
    user.broadcastMessageToHimself(response);
	// need to handle multiple channels in one JOIN command
}

void	MessageServ::handlePartCommand(std::string & command, User & user) {
	std::cout << "Handling PART command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel;

    iss >> cmd >> channel >> std::ws;
	if (channel.empty() || channel[0] != '#')
		throw (NeedMoreParamsException(cmd));
	channel = channel.substr(1);
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(channel));
	_channelServ.leaveChannel(channel, user);
	user.decJoinedChanNb();
	// need to handle multiple channels in one PART command
}

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
	message << ":irc.myyamin.chat " << RPL_INVITING << " " << user.getUsername() << guest->getNickname() << channel << "\r\n";
	std::string	response = message.str();
	_userServ.broadcastPrivateMessage(response.c_str(), nickname);
	// need to handle multiple channels in one INVITE command
}

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

void	MessageServ::handleTopicCommand(std::string & command, User & user) {
	std::cout << "Handling TOPIC command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, topic;

    iss >> cmd >> channel >> std::ws;
	std::getline(iss, topic);
	if (channel.empty() || channel[0] != '#')
		throw (NeedMoreParamsException(cmd));
	channel = channel.substr(1);
	if (_channelServ.DoesChannelExist(channel) == false)
		throw (NoSuchChannelException(channel));
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(channel));
	Channel	*channelObj = _channelServ.getChannel(channel);
	if (channelObj->getTopicMode() == CHANOP_ONLY && channelObj->isOperator(user.getUsername()) == false)
		throw (ChanOPrivsNeededException(channel));
	if (topic.empty()) {
		std::cout << "Topic for #" << channel << ": " << channelObj->getTopic() << std::endl;
	// maybe with broadcast message ?
	}
	else {
		channelObj->setTopic(topic);
		//std::cout << user.getUsername() << " changed the topic of #" << channel << " to " << message << std::endl;
	}
}

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
	message = ": " + user.getNickname() + " :" + message;
	if (recipient[0] == '#')
		recipient = recipient.substr(1);
	if (_channelServ.DoesChannelExist(recipient) == true) {
		if (_channelServ.isUserOnChannel(recipient, user) == false)
			throw (NotOnChannelException(recipient));
		_channelServ.getChannel(recipient)->broadcastMessageOnChannel(message);
	}
	else if (_userServ.isNicknameInUse(recipient) == true) {
		message = ":" + user.getNickname() + "PRIVMSG " + recipient + " :" + message;
		_userServ.broadcastPrivateMessage(message, recipient);
	}
	else
		throw (NoSuchNickException(recipient));
	// need to handle multiple recipients ??
}

void	MessageServ::handlePingCommand(std::string & command, User & user) {
	(void)user;
	std::cout << "Handling PING command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, token;

    iss >> cmd >> token >> std::ws;
	if (token.empty())
		throw (NeedMoreParamsException("PING"));
	std::string response = "PONG irc.myyamin.chat :" + token + "\r\n";
    user.broadcastMessageToHimself(response);
}
