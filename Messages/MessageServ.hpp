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

	typedef void (MessageServ::*CommandHandler)(std::string & command, User & user);

    void	handleCommand(std::string & command, User& user);
    void	getList(std::string const &	arg, std::vector<std::string> &list, int x, User& user);
    void	resetUsersNotif(std::map<std::string, Channel>& channels);

private:

	std::map<std::string, CommandHandler>	_commandMap;
    UserServ&                               _userServ;
    ChannelServ&                            _channelServ;

	void	handleUserCommand(std::string & command, User & user);
    void	handleNickCommand(std::string & command, User & user);
    void	handlePassCommand(std::string & command, User & user);
    void	handleQuitCommand(std::string & command, User & user);
    void	handleJoinCommand(std::string & command, User & user);
    void	handlePartCommand(std::string & command, User & user);
    void	handleInviteCommand(std::string & command, User & user);
    void	handleKickCommand(std::string & command, User & user);
    void	handleTopicCommand(std::string & command, User & user);
    void    handleModeCommand(std::string & command, User & user);
    int     handleSetMode(Channel *channel, char const & mode, std::istringstream &arg, User &user);
    void    handleRemoveMode(Channel *channel, char const & mode, std::istringstream &arg, User &user);
    void	handlePrivmsgCommand(std::string & command, User & user);
    void    handleCapCommand(std::string & command, User & user);
    void	handlePingCommand(std::string & command, User & user);
    void    sendMotd(User &user);
    void    sendWelcomeMessages(User &user);
};
