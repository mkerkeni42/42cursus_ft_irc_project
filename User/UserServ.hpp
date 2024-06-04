/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserServ.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:57:42 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/04 15:39:18 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"
#include "User.hpp"
//#include "MessageServ.hpp"
#include <map>
#include <cstring>

class UserServ {

public:

	UserServ(std::string const & password);
	~UserServ(void);
	
	void	addUser(int fd);
	int		handleUserActivity(int fd);
	void	removeUser(int fd);
	User	*getUserByNickname(std::string const & nickname);

private:
	
	std::map<int, User> 			_users;
	std::map<std::string, User *>	_nicknameMap;
	std::string						_password;
	//MessageServ						_messageServ;
};