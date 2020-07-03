/***********************
 * 叫地主
 * ********************/
#ifndef CALLLANDLORD_H
#define CALLLANDLORD_H

#include "landlordoffline.h"
#include "clock.h"
#include "mypushbutton.h"
#include <ctime>
#include <cstdlib>
void CallLandlord(LandlordOffline& l);
void CallSelf(LandlordOffline& l);
void CallRight(LandlordOffline& l);
void CallLeft(LandlordOffline& l);
void CallOpposite(LandlordOffline& l);

#endif // CALLLANDLORD_H
