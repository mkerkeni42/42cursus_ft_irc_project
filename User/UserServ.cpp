#include "UserServ.hpp"
#include "User.hpp"
#include "../Network/NetworkServ.hpp"

UserServ::UserServ(std::string & password, NetworkServ &networkServ) : _password(password), _networkServ(networkServ), _messageServ(*this, _channelServ), _channelServ() {}

UserServ::~UserServ(void) {}

void	UserServ::addUser(int fd) {
	User	newUser(fd);
	this->_users[fd] = newUser;
	newUser.setUsername("default");
}

void	UserServ::addUserByNickname(std::string const & nickname, User* user) {
	_nicknameMap[nickname] = user;
}

void	UserServ::removeUser(int fd) {
	User	&user = this->_users[fd];
	if (_nicknameMap.find(user.getNickname()) != _nicknameMap.end())
		_nicknameMap.erase(user.getNickname());
	if (_users.find(fd) != _users.end())
		this->_users.erase(fd);
}

int		UserServ::handleUserActivity(int fd) {
	User	&user = _users[fd];
	if (user.receiveData() == -1)
		return (-1);
	while (user.hasBufferedCommand()) {
		std::string	command = user.getBufferedCommand();
		_messageServ.handleCommand(command, user);
	}
	return (0);
}

void	UserServ::broadcastPrivateMessage(const std::string& message, std::string& recipient) {
	User	*user = getUserByNickname(recipient);
	if (send(user->getFD(), message.c_str(), message.size(), 0) == -1)
		std::cerr << "ERROR: send call failed" << std::endl;
}

User	*UserServ::getUserByNickname(std::string const & nickname) {
	std::map<std::string, User*>::iterator it = _nicknameMap.find(nickname);
    if (it != _nicknameMap.end()) {
        return it->second;
    }
    return (NULL);
}

std::string	UserServ::getUsername(int fd) {
	std::map<int, User>::iterator it = _users.find(fd);
    if (it != _users.end()) {
        User	user = it->second;
		return (user.getUsername());
    }
    return (NULL);
}

void	UserServ::removeUserfromNetwork(int fd) {
	_networkServ.removeClient(fd);
}

void	UserServ::updateUserNicknameMap(std::string const & oldNickname, std::string const & newNickname, User* user) {
	_nicknameMap.erase(oldNickname);
	_nicknameMap[newNickname] = user;
}


bool UserServ::isUserRegistered(const std::string & username) {
	std::map<int, User>::const_iterator it;
	for ( it = _users.begin(); it != _users.end(); ++it) {
    	if (it->second.getUsername() == username)
        	return true;
    }
    return false;
}

bool 	UserServ::isNicknameInUse(const std::string & nickname) {
	std::map<std::string, User*>::iterator it;
	for ( it = _nicknameMap.begin(); it != _nicknameMap.end(); ++it) {
    	if (it->first == nickname)
        	return true;
    }
    return false;
}
