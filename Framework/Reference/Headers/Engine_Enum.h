#pragma once



enum COLLISION_GROUP { CHARACTER = 1, MONSTER, PROP, NPC, PORTAL, ANIMAL, TRIGGER, DEFENCE_TOWER, CURLINGGAME_PROP, RESPAWN,
	PLANE_BODY, PLANE_PROJECTILE, PLANEENEMY_BODY, ENEMY_PROJECTILE, PLANE_ITEM, GROUP_END };
static const char* g_szColliderAttackType[7] = { "WEAK", "STRONG", "BOUND", "AIR_BORNE",  "BLOW", "IF_DEAD_BLOW", "STUN" };

enum OBJ_TYPE
{
	OBJ_ANIMAL,
	OBJ_BACKGROUND,
	OBJ_BUILDING,
	OBJ_CAMERA,
	OBJ_CHARACTER,
	OBJ_CHARACTER_PROJECTILE,
	OBJ_CHARACTER_SKILL,
	OBJ_WEAPON,
	OBJ_PLAYER,
	OBJ_MONSTER,
	OBJ_PROP,
	OBJ_EFFECT,
	OBJ_PARTICLE,
	OBJ_TERRAIN,
	OBJ_UI,
	OBJ_DUMMY,
	OBJ_GROUND,
	OBJ_TRAIL,
	OBJ_TREEROCK,
	OBJ_GRASS,
	OBJ_NPC,
	OBJ_DYNAMIC,
	OBJ_SPAWNER,
	OBJ_SKY,
	OBJ_TRIGGER,
	OBJ_PORTAL,
	OBJ_WATER,
	OBJ_BOSS,
	OBJ_MONSTER_PROJECTILE,
	OBJ_DEFENCE_TOWER,
	OBJ_VEHICLE,
	OBJ_QUESTSPOT,
	OBJ_CURLINGGAME_PROP,
	OBJ_MINIGAME_STRUCTURE,
	OBJ_RESPAWN,
	OBJ_GRANDPRIX_ENEMY,
	OBJ_GRANDPRIX_ENEMY_PROJECTILE,
	OBJ_GRANDPRIX_CHARACTER,
	OBJ_GRANDPRIX_CHARACTER_PROJECTILE,
	OBJ_END
};

enum class TIMER_TYPE
{
	DEFAULT,
	GAME_PLAY,
	UI,
	TIMER_END
};

enum PHYSX_RIGID_TYPE
{
	STATIC,
	DYNAMIC,
	GROUND,
	RIGID_TYPE_END,
};

enum PHYSX_COLLIDER_TYPE
{
	BOX,
	SPHERE,
	MESH,
	COLLIDER_TYPE_END,
};



enum class KEY_STATE
{
	NONE,
	TAP,
	HOLD,
	AWAY
};

enum class KEY
{
	LEFT_ARROW,
	RIGHT_ARROW,
	UP_ARROW,
	DOWN_ARROW,
	TAB,
	OPEN_SQUARE_BRACKET,
	CLOSE_SQUARE_BRACKET,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	INSERT, DEL, HOME, END_KEY, PAGE_UP, PAGE_DOWN,
	NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	ALT, CTRL, SHIFT,
	SPACE, ENTER, ESC,
	LBTN, RBTN,
	TYPE_END
};

enum DIMK { DIMK_LBUTTON, DIMK_RBUTTON, DIMK_WHEEL, DIMK_X, DIMK_END };
enum DIMM { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };


enum ELEMENTAL_TYPE { FIRE, WATER, WOOD, LIGHT, DARK, BASIC, ELEMENTAL_END };
namespace Engine
{

#pragma region Channel ID 

	enum CHANNELID /* enum 수정시 아래 문자열도 순서에 맞게 교체 필요 */
	{
		SOUND_UI,
		SOUND_UI2,
		SOUND_VOICE_CHARACTER,
		SOUND_VOICE_MONSTER1,
		SOUND_VOICE_MONSTER2,
		SOUND_VOICE_MONSTER3,
		SOUND_SKILL,
		SOUND_ATTACK,
		SOUND_CHARACTER_HIT,
		SOUND_MONSTERL_HIT,
		SOUND_FOOT_CHARACTER,
		SOUND_FOOT_MONSTER,
		SOUND_BGM_CURR,
		SOUND_SHAKE,
		SOUND_JUMP,
		SOUND_BOSS,
		SOUND_VOICE_NPC,
		SOUND_VOICE_ANIMAL,
		SOUND_CLOTH,
		SOUND_VEHICLE,
		SOUND_AIRPLANE,
		SOUND_BGM_NEXT,
		SOUND_CUTSCENE,
		SOUND_VOICE_ANIMAL2,
		SOUND_VOICE_ANIMAL3,
		MAXCHANNEL
	};

	static const char* szChannelIDNames[CHANNELID::MAXCHANNEL]
	{
		"SOUND_UI",
		"SOUND_UI2",
		"SOUND_VOICE_CHARACTER",
		"SOUND_VOICE_MONSTER1",
		"SOUND_VOICE_MONSTER2",
		"SOUND_VOICE_MONSTER3",
		"SOUND_SKILL",
		"SOUND_ATTACK",
		"SOUND_CHARACTER_HIT",
		"SOUND_MONSTER_HIT",
		"SOUND_FOOT_CHARACTER",
		"SOUND_FOOT_MONSTER",
		"SOUND_BGM_CURR",
		"SOUND_SHAKE",
		"SOUND_JUMP",
		"SOUND_BOSS",
		"SOUND_VOICE_NPC",
		"SOUND_VOICE_ANIMAL",
		"SOUND_CLOTH",
		"SOUND_VEHICLE",
		"SOUND_AIRPLANE",
		"SOUND_BGM_NEXT",
		"SOUND_CUTSCENE"
	};

#pragma endregion


#pragma region Animation Event
	enum ANIM_EVENT_TYPE			{ SOUND, EFFECT, CAMERA, COLLIDER, ANIM_EVENT_TYPE_END};

	enum ANIM_EVENT_SOUND_TYPE		{ PLAY_SOUND, ANIM_EVENT_SOUND_TYPE_END };
	enum ANIM_EVENT_EFFECT_TYPE		{ GENERATE, ANIM_EVENT_EFFECT_TYPE_END };

	static const char* szAnimEventTypeNames[ANIM_EVENT_TYPE::ANIM_EVENT_TYPE_END]{ "Sound", "Effect", "Camera", "Collider" };
	static const wstring strAnimEventTypeNames[ANIM_EVENT_TYPE::ANIM_EVENT_TYPE_END]{ L"Sound", L"Effect", L"Camera", L"Collider" };

	static const char* szAnimEventSoundTypeNames[ANIM_EVENT_SOUND_TYPE::ANIM_EVENT_SOUND_TYPE_END]{ "Play Sound" };
	static const wstring strAnimEventSoundTypeNames[ANIM_EVENT_SOUND_TYPE::ANIM_EVENT_SOUND_TYPE_END]{ L"Play Sound" };

	static const char* szAnimEventEffectTypeNames[ANIM_EVENT_EFFECT_TYPE::ANIM_EVENT_EFFECT_TYPE_END]{ "Generate" };
	static const wstring strAnimEventEffectTypeNames[ANIM_EVENT_EFFECT_TYPE::ANIM_EVENT_EFFECT_TYPE_END]{ L"Generate" };

#pragma endregion

/* 아이템 코드 */
	enum ITEM_CODE
	{
#pragma region Sword = 0
		SWORD_CHRISTMAS = 0,
		SWORD_CROSSSERVER,
		SWORD_DARKNESS,
		SWORD_DARKNESS02,
		SWORD_FIRE,
		SWORD_FIRE02,
		SWORD_FISH,
		SWORD_FLOWER,
		SWORD_FOOD,
		SWORD_LIGHT,
		SWORD_LIGHT02,
		SWORD_QQCYBERPUBKS,
		SWORD_QQSUITS,
		SWORD_RAIDCYBORGX,
		SWORD_WATER,
		SWORD_WATER02,
		SWORD_WOOD,
		SWORD_WOOD02,
#pragma endregion

#pragma region Hammer 18
		HAMMER_CHRISTMAS,
		HAMMER_CROSSSERVEVR,
		HAMMER_DARKNESS,
		HAMMER_DARKNESS02,
		HAMMER_FIRE,
		HAMMER_FIRE02,
		HAMMER_FISH,
		HAMMER_FLOWER,
		HAMMER_FOOD,
		HAMMER_FOOL02,
		HAMMER_LIGHT,
		HAMMER_LIGHT02,
		HAMMER_QQSUITS,
		HAMMER_RAIDCYBORGX,
		HAMMER_WATER,
		HAMMER_WATER02,
		HAMMER_WOOD,
		HAMMER_WOOD02,
#pragma endregion

#pragma region Gauntlet 36
		GAUNTLET_CHRISTMAS,
		GAUNTLET_CROSSSEVER,
		GAUNTLET_FISH,
		GAUNTLET_FLOWER,
		GAUNTLET_FOOD,
		GAUNTLET_QQCYBERPUNKS,
		GAUNTLET_QQSUITS,
		GAUNTLET_RAIDCYBORGX,
		GAUNTLET_T04,
		GAUNTLET_T04RARE,
#pragma endregion

#pragma region Rifle

#pragma region Helmet = 46
		HELMET_HARD,
		HELMET_MYSTIC,
		HELMET_IMMORTAL,
#pragma endregion

#pragma region Armor = 49
		ARMOR_STRONG,
		ARMOR_IMPERIAL,
		ARMOR_CELESTIAL,
#pragma endregion

#pragma region ETC = 52
		CONSUMPSION_HP,
		CONSUMPSION_ENERGY,
		CONSUMPSION_EXP,
		CONSUMPSION_MILK,
		CONSUMPSION_PUDDING,
		CONSUMPSION_BREAD,
		CONSUMPSION_GOLD = 99,
#pragma endregion

#pragma endregion
		CODE_END
	};

}