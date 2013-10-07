int combination(card* c1,card* c2,card* c3,card* c4,card* c5)
{
bool flash(false),straight(true);
card * tested[]={c1,c2,c3,c4,c5};
std::sort(tested,tested+5,ssuits);
if(tested[0]->getsuit()==tested[4]->getsuit()) flash=true;
std::sort(tested,tested+5,snominals);
for (int i = 0; i < 4; i++)
{
if(tested[i]->getnominal()!=tested[i+1]->getnominal()-1)
{
straight=false;
break;
}
}
if (straight && flash) return 80000+(tested[4]->getnominal())*100;
int seqences[2]={1,1},seqnom[2]={0,0},j=0;
for (int i = 0,j=0; i < 4; i++)
{
if(tested[i]->getnominal()==tested[i+1]->getnominal())
{
seqences[j]++;
seqnom[j]=tested[i]->getnominal();
}
else if (seqences[j]!=1)
{
j++;
}
}
if (seqences[0]==4) return 70000+seqnom[0]*100;
if (seqences[0]==3 || seqences[1]==3)
{
if (seqences[1]==2)
{
return 60000+seqnom[0]*100+seqnom[1];
}
if (seqences[0]==2)
{
return 60000+seqnom[1]*100+seqnom[0];
}
if(!(straight || flash))
{
return 30000 + seqnom[0]*100;
}
}
if(flash) return 50000+100*tested[4]->getnominal();
if(straight) return 40000+100*tested[4]->getnominal();
if (seqences[0]==2)
{
if (seqences[1]==2)
{
if(seqnom[0]>seqnom[1]) return 20000+100*seqnom[0]+seqnom[1];
else return 20000+100*seqnom[1]+seqnom[0];
}
return 10000+100*seqnom[0];
}
return 0;
}
class player
{
public:
bool is_alive;  // true if it's a live player, 0 if computer need
bool in_game;   //true if player didn't throw cards          need
bool is_all_in; //true if all in                             need
double bet;                                         //       need
double cash;                                        //       need
double do_bet(double cur_max_bet,int con,card * cards[],double bank);
double check(double need);
void get_bank(double prize);                
void reset();
void throwcards();               //throw cards!
void getcards();                 //get cards!
double play(double cur_max_bet,int con,card * cards[],double bank);
double raw(int con, card * cards[],bool flag);
void set_alive(){is_alive=true;}
int max_nom();
int bestcomb(card cards[]);
player();
~player();
private:
card * onhand;
double raise(double tbet,short int mod);
int toflash(card * cards[],int con);         //number of outs to flash
int tostraight(card * cards[], int con);  //return 0 if you have strait, -2 if straight on table, -1 if no chance to get, and number of outs otherwise
int samekind(card * cards[],int con,int &current_comb); /*return number of outs to next comb & set current comb to code
of combination that we have */
double solt();
};
 
player::player()
{
is_alive=false;
in_game=true;
is_all_in=false;
bet=0;
cash=1000;
onhand=new card[2];
}
player::~player()
{
delete [] onhand;
}
void player::throwcards()
{
delete [] onhand;
onhand=nullptr;
in_game=false;
}
void player::getcards()
{
onhand= new card[2];
}
void player::reset()
{
bet=0;
throwcards();
is_all_in=false;
in_game=true;
getcards();
}
void player::get_bank(double prize)
{
cash+=prize;
}
double player::do_bet(double cur_max_bet,int con,card * cards[],double bank) //Please write it!
{
if (!is_alive)
{
double brain=raw(con,cards,false)+solt();
int mod=(int) brain/20;
return raise(cur_max_bet-bet,mod);
}
else
{
double code=play(cur_max_bet,con,cards,bank);
if (code<0)
{
throwcards();
return 0;
}
if (code==0) return check(cur_max_bet-bet);
if (code>0)  return check(cur_max_bet+code-bet);
}
}
double player::check(double need)
{
if (cash<=need)
{
is_all_in=true;
bet+=cash;
double tmp=cash;
cash=0;
return tmp;
}
else
{
bet+=need;
cash-=need;
return need;
}
}
double player::raise(double tbet,short int mod)
{
if(cash<=tbet*2 && mod>2) mod--;
if(mod<2)
{
throwcards();
return 0;
}
if(mod>4) return check(tbet*2);
switch (mod)
{
case 2:
return check(tbet);
case 3:
return check(tbet+10);
case 4:
return check(tbet*1.5);
}
}
double player::raw(int con, card * cards[],bool flag)
{
if(con==0)
{
if (onhand[0].getnominal()==onhand[1].getnominal()){return 70;}
if (onhand[0].getsuit()==onhand[1].getsuit()){return 50;}
if (std::abs(onhand[0].getnominal() - onhand[1].getnominal())<3)
{
if(std::abs(onhand[0].getnominal() - onhand[1].getnominal())<2){return 40;}
return 20;
}
else
{
return 10;
}
}
int current_comb=0;
int nom_code=samekind(cards,con, current_comb);
int flash_code,straight_code;
flash_code=toflash(cards,con);
straight_code=tostraight(cards,con);
if(!flash_code && !straight_code) current_comb=8;
if(flag) return current_comb;
if(current_comb<8)
{
if(!straight_code) current_comb=4;
if(!flash_code) current_comb=5;
}
if(current_comb>3) return 90;
if(current_comb>5) return 100;
if(current_comb==2 || current_comb==3) return nom_code*2*(5-con)+20;
if(flash_code>0) return flash_code*2*(5-con)+30;
if(straight_code>0) return straight_code*2*(5-con)+25;
return nom_code*(5-con);
}
int player::toflash(card * cards[],int con)
{
int suits[4];
int max=0;
for (int i = 0; i < con; i++)
{
switch (cards[i]->getsuit())
{
case 1: suits[0]++; break;
case 2: suits[1]++; break;
case 3: suits[2]++; break;
case 4: suits[3]++; break;
}
}
for (int i = 0; i < 4; i++){if(suits[i]==5) return -2;}       //if flash on table return negative
for (int i = 0; i < 2; i++){suits[(onhand[i].getsuit())-1]++;}
for (int i = 0; i < 4; i++){if (suits[i]>max){max=suits[i];}}
if(max>4){return 0;}                                          //flash olready
if(max<4){return -1;}                                         //to little chance
return 13-max;                                                //number of outs
}
int player::tostraight(card * cards[], int con)
{
int * nominals=new int[con+2];
int * nominalsT=new int[con+2];
for (int i = 0; i < con; i++)
{
nominals[2+i]=cards[i]->getnominal();
nominalsT[2+i]=cards[i]->getnominal();
}
if(con==5)
{
for(int i=2;i<con+2;i++)
{
if (nominalsT[i]==14) nominalsT[i]=1;
}
std::sort(nominals+2,nominals+con+2);
std::sort(nominalsT+2,nominalsT+con+2);
int maxseq=1;
for(int i=3;i<con+2;i++)
{
if(nominals[i]==nominals[i-1]+1) maxseq++;
}
if(maxseq==5)
{
delete [] nominals;
delete [] nominalsT;
return -2;                                  //straight is on table
}
}
nominalsT[0]=nominals[0]=onhand[0].getnominal();
nominalsT[1]=nominals[1]=onhand[1].getnominal();
for(int i=0;i<2;i++)
{
if (nominalsT[i]==14) nominalsT[i]=1;
}
std::sort(nominals,nominals+con+2);
std::sort(nominalsT,nominalsT+con+2);
int maxseqence[3]={0,0,0},maxTseqence[3]={0,0,0};
int counter=1,counterT=1;
int s[3]={-1,-1,-1},sT[3]={-1,-1,-1};
int offset=0,offseT=0;
for(int i=1;i<con+2;i++)                       //find seqences
{
if(nominals[i]==nominals[i-1]+1) counter++;
if(nominals[i]!=nominals[i-1]+1 || i==con+1)
{
if (counter>1)
{
maxseqence[offset]=counter;
s[offset]=i-counter;
offset++;
}
counter=1;
}
if(nominalsT[i]==nominalsT[i-1]+1) counterT++;
if(nominalsT[i]!=nominalsT[i-1]+1 || i==con+1)
{
if (counterT>1)
{
maxTseqence[offseT]=counterT;
sT[offseT]=i-counterT;
offseT++;
}
counterT=1;
}
}     
if(*std::max_element(maxseqence,maxseqence+2)<2 && *std::max_element(maxTseqence,maxTseqence+2))
{
delete [] nominals;
delete [] nominalsT;
return -1;
}
if(*std::max_element(maxseqence,maxseqence+2)==5 || *std::max_element(maxTseqence,maxTseqence+2)==5)
{
delete [] nominals;
delete [] nominalsT;
return 0;
}
if(*std::max_element(maxseqence,maxseqence+2)==4 && *std::max_element(maxTseqence,maxTseqence+2)==4)
{
delete [] nominals;
delete [] nominalsT;
return 8;  //2-sided straight
}
else
{
if(*std::max_element(maxseqence,maxseqence+2)==4 || *std::max_element(maxTseqence,maxTseqence+2))
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
}
// 4-elemen sequences done!
for (int i = 0; i < 3; i++)
{
if(maxseqence[i]==3)
{
//3 at begin
if(s[i]==0 && nominals[s[i]+maxseqence[i]]==nominals[s[i]+maxseqence[i]+1]-2)
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
//3 at end
else if(s[i]+maxseqence[i]==con+1 && nominals[s[i]]==nominals[s[i]-1]+2)
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
//3 somwere else
else if(nominals[s[i]-1]==nominals[s[i]]-2 || nominals[s[i]+maxseqence[i]]==nominals[s[i]+maxseqence[i]+1]-2)
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
}
if(maxTseqence[i]==3)
{
if(sT[i]==0 && nominalsT[sT[i]+maxTseqence[i]]==nominalsT[s[i]+maxTseqence[i]+1]-2)
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
else if(sT[i]+maxTseqence[i]==con+1 && nominalsT[s[i]]==nominalsT[sT[i]-1]+2)
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
else if(nominalsT[sT[i]-1]==nominalsT[sT[i]]-2 || nominalsT[sT[i]+maxTseqence[i]]==nominalsT[sT[i]+maxTseqence[i]+1]-2)
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
}
}
//3-element seqences done!
for (int i = 0; i < offset; i++)
{
if(maxseqence[i]==2 && offset!=i+1)
{
if(nominals[s[i]+2]==nominals[s[i+1]]-2)
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
}
}
for (int i = 0; i < offseT; i++)
{
if(maxTseqence[i]==2 && offseT!=i+1)
{
if(nominalsT[sT[i]+2]==nominalsT[sT[i+1]]-2)
{
delete [] nominals;
delete [] nominalsT;
return 4;
}
}
}
delete [] nominals;
delete [] nominalsT;
return -1;
}
int player::samekind(card * cards[],int con,int &current_comb)
{
int nominals[13];
int desk_combination;
int count_pairs=0,count_threes=0,count_fours=0;
int player_counts[]={0,0,0};
current_comb=0;
for (int i = 0; i < con; i++)
{
nominals[(cards[i]->getnominal())-2]++;
}
for (int i = 0; i < 13; i++)
{
if(nominals[i]==2) count_pairs++;
if(nominals[i]==3) count_threes++;
if(nominals[i]==4) count_fours++;
}
if(count_pairs) desk_combination=1;
if(count_pairs>1) desk_combination=2;
if(count_threes) desk_combination=3;
if(count_threes && count_pairs) desk_combination=4;
if(count_fours) desk_combination=5;
for (int i = 0; i < 2; i++)
{
nominals[(onhand[i].getnominal())-2]++;
}
if(*std::max_element(nominals,nominals+12)<2)
{
current_comb=0;
return 1;
}
for (int i = 0; i < 13; i++)
{
if(nominals[i]==2) player_counts[0]++;
if(nominals[i]==3) player_counts[1]++;
if(nominals[i]==4) player_counts[2]++;
}
if(player_counts[0]>count_pairs) current_comb=1;
if(player_counts[0]>count_pairs && player_counts[0]>1) current_comb=2;
if(player_counts[1]>count_threes) current_comb=3;
if((player_counts[0] && player_counts[1]) && (player_counts[0]>count_pairs || player_counts[1]>count_threes) ) current_comb=6;
if(player_counts[2]>count_fours) current_comb=7;
if(current_comb==0) return 6;
if(current_comb==1) return con*3+2;
if(current_comb==2) return 4;
if(current_comb==3) return con;
if(current_comb==4) return 1;
 
}
double player::solt()
{
errno_t err;
unsigned int rawrand;
err=rand_s(&rawrand);
double dice=(double) ((double)rawrand/((double) UINT_MAX + 1)*40)-20;
return dice;
}
double player::play(double cur_max_bet,int con,card * cards[],double bank)
{
system("cls");
std::cout<<"Yor cash="<< std::setw(7) <<cash<<std::setw(40)<<' '<<"Bank="<<std::setw(8)<<bank<<std::endl;
std::cout<<"Current max bet is"<<std::setw(7)<<cur_max_bet;
for (int i = 0; i < 5; i++)
{
std::cout<<std::endl;
}
std::cout<<std::setw(30)<<' '<<"Cards on table"<<std::endl;
std::cout<<std::setw(15)<<' ';
for (int i = 0; i < con; i++)
{
std::cout<<'|';
cards[i]->printcard();
std::cout<<'|'<<' ';
}
for (int i = 0; i < 5; i++)
{
std::cout<<std::endl;
}
std::cout<<std::setw(30)<<' '<<"Your cards:"<<std::endl<<std::endl;
std::cout<<std::setw(30)<<' ';
for (int i = 0; i < 2; i++)
{
std::cout<<'|';
onhand[i].printcard();
std::cout<<'|'<<' ';
}
std::cout<<std::endl<<std::endl<<"Enter your bet: type -1, if you whant to throw cards;"<<std::endl<<"0 if you whant to coll; otherwise enter your raise";
double code;
std::cout<<std::endl<<"Your choise:";
std::cin>>code;
return code;
}
int player::max_nom()
{
if(onhand[0].getnominal()>onhand[1].getnominal()) return onhand[0].getnominal();
else return onhand[1].getnominal();
}
int player::bestcomb(card cards[])
{
std::vector<int> combinations(20);
for (int i = 0; i < 3; i++)
{
for (int j = i+1; j < 4; j++)
{
for (int k = j+1; k < 5; k++)
{
combinations.push_back(combination(&onhand[0],&onhand[1],&cards[i],&cards[j],&cards[k]));
}
}
}
for (int i = 0; i < 2; i++)
{
for (int j = 0; j < 2; j++)
{
for (int k = j+1; k < 3; k++)
{
for (int l = k+1; l < 4; l++)
{
for (int m = l+1; l < 5; m++)
{
combinations.push_back(combination(&onhand[i],&cards[j],&cards[k],&cards[l],&cards[m]));
}
}
}
}
}
if(!(*std::max_element(combinations.begin(),combinations.end())))
{
return (std::max(&onhand[0],&onhand[1],snominals))->getnominal();
}
else
{
return *(std::max_element(combinations.begin(),combinations.end()));
}
}


