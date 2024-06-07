/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:06 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/07 23:21:26 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"

class User {

public:

	User(int fd);
	~User(void);

	int			getFD(void) const;
	std::string	getPassword(void) const;
	std::string	getUsername(void) const;
	std::string	getNickname(void) const;
	std::string	getRole(void) const;
	
	void		setUsername(std::string const & username);
	void		setNickname(std::string const & nickname);
	void		setRole(std::string const & role);
	void		setPassword(std::string const & password);
	
	int			receiveData();
    bool		hasBufferedCommand() const;
    std::string	getBufferedCommand();
	
	/*void		sendMessage() const;
	void		receiveMessage() const;
	void		sendPrivateMessage() const;
	void		receivePrivateMessage() const;*/
	
private:

	int			_fd;
	std::string	_password;
	std::string	_buffer;
	std::string	_username;
	std::string	_nickname;
	std::string	_role;
};