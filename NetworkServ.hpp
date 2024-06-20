#ifndef NETWORKSERV_HPP
#define NETWORKSERV_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <poll.h>
#include "User/UserServ.hpp"

class    NetworkServ {
    public:
        NetworkServ(int port, const std::string& password);
        void    run();
    private:
        void    acceptNewConnection();
        void    handleClientActivity(struct pollfd& pfd);
        void    removeClient(int fd);

        int                                _serverFd;
        std::vector<struct pollfd>        _fds;
        std::map<int, struct pollfd>    _fdMap;
        UserServ                        _userServ;
};

#endif
