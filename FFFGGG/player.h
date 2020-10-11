#ifndef PLAYER_H
#define PLAYER_H


class Player
{
    int id;//身份 0为红，1为黑
    int position; //1-n中一个数，表示玩家座位号
    float voted;//每个玩家被投票数
    bool is_leader;//玩家是否为村长
    bool is_dead;//玩家是否死亡
    bool is_openeye;//玩家晚上是否睁眼
public:
    Player(int id,int pos)
    {
        this->id = id;
        position = pos;
        voted = 0;
        is_dead = false;
        is_leader = false;
        is_openeye = false;
    }
    void Set_Voted(float num);
    int Get_Id(){return id;}
    int Get_Position(){return position;}
    float Get_voted();
    bool Get_Death();
    bool Get_Openeye();
    bool Get_Leader();
    void Change_Death();
    void Change_Openeye();
    void Change_Leader();
};

#endif // PLAYER_H
