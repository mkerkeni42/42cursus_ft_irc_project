/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykifadji <ykifadji@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:10:44 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/03 16:12:09 by ykifadji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config.hpp"

class Channel {

public:
	Channel();
	~Channel();

	void	addUser(User& user);
	void	removeUser(User& user);
	void	setTopic(const std::string& topic);
	void	setMode(const std::string& mode);
	const std::vector<User*>&	getUsers();

private:
	std::string	_name;
	std::string	_topic;
	std::string	_mode;
	std::map<std::string, User*> userMap;
	std::vector<User*> users;
};