/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserServ.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkerkeni <mkerkeni@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 10:58:31 by mkerkeni          #+#    #+#             */
/*   Updated: 2024/06/03 11:05:25 by mkerkeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserServ.hpp"

UserServ::UserServ(void) {}

UserServ::UserServ(UserServ const & copy) {
	if (this !=&copy)
		*this = copy;
}

UserServ::~UserServ(void) {}

UserServ	&UserServ::operator=(UserServ const & obj) {
	return (*this);
}
