/************************
 * 扑克牌结构定义
 * *********************/
#ifndef CARD_H
#define CARD_H

//扑克牌点数枚举，1-13分别为扑克牌中的3,4...K，A,2
enum CardValue
{
    Card_Begin = 0,
    Card_A = 1,
    Card_2 = 2,
    Card_3 = 3,
    Card_4 = 4,
    Card_5 = 5,
    Card_6 = 6,
    Card_7 = 7,
    Card_8 = 8,
    Card_9 = 9,
    Card_10 = 10,
    Card_J = 11,
    Card_Q = 12,
    Card_K = 13,

    Card_Black_Joker = 53,
    Card_Red_Joker = 54,
    Card_End
};

//扑克牌四种花色枚举
enum CardSuit
{
    Suit_Begin = 0,
    Diamond = 1,  //方片
    Club = 2,   //梅花
    Heart = 3,  //红桃
    Spade = 4,  //黑桃
    Suit_End
};
 //单张牌的类,用值和花色表示牌
class Card
{
public:
    Card(CardValue v = Card_Begin, CardSuit s = Suit_Begin); //按照牌值和花色构造
    Card(int Number); //按照序号构造，1-13方片3-A, 14-26 梅花3-A, 27-39 红桃3-A, 40-52黑桃3-A,53\54小\大王
    Card(const Card& c); //拷贝构造函数
    void SetValue(const CardValue &v); //设置点数
    void SetSuit(const CardSuit &s);  //设置花色
    CardValue GetValue()const;   //获取点数
    CardSuit GetSuit()const;     //获取花色
protected:
    CardValue Value;  //点数
    CardSuit Suit;  //花色
};

#endif // CARD_H
