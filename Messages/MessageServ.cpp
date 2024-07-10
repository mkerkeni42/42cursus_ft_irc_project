#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

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

MessageServ::~MessageServ(void) {}

void	MessageServ::handleCommand(std::string & command, User *user) {
	std::cout << command << std::endl; // need to delete
	std::istringstream	iss(command);
	std::string			cmd;
	iss >> cmd;
	try {
		std::map<std::string, CommandHandler>::iterator it = _commandMap.find(cmd);
		if (it != _commandMap.end())
			(this->*(it->second))(command, user);
		else
			throw (UnknownCommandException(user->getUsername(), cmd));
		if (user != NULL && !user->getRegistrationStatus() && user->getPasswdStatus() && user->getNickname() != "*" && user->getUsername() != "*") {
			user->setRegistrationStatus(true);
			sendWelcomeMessages(user);
		}
	}
	catch (std::exception &e) {
		user->broadcastMessageToHimself(e.what());
	}
}

void MessageServ::handleCapCommand(std::string & command, User *user) {
	std::cout << "Handling CAP command: " << std::endl;
	std::istringstream iss(command);
	std::string cmd, subCommand, capabilities;

	iss >> cmd >> subCommand >> capabilities >> std::ws;

	std::string	response = user->getNickname() + " ";

	if (subCommand == "LS")
		response += "LS :multi-prefix";
	else if (subCommand == "REQ") {
		response += (capabilities == ":multi-prefix") ? "ACK " : "NAK ";
        response += capabilities;
	}
	else if (subCommand == "END")
		response += "END";
	else
		response += "ERR :Unknown CAP subcommand";

	user->broadcastMessageToHimself(getNotif(user, cmd, SERVER, response));
}

void	MessageServ::handleUserCommand(std::string & command, User *user) {
	std::cout << "Handling USER command" << std::endl;
	std::istringstream	iss(command);
	std::string cmd, username, mode, unused, realname;

	iss >> cmd >> username >>  mode >> unused >> std::ws;
	std::getline(iss, realname);
	if (username.empty() || mode.empty() || unused.empty() || realname.empty())
		throw NeedMoreParamsException(user->getUsername(), cmd);
	if (user->getRegistrationStatus())
		throw AlreadyRegisteredException(user->getUsername());
	user->setRealname(realname);
	user->setUsername(username);
}

void	MessageServ::handleNickCommand(std::string & command, User *user) {
	std::cout << "Handling NICK command" << std::endl;
	std::istringstream	iss(command);
	std::string cmd, nickname;
	iss >> cmd >> nickname >> std::ws;

	if (nickname.empty())
		throw NoNicknameGivenException(user->getUsername());
	if (!_userServ.isUserRegistered(user->getUsername()))
		throw NotRegisteredException(user->getUsername());
	if (!isValidNickname(nickname))
		throw ErroneusNicknameException(user->getUsername(), nickname);
	if (_userServ.isNicknameInUse(nickname))
		throw NicknameInUseException(user->getUsername(), nickname);

	user->broadcastMessageToHimself(getNotif(user, cmd, CLIENT, ":" + nickname));
	_channelServ.broadcastMessageToChannels(getNotif(user, cmd, CLIENT, ":" + nickname), user);
	_channelServ.updateNicknameInChannels(nickname, user);
	user->setNickname(nickname, _userServ);
}

void	MessageServ::handlePassCommand(std::string & command, User *user) {
	std::cout << "Handling PASS command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, password;
    iss >> cmd >> password >> std::ws;

	if (password.empty())
		throw NeedMoreParamsException(user->getUsername(), cmd);
	if (user->getRegistrationStatus())
		throw AlreadyRegisteredException(user->getUsername());
	if (_userServ.getPassword() != password)
		throw PasswdMismatchException(user->getUsername());
    
	user->setPasswdStatus(true);
}

void	MessageServ::handleQuitCommand(std::string & command, User *user) {
	std::cout << "Handling QUIT command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, reason;
    iss >> cmd >> std::ws;
	std::getline(iss, reason);

	if (reason[0] == ':')
		reason = reason.substr(1);
	user->broadcastMessageToHimself(getNotif(user, cmd, CLIENT, ":Quit: " + reason));
	_channelServ.broadcastMessageToChannels(getNotif(user, cmd, CLIENT, ":Quit: " + reason), user);
	
	_channelServ.removeUserFromAllChannels(user);
	_userServ.removeUserfromNetwork(user->getFD());
	_userServ.removeUser(user->getFD());
}

void	MessageServ::handlePingCommand(std::string & command, User *user) {
	(void)user;
	std::cout << "Handling PING command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, token;

    iss >> cmd >> token >> std::ws;
	if (token.empty())
		throw (NeedMoreParamsException(user->getUsername(), "PING"));
	std::string response = "PONG irc.myyamin.chat :" + token + "\r\n";
    user->broadcastMessageToHimself(response);
}
