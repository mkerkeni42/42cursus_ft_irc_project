#include "NetworkServ.hpp"

NetworkServ::NetworkServ(int port, std::string& password) : _userServ(password, *this) {
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	int	opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	fcntl(_serverFd, F_SETFL, O_NONBLOCK);

	struct sockaddr_in	serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);

	if (bind(_serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if (listen(_serverFd, 10) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	struct pollfd	pfd = { _serverFd, POLLIN, 0 };
	_fds.push_back(pfd);
}

void	NetworkServ::run() {
	while (true) {
		int	poll_count = poll(_fds.data(), _fds.size(), -1);
		if (poll_count < 0) {
			perror("poll");
			exit(EXIT_FAILURE);
		}
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

void	NetworkServ::acceptNewConnection() {
	int	clientFd = accept(_serverFd, NULL, NULL);
	if (clientFd < 0) {
		perror("accept");
		return;
	}
	fcntl(clientFd, F_SETFL, O_NONBLOCK);
	struct pollfd	pfd = { clientFd, POLLIN, 0 };
	_fds.push_back(pfd);
	_userServ.addUser(clientFd);
}

void	NetworkServ::handleClientActivity(struct pollfd &pfd) {
	if (_userServ.handleUserActivity(pfd.fd) == -1) {
		removeClient(pfd.fd);
	}
}

void	NetworkServ::removeClient(int fd) {
	close(fd);

	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it) {
		if (it->fd == fd) {
			_fds.erase(it);
			break;
		}
	}
}

NetworkServ::~NetworkServ() {
	for (size_t i = 0; i < _fds.size(); ++i) {
		close(_fds[i].fd);
	}
}

void	NetworkServ::shutdown() {
	for (size_t i = 0; i < _fds.size(); ++i) {
		if (_fds[i].fd != _serverFd) {
			close(_fds[i].fd);
		}
	}
	close(_serverFd);
	_fds.clear();
}


