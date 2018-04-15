#include <iostream>
#include <iomanip>
using namespace std;

class Headquarter;
const int SOLDIER_KIND = 5;
const int HEADQUARTER_KIND = 2;
const int WEAPON_KIND = 3;

class Soldier{
    int life;
    int attack;
    Headquarter *pHeadquarter;
protected:
    int num;
    int kindNo;
    const static string weaponName[WEAPON_KIND];
public:
    static const string names[SOLDIER_KIND];
    static int LifeValues[SOLDIER_KIND];
    int getLife() const {return life;}
    int getNum() const {return num;}
    Soldier(Headquarter *pHeadquarter, int kind, int no_, int time);
private:
    void print(int time, int kind) const;
};


class Headquarter{
    
    friend class Dragon;
    friend class Lion;
    friend class Soldier;
    
    bool stopped = false;
    int lifeMem;
    int no;
    int count = 0;
    int soldierNums[SOLDIER_KIND] = {0};
    int lastBorn = -1;
    int curMakingSeqIdx = 0;
    void stop(int time);
    int next();
    Soldier *pSoldier[1000];
    
public:
    static const int orders[HEADQUARTER_KIND][SOLDIER_KIND];
    static const string names[HEADQUARTER_KIND];
    Headquarter(int no_, int lifeMem): no(no_),lifeMem(lifeMem){}
    void produce(int time);
    string getName(){return names[no];}
    bool isStop(){return stopped;}
    ~ Headquarter();
};

const string Headquarter:: names[HEADQUARTER_KIND] = {"red", "blue"};
const int    Headquarter:: orders[HEADQUARTER_KIND][SOLDIER_KIND] = {
    {2,3,4,1,0}, {3,0,1,2,4}
};

class Dragon:public Soldier{
    int weaponNum = num%3;
    float morale;
public:
    float getMorale(){return morale;}
    string getWeaponName(){return weaponName[weaponNum];}
    Dragon(Headquarter *pHeadquarter, int kind, int count, int time);
};
Dragon::Dragon(Headquarter *pHeadquarter, int kind, int count, int time):
Soldier(pHeadquarter, kind, count, time),
morale(pHeadquarter->lifeMem/static_cast<double>(LifeValues[kind])){
    printf("It has a %s,and it's morale is %.2f\n",
           getWeaponName().c_str(), getMorale());
}

class Ninja:public Soldier{
    int weaponNums[2];
    float morale;
public:
    float getMorale(){return morale;}
    string getWeaponName(int n){return weaponName[weaponNums[n]];}
    Ninja(Headquarter *pHeadquarter, int kind, int count, int time);
};
Ninja::Ninja(Headquarter *pHeadquarter, int kind, int count, int time):
Soldier(pHeadquarter, kind, count, time){
    weaponNums[0] = count%3;
    weaponNums[1] = (count+1)%3;
    printf("It has a %s and a %s\n",
           weaponName[weaponNums[0]].c_str(),
           weaponName[weaponNums[1]].c_str());
}


class Iceman:public Soldier{
    int weaponNum;
public:
    string getWeaponName(){return weaponName[weaponNum];}
    Iceman(Headquarter *pHeadquarter, int kind, int count, int time);
};
Iceman::Iceman(Headquarter *pHeadquarter, int kind, int count, int time):
Soldier(pHeadquarter, kind, count, time),weaponNum(count%3){
    printf("It has a %s\n",weaponName[weaponNum].c_str());
}

class Lion:public Soldier{
    int loyalty;
public:
    Lion(Headquarter *pHeadquarter, int kind, int count, int time);
};
Lion::Lion(Headquarter *pHeadquarter, int kind, int count, int time):
Soldier(pHeadquarter, kind, count, time){
    loyalty = pHeadquarter->lifeMem;
    printf("It's loyalty is %d\n", loyalty);
}

using Wolf = Soldier;



const string Soldier:: names[SOLDIER_KIND] = {"dragon","ninja","iceman","lion","wolf"};
const string Soldier:: weaponName[3] = {"sword", "bomb", "arrow"};
int          Soldier:: LifeValues[SOLDIER_KIND];
Soldier:: Soldier(Headquarter *pHeadquarter, int kind, int no_,  int time):
pHeadquarter(pHeadquarter),life(LifeValues[kind]), kindNo(kind),num(no_){
    print(time, kind);
}
void Soldier:: print(int time, int kind) const {
    printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n",
           time, pHeadquarter->names[pHeadquarter->no].c_str(),
           names[kindNo].c_str(),num, LifeValues[kindNo],
           (pHeadquarter->soldierNums)[kind],
           names[kindNo].c_str(), (pHeadquarter->names)[pHeadquarter->no].c_str());
}

void Headquarter::produce(int time){
    int n = next();
    if(n == -1){
        if(stopped) return;
        else{
            stopped = true;
            stop(time);
            return;
        }
    }
    ++count;
    soldierNums[n]++;
    lifeMem -= Soldier::LifeValues[n];
    switch (n) {
        case 0:
            pSoldier[count] = new Dragon(this, n, count, time);
            break;
        case 1:
            pSoldier[count] = new Ninja(this, n, count, time);
            break;
        case 2:
            pSoldier[count] = new Iceman(this, n, count, time);
            break;
        case 3:
            pSoldier[count] = new Lion(this, n, count, time);
            break;
        case 4:
            pSoldier[count] = new Wolf(this, n, count, time);
            break;
        default:
            break;
    }
}
void Headquarter:: stop(int time){
    cout << setfill ('0') << setw (3) << time << ' ';
    cout << getName() << " headquarter stops making warriors" << endl;
}

int Headquarter::next(){
    int tempCount = lastBorn+1;
    int p = (tempCount)%5,q;
    if (lifeMem >= Soldier::LifeValues[orders[no][p]]) {
        lastBorn = p;
        return orders[no][p];
    }else do{
        q = (++tempCount)%5;
        if(lifeMem >= Soldier::LifeValues[orders[no][q]]){
            lastBorn = q;
            return orders[no][q];
        }
    }while (p!=q);
    return -1;
}

Headquarter::~Headquarter(){
    for(int i = 1; i<=count; i++)
        delete pSoldier[i];
}


int main(){
    int n;
    cin >> n;
    int count = 1;
    while (n--) {
        int M;
        cin >> M;
        Headquarter red(0, M),blue(1, M);
        for(int i = 0; i<5; cin >> Soldier::LifeValues[i++]);
        cout << "Case:" << count++ << endl;
        for (int i=0; ;i++){
            if(red.isStop() && blue.isStop())
                break;
            else{
                if(!red.isStop()){
                    red.produce(i);
                }
                if(!blue.isStop())
                    blue.produce(i);
            }
        }
    }
    return 0;
}


