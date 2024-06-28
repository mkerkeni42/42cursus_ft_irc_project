#include "Channel.hpp"
#include "../User/User.hpp"

Channel::Channel() : _mode(PUBLIC), _passwordMode(DISABLED), _topicMode(PUBLIC), _maxUsersPerChannel(0) {}

Channel::~Channel() {}

void	Channel::addUser(User & user) {
	_users.push_back(&user);
	_nicknameMap[user.getNickname()] = &user;
}

void	Channel::removeUser(User& user) {
	_users.erase(std::remove(_users.begin(), _users.end(), &user), _users.end());
	_nicknameMap.erase(user.getNickname());
}

void	Channel::setName(const std::string& name) { _name = name; }

void	Channel::setTopic(const std::string& newTopic) { _topic = newTopic; }

void	Channel::setPassword(std::string & newPassword) { _password = newPassword; }

void	Channel::setMode(const int & newMode) { _mode = newMode; }

void	Channel::setTopicMode(const int & newMode) { _topicMode = newMode; }

void	Channel::setPasswordMode(const int & newMode) { _passwordMode = newMode; }

void	Channel::setMaxUsersPerChannel(size_t nb) { _maxUsersPerChannel = nb; }

void	Channel::setInvitedUsers(const std::string& username) { _invitedUsers.push_back(username); }

std::string	Channel::getName(void) const { return (_name); }

std::string	Channel::getTopic(void) const { return (_topic); }

std::string	Channel::getPassword(void) const { return (_password); }

int	Channel::getMode(void) const { return (_mode); }

int	Channel::getTopicMode(void) const { return (_topicMode); }

int	Channel::getPasswordMode(void) const { return (_passwordMode); }

size_t	Channel::getMaxUsersPerChannel(void) const { return (_maxUsersPerChannel); }

std::vector<std::string>	Channel::getInvitedUsers(void) const { return (_invitedUsers); }	

const std::map<std::string, User*> &	Channel::getNicknameMap(void) const { return (_nicknameMap); }

User*	Channel::getUserByNickname(const std::string& nickname) {
	std::map<std::string, User*>::iterator	it = _nicknameMap.find(nickname);
	if (it != _nicknameMap.end())
		return it->second;
	return NULL;
}

const std::vector<User*>&	Channel::getUsers() const { return _users; }

bool	Channel::isUserOnChannel(const std::string &nickname) {
	std::map<std::string, User*>::iterator	it = _nicknameMap.find(nickname);
	if (it != _nicknameMap.end())
		return true;
	return false; 
}

void	Channel::addOperator(const std::string& nickname) { _operators.push_back(nickname); }

void	Channel::removeOperator(const std::string& nickname) {
	std::vector<std::string>::iterator it = std::find(_operators.begin(), _operators.end(), nickname);
	if (it != _operators.end())
		_operators.erase(it);
}

bool	Channel::isOperator(std::string const & nickname) const {
	std::vector<std::string>::const_iterator it = std::find(_operators.begin(), _operators.end(), nickname);
	if (it != _operators.end())
		return (true);
	return (false);
}

void	Channel::broadcastMessageOnChannel(const std::string& message, User &sender) {
	for (std::vector<User*>::iterator	it = _users.begin(); it != _users.end(); ++it) {
		User*	user = *it;
		if (user->getNickname() != sender.getNickname()) {
			if (send(user->getFD(), message.c_str(), message.size(), 0))
				std::cerr << "ERROR: send call failed" << std::endl;
		}
	}
}

void	Channel::updateNicknameMap(User *user, std::string & oldNickname) {
	std::cout << 
	_nicknameMap.erase(oldNickname);
	_nicknameMap[user->getNickname()] = user;
}
