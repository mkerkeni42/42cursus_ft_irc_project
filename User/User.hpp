/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:06 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/09 16:54:18 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"
#include "UserServ.hpp"

class	UserServ;

class User {

public:

	User(void);
	User(int fd);
	~User(void);

	int			getFD(void) const;
	std::string	getPassword(void) const;
	std::string	getUsername(void) const;
	std::string	getNickname(void) const;
	std::string	getRole(void) const;
	
	void		setUsername(std::string const & username);
	void		setNickname(std::string const & nickname, UserServ & userServ);
	void		setRole(std::string const & role);
	void		setPassword(std::string const & password);
	// just for testing
	void		setBuffer(std::string const & command) { _buffer = command; }
	
	int			receiveData();
    bool		hasBufferedCommand() const;
    std::string	getBufferedCommand();
	
private:

	int			_fd;
	std::string	_password;
	std::string	_buffer;
	std::string	_username;
	std::string	_nickname;
	std::string	_role;
};
