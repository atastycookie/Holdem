#pragma once
//#define _CRT_RAND_S in stdafx.h
//use <stdexcept>
#ifndef deck_size //default deck size
#define deck_size 52
#endif // !deck_size
std::runtime_error too_many_cards("Error: Too many cards");
class card // instances of this class cannot be copied or assigned
{
public:
card();
~card();
int getsuit() const;
int getnominal() const;
int getcard() const;
void printcard();
private:
unsigned int suit;
unsigned int nominal;
static int cards_in_game;
static card * _tcards[deck_size];
card(const card&); //no copy!
void operator=(const card&); //and no assignment
};
int card::cards_in_game=0;
card * card::_tcards[]={};
bool sdeck(const  card * a,const card * b)
{
return a->getcard() < b->getcard();
}
bool ssuits(const card * a,const card * b)
{
return a->getsuit()<b->getsuit();
}
bool snominals(const  card * a,const card * b)
{
return a->getnominal()<b->getnominal();
}
card::card() //this function MUST be rewrited for LINUX version
{
if(cards_in_game>=deck_size){throw too_many_cards;} //protection from the endless cycle
errno_t err;
unsigned int tmp;
bool isdouble;
do
{
isdouble=false;
err = rand_s(&tmp);
suit=(unsigned int)    ((double)tmp/((double) UINT_MAX + 1) * 4) + 1;
err= rand_s(&tmp);
nominal=(unsigned int) ((double)tmp/((double) UINT_MAX + 1) * 13) +2;
for(int i=0;i<cards_in_game;i++)
{
if(this->nominal==_tcards[i]->nominal && this->suit==_tcards[i]->suit){isdouble=true;}
}
}
while(isdouble);
_tcards[cards_in_game]=this;
cards_in_game++;
}
card::~card()
{
for(int i=0;i<cards_in_game;i++)
{//look for card in deck
if(this==_tcards[i])
{
for(int j=i;j<cards_in_game-1;j++){_tcards[j]=_tcards[j+1];}
cards_in_game--;
_tcards[cards_in_game]=nullptr;
break;
}
}
}
int card::getsuit() const{return suit;}
int card::getnominal() const{return nominal;}
int card::getcard() const{return ((suit-1)*13+nominal);}
void card::printcard()
{
char picnom;
bool ispicture=true;
switch (nominal)
{
case 11: picnom='V'; break;
case 12: picnom='D'; break;
case 13: picnom='K'; break;
case 14: picnom='T'; break;
default: ispicture=false; break;
}
std::cout<<(char)(suit+2)<<' ';
if(ispicture) std::cout<<picnom;
else std::cout<<nominal;
}


