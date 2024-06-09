/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserServ.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:57:42 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/09 23:18:36 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"
#include "User.hpp"
#include "../Messages/MessageServ.hpp"

class	User;
class	MessageServ;

class	UserServ {

public:

	UserServ(std::string const & password);
	~UserServ(void);
	
	void	addUser(int fd);
	int		handleUserActivity(int fd);
	void	removeUser(int fd);
	User	*getUserByNickname(std::string const & nickname);
	void	updateUserNicknameMap(std::string const & oldNickname, std::string const & newNickname, User* user);
	bool 	isUserRegistered(const std::string & username);

private:
	
	std::string						_password;
	MessageServ						_messageServ;
	std::map<int, User> 			_users;
	std::map<std::string, User*>	_nicknameMap;
};
