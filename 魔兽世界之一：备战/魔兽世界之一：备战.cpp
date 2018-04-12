#include <iostream>
#include <iomanip>
using namespace std;
class Soldier{
    int life;
    int attack;
    int num;
public:
    int getLife(){return life;}
    int getNum(){return num;}
    Soldier(int life, int count): life(life), num(count){}
};

class Headquarter{
    int lifeMem;
    int count = 0;
    int *order;
    int soldierNums[5] = {0};
    string name;
    int lastBorn = -1;
public:
    static const int *soldierLifes;
    static const string *soldierNames;
    Headquarter(int lifeMem, string name):lifeMem(lifeMem), name(name){}
    void produce(int n, int time){
        int life = soldierLifes[n];
        string sName = soldierNames[n];
        Soldier bewBorn(life, ++count);
        lifeMem -= life;
        cout << setfill ('0') << std::setw (3) << time << ' ';
        cout << name << ' ' << sName << ' ';
        cout << bewBorn.getNum() << " born with strength ";
        cout << bewBorn.getLife() << ',' << ++soldierNums[n] << ' ';
        cout << sName << " in " << name << " headquarter" << endl;
    };
    int next(){
        int tempCount = lastBorn+1;
        int p = (tempCount)%5,q;
        if (lifeMem >= soldierLifes[order[p]]) {
            lastBorn = p;
            return order[p];
        }else do{
            q = (++tempCount)%5;
            if(lifeMem >= soldierLifes[order[q]]){
                lastBorn = q;
                return order[q];
            }
        }while (p!=q);
        return -1;
    };
    void setOrder(int *o){order = o;};
    void stop(int time){
        cout << setfill ('0') << std::setw (3) << time << ' ';
        cout << name << " headquarter stops making warriors" << endl;
    }
};
const int* Headquarter:: soldierLifes = nullptr;
const string* Headquarter:: soldierNames = nullptr;

int main(){
    int n;
    cin >> n;
    string names[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
    int redOrder[5] = {2,3,4,1,0};
    int blueOrder[5] = {3,0,1,2,4};
    Headquarter:: soldierNames = names;
    int count = 1;
    while (n--) {
        int M;
        cin >> M;
        Headquarter red(M, "red"),blue(M, "blue");
        red.setOrder(redOrder);
        blue.setOrder(blueOrder);
        int lifes[5];
        for(int i = 0; i<5; cin >> lifes[i++]);
        Headquarter:: soldierLifes = lifes;
        cout << "Case:" << count++ << endl;
        bool flag1 = false, flag2 = false;
        for (int i=0; ;i++){
            int redOrder = red.next();
            int blueOrder = blue.next();
            if(redOrder != -1)
                red.produce(redOrder, i);
            else if(flag1 == false){
                red.stop(i);
                flag1 = true;
            }
            if(blueOrder!= -1)
                blue.produce(blueOrder, i);
            else if (flag2 == false){
                blue.stop(i);
                flag2 = true;
            }
            if(flag1 && flag2)
                break;
        }
    }
    return 0;
}
