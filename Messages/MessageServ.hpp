/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageServ.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 14:04:42 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/07 14:04:42 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"

class   UserServ;
class   User;

class	MessageServ {

public:

	MessageServ(UserServ & userServ);
	~MessageServ(void);

	typedef void (MessageServ::*CommandHandler)(std::string & command, User & user);

    void	handleCommand(std::string & command, User& user);

private:

	std::map<std::string, CommandHandler>	_commandMap;     
    UserServ&                               _userServ;
    //ChannelServ&                            _channelServ;

	void	handleUserCommand(std::string & command, User& user);
    void	handleNickCommand(std::string & command, User& user);
    void	handlePassCommand(std::string & command, User& user);
    /*void	handleQuitCommand(std::string & command, User& user);
    void	handleJoinCommand(std::string & command, User& user);
    void	handlePartCommand(std::string & command, User& user);
    void	handleInviteCommand(std::string & command, User& user);
    void	handleKickCommand(std::string & command, User& user);
    void	handleModeCommand(std::string & command, User& user);
    void	handlePrivmsgCommand(std::string & command, User& user);
    void	handlePingCommand(std::string & command, User& user);
    void	handleCapCommand(std::string & command, User& user);*/

};

