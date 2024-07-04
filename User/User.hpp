#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"
#include "UserServ.hpp"

class	UserServ;

class	User {

public:

	User(void);
	User(int fd);
	~User(void);

	int			getFD(void) const;
	bool		getRegistrationStatus(void) const;
	bool		getPasswdStatus(void) const;
	std::string	getUsername(void) const;
	std::string	getNickname(void) const;
	int			getJoinedChanNb(void) const;
	
	void		setRegistrationStatus(bool status);
	void		setPasswdStatus(bool status);
	void		setUsername(std::string const & username);
	void		setNickname(std::string const & nickname, UserServ & userServ);
	void		setRealname(std::string const & mode);
	
	void		incJoinedChanNb(void);
	void		decJoinedChanNb(void);

	bool		isNotified(void) const;
	void		setNotified(bool notified);

	
	int			receiveData();
    bool		hasBufferedCommand() const;
    std::string	getBufferedCommand();
	void		broadcastMessageToHimself(const std::string& message);
	
private:

	int			_fd;
	bool		_registered;
	std::string	_buffer;
	std::string	_username;
	std::string	_nickname;
	std::string	_realname;
	int			_joinedChanNb;
	bool		_notified;
	bool		_passwdStatus;
};
