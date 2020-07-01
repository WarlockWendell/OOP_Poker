/************************
 * 斗地主牌组定义
 * *********************/
#ifndef CARDGROUP_H
#define CARDGROUP_H

#include "card_ddz.h"
#include <QString>
#include <vector>
using std::vector;

//牌型类别
class CardGroup
{
public:
    enum CompareResult
    {
        Smaller, //小
        Equal, //一样
        Larger, //大
        NotMatch, //不匹配
    };
    CardGroup(); //构造
    void copyGroup(const vector<CardDdz> group); //拷贝卡组
    void addOne(const CardDdz &card_ddz); //添加一张牌至牌组
    //HandType_DDZ getHandType(); //获得牌组的类型
    static int priority(HandType_DDZ type); //计算优先级，属于整个类的静态成员函数
    
    //待完善
    CompareResult compareTo(CardGroup &combine); //比较两个牌型，返回比较结果
    CardValue representPoint(HandType_DDZ type); //牌型代表的点数
   


private:
    vector<CardDdz> m_cardset;

    //若牌型符合条件，返回true
    bool isPass()const; //过
    bool isSingle()const; //单
    bool isSeqSingle(bool shunzi); //顺子
    bool isPair()const; //对
    bool isSeqPair(); //连对
    bool isTriple()const; //三张
    bool isPlane(); //三顺
    bool isTriplePair(); //三带二
    bool isPlanePair()const; //飞机带翅膀
    bool isBomb()const; //炸弹
    bool isBombJoker()const; //王炸

};

#endif // CARDGROUP_H
