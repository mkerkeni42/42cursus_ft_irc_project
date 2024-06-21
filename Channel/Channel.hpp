#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"

class	User;

class	Channel {

public:

	Channel();
	~Channel();

	void						addUser(User& user);
	void						removeUser(User& user);

	void						setName(const std::string& name);
	void						setTopic(const std::string& newTopic);
	void						setPassword(std::string & newPassword);
	void						setMode(const int & newMode);
	void						setTopicMode(const int & newMode);
	void						setPasswordMode(const int & newMode);
	void						setMaxUsersPerChannel(size_t nb);
	void						setInvitedUsers(const std::string& username);
	
	std::string					getName(void) const;
	std::string					getTopic(void) const;
	std::string					getPassword(void) const;
	int							getMode(void) const;
	int							getTopicMode(void) const;
	int							getPasswordMode(void) const;
	size_t						getMaxUsersPerChannel(void) const;
	std::vector<std::string>	getInvitedUsers(void) const;
	
	User*						getUserByNickname(const std::string& nickname);
	const std::vector<User*>&	getUsers(void) const;

	void						addOperator(const std::string& username);
	void						removeOperator(const std::string& username);
	bool						isOperator(std::string const & username) const;
	
	void						broadcastMessageOnChannel(const std::string& message);
	
private:

	std::string						_name;
	std::string						_topic;
	std::string						_password;
	int								_mode;
	int								_passwordMode;
	int								_topicMode;
	size_t							_maxUsersPerChannel;
	std::vector<User*>				_users;
	std::map<std::string, User*>	_userMap;
	std::vector<std::string>		_operators;
	std::vector<std::string>		_invitedUsers;
};
