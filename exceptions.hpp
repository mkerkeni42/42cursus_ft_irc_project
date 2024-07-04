#pragma once

#include "config.hpp"

class	NoSuchNickException : public std::exception {
public:
	NoSuchNickException(const std::string &client, const std::string& nick) : _client(client), _nick(nick) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_NOSUCHNICK << " " << _client << " " << _nick << " :No such nick/channel\r\n";
		_message = message.str();
	}
	virtual	~NoSuchNickException() throw () {}
	virtual const char* what() const throw () { 
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_nick;
	std::string	_message;
};

class	NoSuchChannelException : public std::exception {
public:
	NoSuchChannelException(const std::string& client, const std::string& channel) : _client(client), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_NOSUCHCHANNEL << " " << _client << " " << _channel << " :No such channel\r\n";
		_message = message.str();
	}
	virtual	~NoSuchChannelException() throw () {}
	virtual const char* what() const throw () { 
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_channel;
	std::string	_message;
};

class	TooManyChannelsException : public std::exception {
public:
	TooManyChannelsException(const std::string& client, const std::string& channel) : _client(client), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_TOOMANYCHANNELS << " " << _client << " "<< _channel << " :You have joined too many channels\r\n";
		_message = message.str();
	}
	virtual ~TooManyChannelsException() throw () {}
	virtual const char* what() const throw () { 
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_channel;
	std::string	_message;
};

class	NoTextToSendException : public std::exception {
public:
	NoTextToSendException(const std::string& client) : _client(client) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_NOTEXTTOSEND << " " << _client << " :No text to send";
		_message = message.str();
	}
	virtual ~NoTextToSendException() throw () {}
	virtual const char* what() const throw () {
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_message;
};

class	UnknownCommandException : public std::exception {
public:
	UnknownCommandException(const std::string& client, const std::string& command) : _client(client), _command(command) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_UNKNOWNCOMMAND << " " << _client << " " << _command << " :Unknown command\r\n";
		_message = message.str();
	}
	virtual ~UnknownCommandException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_command;
	std::string	_message;
};

class	NoNicknameGivenException : public std::exception {
public:
	NoNicknameGivenException(const std::string& client) : _client(client) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_NONICKNAMEGIVEN << " " << _client << " :No nickname given\r\n";
		_message = message.str();
	}
	virtual ~NoNicknameGivenException() throw () {}
	virtual const char* what() const throw () {
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_message;
};

class	ErroneusNicknameException : public std::exception {
public:
	ErroneusNicknameException(const std::string& client, const std::string& nick) : _client(client), _nick(nick) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_ERRONEUSNICKNAME <<  " " << _client << " " << _nick << " :Erroneus nickname\r\n";
		_message = message.str();
	}
	virtual ~ErroneusNicknameException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_nick;
	std::string	_message;
};

class	NicknameInUseException : public std::exception {
public:
	NicknameInUseException(const std::string& client, const std::string& nick) : _client(client), _nick(nick) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_NICKNAMEINUSE << " " << _client << " " << _nick << " :Nickname is already in use.\r\n";
		_message = message.str();
	}
	virtual ~NicknameInUseException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_nick;
	std::string	_message;
};

class	UserNotInChannelException : public std::exception {
public:
	UserNotInChannelException(const std::string& client, const std::string& nick, const std::string& channel) : _client(client), _nick(nick), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_USERNOTINCHANNEL << " " << _client << " " << _nick << " " << _channel << " :They aren't on that channel\r\n";
		_message = message.str();
	}
	virtual ~UserNotInChannelException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_nick;
	std::string	_channel;
	std::string	_message;
};

class	NotOnChannelException : public std::exception {
public:
	NotOnChannelException(const std::string& client, const std::string& channel) : _client(client), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_NOTONCHANNEL << " " << _client << " " << _channel << " :You are not on that channel\r\n";
		_message = message.str();
	}
	virtual ~NotOnChannelException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_channel;
	std::string	_message;
};

class	UserOnChannelException : public std::exception {
public:
	UserOnChannelException(const std::string& client, const std::string& nick, const std::string& channel) : _client(client), _nick(nick), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_USERONCHANNEL << " " << _client << " " << _nick << " " << _channel << " :Is already on channel\r\n";
		_message = message.str();
	}
	virtual ~UserOnChannelException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_nick;
	std::string	_channel;
	std::string	_message;
};

class	NotRegisteredException : public std::exception {
public:
	NotRegisteredException(const std::string& client) : _client(client) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_NOTREGISTERED << " " << _client << " :You have not registered\r\n";
		_message = message.str();
	}
	virtual ~NotRegisteredException() throw () {}
	virtual const char* what() const throw () {
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_message;
};

class	AlreadyRegisteredException : public std::exception {
public:
	AlreadyRegisteredException(const std::string& client) : _client(client) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_ALREADYREGISTERED << " " << _client << " :You may not reregister\r\n";
		_message = message.str();
	}
	virtual ~AlreadyRegisteredException() throw () {}
	virtual const char* what() const throw () {
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_message;
};

class	NeedMoreParamsException : public std::exception {
public:
	NeedMoreParamsException(const std::string& client, const std::string& command) : _client(client), _command(command) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_NEEDMOREPARAMS << " " << _client << " " << _command << " :Not enough parameters\r\n";
		_message = message.str();
	}
	virtual ~NeedMoreParamsException() throw() {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_command;
	std::string	_message;
};

class	PasswdMismatchException : public std::exception {
public:
	PasswdMismatchException(const std::string & client) : _client(client) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_PASSWDMISMATCH << " " << _client << " :Password incorrect\r\n";
		_message = message.str();
	}
	virtual ~PasswdMismatchException() throw() {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_message;
};

class	ChannelIsFullException : public std::exception {
public:
	ChannelIsFullException(const std::string& client, const std::string& channel) : _client(client), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_CHANNELISFULL << " " << _client << " " << _channel << " :Cannot join channel (+l)\r\n";
		_message = message.str();
	}
	virtual ~ChannelIsFullException() throw() {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_channel;
	std::string	_message;
};

class	UnknownModeException : public std::exception {
public:
	UnknownModeException(const std::string& client, const std::string& param) : _client(client), _param(param) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_UNKNOWNMODE << " " << _client << " " << _param << " :Is unknown mode char to me\r\n";
		_message = message.str();
	}
	virtual ~UnknownModeException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_client;
	std::string	_param;
	std::string	_message;
};

class	InviteOnlyChanException : public std::exception {
public:
	InviteOnlyChanException(const std::string& client, const std::string& channel) : _client(client), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_INVITEONLYCHAN << " " << _client << " " << _channel << " :Cannot join channel (+i)\r\n";
		_message = message.str();
	}
	virtual ~InviteOnlyChanException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_channel;
	std::string	_message;
};

class	BadChannelKeyException : public std::exception {
public:
	BadChannelKeyException(const std::string& client, const std::string& channel) : _client(client), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_BADCHANNELKEY << " " << _client << " " << _channel << " :Cannot join channel (+k)\r\n";
		_message = message.str();
	}
	virtual ~BadChannelKeyException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_channel;
	std::string	_message;
};

class	BadChanMaskException : public std::exception {
public:
	BadChanMaskException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_BADCHANMASK << " " << _channel << " :Bad channel mask\r\n";
		_message = message.str();
	}
	virtual ~BadChanMaskException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	ChanOPrivsNeededException : public std::exception {
public:
	ChanOPrivsNeededException(const std::string& client, const std::string& channel) : _client(client), _channel(channel) {
		std::ostringstream	message;
		message << ":irc.myyamin.chat " << ERR_CHANOPRIVSNEEDED << " " << _client << " " << _channel << " :You're not channel operator\r\n";
		_message = message.str();
	}
	virtual ~ChanOPrivsNeededException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string _client;
	std::string	_channel;
	std::string	_message;
};
