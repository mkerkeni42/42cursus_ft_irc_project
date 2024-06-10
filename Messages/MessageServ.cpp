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
	/*_commandMap["PART"] = &MessageServ::handlePartCommand;
	_commandMap["INVITE"] = &MessageServ::handleInviteCommand;
	_commandMap["KICK"] = &MessageServ::handleKickCommand;
	_commandMap["TOPIC"] = &MessageServ::handleTopicCommand;
	_commandMap["MODE"] = &MessageServ::handleModeCommand;
	_commandMap["PRIVMSG"] = &MessageServ::handlePrivmsgCommand;
	_commandMap["PING"] = &MessageServ::handlePingCommand;
	_commandMap["CAP"] = &MessageServ::handleCapCommand;*/
}

void	MessageServ::handleCommand(std::string & command, User& user) {
	std::string	cmd = get_cmd(command);
	std::map<std::string, CommandHandler>::iterator it = _commandMap.find(cmd);
    if (it != _commandMap.end())
    	(this->*(it->second))(command, user);
    else
    	throw UnknownCommandException(cmd);
	
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
	for (size_t i = 0; i < username.length(); i++) {
		if (!std::isalnum(username[i]) || username[i] != '_' || username[i] != '-')
			return;
	}
	if (_userServ.isUserRegistered(username) == true)
		throw (AlreadyRegisteredException());
	for (size_t i = 0; i < mode.length(); i++) {
		if (!std::isdigit(mode[i]))
			return; //don't no if need to display error
	}
	if (mode.compare("0") && mode.compare("8") && mode.compare("10"))
		return; //don't no if need to display error
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
	// need to decide password policy
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
    std::string cmd, channel;
    iss >> cmd >> channel >> std::ws;
	if (channel.empty() || channel[0] != '#')
		throw (NeedMoreParamsException(cmd));
	if (_channelServ.isUserOnChannel(channel, user) == true)
		throw (UserOnChannelException(user.getUsername(), channel));
	// need to check if channelServ is full ERR_CHANNELISFULL
	//need to check if user has reached max number of channel joined ERR_TOOMAYCHANNELS
	//need to check channel mode and user mode ERR_CHNOPRIVNEEDED
	//if channel already exists, join
	//if channel doesn't exists, create it

}

/*void	MessageServ::handlePartCommand(std::string & command, User & user) {
	std::cout << "Handling PART command" << std::endl;
}

void	MessageServ::handleInviteCommand(std::string & command, User & user) {
	std::cout << "Handling INVITE command" << std::endl;
}

void	MessageServ::handleKickCommand(std::string & command, User & user) {
	std::cout << "Handling KICK command" << std::endl;
}

void	MessageServ::handleTopicCommand(std::string & command, User & user) {
	std::cout << "Handling TOPIC command" << std::endl;
}

void	MessageServ::handleModeCommand(std::string & command, User & user) {
	std::cout << "Handling MODE command" << std::endl;
}

void	MessageServ::handlePrivmsgCommand(std::string & command, User & user) {
	std::cout << "Handling PRIVMSG command" << std::endl;
}

void	MessageServ::handlePingCommand(std::string & command, User & user) {
	std::cout << "Handling PING command" << std::endl;
}

void	MessageServ::handleCapCommand(std::string & command, User & user) {
	std::cout << "Handling CAP command" << std::endl;
}*/
