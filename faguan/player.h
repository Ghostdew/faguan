#ifndef PLAYER_H
#define PLAYER_H


class Player
{
    int id;//身份 0为红，1为黑
    int position; //1-n中一个数，表示玩家座位号
    bool is_leader;//玩家是否为村长
    bool is_dead;//玩家是否死亡
    bool is_openeye;//玩家晚上是否睁眼
public:
    Player(int id,int pos)
    {
        this->id = id;
        this->position = pos;
        this->is_dead = false;
        this->is_leader = false;
        this->is_openeye = false;
    }
    int Get_Id(){return this->id;}
    int Get_Position(){return this->position;}
    bool Get_Death();
    bool Get_Openeye();
    bool Get_Leader();
    void Change_Death();
    void Change_Openeye();
    void Change_Leader();
};

#endif // PLAYER_H
