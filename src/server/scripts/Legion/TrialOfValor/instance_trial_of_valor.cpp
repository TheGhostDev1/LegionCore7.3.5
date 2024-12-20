#include "trial_of_valor.h"
#include "ScriptPCH.h"
#include "WorldPacket.h"
#include "InstancePackets.h"

DoorData const doorData[] =
{
    { Data::GameObjects::GoGarmDoor,    Data::BossIDs::GarmID,      DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { Data::GameObjects::GoGarmDoor2,   Data::BossIDs::GarmID,      DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { Data::GameObjects::GoOdynDoor,    Data::BossIDs::OdynID,      DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { Data::GameObjects::GoGarmDoor2,   Data::BossIDs::HelyaID,     DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { 0,                                0,                          DOOR_TYPE_ROOM,     BOUNDARY_NONE }
};

ObjectData const creatureData[] =
{
    { Data::Creatures::Odyn,            Data::BossIDs::OdynID   },
    { Data::Creatures::Guarm,           Data::BossIDs::GarmID   },
    { Data::Creatures::Helya,           Data::BossIDs::HelyaID  },
    { 0,                                                    0   } // END
};

ObjectData const objectData[] =
{
    { 0,                    0, }
};

class instance_trial_of_valor : public InstanceMapScript
{
public:
    instance_trial_of_valor() : InstanceMapScript(TrialOfValorScriptName, 1648) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_trial_of_valor_InstanceMapScript(map);
    }

    struct instance_trial_of_valor_InstanceMapScript : InstanceScript
    {
        WorldLocation loc_res_pla;
        ObjectGuid OdynGUID;
        ObjectGuid HymdallGUID;
        ObjectGuid HyrjaGUID;
        ObjectGuid GuarmGUID;
        ObjectGuid HelyaGUID;
        ObjectGuid OdynTovChest;
        ObjectGuid GoLightRay;
        std::vector<ObjectGuid> GoLightRayGUID{};
        std::map<uint32, ObjectGuid> runicBrandGUIDconteiner;
        uint32 OdynIntroEvent{};
        uint32 OdynEvadeComplete{};
        uint32 OdynTestOfAges{};
        uint32 OdynMeetPhase1{};
        uint32 OdynMeetPhase2{};
        uint32 GoRunicColour[5];
        uint32 checkTimer = 1000;
        uint32 checkTimerAura = 1000;

        explicit instance_trial_of_valor_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
        {
            SetHeaders(DataHeader);
            SetBossNumber(Data::BossIDs::Max);
        }

        void Initialize() override
        {
            LoadDoorData(doorData);
            LoadObjectData(creatureData, objectData);
        }

        void OnCreatureCreate(Creature* creature) override
        {
            InstanceScript::OnCreatureCreate(creature);

            switch (creature->GetEntry())
            {
                case Data::Creatures::Odyn:
                    OdynGUID = creature->GetGUID();

                    if (GetBossState(Data::BossIDs::OdynID) == DONE)
                    {
                        instance->SummonCreature(116270, NpcCosmeticOnePos);
                        instance->SummonCreature(116268, NpcCosmeticTwoPos);
                        instance->SummonCreature(116269, NpcCosmeticThreePos);
                        instance->SummonCreature(116271, NpcCosmeticFourPos);
                        instance->SummonCreature(101459, SpoilsPos);
                        creature->NearTeleportTo(2399.73f, 528.816f, 748.997f, 5.48697f);
                    }
                    break;
                case Data::Creatures::Hymdall:
                    HymdallGUID = creature->GetGUID();
                    break;
                case Data::Creatures::Hyrja:
                    HyrjaGUID = creature->GetGUID();
                    break;
                case Data::Creatures::Guarm:
                    if (GetBossState(Data::BossIDs::GarmID) != DONE)
                        creature->CreateConversation(3916);

                    GuarmGUID = creature->GetGUID();
                    break;
                case Data::Creatures::Helya:
                    HelyaGUID = creature->GetGUID();

                    if (GetBossState(Data::BossIDs::HelyaID) == DONE)
                    {
                        instance->SummonCreature(116760, Odyn);
                        instance->SummonCreature(116761, Hymdall);
                        instance->SummonCreature(116818, Eyir);
                        instance->SummonCreature(116830, Hyrja);
                        for (uint8 i = 0; i < 3; ++i)
                            instance->SummonCreature(116763, StormforgedSentinel[i]);
                        for (uint8 i = 0; i < 2; ++i)
                            instance->SummonCreature(116762, ChosenofEyir[i]);
                        for (auto guid : GoLightRayGUID)
                            if (auto gameobject = instance->GetGameObject(guid))
                                gameobject->SetRespawnTime(86400);
                        if (auto go = instance->GetGameObject(GoLightRay))
                            go->SetRespawnTime(86400);
                    }
                    break;
                default:
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* go) override
		{
            InstanceScript::OnGameObjectCreate(go);

            switch (go->GetEntry())
            {
            case Data::GameObjects::GoTovOdynChest:
            OdynTovChest = go->GetGUID();
                break;
            case Data::GameObjects::GoLightRay1:
                GoLightRayGUID.push_back(go->GetGUID());
                break;
            case Data::GameObjects::GoLightRay2:
                GoLightRay = go->GetGUID();
                break;
            case Data::GameObjects::GoGarmDoor:
            case Data::GameObjects::GoGarmDoor2:
            case Data::GameObjects::GoOdynDoor:
                AddDoor(go, true);
                break;
            case Data::GameObjects::GoRunicBrandPure:
                GoRunicColour[0] = go->GetEntry();
                runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                break;
            case Data::GameObjects::GoRunicBrandOrange:
                GoRunicColour[1] = go->GetEntry();
                runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                break;
            case Data::GameObjects::GoRunicBrandYellow:
                GoRunicColour[2] = go->GetEntry();
                runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                break;
            case Data::GameObjects::GoRunicBrandBlue:
                GoRunicColour[3] = go->GetEntry();
                runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                break;
            case Data::GameObjects::GoRunicBrandGreen:
                GoRunicColour[4] = go->GetEntry();
                runicBrandGUIDconteiner[go->GetEntry()] = go->GetGUID();
                break;
            default:
				break;
			}
		}

        bool SetBossState(uint32 type, EncounterState state) override
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (state)
            {
            case IN_PROGRESS:
            {
                switch (type)
                {
                case Data::BossIDs::OdynID:
                    for (auto i : runicBrandGUIDconteiner)
                        for (uint8 i = 0; i < 5; ++i)
                            if (auto rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[i]]))
                                rune->SetGoState(GO_STATE_READY);
                    break;
                }
                break;
            }
            case DONE:
			{
                switch (type)
                {
                case Data::BossIDs::OdynID:
                    if (auto chest = instance->GetGameObject(OdynTovChest))
                        chest->SetRespawnTime(86400);

                    instance->SummonCreature(116270, NpcCosmeticOnePos);
                    instance->SummonCreature(116268, NpcCosmeticTwoPos);
                    instance->SummonCreature(116269, NpcCosmeticThreePos);
                    instance->SummonCreature(116271, NpcCosmeticFourPos);
                    instance->SummonCreature(101459, SpoilsPos);
                    instance->ApplyOnEveryPlayer([&](Player* player)
                    {
                        player->AddDelayedEvent(100, [player]() -> void
                        {
                            player->CombatStop(true);
                            player->getHostileRefManager().deleteReferences();
                        });

                        instance->ToInstanceMap()->PermBindAllPlayers(player);

                        if (auto victim = instance->GetCreature(OdynGUID))
                        {
                            uint16 encounterId = sObjectMgr->GetDungeonEncounterByCreature(victim->GetEntry());

                            instance->UpdateEncounterState(ENCOUNTER_CREDIT_KILL_CREATURE, Data::Creatures::Odyn, victim, player);
                            victim->GetMap()->SendToPlayers(WorldPackets::Instance::BossKillCredit(encounterId).Write());
                        }

                        if (!player->IsPlayerLootCooldown(232466, TYPE_SPELL, instance->GetDifficultyID()))
                            if (auto odyn = instance->GetCreature(OdynGUID))
                                odyn->CastSpell(player, 232466, false);
                    });
					break;
                case Data::BossIDs::HelyaID:
                    instance->SummonCreature(116760, Odyn);
                    instance->SummonCreature(116761, Hymdall);
                    instance->SummonCreature(116818, Eyir);
                    instance->SummonCreature(116830, Hyrja);
                    for (uint8 i = 0; i < 3; ++i)
                        instance->SummonCreature(116763, StormforgedSentinel[i]);
                    for (uint8 i = 0; i < 2; ++i)
                        instance->SummonCreature(116762, ChosenofEyir[i]);
                    for (auto guid : GoLightRayGUID)
                        if (auto gameobject = instance->GetGameObject(guid))
                            gameobject->SetRespawnTime(86400);
                    if (auto go = instance->GetGameObject(GoLightRay))
                        go->SetRespawnTime(86400);
                    break;
                default:
                    break;
                }
                break;
            }
            default:
                break;
            }
            return true;
        }

        void SetData(uint32 type, uint32 data) override
        {
            switch (type)
            {
                case Data::DataMisc::OdynIntroEvent:
                    OdynIntroEvent = data;
                    SaveToDB();
                    break;
                case Data::DataMisc::OdynEvadeComplete:
                    OdynEvadeComplete = data;
                    SaveToDB();
                    break;
                case Data::DataMisc::OdynEventComplete1:
                    OdynMeetPhase1 = data;
                    break;
                case Data::DataMisc::OdynEventComplete2:
                    OdynMeetPhase2 = data;
                    break;
                case Data::DataMisc::OdynTestOfAges:
                    OdynTestOfAges = data;
                    break;
                case Data::DataMisc::OdynRunesActivated:
                    if (auto rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[data]]))
                        rune->SetGoState(GO_STATE_ACTIVE);
                    break;
                case Data::DataMisc::OdynRunesDeactivated:
                    if (auto rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[data]]))
                        rune->SetGoState(GO_STATE_READY);
                    if (auto odyn = instance->GetCreature(OdynGUID))
                        odyn->AI()->SetData(1, data);
                    break;
                case Data::DataMisc::OdynDeactivatedPurpleRune:
                    if (auto rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[0]]))
                        rune->SetGoState(GO_STATE_READY);
                    break;
                case Data::DataMisc::OdynDeactivatedBlueRune:
                    if (auto rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[3]]))
                        rune->SetGoState(GO_STATE_READY);
                    break;
                case Data::DataMisc::OdynDeactivatedGreenRune:
                    if (auto rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[4]]))
                        rune->SetGoState(GO_STATE_READY);
                    break;
                case Data::DataMisc::OdynDeactivatedYellowRune:
                    if (auto rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[2]]))
                        rune->SetGoState(GO_STATE_READY);
                    break;
                case Data::DataMisc::OdynDeactivatedOrangeRune:
                    if (auto rune = instance->GetGameObject(runicBrandGUIDconteiner[GoRunicColour[1]]))
                        rune->SetGoState(GO_STATE_READY);
                    break;
                default:
                    break;
            }
        }

        uint32 GetData(uint32 type) const override
        {
            switch (type)
            {
                case Data::DataMisc::OdynIntroEvent:
                    return OdynIntroEvent;
                case Data::DataMisc::OdynEvadeComplete:
                    return OdynEvadeComplete;
                case Data::DataMisc::OdynEventComplete1:
                    return OdynMeetPhase1;
                case Data::DataMisc::OdynEventComplete2:
                    return OdynMeetPhase2;
                case Data::DataMisc::OdynTestOfAges:
                    return OdynTestOfAges;
                default:
                    return 0;
            }
        }

        ObjectGuid GetGuidData(uint32 type) const override
        {
            switch (type)
            {
                case Data::Creatures::Odyn:
                    return OdynGUID;
                case Data::Creatures::Hymdall:
                    return HymdallGUID;
                case Data::Creatures::Hyrja:
                    return HyrjaGUID;
                case Data::Creatures::Guarm:
                    return GuarmGUID;
                case Data::Creatures::Helya:
                    return HelyaGUID;
            }
            return ObjectGuid::Empty;
        }

        void Update(uint32 diff) override
        {
            if (checkTimer <= diff)
            {
                checkTimer = 1000;

                instance->ApplyOnEveryPlayer([&](Player* player)
                {
                    if (player->IsInWater())
                        player->CastSpell(player, 230197, false);
                    else
                        if (player->HasAura(230197))
                            player->RemoveAura(230197);

                    if (player->GetCurrentAreaID() == 8440)
                    {
                        if (player->GetPositionZ() <= 600.00f)
                        {
                            player->Kill(player);
                            player->RepopAtGraveyard();
                        }
                    }
                });
            }
            else
                checkTimer -= diff;

            if (checkTimerAura <= diff)
            {
                checkTimerAura = 1000;

                instance->ApplyOnEveryPlayer([&](Player* player)
                {
                    if (player->GetCurrentAreaID() == 8440 && player->HasAura(192635))
                        if (player->GetPositionX() <= 2585.00f)
                            player->RemoveAura(192635);
                });
            }
            else
                checkTimerAura -= diff;
        }

        WorldLocation* GetClosestGraveYard(float x, float y, float z) override
        {
            loc_res_pla.WorldRelocate(1648, x, y, z);

            uint32 graveyardId = 0;

            if (GetBossState(Data::BossIDs::GarmID) == DONE)
                graveyardId = 5827;
            else if (GetBossState(Data::BossIDs::OdynID) == DONE)
                graveyardId = 5839;
            else
                graveyardId = 5775;

            if (graveyardId)
            {
                if (WorldSafeLocsEntry const* gy = sWorldSafeLocsStore.LookupEntry(graveyardId))
                {
                    loc_res_pla.WorldRelocate(gy->MapID, gy->Loc.X, gy->Loc.Y, gy->Loc.Z);
                }
            }
            return &loc_res_pla;
        }

        void WriteSaveDataMore(std::ostringstream& data) override
        {
            data << OdynIntroEvent << " " << OdynEvadeComplete;
        }

        void ReadSaveDataMore(std::istringstream& data) override
        {
            data >> OdynIntroEvent >> OdynEvadeComplete;
        }
    };
};

void AddSC_instance_trial_of_valor()
{
    new instance_trial_of_valor();
}
