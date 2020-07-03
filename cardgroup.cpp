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

//获得卡组
vector <CardDdz> CardGroup::getGroup()const{
    return m_cardset;
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

    //王不能出现在顺子中
    if(maxvalue==53 || maxvalue==54)
        return false;
    //2不能出现在顺子中
    if(m_cardset[0].GetValue()==2||m_cardset[1].GetValue()==2)
        return false;
    //判断最后一张牌是否满足条件(由于CardValue的定义，需要额外考虑A)
    if(m_cardset[0].GetValue()==CardValue::Card_A){ //如果最大点数为A
        if(m_cardset[N-1].GetValue()!=CardValue::Card_K) //排序后最后一张牌应该是K
            return false;
    }
    else{ //判断按点数升序排序后第一张是否合法(第一张不是A的情况）
        if(m_cardset[1].GetValue()-m_cardset[0].GetValue()!=1)
            return false;
    }

    //判断后面N-1张的大小关系是否满足顺子条件
    for(unsigned int i=1; i<N-1; i++){
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
    //王不能出现在连对中
    if(maxvalue==53||maxvalue==54)
        return false;
    //2不能出现在连对中
    if(m_cardset[0].GetValue()==2||m_cardset[2].GetValue()==2)
        return false;
    if(m_cardset[0].GetValue()==CardValue::Card_A){ //如果最大点数为A
        if(m_cardset[N-1].GetValue()!=13) //按点数排序后最后一对应为K""
            return false;
    }
    else{ //保证第一对牌的合法性
        if(m_cardset[2].GetValue()-m_cardset[1].GetValue()!=1)
            return false;
    }
    //判断后面N-1对牌
    unsigned int m=(N-2)/2;
    for(unsigned int i=1; i<=m; i++){
        if(m_cardset[i*2].GetValue()-m_cardset[i*2+2].GetValue()!=-1)
            return false;
    }
    return true;
}

//判断牌型是否为"三张"
bool CardGroup::isTriple()const{
    unsigned int N=m_cardset.size();
    return (N==3&&m_cardset[0]==m_cardset[1]&&m_cardset[1]==m_cardset[2]);
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
    //修改：四人斗地主中王炸类型为四张王，并非两张王
    unsigned int N=m_cardset.size();
    if(N!=4) //张数
        return false;
    //牌型
    for(int i =0; i< 4; i++)
    {
        if(m_cardset[i] != 53 || m_cardset[i] != 54)
            return false;
    }
    return true;
}

//获得牌组的类型
HandType_DDZ CardGroup::getHandType(){
    if(isPass()){
        m_cardgrouptype = HandType_DDZ::Pass;  //过
        return HandType_DDZ::Pass;
    }
    else if(isSingle()){
        m_cardgrouptype = HandType_DDZ::Single; //单
        return HandType_DDZ::Single;
    }
    else if(isSeqSingle(true)){
        m_cardgrouptype = HandType_DDZ::SeqSingle; //顺子
        return HandType_DDZ::SeqSingle;
    }
    else if(isPair()){
        m_cardgrouptype = HandType_DDZ::Pair; //对
        return HandType_DDZ::Pair;
    }
    else if(isSeqPair()){
        m_cardgrouptype = HandType_DDZ::SeqPair; //连对
        return HandType_DDZ::SeqPair;
    }
    else if(isTriple()){
        m_cardgrouptype = HandType_DDZ::Triple; //三张
        return HandType_DDZ::Triple;
    }
    else if(isPlane()){
        m_cardgrouptype = HandType_DDZ::Plane; //三顺
        return HandType_DDZ::Plane;
    }
    else if(isTriplePair()){
        m_cardgrouptype = HandType_DDZ::TriplePair; //三带二
        return HandType_DDZ::TriplePair;
    }
    else if(isPlanePair()){
       m_cardgrouptype = HandType_DDZ::PlanePair; //飞机带翅膀
        return HandType_DDZ::PlanePair;
    }
    else if(isBomb()){
        m_cardgrouptype = HandType_DDZ::Bomb; //炸弹
        return HandType_DDZ::Bomb;
    }
    else if(isBombJoker()){
       m_cardgrouptype = HandType_DDZ::BombJoker; //王炸
        return HandType_DDZ::BombJoker;
    }
    else{
        m_cardgrouptype = HandType_DDZ::Unknown;//未知
        return HandType_DDZ::Unknown;
    }
}

//设置牌组的类型
void CardGroup::setHandType(HandType_DDZ type){
    m_cardgrouptype = type;
}

//获得牌组张数
int CardGroup::getGroupCount()const{
    return m_cardset.size();
}

//设置张数
void CardGroup::setGroupCount(int cnt){
    m_count = cnt;
}

//计算优先级
int CardGroup::priority(HandType_DDZ type){
    switch (type) {
        //优先级应该有王炸>8炸>7炸>6炸>5炸>4炸
        case HandType_DDZ::BombJoker:
            return 9;
        case HandType_DDZ::Bomb:
            return m_cardset.size();
        case HandType_DDZ::Pass:
            return 2;
        case HandType_DDZ::Unknown:
            return 1;
    default: return 3;
    }
}

//牌型代表的点数
int CardGroup::representPoint(HandType_DDZ type){
    switch ((int)type) {
    case HandType_DDZ::Pass: //过
        return CardValue::Card_Begin;
    case HandType_DDZ::Single: //单
    {
        if(m_cardset[0].GetValue()==CardValue::Card_A)
            return 14;
        else if(m_cardset[0].GetValue()==CardValue::Card_2)
            return 15;
        return m_cardset[0].GetValue();
    }
    case HandType_DDZ::SeqSingle: //顺子
    {
        std::sort(m_cardset.begin(),m_cardset.end());//升序排序
        unsigned int N=m_cardset.size();
        if(m_cardset[0].GetValue()==CardValue::Card_A){ //如果最大牌是A
            return 14;
        }
        else return m_cardset[N-1].GetValue();
    }
    case HandType_DDZ::Pair: //对子
    {
        if(m_cardset[0].GetValue()==CardValue::Card_A)
            return 14;
        else if(m_cardset[0].GetValue()==CardValue::Card_2)
            return 15;
        return m_cardset[0].GetValue();
    }
    case HandType_DDZ::SeqPair: //连对
    {
        std::sort(m_cardset.begin(),m_cardset.end());//升序排序
        unsigned int N=m_cardset.size();
        if(m_cardset[0].GetValue()==CardValue::Card_A){
            return 14;
        }
        else return m_cardset[N-1].GetValue();
    }
    case HandType_DDZ::Triple: //三
    {
        if(m_cardset[0].GetValue()==CardValue::Card_A)
            return 14;
        else if(m_cardset[0].GetValue()==CardValue::Card_2)
            return 15;
        return m_cardset[0].GetValue();
    }
    case HandType_DDZ::TriplePair: //三带二
    {
        unsigned int count[54];
        unsigned int n=m_cardset.size();
        for(unsigned int i=0;i<n;i++) //获得每种点数的牌有多少张
            count[m_cardset[i].GetValue()]++;
        CardGroup cg_tri;
        for(unsigned int i=0;i<n;i++){
            if(count[m_cardset[i].GetValue()]==3)
                cg_tri.addOne(m_cardset[i]);
        }
        return cg_tri.representPoint(HandType_DDZ::Triple);
    }
    case HandType_DDZ::Plane: //飞机
    {
        std::sort(m_cardset.begin(),m_cardset.end());//升序排序
        unsigned int N=m_cardset.size();
        if(m_cardset[0].GetValue()==CardValue::Card_A){
            return 14;
        }else
            return m_cardset[N-1].GetValue();
    }
    case HandType_DDZ::PlanePair: //飞机带翅膀
    {
        unsigned int cnt[54]={0};
        unsigned int N=m_cardset.size();
        for(unsigned int i=0;i<N;i++){
            cnt[m_cardset[i].GetValue()]++;
        }
        CardGroup cg;
        for(unsigned int i=0;i<N;i++){
            if(cnt[m_cardset[i].GetValue()]==3){
                cg.addOne(m_cardset[i]);
            }
        }
        return cg.representPoint(HandType_DDZ::Plane);
    }
    case HandType_DDZ::Bomb: //炸弹
        return m_cardset[0].GetValue();
    case HandType_DDZ::BombJoker: //王炸
        return 53;
    }
    return CardValue::Card_Begin;
}

//设置牌组的价值
void CardGroup::setGroupValue(int value){
    m_value = value;
}

//比较两个牌型，返回比较结果
CardGroup::CompareResult CardGroup::compareTo(CardGroup &comgroup){
    HandType_DDZ thisType=getHandType();
    HandType_DDZ comType=comgroup.getHandType();
    int thisPriority=priority(thisType);
    int comPriority=priority(comType);
    //返回比较结果
    if(thisPriority>comPriority) //优先级大
        return CompareResult::Larger;
    else if(thisPriority<comPriority) //优先级小
        return CompareResult::Smaller;
    else if(thisPriority==comPriority){ //优先级相同
        //优先级相同时，这里都不是炸弹，需要类型相同才可比较
        if(thisType != comType)
            return CompareResult::NotMatch;
        int thisValue = representPoint(thisType);
        int comValue = comgroup.representPoint(comType);
        if(thisValue>comValue) //点数大
            return CompareResult::Larger;
        if(thisValue<comValue) //点数小
            return CompareResult::Smaller;
        return CompareResult::Equal; //点数也一样
    }
    return CompareResult::NotMatch; //无法比较
}
