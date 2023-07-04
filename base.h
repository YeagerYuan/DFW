#ifndef BASE_H
#define BASE_H

// 地图大小
#define MAPSIZE 70


#define FAIL -1
#define SUCCESS 0

typedef enum ITEMTYPE_ {
    NONE,
    BOMB,
    BLOCK,
    ROBOT,
}ITEMTYPE;


typedef struct MOVING_ENCOUNTERED_ {
    int been_bombed;
    int been_blocked;
    int blocked_pos;
    int bombed_pos;
}MOVING_ENCOUNTERED;

typedef enum HOUSETYPE_
{
    STPOINT,      // 起点
    LAND,         // 空地
    PROPHOUSE,    // 道具屋
    GIFTHOUSE,    // 礼品屋
    MAGICHOUSE,   // 魔法屋
    HOSPITAL,     // 医院
    JAIL,         // 监狱
    MINERALFILED, // 矿地
} HOUSETYPE;

typedef struct LOCATION_{
    int houseID;
    struct LOCATION_ *next;
}LOCATION;


/*
 *每个地图块的结构体定义
 */

typedef struct MAPBLOCK_
{
    int HouseOwnerId;    // 房产主人Id
    char house_flag;     // 房屋表示
    int HouseLevel;      // 房屋等级
    HOUSETYPE HouseType; // 房屋类型
    int MapValue;        // 当前地块价值
    ITEMTYPE ItemType;       // 道具类型
    int PlayerId;        // 站在当前位置的玩家，如果没有玩家就是-1
} MAPBLOCK;

/*
 *每个玩家的结构体定义
 */
typedef struct PLAYER_
{
    int PlayerId;         // 用户ID
    char *Name;           // 用户名称
    int CurPos;           // 当前位置
    int LastPos;          // 上一轮的位置
    LOCATION *HouseId;         // 名下房产位置
    int BombNum;          // 炸弹数量
    int RobotNum;         // 机器娃娃数量
    int BlockNum;         // 路障数量
    int SleepTime;        // 跳过轮数
    int Money;            // 可移动资产
    int Point;            // 点数
    struct PLAYER_ *next; // 下一个玩家
    // 固定资产待定
} PLAYER;

/*
 *游戏状态的记录
 */
typedef struct GAME_
{
    int play_num;           // 玩家数量
    PLAYER *current_player; // 当前玩家
    PLAYER *player;         // 指向玩家的指针
    char path[50];          // 存档路径
    MAPBLOCK map[MAPSIZE];  // 地图
} GAME;
//static GAME game;
#endif
