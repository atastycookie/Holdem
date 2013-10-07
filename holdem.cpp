#include "stdafx.h"
#include "card.h"
#include "player.h"
#include "party.h"
#include "desk.h"
 
 
int _tmain(int argc, _TCHAR* argv[])
{
try
{
int number;
std::cout<<"Please enter number of players"<<std::endl<<"Number of players=";
std::cin>>number;
if (number==0) return 0;
desk newdesk(number);
bool flag;
do
{
flag=newdesk.get_bets();
} while (!flag);
}
catch(const std::runtime_error& err)
{
std::cout<<err.what();
}
}


