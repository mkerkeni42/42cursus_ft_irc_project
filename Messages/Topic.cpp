#include "MessageServ.hpp"
#include "../User/UserServ.hpp"
#include "../User/User.hpp"
#include "../Channel/ChannelServ.hpp"

bool	MessageServ::handleTopicCommand(std::string & command, User *user) {
	std::istringstream iss(command);
    std::string cmd, channel, topic;
    iss >> cmd >> channel;
	std::getline(iss >> std::ws, topic);

	if (channel.empty() || channel[0] != '#')
		throw NeedMoreParamsException(user->getNickname(), cmd);
	channel = channel.substr(1);
	if (!_channelServ.DoesChannelExist(channel))
		throw NoSuchChannelException(user->getNickname(), "#" + channel);
	if (!_channelServ.isUserOnChannel(channel, user))
		throw NotOnChannelException(user->getNickname(), "#" + channel);

	Channel	*channelObj = _channelServ.getChannel(channel);
	if (channelObj->getTopicMode() == CHANOP_ONLY && !channelObj->isOperator(user->getNickname()))
		throw ChanOPrivsNeededException(user->getNickname(), "#" + channel);
	if (topic.empty()) {
		user->broadcastMessageToHimself(getRPL(user, RPL_TOPIC, "#" + channel + " :" + channelObj->getTopic()));
		channelObj->broadcastMessageOnChannel(getRPL(user, RPL_TOPIC, "#" + channel + " :" + channelObj->getTopic()), user, 0);
	}
	else if (topic == "::")
		channelObj->broadcastMessageOnChannel(getNotif(user, cmd, CLIENT, "#" + channel + ":"), user, 0);
	else {
		channelObj->setTopic(topic);
		user->broadcastMessageToHimself(getNotif(user, cmd, CLIENT, "#" + channel + " " + topic));
		channelObj->broadcastMessageOnChannel(getNotif(user, cmd, CLIENT, "#" + channel + " " + topic), user, 0);
	}
	
	return true;
}
