/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:10:44 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/14 11:24:19 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	void						setTopic(const std::string& newTopic);
	void						setMode(const int & newMode);
	void						setPasswordMode(const int & newMode);
	void						setTopicMode(const int & newMode);
	void						setPassword(std::string & newPassword);
	void						setMaxUsersPerChannel(size_t nb);
	void						setName(const std::string& name);
	void						setBannedUsers(const std::string& username);
	void						setInvitedUsers(const std::string& username);
	
	User*						getUserByNickname(const std::string& nickname);
	size_t						getMaxUsersPerChannel(void) const;
	int							getMode(void) const;
	int							getPasswordMode(void) const;
	int							getTopicMode(void) const;
	std::string					getName(void) const;
	std::string					getTopic(void) const;
	std::string					getPassword(void) const;
	const std::vector<User*>&	getUsers(void) const;
	std::vector<std::string>	getBannedUsers(void) const;
	std::vector<std::string>	getInvitedUsers(void) const;

	void						addOperator(const std::string& username);
	void						removeOperator(const std::string& username);
	bool						isOperator(std::string const & username) const;
	
	void						broadcastMessage(const std::string& message, User& sender);
	
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
	std::vector<std::string>		_bannedUsers;
	std::vector<std::string>		_invitedUsers;
};
