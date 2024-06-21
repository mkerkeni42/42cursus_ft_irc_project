/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:06 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/19 21:56:02 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"
#include "UserServ.hpp"

class	UserServ;

class	User {

public:

	User(void);
	User(int fd);
	~User(void);

	int			getFD(void) const;
	std::string	getPassword(void) const;
	std::string	getUsername(void) const;
	std::string	getNickname(void) const;
	int			getMode(void) const;
	int			getJoinedChanNb(void) const;
	
	void		setPassword(std::string const & password);
	void		setUsername(std::string const & username);
	void		setNickname(std::string const & nickname, UserServ & userServ);
	void		setMode(int const & mode);
	
	void		incJoinedChanNb(void);
	void		decJoinedChanNb(void);
	
	int			receiveData();
    bool		hasBufferedCommand() const;
    std::string	getBufferedCommand();
	void		broadcastMessageToHimself(const std::string& message);
	
private:

	int			_fd;
	std::string	_password;
	std::string	_buffer;
	std::string	_username;
	std::string	_nickname;
	int			_mode;
	int			_joinedChanNb;
};
