#include "NetworkServ.hpp"

NetworkServ::NetworkServ(int port, std::string& password) : _userServ(password, *this) {
    // Create the server socket
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking mode
    fcntl(_serverFd, F_SETFL, O_NONBLOCK);

    // Set up the server address structure
    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(_serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Put the socket in listening mode
    if (listen(_serverFd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Add the server socket to the pollfd structure
    struct pollfd pfd = { _serverFd, POLLIN, 0 };
    _fds.push_back(pfd);
}

void NetworkServ::run() {
    while (true) {
        // Poll the file descriptors
        int poll_count = poll(_fds.data(), _fds.size(), -1);
        if (poll_count < 0) {
            perror("poll");
            exit(EXIT_FAILURE);
        }

        // Handle events on each file descriptor
        for (size_t i = 0; i < _fds.size(); ++i) {
            if (_fds[i].revents & POLLIN) {
                if (_fds[i].fd == _serverFd) {
                    acceptNewConnection();
                } else {
                    handleClientActivity(_fds[i]);
                }
            }
        }
    }
}

void NetworkServ::acceptNewConnection() {
    // Accept the new client connection
    int clientFd = accept(_serverFd, NULL, NULL);
    if (clientFd < 0) {
        perror("accept");
        return;
    }

    // Set the client socket to non-blocking mode
    fcntl(clientFd, F_SETFL, O_NONBLOCK);

    // Add the new client socket to the pollfd structure
    struct pollfd pfd = { clientFd, POLLIN, 0 };
    _fds.push_back(pfd);
    _fdMap[clientFd] = pfd;

    // Add the user to the _UserServ
    _userServ.addUser(clientFd);
    std::ostringstream message;
    message << ":irc.myyamin.chat 001 " << _userServ.getUsername(clientFd) << " Welcome to the irc Network, " << _userServ.getUsername(clientFd) << "[!" << _userServ.getUsername(clientFd) <<  "@localhost\r\n";
    std::string response = message.str();
    int bytes = send(clientFd, response.c_str(), response.size(), 0);
    std::cout << bytes << std::endl;
}

void NetworkServ::handleClientActivity(struct pollfd &pfd) {
    if (_userServ.handleUserActivity(pfd.fd) == -1) {
        removeClient(pfd.fd);
    }
}

void NetworkServ::removeClient(int fd) {
    // Close the client socket
    close(fd);

    // Remove the client socket from the pollfd structure
    for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it) {
        if (it->fd == fd) {
            _fds.erase(it);
            break;
        }
    }

    // Remove the client socket from the map
    _fdMap.erase(fd);

    // Remove the user from the _UserServ
    //_userServ.removeUser(fd);
}
