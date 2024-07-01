#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

void	MessageServ::handleTopicCommand(std::string & command, User & user) {
	std::cout << "Handling TOPIC command" << std::endl;
	std::istringstream iss(command);
    std::string cmd, channel, topic;

    iss >> cmd >> channel >> std::ws;
	std::getline(iss, topic);
	if (channel.empty() || channel[0] != '#')
		throw (NeedMoreParamsException(user.getNickname(), cmd));
	channel = channel.substr(1);
	if (_channelServ.DoesChannelExist(channel) == false)
		throw (NoSuchChannelException(user.getNickname(), channel));
	if (_channelServ.isUserOnChannel(channel, user) == false)
		throw (NotOnChannelException(user.getNickname(), channel));
	Channel	*channelObj = _channelServ.getChannel(channel);
	if (channelObj->getTopicMode() == CHANOP_ONLY && channelObj->isOperator(user.getUsername()) == false)
		throw (ChanOPrivsNeededException(user.getNickname(), channel));
	if (topic.empty()) {
		std::ostringstream	topicMsg;
		topicMsg << ":irc.myyamin.chat " << RPL_TOPIC << " " << user.getNickname() << " #" << channel << " :" << _channelServ.getChannel(channel)->getTopic() << "\r\n";
		std::string	response = topicMsg.str();
		user.broadcastMessageToHimself(response);
		_channelServ.getChannel(channel)->broadcastMessageOnChannel(response, user);
	}
	else if (topic == "::") {
		std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost TOPIC #" + channel + " :\r\n";
		_channelServ.getChannel(channel)->broadcastMessageOnChannel(response, user);
	}
	else {
		channelObj->setTopic(topic);
		std::string response = ":" + user.getNickname() + "!" + user.getUsername() + "@localhost TOPIC #" + channel + " " + topic + "\r\n";
		user.broadcastMessageToHimself(response);
		_channelServ.getChannel(channel)->broadcastMessageOnChannel(response, user);
	}
}
