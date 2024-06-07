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

class	MessageServ {

public:

	MessageServ(std::string const & message);
	~MessageServ(void);

private:
	std::string	_message;
	std::string	commands[12];
};