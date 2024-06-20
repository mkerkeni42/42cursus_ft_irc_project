/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 14:07:44 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/05/31 14:07:44 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <exception>
#include <cctype>
#include <cstdlib>
#include <unistd.h>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <algorithm>
#include <string>
#include <cstdio>
#include <fcntl.h>
#include <poll.h>

#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[1;35m"
#define ORANGE "\033[1;38;5;208m"
#define LIGHT_CYAN "\033[1;36m"
#define END "\033[0m"

#define ERR_NOSUCHNICKNAME 401
#define ERR_NOSUCHCHANNEL 403
#define ERR_TOOMANYCHANNELS 405
#define ERR_NOORIGIN 409 //PING
#define ERR_NORECIPIENT 411 // PONG
#define ERR_NOTEXTTOSEND 412
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL 442
#define ERR_USERONCHANNEL 443
#define ERR_NOTREGISTERED 451
#define ERR_ALREADYREGISTERED 462
#define ERR_NEEDMOREPARAMS 461
#define ERR_PASSWDMISMATCH 464
#define ERR_KEYSET 467
#define ERR_CHANNELISFULL 471
#define ERR_UNKNOWNMODE 472
#define ERR_INVITEONLYCHAN 473
#define ERR_BADCHANNELKEY 475
#define ERR_CHANOPRIVSNEEDED 471

#define MAX_CHANNELS_PER_USER 10

#define	PUBLIC 0
#define INVITE_ONLY 1
#define PROTECTED 2

#define CHANOP_ONLY 3

#define DISABLED 0
#define ENABLED 1
