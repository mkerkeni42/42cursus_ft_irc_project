/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 15:20:09 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/04 15:20:09 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config.hpp"

class	NoSuchNickException : public std::exception {
public:
	NoSuchNickException(const std::string& nick) : _nick(nick) {
		std::ostringstream	message;
		message << "401 ERR_NOSUCHNICKNAME " << _nick << " No such nick/channel";
		_message = message.str();
	}
	virtual	~NoSuchNickException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_nick;
	std::string	_message;
};

class	NoSuchChannelException : public std::exception {
public:
	NoSuchChannelException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "403 ERR_NOSUCHCHANNEL " << _channel << " No such channel";
		_message = message.str();
	}
	virtual	~NoSuchChannelException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	CannotSendToChanException : public std::exception {
public:
	CannotSendToChanException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "404 ERR_CANNOTSENDTOCHAN " << _channel << " Cannot send to channel";
		_message = message.str();
	}
	virtual	~CannotSendToChanException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	TooManyChannelsException : public std::exception {
public:
	TooManyChannelsException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "405 ERR_TOOMANYCHANNELS " << _channel << " You have joined too many channels";
		_message = message.str();
	}
	virtual ~TooManyChannelsException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	NoOriginSpecifiedException : public std::exception {
public:
	virtual const char* what() const throw() {
		return ("409 ERR_NOORIGIN No origin specified");
	}
};

class	NoRecipientException : public std::exception {
public:
	NoRecipientException(const std::string& command) : _command(command) {
		std::ostringstream	message;
		message << "411 ERR_NORECIPIENT No recipient given (" << _command << ")";
	_message = message.str();
	}
	virtual	~NoRecipientException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_command;
	std::string	_message;
};

class	NoTextToSendException : public std::exception {
public:
	virtual const char* what() const throw() { 
		return ("412 ERR_NOTEXTTOSEND No text to send");
	}
};

class	UnknownCommandException : public std::exception {
public:
	UnknownCommandException(const std::string& command) : _command(command) {
		std::ostringstream	message;
		message << "421 ERR_UNKNOWNCOMMAND " << _command << " Unknown command";
		_message = message.str();
	}
	virtual ~UnknownCommandException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_command;
	std::string	_message;
};

class	NoNicknameGivenException : public std::exception {
public:
	virtual const char* what() const throw() { 
		return ("431 ERR_NONICKNAMEGIVEN No nickname given");
	}
};

class	ErroneusNicknameException : public std::exception {
public:
	ErroneusNicknameException(const std::string& nick) : _nick(nick) {
		std::ostringstream	message;
		message << "432 ERR_ERRONEUSNICKNAME " << _nick << " Erroneus nickname";
		_message = message.str();
	}
	virtual ~ErroneusNicknameException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_nick;
	std::string	_message;
};

class	NicknameInUseException : public std::exception {
public:
	NicknameInUseException(const std::string& nick) : _nick(nick) {
		std::ostringstream	message;
		message << "433 ERR_NICKNAMEINUSE " << _nick << " Nickname is already in use";
		_message = message.str();
	}
	virtual ~NicknameInUseException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_nick;
	std::string	_message;
};

class	UserNotInChannelException : public std::exception {
public:
	UserNotInChannelException(const std::string& nick, const std::string& channel) : _nick(nick), _channel(channel) {
		std::ostringstream	message;
		message << "441 ERR_USERNOTINCHANNEL " << _nick << " " << _channel << " They aren't on that channel";
		_message = message.str();
	}
	virtual ~UserNotInChannelException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_nick;
	std::string	_channel;
	std::string	_message;
};

class	NotOnChannelException : public std::exception {
public:
	NotOnChannelException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "442 ERR_NOTONCHANNEL " << _channel << " You are not on that channel";
		_message = message.str();
	}
	virtual ~NotOnChannelException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	UserOnChannelException : public std::exception {
public:
	UserOnChannelException(const std::string& user, const std::string& channel) : _user(user), _channel(channel) {
		std::ostringstream	message;
		message << "443 ERR_USERONCHANNEL " << _user << " " << _channel << " is already on channel";
		_message = message.str();
	}
	virtual ~UserOnChannelException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_user;
	std::string	_channel;
	std::string	_message;
};

class	NotRegisteredException : public std::exception {
public:
	virtual const char* what() const throw() { 
		return ("451 ERR_NOTREGISTERED You have not registered");
	}
};

class	AlreadyRegisteredException : public std::exception {
public:
	virtual const char* what() const throw() { 
		return ("462 ERR_ALREADYREGISTERED You may not reregister");
	}
};

class	NeedMoreParamsException : public std::exception {
public:
	NeedMoreParamsException(const std::string& command) : _command(command) {
		std::ostringstream	message;
		message << "461 ERR_NEEDMOREPARAMS " << _command << " NotEnoughParameters";
		_message = message.str();
	}
	virtual ~NeedMoreParamsException() throw() {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_command;
	std::string	_message;
};

class	PasswdMismatchException : public std::exception {
public:
	virtual const char* what() const throw() { 
		return ("464 ERR_PASSWDMISMATCH Password incorrect");
	}
};

class	KeySetException : public std::exception {
public:
	KeySetException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "467 ERR_KEYSET " << _channel << " Channel key already set";
		_message = message.str();
	}
	virtual ~KeySetException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	ChannelIsFullException : public std::exception {
public:
	ChannelIsFullException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "471 ERR_CHANNELISFULL " << _channel << " Cannot join channel (+l)";
		_message = message.str();
	}
	virtual ~ChannelIsFullException() throw() {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	UnknownModeException : public std::exception {
public:
	UnknownModeException(const std::string& param) : _param(param) {
		std::ostringstream	message;
		message << "472 ERR_UNKNOWNMODE " << _param << " Is unknown mode char to me";
		_message = message.str();
	}
	virtual ~UnknownModeException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_param;
	std::string	_message;
};

class	InviteOnlyChanException : public std::exception {
public:
	InviteOnlyChanException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "473 ERR_INVITEONLYCHAN " << _channel << " Cannot join channel (+i)";
		_message = message.str();
	}
	virtual ~InviteOnlyChanException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	BannedFromChanException : public std::exception {
public:
	BannedFromChanException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "474 ERR_BANNEDFROMCHAN " << _channel << " Cannot join channel (+b)";
		_message = message.str();
	}
	virtual ~BannedFromChanException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	BadChannelKeyException : public std::exception {
public:
	BadChannelKeyException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "475 ERR_BADCHANNELKEY " << _channel << " Cannot join channel (+k)";
		_message = message.str();
	}
	virtual ~BadChannelKeyException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};

class	ChanOPrivsNeededException : public std::exception {
public:
	ChanOPrivsNeededException(const std::string& channel) : _channel(channel) {
		std::ostringstream	message;
		message << "471 ERR_CHANOPRIVSNEEDED " << _channel << " You're not channel operator";
		_message = message.str();
	}
	virtual ~ChanOPrivsNeededException() throw () {}
	virtual const char* what() const throw() { 
		return (_message.c_str());
	}
private:
	std::string	_channel;
	std::string	_message;
};
