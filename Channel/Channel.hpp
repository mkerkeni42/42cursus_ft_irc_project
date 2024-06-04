/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykifadji <ykifadji@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 15:10:44 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/04 14:52:22 by ykifadji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"

class Channel {

public:
	Channel();
	~Channel();

	void	addUser(User& user);
	void	removeUser(User& user);

	void	setTopic(const std::string& newTopic);
	void	setMode(const std::string& newMode);

	User*	getUserByNickname(const std::string& nickname);
	void	broadcastMessage(const std::string& message, User& sender);
	const std::vector<User*>&	getUsers() const;

private:
	std::string	_name;
	std::string	_topic;
	std::string	_mode;
	std::map<std::string, User*> _userMap;
	std::vector<User*> _users;
};
