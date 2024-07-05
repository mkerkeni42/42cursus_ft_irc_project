#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

bool	MessageServ::isValidNickname(std::string nickname) {
	if (nickname.length() > 9 || nickname[0] == '#' || nickname[0] == ':')
		return false;
	for (size_t i = 0; i < nickname.length(); i++) {
		if (!std::isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '-' && nickname[i] != '[' \
			&& nickname[i] != ']' && nickname[i] != '(' && nickname[i] != ')' && nickname[i] != '\\' && nickname[i] != '|' )
				return false;
	}
	return true;
}

void	MessageServ::getList(std::string const &arg, std::vector<std::string> &list, int x, User& user) {
	if (arg.find(",") != std::string::npos) {
		std::stringstream ss(arg);
    	std::string item;
    	while (std::getline(ss, item, ',')) {
			if (!item.empty() && item[0] == '#')
            	item.erase(0, 1);
			else if (x == 1)
				throw (NoSuchChannelException(user.getNickname(), item));
			list.push_back(item);
   		}
	}
	else {
		if (!arg.empty() && arg[0] == '#')
			list.push_back(arg.substr(1));
		else if (x == 0)
			list.push_back(arg);
		else
			throw (NoSuchChannelException(user.getNickname(), arg));
	}
}

void	MessageServ::resetUsersNotif(std::map<std::string, Channel>& channels) {
	std::map<std::string, Channel>::iterator	it;
	for (it = channels.begin(); it != channels.end(); ++it) {
		Channel	&channel = it->second;
		std::vector<User*>	&users = channel.getUsers();
		for (std::vector<User*>::iterator	it = users.begin(); it != users.end(); ++it) {
			User*	user = *it;
			user->setNotified(false);
		}
	}	
}

/*void	MessageServ::printMap(std::map<std::string, User*> map) {
	std::map<std::string, User*>::iterator	it;
	for (it = map.begin(); it != map.end(); ++it)
        std::cout << it->first << std::endl;
}*/
