/**
    This plugin can be used for common player customizations
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Configuration/Config.h"
#include "ChannelMgr.h"
#include "Chat.h"

class PlyerLoginChat : public PlayerScript
{
public:
    PlyerLoginChat() : PlayerScript("PlyerLoginChat") {}

    void OnLogin(Player* player) override
    {

        ChannelMgr* cMgr = ChannelMgr::forTeam(player->GetTeamId());
        if (!cMgr)
            return;

        std::string channelName = sConfigMgr->GetStringDefault("LoginChat.name", "world");

        if (channelName.empty())
            return;

        if (isdigit(channelName[0]))
            return;

        //                                                    0          1
        QueryResult result = CharacterDatabase.PQuery("SELECT channelId, password FROM channels WHERE name = '%s'", channelName.c_str());

        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 channelDBId = fields[0].GetUInt32();
                std::string password = fields[1].GetString();

                if (channelDBId)
                {
                    ChatChannelsEntry const* chan = sChatChannelsStore.LookupEntry(channelDBId);
                    if (!chan)
                        return;
                }

                if (Channel* channel = cMgr->GetJoinChannel(channelName, channelDBId))
                    channel->JoinChannel(player, password);
    
                cMgr->LoadChannels();
            }
            while (result->NextRow());
        }
    }
};

void AddLoginChatScripts()
{
    new PlyerLoginChat();
}

