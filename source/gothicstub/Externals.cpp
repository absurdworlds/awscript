/*
 * Copyright(C) 2015  hedede <haddayn@gmail.com>
 *
 * License LGPLv3 or later:
 * GNU Lesser GPL version 3 <http:
 * This is free software: you are free to change and redistribute it.
 * There is NO WARRANTY, to the extent permitted by law.
 */
/*
 * Based on G2MDK Externals.d
 */

void Print(string s0)
{
}


void PrintMulti(string s0, string s1, string s2, string s3, string s4)
{
}


void PrintDebug(string s)
{
}





void PrintScreen(int dialogNr, string msg, int posx, int posy,
                 string font, int timeSec)
{
}


int Hlp_Random(int n0) { return 0; }


int Hlp_StrCmp(string s1, string s2) { return 0; }

int Hlp_IsValidNpc(C_NPC self) { return 0; }

int Hlp_IsValidItem(C_ITEM item) { return 0; }

int Hlp_IsItem(C_Item item, int instanceName)



C_NPC 	Hlp_GetNpc(int instanceName) { }

int Hlp_GetInstanceID 		(C_NPC npc) { return 0; }

int Hlp_GetInstanceID 		(C_ITEM item) { return 0; }





void AI_Wait(C_NPC n0, float n1) { }



int Npc_GetStateTime(C_NPC self) { return 0; }

void Npc_SetStateTime(C_NPC self, int seconds) { }



int Wld_GetDay() {}

int Wld_IsTime(int hour1, int min1, int hour2, int min2) { return 0; }



void Wld_InsertNpc(int npcInstance, string spawnPoint) { }

void Wld_InsertNpcAndRespawn(int instance, string spawnPoint, FLOAT spawnDelay)




void AI_PlayAni(C_NPC n0, string s0) { }

void AI_StandUp(C_NPC self) {}


void AI_StandUpQuick(C_NPC self)


void AI_Quicklook(C_NPC self, C_NPC other) { }

void AI_LookAt(C_NPC self, string name) { }

void AI_LookAtNpc(C_NPC self, C_NPC other) { }

void AI_StopLookAt(C_NPC self) { }

void AI_PointAt(C_NPC self, string name) { }

void AI_PointAtNpc(C_NPC self, C_NPC other) { }

void AI_StopPointAt(C_NPC self) { }



void AI_TakeItem(C_NPC self, C_ITEM item) {}

void AI_DropItem(C_NPC self, int itemid) {}



void AI_UseItem(C_NPC self, int itemInstance) { }

void AI_UseItemToState(C_NPC self, int itemInstance, int state) { }




int AI_UseMob(C_NPC self, string schemeName, int targetState) { return 0; }


int Wld_IsMobAvailable(C_NPC self, string schemeName) {}

int Wld_GetMobState(C_NPC self, string schemeName);




void AI_SetWalkmode(C_NPC n, int n0) { }



void AI_GotoWP(C_NPC n0, string s0) { }

void AI_GotoFP(C_NPC self, string fpName) {}


void AI_GotoNextFP(C_NPC self, string fpName) { }

void AI_GotoNpc(C_NPC self, C_NPC other) {}

void AI_GotoItem(C_NPC self, C_ITEM item) {}

void AI_GotoSound(C_NPC n0) { }

void AI_Teleport(C_NPC self, string waypoint);



string Npc_GetNearestWP(C_NPC self) { return ""; }

string	Npc_GetNextWP(C_NPC self) {return ""; }


int Wld_IsFPAvailable(C_NPC self, string fpName) {}

int Wld_IsNextFPAvailable(C_NPC self, string fpName)

int Npc_IsOnFP(C_NPC self, string name)

int Npc_IsWayBlocked(C_NPC self) {}




void AI_TurnToNpc(C_NPC n0, C_NPC n1) { }

void AI_TurnAway(C_NPC n0, C_NPC n1) { }

void AI_WhirlAround(C_NPC self, C_NPC other) {}

void AI_TurnToSound(C_NPC self) { }

void AI_AlignToWP			(C_NPC self) { }

void AI_Dodge(C_NPC npc) { }




void Mdl_ApplyOverlayMds(C_NPC n0, string s1) { }


void Mdl_RemoveOverlayMDS	(C_NPC self, string overlayName) {}

void Mdl_ApplyOverlayMDSTimed(C_NPC self, string overlayname, FLOAT timeTicks);




void Mdl_ApplyRandomAni(C_NPC n0, string s1, string s2) { }

void Mdl_ApplyRandomAniFreq(C_NPC n0, string s1, FLOAT f2) { }




void Mdl_StartFaceAni(C_NPC self, string name, FLOAT intensity, FLOAT holdTime) {}

void Mdl_ApplyRandomFaceAni(C_NPC self, string name, FLOAT timeMin, FLOAT timeMinVar, FLOAT timeMax, FLOAT timeMaxVar, FLOAT probMin) {}










int Npc_GetBodyState(C_NPC self) { return 0; }



int Npc_HasBodyFlag(C_NPC self, int bodyFlag) {}

void AI_PlayAniBS (C_NPC npc, string aniname, int bodystate) { }








void Npc_SetToFistMode(C_NPC self) { }

void Npc_SetToFightMode(C_NPC self, int weapon) { }

int Npc_IsInFightMode(C_NPC self, int fmode) { return 0; }




void AI_DrawWeapon(C_NPC n0) { }

void AI_ReadyMeleeWeapon(C_NPC self) {}

void AI_ReadyRangedWeapon(C_NPC self) {}

void AI_RemoveWeapon(C_NPC n0) { }


C_Item Npc_GetReadiedWeapon(C_NPC n0) { }


int Npc_HasReadiedWeapon(C_NPC self) { return 0; }

int Npc_HasReadiedMeleeWeapon(C_NPC self) { return 0; }

int Npc_HasReadiedRangedWeapon(C_NPC self) { return 0; }

int Npc_HasRangedWeaponWithAmmo(C_NPC npc)




int Npc_GetTarget(C_NPC self)




int Npc_GetNextTarget(C_NPC self)










int Npc_IsNextTargetAvailable(C_NPC self)







void Npc_SetTarget(C_NPC self, C_NPC other)

void AI_Attack(C_NPC self)



void AI_FinishingMove(C_NPC self, C_NPC other)

void AI_Defend(C_NPC self)








void AI_Flee(C_NPC self) {}


void AI_AimAt(C_NPC attacker, C_NPC target) {}

void AI_ShootAt(C_NPC attacker, C_NPC target) { }

void AI_StopAim(C_NPC attacker) { }



int Npc_AreWeStronger(C_NPC self, C_NPC other) { return 0; }
int Npc_IsAiming(C_NPC self, C_NPC other) { return 0; }


void Wld_InsertItem(int itemInstance, string spawnPoint) { }


void AI_LookForItem(C_NPC self, int instance) {}

int Wld_RemoveItem(C_ITEM item) { }



void CreateInvItem(C_NPC n0, int n1) { }

void CreateInvItems(C_NPC n0, int n1, int n2) { }

C_Item Npc_GetInvItem(C_NPC self, int itemInstance) {}

int Npc_HasItems(C_NPC n0, int itemInstance) { return 0; }

int Npc_GetInvItemBySlot(C_NPC self, int category, int slotNr)




void Npc_RemoveInvItem(C_NPC owner, int itemInstance) {}

void Npc_RemoveInvItems(C_NPC owner, int itemInstance, int amount) { }


void Mob_CreateItems(string mobName, int itemInstance, int amount) {}

int Mob_HasItems(string mobName, int itemInstance) { return 0; }



void EquipItem(C_NPC n0, int n1) { }

void AI_EquipBestMeleeWeapon(C_NPC self) {}

void AI_EquipBestRangedWeapon(C_NPC self) {}

void AI_EquipBestArmor(C_NPC self) {}

void AI_UnequipWeapons(C_NPC self) {}

void AI_UnequipArmor(C_NPC self) {}

void AI_EquipArmor(C_NPC owner, C_ITEM armor_from_owners_inventory)


C_Item Npc_GetEquippedMeleeWeapon(C_NPC n0) { }

C_Item Npc_GetEquippedRangedWeapon(C_NPC n0) { }

C_Item Npc_GetEquippedArmor(C_NPC n0) { }


int Npc_HasEquippedWeapon(C_NPC self) { return 0; }

int Npc_HasEquippedMeleeWeapon(C_NPC self) { return 0; }

int Npc_HasEquippedRangedWeapon(C_NPC self) { return 0; }

int Npc_HasEquippedArmor(C_NPC self) { return 0; }



int Npc_OwnedByNpc(C_ITEM item, C_NPC npc) { return 0; }

int Npc_OwnedByGuild(C_ITEM item, int guild) { return 0; }

int Npc_IsDetectedMobOwnedByNpc(C_NPC user, C_NPC owner) { return 0; }


int Npc_IsDetectedMobOwnedByGuild(C_NPC user, int ownerguild) { return 0; }


void Npc_GiveItem(C_NPC n0, C_ITEM n1, C_NPC n2) { }
int Npc_StartItemReactModules(C_NPC self, C_NPC other, C_ITEM item) { return 0; }
int Npc_HasOffered(C_NPC self, C_NPC other, int itemInstance) { return 0; }
void ExitGame() {}
void ExitSession() {}
int PlayVideo(string Filename) {}
int PlayVideoEx(string Filename, int ScreenBlend, int ExitSession) {}
void SetPercentDone(int PercentDone) {}
void IntroduceChapter(string Titel, string Untertitel, string Texture, string Sound, int WaitTime) {}


int Doc_Create() {}
int Doc_CreateMap() {}
void Doc_SetPages(int Document, int Count) {}
void Doc_SetPage(int Document, int Page, string Texture, int Scale) {}
void Doc_SetMargins(int Document, int Page, int Left, int Top, int Right, int Bottom, int Pixels) {}
void Doc_SetFont(int Document, int Page, string Font) {}
void Doc_SetLevel(int Document, string Level) {}
void Doc_SetLevelCoords(int Document, int Left, int Top, int Right, int Bottom) {}
void Doc_PrintLine(int Document, int Page, string Text) {}

void Doc_PrintLines(int Document, int Page, string Text) {}
			

void Doc_Show(int Document) {}

void AI_Output(C_NPC self, C_NPC target, string outputName) {}

void AI_OutputSVM				(C_NPC self, C_NPC target, string svmname) {}

void AI_OutputSVM_Overlay(C_NPC self, C_NPC target, string svmname) {}


void AI_WaitTillEnd(C_NPC self, C_NPC other) {}


void AI_Ask(C_NPC self, func anserYes, func answerNo) { }


void AI_AskText(C_NPC self, func funcYes, func funcNo, string strYes, string strNo);

void AI_WaitForQuestion(C_NPC self, func scriptFunc) { }


void Npc_SetRefuseTalk(C_NPC self, int timeSec) { }

int Npc_RefuseTalk(C_NPC self) { return 0; }



void Npc_MemoryEntry(C_NPC self, int source, C_NPC offender, int newsid, C_NPC victim) { }


void Npc_MemoryEntryGuild(C_NPC self, int source, C_NPC offender, int newsid, C_NPC victimguild) { }

int Npc_HasNews(C_NPC self, int newsID, C_NPC offender, C_NPC victim) {}


int Npc_IsNewsGossip(C_NPC self, int newsNumber) {}

C_NPC  Npc_GetNewsWitness(C_NPC self, int newsNumber) {}

C_NPC  Npc_GetNewsVictim(C_NPC self, int newsNumber) {}

C_NPC 	Npc_GetNewsOffender(C_NPC self, int newsNumber) { return 0; }

int Npc_IsDead(C_NPC n0) { return 0; }

int Npc_KnowsInfo(C_NPC self, int infoInstance) { return 0; }


int Npc_CheckInfo(C_NPC npc, int important) { return 0; }

int NPC_GiveInfo(C_NPC npc, int important) { return 0; }

int Npc_CheckAvailableMission(C_NPC npc, int missionState, int important) { return 0; }


int Npc_CheckRunningMission(C_NPC npc, int important) { return 0; }


int Npc_CheckOfferMission(C_NPC npc, int important) { return 0; }

void Mis_SetStatus(int missionName, int newStatus) {}

int Mis_GetStatus(int missionName) { return 0; }

int Mis_OnTime(int missionName) { return 0; }

void AI_StopProcessInfos(C_NPC npc)

int Npc_IsPlayer(C_NPC player) { return 0; }

int Wld_DetectPlayer(C_NPC self) { return 0; }

int Npc_HasDetectedNpc(C_NPC self, C_NPC other) { return 0; }

int Npc_IsNear(C_NPC self, C_NPC  other) { return 0; }

int Npc_GetDistToNpc(C_NPC npc1, C_NPC npc2) { return 0; }

int Npc_GetHeightToNpc(C_NPC npc1, C_NPC npc2) { return 0; }


int Npc_GetDistToWP				(C_NPC self, string wpName)

int Npc_GetDistToItem(C_NPC npc, C_ITEM item) { return 0; }

int Npc_GetDistToPlayer(C_NPC npc1) { return 0; }

int Snd_GetDistToSource(C_NPC self) { return 0; }

int Npc_GetTrueGuild(C_NPC npc) { return 0; }

void Npc_SetAttitude(C_NPC self, int att) { }

void Npc_SetTempAttitude(C_NPC self, int att) { }

int Npc_GetAttitude(C_NPC self, C_NPC other) { return 0; }

int Npc_SetTrueGuild(C_NPC npc, int guildID) { }

void Wld_SetGuildAttitude(int guild1, int attitude, int guild2) { }

int Wld_GetGuildAttitude(int guild1, int guild2) { return 0; }
int Npc_GetPermAttitude(C_NPC self, C_NPC other) {}
void Wld_ExchangeGuildAttitudes(string name) { }
int Npc_GetGuildAttitude(C_NPC npc, C_NPC npc) { return 0; }
void Npc_SetKnowsPlayer(C_NPC self, C_NPC player) { }
int Npc_KnowsPlayer(C_NPC self, C_NPC player) { return 0; }
void AI_StartState(C_NPC self, func what, int stateBehaviour, string wpName) {}
void Npc_ClearAIQueue(C_NPC self) { }
void AI_SetNpcsToState(C_NPC self, func aiStateFunc, int radius) {}
int Npc_IsInState(C_NPC self, func state) { return 0; }
int Npc_WasInState(C_NPC self, func state) { return 0; }



void TA							(c_npc self, int start_h, int stop_h, func state, string waypoint) { }
void TA_Min(c_npc self, int start_h, int start_m, int stop_h, int stop_m, func state, string waypoint) { }
void AI_ContinueRoutine(C_NPC self) { }
int Npc_IsInRoutine(C_NPC self, func state) { return 0; }

void Npc_ExchangeRoutine(C_NPC self, string routineName) {}
void Wld_SetObjectRoutine(int hour1, int min1, string objName, int state) { }
void Wld_SetMobRoutine(int hour1, int min1, string objName, int state) { }



void Rtn_Exchange(string oldRoutine, string newRoutine) {}
void TA_BeginOverlay(C_NPC self) {}
void TA_EndOverlay(C_NPC self) {}
void TA_RemoveOverlay(C_NPC self) {}


void Mdl_SetModelScale(C_NPC self, FLOAT x, FLOAT y, FLOAT z) {}
void Mdl_SetModelFatness(C_NPC self, FLOAT fatness) {}

void Npc_ChangeAttribute(C_NPC self, int atr, int value) {}
int Npc_HasTalent(C_NPC self, int tal) { return 0; }
int Npc_HasFightTalent(C_NPC self, int tal) { return 0; }
void Npc_CreateSpell(C_NPC self, int spellnr) {}
void Npc_LearnSpell(C_NPC self, int spellnr) {}
void Npc_SetTeleportPos(C_NPC self) {}
int Npc_GetActiveSpell(C_NPC self) { return 0; }
int Npc_GetActiveSpell(C_NPC self) { return 0; }
int Npc_GetLastHitSpellID			(C_NPC self) { return 0; }
int Npc_GetLastHitSpellCat(C_NPC self) { return 0; }
int Npc_GetActiveSpellCat(C_NPC self) { return 0; }
int Npc_SetActiveSpellInfo(C_NPC npc, int i1) { return 0; }
int Npc_GetActiveSpellLevel(C_NPC self)
void AI_ReadySpell(C_NPC self, int spellID, int investMana);
void AI_UnreadySpell(C_NPC self);
int Npc_HasSpell(C_NPC self, int spellID); 

void Npc_PercEnable(C_NPC self, int percID, func function) { }
void Npc_PercDisable(C_NPC self, int percID) { }
void Npc_SetPercTime(C_NPC self, FLOAT seconds) { }
void Perc_SetRange(int percID, int range) { }
void Npc_SendPassivePerc(C_NPC npc1, int Perc_type, C_NPC npc2, C_NPC npc3) { }
void Npc_SendSinglePerc(C_NPC self, C_NPC target, int percID) {}
void Npc_PerceiveAll(C_NPC self) {}

int Wld_DetectNpc(C_NPC self, int instance, func aiState, int guild) { return 0; }
int Wld_DetectNpcEx(C_NPC self, int npcInstance, func aiState, int guild, int detectPlayer)
int Wld_DetectItem(C_NPC self, int flags) { return 0; }
string Npc_GetDetectedMob(C_NPC self) { return 0; }

int Npc_CanSeeNpc(C_NPC npc1, C_NPC npc2) { return 0; }
int Npc_CanSeeNpcFreeLOS		(C_NPC self, C_NPC other) {}
int Npc_CanSeeItem(C_NPC npc1, C_ITEM item) { return 0; }
int Npc_CanSeeSource(C_NPC self) { return 0; }

void TA_CS(C_NPC self, string csName, string roleName) {}

void AI_PlayCutscene(C_NPC self, string csName) {}
int Hlp_CutscenePlayed(string csName) { return 0; }
int Npc_IsInCutscene(C_NPC self) { return 0; }

void Snd_Play(string s0) { }
void Snd_Play3D 					(C_NPC n0, string s1) { }
int Snd_IsSourceNpc(C_NPC self) { return 0; }
int Snd_IsSourceItem(C_NPC self) { return 0; }






void Wld_AssignRoomToGuild(string s0, int guild) { }
void Wld_AssignRoomToNpc(string s0, C_NPC roomowner) { }
C_NPC 	Wld_GetPlayerPortalOwner()
int Wld_GetPlayerPortalGuild()
C_NPC 	Wld_GetFormerPlayerPortalOwner()
int Wld_GetFormerPlayerPortalGuild()
int Npc_IsPlayerInMyRoom(C_NPC npc)
int Npc_WasPlayerInMyRoom(C_NPC npc)

string IntToString(int x)
{
	// TODO
}

int   FloatToInt(float x)
{
	return int(x);
}

float IntToFloat(int x)
{
	return float(x);
}

string ConcatStrings(string str1, string str2) {}
void PrintDebugInst(String text) { }
void PrintDebugInstCh(int ch, String text) { }
void PrintDebugCh(int ch, String text) { }
void Log_CreateTopic(string name, int section)
void Log_SetTopicStatus(string name, int status)
void Log_AddEntry(string topic, string entry)

void Game_InitGerman() {}
void Game_InitEnglish() {}
