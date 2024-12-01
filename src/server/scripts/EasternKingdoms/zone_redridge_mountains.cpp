/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* Script Data Start
SDName: Redridge Mountains
SD%Complete: 0
SDComment:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "QuestData.h"

enum DirtStained
{
	ITEM_DIRT_STAINED_SCROLL		= 58898,
	QUEST_HE_WHO_CONTROLS_ETTINS	= 26519,
};

class player_dirt_stained_scroll : public PlayerScript
{
public:
	player_dirt_stained_scroll() : PlayerScript("player_dirt_stained_scroll") {}

	void OnItemLoot(Player* player, Item* item, uint32 count)
	{
		if (player->GetQuestStatus(QUEST_HE_WHO_CONTROLS_ETTINS == QUEST_STATUS_REWARDED))
			return;

		if (player->GetQuestStatus(QUEST_HE_WHO_CONTROLS_ETTINS) == QUEST_STATUS_NONE || player->HasItemCount(ITEM_DIRT_STAINED_SCROLL, 1, false))
			if (Quest const* quest = sQuestDataStore->GetQuestTemplate(QUEST_HE_WHO_CONTROLS_ETTINS))
				player->AddQuest(quest, player);
	}
};

void AddSC_redridge_mountains()
{
	new player_dirt_stained_scroll();
}
