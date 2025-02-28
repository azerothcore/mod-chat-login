/**
    This plugin can be used for common player customizations
 */

#include "ChannelMgr.h"
#include "Chat.h"
#include "Configuration/Config.h"
#include "Player.h"
#include "ScriptMgr.h"

class LoginChat : public PlayerScript{
public:

    LoginChat() : PlayerScript("LoginChat", {
        PLAYERHOOK_ON_LOGIN
    }) { }

    void OnPlayerLogin(Player* player) override
    {
        std::string channelName = sConfigMgr->GetOption<std::string>("LoginChat.name", "world");
        QueryResult result = CharacterDatabase.Query("SELECT channelId FROM channels WHERE name = '{}'", channelName.c_str());

        if (!result)
            return;

    	if (ChannelMgr* cMgr = ChannelMgr::forTeam(player->GetTeamId()))
    	{
        	if (Channel* channel = cMgr->GetJoinChannel("world", 9))
            	channel->JoinChannel(player, "");

    		cMgr->LoadChannels();
        }

    }
};

void AddLoginChatScripts() {
    new LoginChat();
}

