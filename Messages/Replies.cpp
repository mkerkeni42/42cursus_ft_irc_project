#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"

std::string MessageServ::getRPL(User *user, int code, std::string message) {
    std::ostringstream  reply;

    reply << ":irc.myyamin.chat " << code << " " << user->getNickname() << " " << message << "\r\n";
    std::string response = reply.str();
    return (response);
}

std::string MessageServ::getNotif(User *user, std::string command, int prefix, std::string message) {
    std::ostringstream  reply;
	std::string			sender;

	if (prefix == CLIENT)
		sender = ":" + user->getNickname() + "!~" + user->getUsername() + "@localhost";
	else if (prefix == SERVER)
		sender = ":irc.myyamin.chat";
    reply << sender << " " << command << " " << message << "\r\n";
    std::string response = reply.str();
    return (response);
}
