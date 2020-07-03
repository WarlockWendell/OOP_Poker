#ifndef PUTCARDS_H
#define PUTCARDS_H

#include <QObject>
#include "landlordoffline.h"
#include "clock.h"
#include "mypushbutton.h"
#include "handcards_landlord.h"
#include <ctime>
#include <cstdlib>
void PutCards(LandlordOffline& l);
void PutSelf(LandlordOffline& l);
void PutRight(LandlordOffline& l);
void PutLeft(LandlordOffline& l);
void PutOpposite(LandlordOffline& l);
std::vector<CardDdz> GetSelectedCards(LandlordOffline& l);
#endif // PUTCARDS_H
