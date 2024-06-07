/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageServ.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 14:05:12 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/07 14:05:12 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageServ.hpp"

std::string	get_cmd(std::string message) {
	size_t	pos = message.find(" ");
	if (pos == std::string::npos)
		return (message);
	return(message.substr(0, pos));
}

MessageServ::MessageServ(std::string const & message) : _message(message) {
	_commands = {"USER", "NICK", "PASS", "QUIT", "JOIN", "PART", "INVITE", "KICK", "MODE", "PRIVMSG", "PING", "CAP"};
	std::string	cmd = get_cmd(message);
	switch(cmd) {
		case "USER":
			//traiter la commande USER
			break;
		case "NICK":
			//traiter la commande NICK
			break;
		case "PASS":
			//traiter la commande PASS
			break;
		case "QUIT":
			//traiter la commande QUIT
			break;
		case "JOIN":
			//traiter la commande JOIN
			break;
		case "PART":
			//traiter la commande PART
			break;
		case "INVITE":
			//traiter la commande INVITE
			break;
		case "KICK":
			//traiter la commande KICK
			break;
		case "MODE":
			//traiter la commande MODE
			break;
		case "PRIVMSG":
			//traiter la commande PRIVMSG
			break;
		case "PING":
			//traiter la commande PING
			break;
		case "CAP":
			//traiter la commande CAP
			break;
		default:
			throw (UnknownCommandException(cmd));
			break;
	}
}

MessageServ::~MessageServ(void) {}