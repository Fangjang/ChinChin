#include "CommandHandler.hpp"

CommandHandler::CommandHandler(dpp::cluster& bot) : bot(bot) {}

void CommandHandler::handleCommand(const dpp::message_create_t& event) {
	if (event.msg.content._Starts_with(COMMAND_PREFIX))
	{
		std::string command = event.msg.content.substr(COMMAND_PREFIX.size());
		
		std::istringstream iss(command);
		std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>() };

		if (tokens.empty())
		{
			bot.message_create(dpp::message(event.msg.channel_id, "No command provided.")
				.set_reference(event.msg.id));
			help(event);
		}
		else if (command._Starts_with("pre"))
		{
			prefixChange(event, tokens[1]);
		}
		else if (command._Starts_with("help"))
		{
			help(event);
		}
		else if (command._Starts_with("kick"))
		{
			kickUser(event);
		}
		else if (command._Starts_with("ban"))
		{
			banUser(event);
		}
	}
	else
	{
		if (event.msg.mentions.size() > 0)
		{
			for (auto& mention : event.msg.mentions)
			{
				if (mention.second.user_id == bot.me.id)
				{
					help(event);
				}
			}
		}
	}
}

void CommandHandler::prefixChange(const dpp::message_create_t& event, const std::string newPrefix)
{
	std::string new_prefix = newPrefix;
	COMMAND_PREFIX = new_prefix;
	bot.message_create(dpp::message(event.msg.channel_id, "Prefix changed to: " + new_prefix).set_reference(event.msg.id));
}

void CommandHandler::help(const dpp::message_create_t& event)
{
	dpp::embed embed = dpp::embed().
		set_color(0x3498DB).
		set_title("Bot Commands").
		set_description("Here are the available commands:").
		add_field(COMMAND_PREFIX + "kick @mention", "Kicks a user from the guild.", false).
		add_field(COMMAND_PREFIX + "ban @mention", "Bans a user from the guild.", false).
		add_field(COMMAND_PREFIX + "dm", "Sends a direct message to you.", false).
		add_field(COMMAND_PREFIX + "dm @mentions", "Sends a direct message to the mentioned user", false).
		add_field(COMMAND_PREFIX + "prefix new_prefix", "Changes the command prefix to the new prefix.", false).
		set_image(bot.me.get_avatar_url()).
		set_timestamp(time(0));

	event.reply(dpp::message().add_embed(embed).set_reference(event.msg.id));
}

void CommandHandler::banUser(const dpp::message_create_t& event)
{	
	// Get the message object
	auto& message = event.msg;
	auto guild = dpp::find_guild(message.guild_id);

	// Check if the user has the required role to use this command
	if (guild) {
		auto member = guild->members.find(message.author.id);
		if (member != guild->members.end()) {
			auto roles = member->second.get_roles();
			bool hasRole = false;
			for (auto& role : roles) {
				auto r = dpp::find_role(role);
				if (r && r->name == REQUIRED_ROLE_NAME) {
					hasRole = true;
					break;
				}
			}
			if (hasRole) {
				// Check if the user mentioned exactly one user
				if (message.mentions.size() == 1) {
					//  
					dpp::snowflake user_id_to_ban = message.mentions[0].second.user_id;
					bot.guild_ban_add(message.guild_id, user_id_to_ban);
					bot.message_create(dpp::message(message.channel_id,
						message.mentions[0].first.username + " has been banned!")
						.set_reference(message.id));
				}
				// If the user didn't mention exactly one user
				else {
					bot.message_create(dpp::message(message.channel_id, "Please mention one user to ban.")
						.set_reference(message.id));
				}
			}
			// If the user doesn't have the required role
			else {
				bot.message_create(dpp::message(message.channel_id, "You do not have permission to use this command!")
					.set_reference(message.id));
			}
		}
	}
}

void CommandHandler::kickUser(const dpp::message_create_t& event)
{
	auto& message = event.msg;
	auto guild = dpp::find_guild(event.msg.guild_id);

	if (guild)
	{
		auto member = guild->members.find(message.author.id);
		if (member != guild->members.end()) {
			auto roles = member->second.get_roles();
			bool hasRole = false;
			for (auto& role : roles) {
				auto r = dpp::find_role(role);
				if (r && r->name == REQUIRED_ROLE_NAME) {
					hasRole = true;
					break;
				}
			}
			if (hasRole) {
				// Check if the user mentioned exactly one user
				if (message.mentions.size() == 1) {
					dpp::snowflake user_id_to_kick = message.mentions[0].second.user_id;
					bot.guild_member_kick(message.guild_id, user_id_to_kick);
					bot.message_create(dpp::message(message.channel_id,
						message.mentions[0].first.username + " has been kicked!")
						.set_reference(message.id));
				}
				// If the user didn't mention exactly one user
				else {
					bot.message_create(dpp::message(message.channel_id, "Please mention one user to kick.")
						.set_reference(message.id));
				}
			}
			// If the user doesn't have the required role
			else {
				bot.message_create(dpp::message(message.channel_id, "You do not have permission to use this command!")
					.set_reference(message.id));
			}
		}
	}
}

void CommandHandler::dmUser(const dpp::message_create_t& event)
{

}
