#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"
#include "User.hpp"
#include "../Messages/MessageServ.hpp"
#include "../Channel/ChannelServ.hpp"

class	User;
class	MessageServ;
class	NetworkServ;

class	UserServ {

public:

	UserServ(std::string & password, NetworkServ &networkServ);
	~UserServ(void);
	
	void		addUser(int fd);
	void		addUserByNickname(std::string const & nickname, User* user);
	void		removeUser(int fd);
	
	int			handleUserActivity(int fd);
	void		broadcastPrivateMessage(const std::string& message, std::string& recipient);
	
	User		*getUserByNickname(std::string const & nickname);
	std::string	getPassword() const;

	void		removeUserfromNetwork(int fd);
	void		updateUserNicknameMap(std::string const & oldNickname, std::string const & newNickname, User* user);
	
	bool 		isUserRegistered(const std::string & username);
	bool 		isNicknameInUse(const std::string & nickname);

private:
	
	std::string						_password;
	NetworkServ&					_networkServ;
	MessageServ						_messageServ;
	ChannelServ						_channelServ;
	std::map<int, User*> 			_users;
	std::map<std::string, User*>	_nicknameMap;
};
