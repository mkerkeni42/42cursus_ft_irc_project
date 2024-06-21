#ifndef NETWORKSERV_HPP
#define NETWORKSERV_HPP

#include "../config.hpp"
#include "../exceptions.hpp"
#include "../User/UserServ.hpp"


class    NetworkServ {
    public:
        NetworkServ(int port, std::string& password);
        void    run();
        void    removeClient(int fd);
    private:
        void    acceptNewConnection();
        void    handleClientActivity(struct pollfd& pfd);

        int                             _serverFd;
        std::vector<struct pollfd>      _fds;
        std::map<int, struct pollfd>    _fdMap;
        UserServ                        _userServ;
};

#endif
