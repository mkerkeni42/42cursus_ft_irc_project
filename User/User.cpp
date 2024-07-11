#include "User.hpp"
#include "UserServ.hpp"

User::User(void) {}

User::User(int fd) : _fd(fd), _registered(false), _username("*"), _nickname("*"), _joinedChanNb(0), _notified(false), _passwdStatus(false) {}

User::~User(void) {}

int	User::getFD(void) const { return (this->_fd); }

bool	User::getRegistrationStatus(void) const { return (this->_registered); }

bool	User::getPasswdStatus(void) const { return (this->_passwdStatus); }

std::string	User::getUsername(void) const { return (this->_username); }

std::string	User::getNickname(void) const { return (this->_nickname); }

int	User::getJoinedChanNb(void) const { return (this->_joinedChanNb); }

void	User::setRegistrationStatus(bool status) { this->_registered = status; }

void	User::setPasswdStatus(bool status) { this->_passwdStatus = status; }

void	User::setUsername(std::string const & username) { this->_username = username; }

void	User::setNickname(std::string const & nickname, UserServ & userServ) {
    if (getNickname() == "*") {
       this->_nickname = nickname;
       userServ.addUserByNickname(nickname, this);
       return;
    }
    std::string    oldNickname = this->_nickname;
    this->_nickname = nickname;
    this->_notified = false;
    userServ.updateUserNicknameMap(oldNickname, nickname, this);
}

void	User::setRealname(std::string const & realname) { this->_realname = realname; }

void    User::incJoinedChanNb(void) { this->_joinedChanNb++; }

void    User::decJoinedChanNb(void) {
    if (this->_joinedChanNb > 0)
        this->_joinedChanNb--;
}

bool	User::isNotified(void) const { return (_notified); }

void	User::setNotified(bool notified) { _notified = notified; }

void		User::broadcastMessageToHimself(const std::string& message) {
    std::cout << ">> " << message;
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
    return _buffer.find("\n") != std::string::npos;
}

std::string User::getBufferedCommand() {
    size_t  pos;
    int     x = 0;
    
    pos = _buffer.find("\r\n");
    if (pos != std::string::npos)
        x = 2;
    else {
        pos = _buffer.find("\n");
        if (pos != std::string::npos)
            x = 1;
        else
            return ("");
    }
    std::string command = _buffer.substr(0, pos);
    _buffer.erase(0, pos + x);
    return command;
}
