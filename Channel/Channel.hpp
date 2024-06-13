/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:10:44 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/13 14:54:41 by mkerkeni         ###   ########.fr       */
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
	void						setPassword(std::string & newPassword);
	void						setMaxUsersPerChannel(size_t nb);
	void						setName(const std::string& name);
	void						setOperator(const std::string& username);
	void						setBannedUsers(const std::string& username);
	void						setInvitedUsers(const std::string& username);
	
	User*						getUserByNickname(const std::string& nickname);
	void						broadcastMessage(const std::string& message, User& sender);
	size_t						getMaxUsersPerChannel(void) const;
	int							getMode(void) const;
	std::string					getTopic(void) const;
	std::string					getPassword(void) const;
	std::string					getOperator(void) const;
	const std::vector<User*>&	getUsers(void) const;
	std::vector<std::string>	getBannedUsers(void) const;
	std::vector<std::string>	getInvitedUsers(void) const;

private:

	std::string						_name;
	std::string						_topic;
	std::string						_operator;
	std::string						_password;
	int								_mode;
	size_t							_maxUsersPerChannel;
	std::map<std::string, User*>	_userMap;
	std::vector<User*>				_users;
	std::vector<std::string>		_bannedUsers;
	std::vector<std::string>		_invitedUsers;
};
