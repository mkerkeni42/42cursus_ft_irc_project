/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageServ.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 14:05:12 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/07 14:05:12 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	//_commandMap["PING"] = &MessageServ::handlePingCommand;
}

void	MessageServ::handleCommand(std::string & command, User& user) {
	std::string	cmd = get_cmd(command);
	std::map<std::string, CommandHandler>::iterator it = _commandMap.find(cmd);
    if (it != _commandMap.end())
    	(this->*(it->second))(command, user);
    else
    	throw (UnknownCommandException(cmd));
	
}

MessageServ::~MessageServ(void) {}

void	MessageServ::handleUserCommand(std::string & command, User & user) {
	std::cout << "Handling USER command" << std::endl;
	std::istringstream	iss(command);
	std::string cmd, username, mode, unused, realname;

	iss >> cmd >> username >> mode >> unused >> std::ws;
	std::getline(iss, realname);
	if (username.empty() || mode.empty() || unused.empty() || realname.empty())
		throw (NeedMoreParamsException(cmd));
	// set max size for username (64 ?)
	for (size_t i = 0; i < username.length(); i++) {
		if (!std::isalnum(username[i]) || username[i] != '_' || username[i] != '-')
			return;
	}
	if (_userServ.isUserRegistered(username) == true)
		throw (AlreadyRegisteredException());
	for (size_t i = 0; i < mode.length(); i++) {
		if (!std::isdigit(mode[i]))
			return;
	}
	if (mode != "0" && mode != "8" && mode != "10")
		return;
	user.setMode(std::atoi(mode.c_str()));
	user.setUsername(username);
}

void	MessageServ::handleNickCommand(std::string & command, User & user) {
	std::cout << "Handling NICK command" << std::endl;
	std::istringstream	iss(command);
	std::string cmd, nickname;

	iss >> cmd >> nickname >> std::ws;
	if (nickname.empty())
		throw (NoNicknameGivenException());
	if (_userServ.isUserRegistered(user.getUsername()) == false)
		throw (NotRegisteredException());
	for (size_t i = 0; i < nickname.length(); i++) {
		if (!std::isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '-')
			throw (ErroneusNicknameException(nickname));
	}
    if (_userServ.isNicknameInUse(nickname) == true) {
        throw (NicknameInUseException(nickname));
	}
	user.setNickname(nickname, _userServ);
}

void	MessageServ::handlePassCommand(std::string & command, User & user) {
	std::cout << "Handling PASS command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, password;
    iss >> cmd >> password >> std::ws;
	// need to decide password policy: between 8 and 24 characters
    user.setPassword(password);
}

void	MessageServ::handleQuitCommand(std::string & command, User & user) {
	std::cout << "Handling QUIT command" << std::endl;
	(void)command;
	(void)user;
	std::exit(EXIT_SUCCESS);
}

void	MessageServ::handleJoinCommand(std::string & command, User & user) {
	std::cout << "Handling JOIN command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, arg; // rename arg --> passwd

    iss >> cmd >> channel >> arg >> std::ws;
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
		//open channel window
		return;
	}
	if (_channelServ.isChannelFull(channel) == true)
		throw (ChannelIsFullException(channel));
	if (_channelServ.getChannel(channel)->getMode() == INVITE_ONLY \
		&& _channelServ.isUserInvited(channel, user) == false)
			throw (InviteOnlyChanException(channel));
	if (_channelServ.getChannel(channel)->getMode() == PROTECTED && (arg.empty()
		|| _channelServ.getChannel(channel)->getPassword() != arg)) {
			throw (BadChannelKeyException(channel));
	}
	if (_channelServ.isUserBanned(channel, user) == true) // need to be deleted
		throw (BannedFromChanException(channel));
	_channelServ.joinChannel(channel, user);
	user.incJoinedChanNb();
}

void	MessageServ::handlePartCommand(std::string & command, User & user) {
	std::cout << "Handling PART command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel;

    iss >> cmd >> channel >> std::ws;
	// if we are out of chanel window
	if (channel.empty() || channel[0] != '#')
		throw (NeedMoreParamsException(cmd));
	channel = channel.substr(1);
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(channel));
	_channelServ.leaveChannel(channel, user);
	user.decJoinedChanNb();
	//need to find a way to detect if we are inside channel window or not
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
	if (_channelServ.isUserBanned(channel, *guest) == true) // need to delete it
		throw (BannedFromChanException(channel));
	channelObj->setInvitedUsers((*guest).getUsername());
	//need to send invitation message to guest here (with PRIVMSG ?)
}

void	MessageServ::handleKickCommand(std::string & command, User & user) {
	std::cout << "Handling KICK command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, nickname, message;

    iss >> cmd >> channel >> std::ws;
	std::getline(iss, message);
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
}

void	MessageServ::handleTopicCommand(std::string & command, User & user) {
	std::cout << "Handling TOPIC command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, message; //rename topic

    iss >> cmd >> channel >> std::ws;
	std::getline(iss, message);
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
	if (message.empty())
		std::cout << "Topic for #" << channel << ": " << channelObj->getTopic() << std::endl;
	else {
		channelObj->setTopic(message);
		//std::cout << user.getUsername() << " changed the topic of #" << channel << " to " << message << std::endl;
	}
}

void	MessageServ::handleModeCommand(std::string & command, User & user) {
	std::cout << "Handling MODE command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, mode, arg;
    iss >> cmd >> channel >> mode >> arg >> std::ws;
	if (channel.empty() || channel[0] != '#' || mode.empty())
		throw (NeedMoreParamsException(cmd));
	channel = channel.substr(1);
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
			channel->setPasswordMode(ENABLED);
			//needto set password here
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
				if (std::isdigit(arg[i]) == false)
					return;
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
		case 'k':
			channel->setPasswordMode(DISABLED);
			channel->setMode(PUBLIC);
			//need to delete password, try to put NULL
			break;
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
	if (recipient[0] == '#')
		recipient = recipient.substr(1);
	if (_channelServ.DoesChannelExist(recipient) == true) {
		if (_channelServ.isUserOnChannel(recipient, user) == false)
			throw (NotOnChannelException(recipient));
		_channelServ.getChannel(recipient)->broadcastMessage(message, user);
		std::cout << user.getNickname() << ": " << message << std::endl;
	}
	else if (_userServ.isNicknameInUse(recipient) == true) {
		// need to find a way to send in private window to target
		std::cout << user.getNickname() << ": " << message << std::endl;
	}
	else
		throw (NoSuchNickException(recipient));
	
}

/*void	MessageServ::handlePingCommand(std::string & command, User & user) {
	std::cout << "Handling PING command" << std::endl;
}*/
