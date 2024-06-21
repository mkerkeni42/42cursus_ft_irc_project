#include "User.hpp"
#include "UserServ.hpp"

User::User(void) {}

User::User(int fd) : _fd(fd), _username("*"), _joinedChanNb(0) {}

User::~User(void) {}

int	User::getFD(void) const { return (this->_fd); }

std::string	User::getPassword(void) const { return (this->_password); }

std::string	User::getUsername(void) const { return (this->_username); }

std::string	User::getNickname(void) const { return (this->_nickname); }

int	User::getMode(void) const { return (this->_mode); }

int	User::getJoinedChanNb(void) const { return (this->_joinedChanNb); }

void	User::setPassword(std::string const & password) { this->_password = password; }

void	User::setUsername(std::string const & username) { this->_username = username; }

void	User::setNickname(std::string const & nickname, UserServ & userServ) {
    if (getNickname().empty()) {
       this->_nickname = nickname;
       userServ.addUserByNickname(nickname, this);
       return;
    }
    std::string    oldNickname = this->_nickname;
    this->_nickname = nickname;
    userServ.updateUserNicknameMap(oldNickname, nickname, this);
    
}

void	User::setMode(int const & mode) { this->_mode = mode; }

void    User::incJoinedChanNb(void) { this->_joinedChanNb++; }

void    User::decJoinedChanNb(void) {
    if (this->_joinedChanNb > 0)
        this->_joinedChanNb--;
}

void		User::broadcastMessageToHimself(const std::string& message) {
    if (send(this->_fd, message.c_str(), message.size(), 0) == -1) {
        std::cerr << "ERROR: send call failed" << std::endl;
    }
}

int User::receiveData() {
    char buf[512];
    int bytesRead = recv(_fd, buf, sizeof(buf), 0);
    if (bytesRead <= 0) {
        std::cerr << "ERROR: recv call failed" << std::endl;
        return -1;
    }
    _buffer.append(buf, bytesRead);
    return 0;
}

bool User::hasBufferedCommand() const {
    return _buffer.find("\r\n") != std::string::npos;
}

std::string User::getBufferedCommand() {
    size_t pos = _buffer.find("\r\n");
    std::string command = _buffer.substr(0, pos);
    _buffer.erase(0, pos + 2);
    return command;
}
