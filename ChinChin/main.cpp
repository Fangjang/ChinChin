#include <dpp/dpp.h>
#include "CommandHandler.hpp"

const std::string BOT_TOKEN = "MTAwNjIyOTg0MjMwMTMwOTA3OQ.GXvN0U.4n6m0xQwdlUXDvCYZXhMphJh0BEVIKxJ8Qv2zE";

int main() {
    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    CommandHandler commandHandler(bot);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_message_create([&commandHandler](const dpp::message_create_t& event) {
        commandHandler.handleCommand(event);
        });

    bot.start(dpp::st_wait);

    return 0;
}