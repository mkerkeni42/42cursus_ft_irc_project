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
	
	void						updateNicknameMap(User *user, std::string &oldNickname);
	
	std::string					getName(void) const;
	std::string					getTopic(void) const;
	std::string					getPassword(void) const;
	int							getMode(void) const;
	int							getTopicMode(void) const;
	int							getPasswordMode(void) const;
	size_t						getMaxUsersPerChannel(void) const;
	std::vector<std::string>	getInvitedUsers(void) const;
	const std::map<std::string, User*> &	getNicknameMap(void) const;
	
	User*						getUserByNickname(const std::string& nickname);
	const std::vector<User*>&	getUsers(void) const;
	bool						isUserOnChannel(const std::string &nickname);

	void						addOperator(const std::string& nickname);
	void						removeOperator(const std::string& nickname);
	bool						isOperator(std::string const & nickname) const;
	
	void						broadcastMessageOnChannel(const std::string& message, User &sender);
	
private:

	std::string						_name;
	std::string						_topic;
	std::string						_password;
	int								_mode;
	int								_passwordMode;
	int								_topicMode;
	size_t							_maxUsersPerChannel;
	std::vector<User*>				_users;
	std::map<std::string, User*>	_nicknameMap;
	std::vector<std::string>		_operators;
	std::vector<std::string>		_invitedUsers;
};
