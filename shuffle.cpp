#include "shuffle.h"
#include <vector>
#include <ctime>
#include <cstdlib>
std::vector<int> GetCards(int Len)
{
    std::vector<int> cards;
    for (int i = 1; i <= Len; i++)
        cards.push_back(i);
    srand((unsigned int)(time(NULL)));
    for(int j =0; j<3; j++)
        for (int i = 0; i < Len; i++)
        {
            int index = rand() % Len;
            int temp = cards[i];
            cards[i] = cards[index];
            cards[index] = temp;
        }  //只洗了一次牌，可以使用for循环添加洗牌的次数
    return cards;
}

