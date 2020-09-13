#ifndef PLATFORM_H
#define PLATFORM_H
#include<vector>
#include<time.h>
#include<algorithm>


class Player;


class Platform
{
    int playernum;//玩家人数
    int status;//当前状态 0为第一天村长选举阶段，1为白天发言阶段，2为白天放逐阶段，3为夜晚
    int night_type;//夜晚类型 0为红夜，1为黑夜
public:
    Platform();
    std::vector<Player>P;//玩家容器
    int Get_Night();//获取夜晚类型
    int Get_PlayerNum();//获取玩家人数
    int GameOver();//判断游戏是否结束 0未结束 1结束，红牌玩家胜利，2结束，黑牌玩家胜利
    int Get_Openeyes(int id);//获取对应阵营夜晚睁眼人数
    int Get_IdNumber(int id);//获取对应阵营当前存活人数
    int Choose_Leader(int pos);//村长死亡后选择另外一人成为村长
    void init(int num);//初始化
    void Set_Playernum(int num);
    void Change_Night();//改变夜晚类型


};

#endif // PLATFORM_H
