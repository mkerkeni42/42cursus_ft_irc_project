#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"

/*std::string getResponse(User &user, int code, std::string message) {
    std::ostringstream  reply;

    reply << ":irc.myyamin.chat " << code << " " << user.getNickname() << " :" << message << "\r\n";
    std::string response = 
}*/

void MessageServ::sendWelcomeMessages(User &user) {
	std::ostringstream	welcome;
	welcome << ":irc.myyamin.chat " << RPL_WELCOME << " " << user.getNickname() << " :Welcome to the IRC network\r\n";
    std::string response = welcome.str();
	user.broadcastMessageToHimself(response.c_str());

    std::ostringstream	yourHost;
	yourHost << ":irc.myyamin.chat "<< RPL_YOURHOST << " " << user.getNickname() << " :Your host is irc.myyamin.chat, running version 1.0\r\n";
	response = yourHost.str();
    user.broadcastMessageToHimself(response.c_str());

    std::ostringstream	created;
	created << ":irc.myyamin.chat " << RPL_CREATED << " "  << user.getNickname() << " :This server was created today\r\n";
	response = created.str();
    user.broadcastMessageToHimself(response.c_str());

    std::ostringstream	myInfo;
	myInfo << ":irc.myyamin.chat " << RPL_MYINFO << " " << user.getNickname() << " :irc.myyamin.chat 1.0 o o\r\n";
	response = myInfo.str();
    user.broadcastMessageToHimself(response.c_str());
    sendMotd(user);
}

void MessageServ::sendMotd(User &user) {
    std::ostringstream  motdStart;
    motdStart << ":irc.myyamin.chat " << RPL_MOTDSTART << " " << user.getNickname() << " :- irc.myyamin.chat Message of the Day - \r\n";
    std::string response = motdStart.str();
    user.broadcastMessageToHimself(response.c_str());

    std::ostringstream  motd;
    motd << ":irc.myyamin.chat " << RPL_MOTD << " " << user.getNickname() << " :-:\033[1;33m Welcome to Myyamin Chat, the best IRC server ! \
                                                                        \033[96m...............................\033[1;33m:+-\033[96m...........\033[1;33m++\033[96m........................... \
                                                                        \033[96m...............................\033[1;33m-++\033[96m.........\033[1;33m-++-\033[96m........................... \
                                                                        \033[96m.....................\033[1;33m::\033[96m.........\033[1;33m:+:\033[96m.......\033[1;33m-++\033[96m..........\033[1;33m--\033[96m................. \
                                                                        \033[96m......................\033[1;33m:+:\033[96m........\033[1;33m::-\033[96m......\033[1;33m:+-\033[96m.......\033[1;33m-:+-\033[96m.................. \
                                                                        \033[96m........................\033[1;33m-++:\033[96m....\033[1;33m:+++++++++:\033[96m.......\033[1;33m-++-\033[96m.................... \
                                                                        \033[96m..........\033[1;33m::--\033[96m............\033[1;33m-:-:+++++++++++++++:\033[96m..\033[1;33m-:+-\033[96m...................... \
                                                                        \033[96m..............\033[1;33m-:++:-\033[96m.......\033[1;33m:+++++++++++++++++++:-:\033[96m.............\033[1;33m:++:\033[96m....... \
                                                                        \033[96m.....\033[37m*****\033[96m........\033[1;33m-:++:-.-+++++++++++++++++++++++-\033[96m........\033[1;33m-::+:\033[96m........... \
                                                                        \033[96m...\033[37m********\033[96m..........\033[1;33m-::-+++++++++++++++++++++++++-.-+++++:-\033[96m.............. \
                                                                        \033[96m....\033[37m********\033[96m...........\033[1;33m-+++++++++++++++++++++++++++---\033[96m.................... \
                                                                        \033[96m.......................\033[1;33m:+++++++++++++++++++++++++++:\033[96m..........\033[37m******\033[96m...... \
                                                                        \033[96m.......................\033[1;33m-+++++++++++++++++++++++++++-\033[96m.........\033[37m********\033[96m..... \
                                                                        \033[96m................\033[1;33m--:+++:.:+++++++++++++++++++++++++:.--\033[96m.........\033[37m****\033[96m....... \
                                                                        \033[96m.............\033[1;33m:++::--\033[96m.....\033[1;33m:+++++++++++++++++++++++:.-:+++-\033[96m................. \
                                                                        \033[96m........\033[1;33m-::+-\033[96m.............\033[1;33m-+++++++++++++++++++++-\033[96m......\033[1;33m-:++:-\033[96m............. \
                                                                        \033[96m......................\033[1;33m:+:\033[96m......\033[1;33m-+++++++++++:\033[96m..\033[1;33m:++-\033[96m...............\033[1;33m-\033[96m........ \
                                                                        \033[96m....................\033[1;33m:+:\033[96m........\033[1;33m:\033[96m....\033[1;33m-:--\033[96m........\033[1;33m-++:\033[96m...................... \
                                                                        \033[96m..................\033[1;33m-:\033[96m..........\033[1;33m:+:\033[96m.......\033[1;33m++-\033[96m........\033[1;33m:+\033[96m..................... \
                                                                        \033[96m.............................\033[1;33m:+:\033[96m........\033[1;33m-+:\033[96m.........\033[1;33m-+-\033[96m................... \
                                                                        \033[96m............................\033[1;33m:+-\033[96m..........\033[1;33m:+-\033[96m.............................. \
                                                                        \033[96m...........................\033[1;33m-+-\033[96m............\033[1;33m+:\033[96m..............................\033[0m\r\n";
    response = motd.str();
    user.broadcastMessageToHimself(response.c_str());

    std::ostringstream motdEnd;
    motdEnd << ":irc.myyamin.chat " << RPL_ENDOFMOTD << user.getNickname() << " :- End of /MOTD command.\r\n";
    response = motdEnd.str();
    user.broadcastMessageToHimself(response.c_str());
}
