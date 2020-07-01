#include "cardgroup.h"
#include <algorithm>
using std::sort;

CardGroup::CardGroup(){}

//拷贝卡组
void CardGroup::copyGroup(const vector<CardDdz> group){
    m_cardset = group;
}

//添加一张牌至牌组
void CardGroup::addOne(const CardDdz &card_ddz){
    m_cardset.push_back(card_ddz);
}

//判断牌型是否为"过"
bool CardGroup::isPass()const{
    return m_cardset.size()==0;
}

//判断牌型是否为"单"
bool CardGroup::isSingle()const{
   return m_cardset.size()==1;
}

//判断牌型是否为"顺子",参数顺子应为true
bool CardGroup::isSeqSingle(bool shunzi){
    unsigned int N = m_cardset.size();
    if(shunzi){ //如果要求是顺子
        if(N<5) //牌数小于5张
            return false;
    }

    sort(m_cardset.begin(), m_cardset.end());//升序排序
    int maxvalue = m_cardset[N-1].GetValue();

    //最大点数大于A的顺子不合法
    if(maxvalue==2 || maxvalue==53 || maxvalue==54)
        return false;

    //判断最后一张牌是否满足条件(由于CardValue的定义，需要额外考虑A)
    if(maxvalue==1){ //如果最大点数为A
        if(m_cardset[N-1].GetValue()!=13)
            return false;
    }
    else{
        if(m_cardset[N-2].GetValue()-m_cardset[N-1].GetValue()!=-1)
            return false;
    }

    //判断前N-1张的大小关系是否满足顺子条件
    for(unsigned int i=0; i<N-2; i++){
        if(m_cardset[i].GetValue()-m_cardset[i+1].GetValue()!=-1)
            return false;
    }
    return true; //该牌组是顺子
}

//判断牌型是否为"对子"
bool CardGroup::isPair()const{
    unsigned int N = m_cardset.size();
    return N==2&&m_cardset[0]==m_cardset[1];
}

//判断牌型是否为"连对"
bool CardGroup::isSeqPair(){
    unsigned int N = m_cardset.size();

    //判断牌的张数
    if(N<6||N%2!=0)
        return false;
    //判断是否满足"连对"两两相等的特点：AABBCC..
    std::sort(m_cardset.begin(),m_cardset.end());//升序排序
    unsigned int n = N/2;
    for(unsigned int i=0; i<n; i++){
        if(!(m_cardset[i*2]==m_cardset[i*2+1]))
            return false;
    }
    //判断是否满足"连对"连续的特点
    //判断最后一对牌是否满足条件(由于CardValue的定义，需要额外考虑A)
    int maxvalue = m_cardset[N-1].GetValue();
    if(maxvalue==1){ //如果最大点数为A
        if(m_cardset[N-3].GetValue()!=13)
            return false;
    }
    else{
        if(m_cardset[N-3].GetValue()-m_cardset[N-1].GetValue()!=-1)
            return false;
    }
    //判断前N-2对牌
    unsigned int m=(N-4)/2;
    for(unsigned int i=1; i<=m; i++){
        if(m_cardset[i*2-1].GetValue()-m_cardset[i*2].GetValue()!=-1)
            return false;
    }
    return true;
}

//判断牌型是否为"三张"
bool CardGroup::isTriple()const{
    unsigned int N=m_cardset.size();
    return (N=3&&m_cardset[0]==m_cardset[1]&&m_cardset[1]==m_cardset[2]);
}

//判断牌型是否为"三顺":555666...
bool CardGroup::isPlane(){
    unsigned int N=m_cardset.size();
    if(N<6||N%3!=0) //张数
        return false;
    std::sort(m_cardset.begin(),m_cardset.end());//升序排序
    unsigned int n=N/3;
    for(unsigned int i=0; i<n; i++){
        if(m_cardset[i*3]!=m_cardset[3*i+1]||m_cardset[i*3]!=m_cardset[3*i+2])
            return false;
    }
    CardGroup cg;
    for(unsigned int i=0; i<n; i++){
        cg.addOne(m_cardset[3*i]);
    }
    return cg.isSeqSingle(false);
}

//判断牌型是否为"三带二"：55577
bool CardGroup::isTriplePair(){
    unsigned int N=m_cardset.size();
    if(N!=5) //张数
        return false;
    std::sort(m_cardset.begin(),m_cardset.end());//升序排序
    //判断是否满足三带二特点
    //类型一：33344
    if(m_cardset[0]==m_cardset[1]&&m_cardset[1]==m_cardset[2]
            &&m_cardset[3]==m_cardset[4]&&!(m_cardset[2]==m_cardset[3]))
        return true;
    //类型二：33444
    if(m_cardset[0]==m_cardset[1]&&m_cardset[2]==m_cardset[3]
            &&m_cardset[3]==m_cardset[4]&&!(m_cardset[1]==m_cardset[2]))
        return true;
    return false;
}

//判断牌型是否为"飞机带翅膀":555666777＋99JJKK
bool CardGroup::isPlanePair()const{
    unsigned int N=m_cardset.size();
    unsigned int cnt[54]={0};
    //计算每种单牌有多少张
    for(unsigned int i=0;i<N;i++){
        cnt[m_cardset[i].GetValue()]++;
    }
    CardGroup cg1,cg2;
    //把对子放在一组新牌中
    for(unsigned int i=0; i<N; i++){
        if(cnt[m_cardset[i].GetValue()]==2) //如果是对子
                cg1.addOne(m_cardset[i]);
        else cg2.addOne(m_cardset[i]);
    }
    if(!cg2.isPlane()) return false; //判断剩下的牌是否为飞机
    return cg1.m_cardset.size()*3==cg2.m_cardset.size()*2;
}

//判断牌型是否为"炸弹"
bool CardGroup::isBomb()const{
    unsigned int N=m_cardset.size();
    if(N<4) //张数≥4
        return false;
    for(unsigned int i=1; i<=N-1; i++){
        if(!(m_cardset[i]==m_cardset[0]))
            return false;
    }
    return true;
}

//判断牌型是否为"王炸"
bool CardGroup::isBombJoker()const{
    unsigned int N=m_cardset.size();
    if(N!=2) //张数
        return false;
    //牌型
    if(m_cardset[0].GetValue()==53&&m_cardset[1].GetValue()==54)
        return true;
    if(m_cardset[0].GetValue()==54&&m_cardset[1].GetValue()==53)
        return true;
    return false;
}

//获得牌组的类型
HandType_DDZ CardGroup::getHandType(){
    if(isPass()) return HandType_DDZ::Pass;  //过
    if(isSingle()) return HandType_DDZ::Single; //单
    if(isSeqSingle(true)) return HandType_DDZ::SeqSingle; //顺子
    if(isPair()) return HandType_DDZ::Pair; //对
    if(isSeqPair()) return HandType_DDZ::SeqPair; //连对
    if(isTriple()) return HandType_DDZ::Triple; //三张
    if(isPlane()) return HandType_DDZ::Plane; //三顺
    if(isTriplePair()) return HandType_DDZ::TriplePair; //三带二
    if(isPlanePair()) return HandType_DDZ::PlanePair; //飞机带翅膀
    if(isBomb()) return HandType_DDZ::Bomb; //炸弹
    if(isBombJoker()) return HandType_DDZ::BombJoker; //王炸
    return HandType_DDZ::Unknown;//未知
}

//计算优先级
int CardGroup::priority(HandType_DDZ type){
    switch (type) {
        case HandType_DDZ::BombJoker:
            return 5;
        case HandType_DDZ::Bomb:
            return 4;
        case HandType_DDZ::Pass:
            return 2;
        case HandType_DDZ::Unknown:
            return 1;
    default: return 3;
    }
}
