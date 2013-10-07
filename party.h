#pragma once
class party
{
public:
party(int number);
~party();
player* next_in_game();           //return pointer to next in game player
player * nn_allin();              //return pointer to next not "all in" player
int alive_in_party();             //return number of alive players in game
bool bets_equality();             //return true if bets are equal
void clean_from_poor();           //remove from party players with 0 cash
void reset();
player * next();                      //moves playerptr to nex player in party
player * next(player * man);
player * playerptr;
int players_in_party;
private:
player * tparty;              
int current_player_number;
};
party::party(int number)
{
tparty= new player[number];
playerptr=tparty;
players_in_party=number;
current_player_number=0;
tparty[0].is_alive=true;
}
party::~party()
{
delete [] tparty;
}
player * party::next()
{
if(current_player_number<players_in_party-1)
{
current_player_number++;
playerptr++;
}
else
{
current_player_number=0;
playerptr=tparty;
}
return playerptr;
}
player* party::next_in_game()
{
int i=0;
do
{
i++;
next();
if(i==players_in_party) return nullptr;
} while (!(playerptr->in_game));
return playerptr;
}
player * party::nn_allin()
{
int detector=current_player_number;
do
{
next_in_game();
if (current_player_number==detector)
{
return nullptr;
}
} while (playerptr->is_all_in);
return playerptr;
}
int party::alive_in_party()
{
int souls=0;
for (int i = 0; i < players_in_party; i++)
{
if(playerptr->is_alive) souls++;
next();
}
next();
return souls;
}
bool party::bets_equality()
{
double Tbet=nn_allin()->bet;
bool flag(true);
for (int i = 0; i < players_in_party; i++)
{
if (playerptr->in_game && playerptr->bet!=Tbet)
{
flag=false;
}
next();
}
next();
return flag;
}
void party::clean_from_poor()
{
int num=0;
for (int i = 0; i < players_in_party; i++)
{
if(playerptr->cash) num++;
playerptr->throwcards();
next();
}
player * tptr=new player[num];
num=0;
for (int i = 0; i < players_in_party; i++)
{
if(playerptr->cash)
{
tptr[num]=*playerptr;
num++;
}
next();
}
delete[] tparty;
tparty=tptr;
players_in_party=num;
}
void party::reset()
{
playerptr=tparty;
current_player_number=0;
}
player * party::next(player * man)
{
while (playerptr!=man) next();
return next();
}
#pragma once
class desk
{
public:
desk(int number);
~desk(){}
bool opencard(); //return 0 if succesful
bool set_max_bet(double min_bet); //return 0 if succesful
void renew_cards();              
bool get_bets();
private:
card cards_on_table[5];
card * for_players[5];
int visible_cards;
double minimal_bet;
double cur_max_bet;
party  players;
player * button;
double bank;      //It is cash on table
int round;        //0 if preflop, 1 if flop, 2 if turn, 3 if reaver
};
desk::desk(int number):cards_on_table(),players(number) //initialaize massive of cards on table
{
visible_cards=0;
minimal_bet=10;
cur_max_bet=10;
bank=0;
round=0;
button=players.playerptr;
for (int i = 0; i < 5; i++)
{
for_players[i]=&cards_on_table[i];
}
}
bool desk::opencard()
{
if(visible_cards==0) {visible_cards=3; return 0;}
else if(visible_cards<5) {visible_cards++; return 0;}
else return 1;
}
bool desk::set_max_bet(double newbet)
{
if(newbet<=0 || newbet<cur_max_bet) return true;
else
{
cur_max_bet=newbet;
return 0;
}
}
bool desk::get_bets()
{
int delta=0;
delta=button->check(minimal_bet);
bank+=delta;
set_max_bet(delta);
button=players.next(button);
delta=button->check(minimal_bet*2);
bank+=delta;
set_max_bet(delta);
bool ff=false;
do
{
do
{
if (players.nn_allin()!=nullptr)
{
bank+=players.playerptr->do_bet(cur_max_bet,visible_cards,for_players,bank);
set_max_bet(players.playerptr->bet);
}
else 
{
ff=true;
break;
}
} while (players.bets_equality()!=true);
if(ff==true) break;
} while (!opencard());
bool tie=false;
int winnercomb=0;
players.reset();
player * winner=nullptr;
for (int i = 0; i < players.players_in_party; i++)
{
if (players.playerptr->bestcomb(cards_on_table)>winnercomb)
{
winnercomb=players.playerptr->bestcomb(cards_on_table);
winner=players.playerptr;
}
players.next();
}
for (int i = 0; i < players.players_in_party; i++)
{
if (players.playerptr->bestcomb(cards_on_table)==winnercomb && players.playerptr!=winner)
{
tie=true;
}
}
if (tie)
{
int number_of_winners=0;
std::vector<player*> winners(2);
for (int i = 0; i < players.players_in_party; i++)
{
if (players.playerptr->bestcomb(cards_on_table)==winnercomb)
{
number_of_winners++;
winners.push_back(players.playerptr);
}
}
for each (player* iter in winners)
{
iter->get_bank(bank/number_of_winners);
}
}
else
{
winner->get_bank(bank);
}
bank=0;
players.clean_from_poor();
if (!players.alive_in_party())
{
std::cout<<"End Game";
return true;
}
std::cout<<"whant to play more? y/n"<<std::endl;
char tmp;
std::cin>>tmp;
if(tmp='y')
{
for (int i = 0; i < players.players_in_party; i++)
{
players.playerptr->reset();
players.next();
}
round=0;
renew_cards();
for (int i = 0; i < 5; i++)
{
for_players[i]=&cards_on_table[i];
}
return false;
}
else return true;
}
void desk::renew_cards()
{
for (int i = 0; i < 5; i++)
{
(cards_on_table[i]).~card();
}
for (int i = 0; i < 5; i++)
{
cards_on_table[i].card::card();
}
}


