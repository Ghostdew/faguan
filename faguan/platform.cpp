#include "platform.h"
#include "player.h"
#include <QDebug>
bool cmp( Player &a , Player &b)
{
    return a.Get_Position()<b.Get_Position();
}//比较函数


Platform::Platform()//根据玩家人数初始化平台数据
{
    playernum = 0;//默认人数为0
    status = 0;//默认第一天发言阶段
    night_type = 0;//夜晚属性初始化（默认红夜）
}

void Platform::init(int num)
{
    Set_Playernum(num);
    srand(time(NULL));//利用1-n随机数分配每人身份存入玩家容器
    std::vector<int>Temp;
    for(int i=1;i<=num;i++)
    {
        int key;
        Temp.push_back(key);
        while(!Temp.empty())
        {
            key =rand()%num+1;
            if(find(Temp.begin(),Temp.end(),key)==Temp.end())
            {
               Temp.push_back(key);
               break;
            }
        }
        if(i<=num/2)
        {
            Player a(1,key);
            this->P.push_back(a);
        }else
        {
            Player b(0,key);
            this->P.push_back(b);
        }
    }
    std::sort(P.begin(),P.end(),cmp);//玩家容器初始化，按pos进行顺序排序
}

int Platform::Get_Night()//获取夜晚类型
{
    return night_type;
}

int Platform::Get_PlayerNum()//获取玩家人数
{
    return playernum;
}

int Platform::GameOver()//判断游戏是否结束 0未结束 1结束，红牌玩家胜利，2结束，黑牌玩家胜利
{
    if(Get_IdNumber(0)==0)
    {
        return 2;
    }
    if(Get_IdNumber(1)==0)
    {
        return 1;
    }
    return 0;
}

int Platform::Get_Openeyes(int id)//获取对应阵营夜晚睁眼人数
{
    int count = 0;
    for(unsigned int i=0;i<P.size();i++)
    {
        if(P[i].Get_Openeye()&&P[i].Get_Id()==id)
            count++;
    }
    return count;
}

int Platform::Get_IdNumber(int id)//获取对应阵营当前存活人数
{
    int count = 0;
    for(unsigned int i=0;i<P.size();i++)
    {
        if((!P[i].Get_Death())&&(P[i].Get_Id()==id))
            count++;
    }
    return count;
}

void Platform::Change_Night()//改变夜晚类型
{
    srand(time(NULL));
    int temp = rand()%2;
    night_type=temp;
}

void Platform::Set_Playernum(int num)
{
    playernum = num;
}

int Platform::Choose_Leader(int pos)//村长死亡后选择另外一人成为村长
{
    P[pos-1].Change_Leader();
    return pos;
}









