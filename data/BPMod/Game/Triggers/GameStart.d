
func void GameIntro()
{
	AI_Wait(hero,1);
	AI_Standup(hero);
	AI_PlayAni(hero,"r_LegShake");
	AI_Wait(hero,1);
	AI_StandupQuick(hero);
	AI_Output(hero,hero,"MON_Hero_Start_15_00");	//Где я? Не узнаю это место.
	AI_Wait(hero,1);
	AI_StandupQuick(hero);
	AI_PlayAni(hero,"t_Search");
	AI_Wait(hero,1);
	AI_StandupQuick(hero);
	AI_GotoWP(hero,"AW_WAYTOCITY_WANDERABOUT_01");
	AI_Wait(hero,1);
	AI_StandupQuick(hero);
	AI_Output(hero,hero,"MON_Hero_Start_15_00");	//Эта дорога подсказывает мне, что я в России.
	AI_Output(hero,hero,"MON_Hero_Start_15_00");	//Пойду вдоль нее, она меня должна куда-нибудь вывести.
	AI_Wait(hero,1);
	AI_StandupQuick(hero);
	AI_GotoWP(hero,"AW_WAYTOCITY_WANDERABOUT_05");
	AI_Wait(hero,1);
};

func void GameIntro_Wanderabout()
{
	Wld_InsertNpc(sld_3006_Patrol,"AW_WAYTOCITY_WANDERABOUT_05_A");
	Wld_InsertNpc(sld_3007_Patrol,"AW_WAYTOCITY_WANDERABOUT_05_B");
	Wld_InsertNpc(sld_3008_PatrolCapt,"AW_WAYTOCITY_WANDERABOUT_05_C");
};
