/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserServ.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:57:42 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/03 11:07:59 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config.hpp"

class UserServ {

public:

	UserServ(void);
	UserServ(UserServ const & copy);
	~UserServ(void);

	UserServ	&operator=(UserServ const & obj);

private:
	//container with all users
};