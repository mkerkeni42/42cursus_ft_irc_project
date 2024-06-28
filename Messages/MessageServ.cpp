#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

static std::string	get_cmd(std::string message) {
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
	if (nickname.empty()) {
		throw (NoNicknameGivenException());
	}
	if (_userServ.isUserRegistered(user.getUsername()) == false)
		throw (NotRegisteredException());
	if (nickname.length() > 9 || nickname[0] == '#' || nickname[0] == ':') {
		throw (ErroneusNicknameException(nickname));
	}
	for (size_t i = 0; i < nickname.length(); i++) {
		if (!std::isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '-' && nickname[i] != '[' \
			&& nickname[i] != '[' && nickname[i] != '(' && nickname[i] != ')' && nickname[i] != '\\' && nickname[i] != '|' )
				throw (ErroneusNicknameException(nickname));
	}
    if (_userServ.isNicknameInUse(nickname) == true) {
        throw (NicknameInUseException(user.getUsername(), nickname));
	}
	std::string message = ":" + user.getNickname() + " NICK " + nickname + "\r\n";
	std::string	oldNickname = user.getNickname();
	user.setNickname(nickname, _userServ);
	std::map<std::string, Channel> &channels = _channelServ.getChannelsList();
	std::map<std::string, Channel>::iterator	it;
	for (it = channels.begin(); it != channels.end(); ++it) {
		Channel	&channel = it->second;
		if (channel.isUserOnChannel(oldNickname) == true) {
			channel.updateNicknameMap(&user, oldNickname);
		}
	}
	user.broadcastMessageToHimself(message);
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
	 std::istringstream iss(command);
    std::string cmd, reason;

    iss >> cmd >> std::ws;
	getline(iss, reason);
	std::string message = ":" + user.getNickname() + "!" + user.getUsername() +"@localhost QUIT " + reason + "\r\n";
	std::cout << message;
	user.broadcastMessageToHimself(message);
	_channelServ.broadcastMessageToChannels(message, user);
	_channelServ.removeUserFromAllChannels(user);
	_userServ.removeUserfromNetwork(user.getFD());
	_userServ.removeUser(user.getFD());
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

void	MessageServ::getList(std::string const &	arg, std::vector<std::string> &list, int x) {
	if (arg.find(",") != std::string::npos) {
		std::stringstream ss(arg);
    	std::string item;
    	while (std::getline(ss, item, ',')) {
			if (!item.empty() && item[0] == '#')
            	item.erase(0, 1);
			else if (x == 1)
				throw (NoSuchChannelException(item));
			list.push_back(item);
   		}
	}
	else {
		if (!arg.empty() && arg[0] == '#')
			list.push_back(arg.substr(1));
		else if (x == 0)
			list.push_back(arg);
		else
			throw (NoSuchChannelException(arg));
	}
}
