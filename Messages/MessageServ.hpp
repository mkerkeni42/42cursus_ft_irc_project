#pragma once

#include "../config.hpp"
#include "../exceptions.hpp"
#include "../Channel/Channel.hpp"

class   UserServ;
class   User;
class   ChannelServ;

class	MessageServ {

public:

	MessageServ(UserServ & userServ, ChannelServ & channelServ);
	~MessageServ(void);

	typedef bool (MessageServ::*CommandHandler)(std::string & command, User & user);

    void	    handleCommand(std::string & command, User& user);
    void	    getList(std::string const &	arg, std::vector<std::string> &list, int x, User& user);
    void	    resetUsersNotif(std::map<std::string, Channel>& channels);
    bool	    isValidNickname(std::string nickname);
    //void        printMap(std::map<std::string, User*> map);

    std::string getRPL(User &user, int code, std::string message);
    std::string getNotif(User &user, std::string command, int prefix, std::string message);

private:

	std::map<std::string, CommandHandler>	_commandMap;
    UserServ&                               _userServ;
    ChannelServ&                            _channelServ;

	bool	handleUserCommand(std::string & command, User & user);
    bool	handleNickCommand(std::string & command, User & user);
    bool	handlePassCommand(std::string & command, User & user);
    bool	handleQuitCommand(std::string & command, User & user);
    bool	handleJoinCommand(std::string & command, User & user);
    bool	handlePartCommand(std::string & command, User & user);
    bool	handleInviteCommand(std::string & command, User & user);
    bool	handleKickCommand(std::string & command, User & user);
    bool	handleTopicCommand(std::string & command, User & user);
    bool    handleModeCommand(std::string & command, User & user);
    int     handleSetMode(Channel *channel, char const & mode, std::istringstream &arg, User &user);
    void    handleRemoveMode(Channel *channel, char const & mode, std::istringstream &arg, User &user);
    bool	handlePrivmsgCommand(std::string & command, User & user);
    bool    handleCapCommand(std::string & command, User & user);
    bool	handlePingCommand(std::string & command, User & user);
    void    sendMotd(User &user);
    void    sendWelcomeMessages(User &user);
};
