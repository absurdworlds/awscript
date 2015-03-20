/*Wld_PlayEffect("SLEEP_BLEND",hero,hero,0,0,0,TRUE);
или
Wld_PlayEffect("BLACK_SCREEN",hero,hero,0,0,0,TRUE);
FONT_INDICATORS_20*/


const int spacing = 1;
var int offset;

const int Numbars = 15;
var int HPBars;
var int HealBars;

var int DecayWait;
var int BarsWait;
var int RandomWait;
var int Cycle_Time;

var int Hero_Dex;
var int Hero_State;
var int Hero_Movemode;
var int Hero_Sprint;
var int Hero_Temphealpoints;
const int State_Default = 0;
const int State_Mobinteract = 1;

func void B_Cycle_Start()
{
	Wld_SendTrigger("CYCLE_TRIGGER");
};

func void B_CYCLE_FUNCTION()
{
	var string constr;

	if(hero.attribute[ATR_HITPOINTS] == 0)
	{
		Wld_SendTrigger("CYCLE_TRIGGER");
		return; 
	};

	if(!C_BodyStateContains(hero,BS_SWIM)) {
		if(C_BodyStateContains(hero,BS_RUN))
		{
			Hero_movemode = 1;
		}
		else if(C_BodyStateContains(hero,BS_WALK))
		{
			if(Hero_movemode == 1)
			{
				if(!Hero_Sprint) {
					if(hero.aivar[ATR_Stamina] > 20) {
						Mdl_ApplyOverlayMDS(hero,"Humans_Sprint.mds");
						Hero_Sprint = true;
					};
				} else {
					Mdl_RemoveOverlayMDS(hero,"Humans_Sprint.mds");
					Hero_Sprint = false;
				};
			};
			Hero_movemode = 2;
		} else if(!C_BodyStateContains(hero,BS_JUMP)) {
			Hero_movemode = 0;
			if(Hero_Sprint == true) {
				Mdl_RemoveOverlayMDS(hero,"Humans_Sprint.mds");
				Hero_Sprint = false;
			};
		};
		if(hero.aivar[ATR_Stamina] <= 0) {
			if(Hero_Sprint == true) {
				Mdl_RemoveOverlayMDS(hero,"Humans_Sprint.mds");
				Hero_Sprint = false;
			};
		};

		if(Npc_IsInFightMode(hero,FMODE_NONE)) {
			hero.damage[DAM_BLUNT] = 0;
			if(C_BodyStateContains(hero,BS_MOBINTERACT)) {
				if(Hero_State != State_Mobinteract)
				{
					Hero_State = State_Mobinteract;
					//print("MOBINTERACT");
					Hero_Dex = hero.attribute[ATR_DEXTERITY];
					hero.attribute[ATR_DEXTERITY] = Npc_GetTalentSkill(hero,NPC_TALENT_PICKLOCK) * 3;
				};
			}
			else if((Hero_State == State_Mobinteract) && (!C_BodyStateContains(hero,BS_MOBINTERACT))) {
				Hero_State = State_Default;
				hero.attribute[ATR_DEXTERITY] = Hero_Dex;
			};
		} else {
			if(Npc_IsInFightMode(hero,FMODE_FIST)) {
				hero.damage[DAM_BLUNT] = (hero.attribute[ATR_STRENGTH]) / 5;
			};
		};
	};


	/*constr = ConcatStrings("Здоровье: ", IntToString(hero.attribute[ATR_HITPOINTS]));
	if(hero.aivar[AIV_HealPoints] != 0) {
		if(hero.aivar[AIV_HealPoints] > 0) {
			constr = ConcatStrings(constr, "(+");
		} else {
			constr = ConcatStrings(constr, "(");
		};
		constr = ConcatStrings(constr, IntToString(hero.aivar[AIV_HealPoints]));
		constr = ConcatStrings(constr, ")");
	};

	constr = ConcatStrings(constr, "/");
	constr = ConcatStrings(constr, IntToString(hero.attribute[ATR_HITPOINTS_MAX]));

	if(hero.aivar[AIV_HealPoints] > 0) {
		if(Hero_TempHealPoints <= 0) {
			Npc_ChangeAttribute(hero,ATR_HITPOINTS,1);
		} else {
			Hero_TempHealPoints -= 1;
		};
		hero.aivar[AIV_HealPoints] -=1;
	} else if(hero.aivar[AIV_HealPoints] < 0) {
		Npc_ChangeAttribute(hero,ATR_HITPOINTS,-1);
		hero.aivar[AIV_HealPoints] +=1;
	};

	if(Hero_TempHealPoints > 0) {
		DecayWait += 1;
		if(DecayWait >= 60) {
			if(hero.attribute[ATR_HITPOINTS] > (hero.attribute[ATR_HITPOINTS_MAX]/100)) {
				Npc_ChangeAttribute(hero,ATR_HITPOINTS,-(hero.attribute[ATR_HITPOINTS_MAX]/100));
			};
			Hero_TempHealPoints -=hero.attribute[ATR_HITPOINTS_MAX]/100;
			DecayWait = 0;
		};
	};

	if(hero.aivar[AIV_ManaPoints] > 0) {
		Npc_ChangeAttribute(hero,ATR_MANA,1);
		hero.aivar[AIV_ManaPoints] -=1;
	} else if(hero.aivar[AIV_ManaPoints] < 0) {
		Npc_ChangeAttribute(hero,ATR_MANA,-1);
		hero.aivar[AIV_ManaPoints] +=1;
	};

	constr = "Выносливость: ";
	constr = ConcatStrings(constr, IntToString(hero.aivar[ATR_Stamina]));
	constr = ConcatStrings(constr, "/");
	constr = ConcatStrings(constr, IntToString(hero.aivar[ATR_Stamina_MAX]));*/

	if(!Hero_Sprint)
	{
		PrintScreen("test", 5, 8, "FONT_OLD_10_WHITE.TGA", 1);
	};


	if(Cycle_Time % 2 == 0) {
		if(Hero_Sprint && !C_BodyStateContains(hero,BS_WALK) && hero.aivar[ATR_STAMINA] > 0) {
			hero.aivar[ATR_Stamina] -= 1;
		};
	};

	if(Cycle_Time % 10 == 0) {
		if(Cycle_Time % 60 == 0) {
			if(!Hero_Sprint && hero.aivar[ATR_STAMINA] < hero.aivar[ATR_STAMINA_MAX]) {
				hero.aivar[ATR_Stamina] += 1;
			};
		};
	};


	Cycle_Time += 1;
	Wld_SendTrigger("CYCLE_TRIGGER");
};
