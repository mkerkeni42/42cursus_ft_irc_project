/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykifadji <ykifadji@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 16:12:23 by ykifadji          #+#    #+#             */
/*   Updated: 2024/06/03 16:14:33 by ykifadji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.cpp"

Channel::Channel() {}

Channel::~Channel() {}

void	Channel::addUser(User& user) {
	users.pushback(&user);
	userMap[user.getNickName()] = &user;
}
