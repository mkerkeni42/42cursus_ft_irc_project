/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:06 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/03 14:58:59 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config.hpp"

class User {

public:

	User(void);
	User(User const &copy);
	~User(void);

	User	&operator=(User const & obj);

	std::string	getUsername(void) const;
	std::string	getNickname(void) const;
	std::string	getStatus(void) const;
	std::string	getRole(void) const;
	
	void		setUsername(std::string username);
	void		setNickname(std::string nickname);
	void		setStatus(std::string status);
	void		setRole(std::string role);
	
	void		joinChannel() const;
	void		quitChannel() const;
	void		sendMessage() const;
	void		receiveMessage() const;
	void		sendPrivateMessage() const;
	void		receivePrivateMessage() const;
	
private:
	std::string	_username;
	std::string	_nickname;
	std::string	_status;
	std::string	_role;
};
