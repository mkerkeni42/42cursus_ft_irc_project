/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:10:44 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/12 22:36:46 by mkerkeni         ###   ########.fr       */
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
	
	User*						getUserByNickname(const std::string& nickname);
	void						broadcastMessage(const std::string& message, User& sender);
	size_t						getMaxUsersPerChannel(void) const;
	int							getMode(void) const;
	std::string					getPassword(void) const;
	const std::vector<User*>&	getUsers(void) const;

private:
	std::string	_name;
	std::string	_topic;
	std::string	_password;
	int			_mode;
	size_t		_maxUsersPerChannel;
	std::map<std::string, User*> _userMap;
	std::vector<User*> _users;
};
