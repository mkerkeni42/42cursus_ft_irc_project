#include "Channel.hpp"
#include "../User/User.hpp"

Channel::Channel(std::string const & name) : _name(name), _mode(PUBLIC), _passwordMode(DISABLED), _topicMode(PUBLIC), _maxUsersPerChannel(0) {}

Channel::~Channel() {}

void	Channel::addUser(User *user) {
	_users.push_back(user);
	_nicknameMap[user->getNickname()] = user;
}

void	Channel::removeUser(User *user) {
	_users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
	_nicknameMap.erase(user->getNickname());
}

void	Channel::setTopic(const std::string& newTopic) { _topic = newTopic; }

void	Channel::setPassword(std::string& newPassword) { _password = newPassword; }

void	Channel::setMode(const int& newMode) { _mode = newMode; }

void	Channel::setTopicMode(const int& newMode) { _topicMode = newMode; }

void	Channel::setPasswordMode(const int& newMode) { _passwordMode = newMode; }

void	Channel::setMaxUsersPerChannel(size_t nb) { _maxUsersPerChannel = nb; }

void	Channel::updateNicknameMap(User* user, const std::string& newNickname) {
	std::map<std::string, User*>::iterator	it = _nicknameMap.find(user->getNickname());
	if (it != _nicknameMap.end())
		_nicknameMap.erase(it);
	_nicknameMap[newNickname] = user;
	if (isOperator(user->getNickname()) == true) {
		removeOperator(user->getNickname());
		addOperator(newNickname);
	}
}

std::string	Channel::getName(void) const { return (_name); }

std::string	Channel::getTopic(void) const { return (_topic); }

std::string	Channel::getPassword(void) const { return (_password); }

int	Channel::getMode(void) const { return (_mode); }

int	Channel::getTopicMode(void) const { return (_topicMode); }

int	Channel::getPasswordMode(void) const { return (_passwordMode); }

size_t	Channel::getMaxUsersPerChannel(void) const { return (_maxUsersPerChannel); }

std::vector<std::string>	Channel::getInvitedUsers(void) const { return (_invitedUsers); }	

User*	Channel::getUserByNickname(const std::string& nickname) {
	std::map<std::string, User*>::iterator	it = _nicknameMap.find(nickname);
	if (it != _nicknameMap.end())
		return it->second;
	return NULL;
}

std::vector<User*>&	Channel::getUsers() { return _users; }

bool	Channel::isUserOnChannel(const std::string& nickname) {
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

bool	Channel::isOperator(const std::string& nickname) const {
	std::vector<std::string>::const_iterator it = std::find(_operators.begin(), _operators.end(), nickname);
	if (it != _operators.end())
		return (true);
	return (false);
}

void	Channel::addInvitedUser(const std::string& nickname) { _invitedUsers.push_back(nickname); }

void	Channel::removeInvitedUser(const std::string& nickname) {
	std::vector<std::string>::iterator it = std::find(_invitedUsers.begin(), _invitedUsers.end(), nickname);
	if (it != _invitedUsers.end())
		_invitedUsers.erase(it);
}

bool	Channel::isInvited(const std::string& nickname) const {
	std::vector<std::string>::const_iterator it = std::find(_invitedUsers.begin(), _invitedUsers.end(), nickname);
	if (it != _invitedUsers.end())
		return (true);
	return (false);
}

void	Channel::addMode(char newMode) {
	if (_modes.empty() || _modes[0] != '+')
		_modes.insert(_modes.begin(), '+');
		
	std::vector<char>::iterator	it = std::lower_bound(_modes.begin(), _modes.end(), newMode);
	if (it != _modes.end() && *it == newMode)
		return;
	_modes.insert(it, newMode);
}

void	Channel::deleteMode(char mode) {
	std::vector<char>::iterator	it = std::find(_modes.begin(), _modes.end(), mode);
	if (it != _modes.end())
		_modes.erase(it);
	if (_modes.size() == 1 && _modes[0] == '+')
		_modes.clear();
}

std::string	Channel::getModes(void) const { return (std::string(_modes.begin(), _modes.end())); }

void	Channel::broadcastMessageOnChannel(const std::string& message, User *sender, int x) {
	for (std::vector<User*>::iterator	it = _users.begin(); it != _users.end(); ++it) {
		User*	user = *it;
		if (user->getNickname() != sender->getNickname() && !user->isNotified()) {
			std::cout << ">> " << message;
			if (send(user->getFD(), message.c_str(), message.size(), 0) == -1)
				std::cerr << "ERROR: send call failed" << std::endl;
			if (x == 1)
				user->setNotified(true);
		}
	}
}

std::string	Channel::getNicknamesList(void) {
	std::string	names;
	std::map<std::string, User*>::iterator	it;
	for (it = _nicknameMap.begin(); it !=_nicknameMap.end(); ++it) {
		if (!names.empty())
			names += " ";
		if (this->isOperator(it->first))
            names += "@";
		names += it->first;
	}
	return names;
}
