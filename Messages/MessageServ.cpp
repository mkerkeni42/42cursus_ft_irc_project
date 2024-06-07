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

std::string	get_cmd(std::string message) {
	size_t	pos = message.find(" ");
	if (pos == std::string::npos)
		return (message);
	return(message.substr(0, pos));
}

MessageServ::MessageServ(UserServ & userServ) : _userServ(userServ) {
	std::string	message = "USER mkerkeni 0 * :Myriam Kerkeni"; // just for testing
	_command_map["USER"] = &MessageServ::handleUserCommand;
	/*_command_map["NICK"] = &MessageServ::handleNickCommand;
	_command_map["PASS"] = &MessageServ::handlePassCommand;
	_command_map["QUIT"] = &MessageServ::handleQuitCommand;
	_command_map["JOIN"] = &MessageServ::handleJoinCommand;
	_command_map["PART"] = &MessageServ::handlePartCommand;
	_command_map["INVITE"] = &MessageServ::handleInviteCommand;
	_command_map["KICK"] = &MessageServ::handleKickCommand;
	_command_map["MODE"] = &MessageServ::handleModeCommand;
	_command_map["PRIVMSG"] = &MessageServ::handlePrivmsgCommand;
	_command_map["PING"] = &MessageServ::handlePingCommand;
	_command_map["CAP"] = &MessageServ::handleCapCommand;*/
}

void	MessageServ::handleCommand(std::string & command, User& user) {
	std::string	cmd = get_cmd(command);
	std::map<std::string, CommandHandler>::iterator it = _command_map.find(cmd);
    if (it != _command_map.end())
    	(this->*(it->second))(command, user);
    else
    	throw UnknownCommandException(cmd);
	
}

MessageServ::~MessageServ(void) {}

int		isUserRegistered(void) {
	//here need to find in map of UserServ class containing list of users
	
	return (0);
}

void	MessageServ::handleUserCommand(std::string & command, User & user) {
	std::cout << "Handling USER command" << std::endl;
	std::istringstream	iss(command);
	std::string cmd, username, mode, unused, realname;

	iss >> cmd >> username >> mode >> unused >> std::ws;
	std::getline(iss, realname);
	if (username.empty() || mode.empty() || unused.empty() || realname.empty()) {
		throw (NeedMoreParamsException(cmd));
		return;
	}
	if (isUserRegistered()) {
		throw (AlreadyRegisteredException());
		return;
	}
	user.setUsername(username);
}

/*void	MessageServ::handleNickCommand(std::string & command, User & user) {
	std::cout << "Handling NICK command" << std::endl;
}

void	MessageServ::handlePassCommand(std::string & command, User & user) {
	std::cout << "Handling PASS command" << std::endl;
}

void	MessageServ::handleQuitCommand(std::string & command, User & user) {
	std::cout << "Handling QUIT command" << std::endl;
}

void	MessageServ::handleJoinCommand(std::string & command, User & user) {
	std::cout << "Handling JOIN command" << std::endl;
}

void	MessageServ::handlePartCommand(std::string & command, User & user) {
	std::cout << "Handling PART command" << std::endl;
}

void	MessageServ::handleInviteCommand(std::string & command, User & user) {
	std::cout << "Handling INVITE command" << std::endl;
}

void	MessageServ::handleKickCommand(std::string & command, User & user) {
	std::cout << "Handling KICK command" << std::endl;
}

void	MessageServ::handleModeCommand(std::string & command, User & user) {
	std::cout << "Handling MODE command" << std::endl;
}

void	MessageServ::handlePrivmsgCommand(std::string & command, User & user) {
	std::cout << "Handling PRIVMSG command" << std::endl;
}

void	MessageServ::handlePingCommand(void) {
	std::cout << "Handling PING command" << std::endl;
}

void	MessageServ::handleCapCommand(void) {
	std::cout << "Handling CAP command" << std::endl;
}*/