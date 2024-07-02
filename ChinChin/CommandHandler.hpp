#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include <dpp/dpp.h>

class CommandHandler {
public:
    CommandHandler(dpp::cluster& bot);

    void handleCommand(const dpp::message_create_t& event);

private:
    dpp::cluster& bot;
    const std::string REQUIRED_ROLE_NAME = "ChinChinController";
    std::string COMMAND_PREFIX = "m<";

	void prefixChange(const dpp::message_create_t& event,const std::string newPrefix);
    void help(const dpp::message_create_t& event);
    void banUser(const dpp::message_create_t& event);
    void kickUser(const dpp::message_create_t& event);
	void dmUser(const dpp::message_create_t& event);
};

#endif // COMMANDHANDLER_HPP