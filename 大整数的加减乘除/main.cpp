#include <iostream>
using namespace std;
class BigNum{
    string num;
    static string remove_zero(const string &a);
public:
    BigNum() = default;
    BigNum(const string a);
    string getNumStr() const {return num;}
    BigNum operator + (const BigNum &a);
    BigNum operator - (const BigNum &a);
    BigNum operator * (const BigNum &a);
    BigNum operator / (const BigNum &a);
    BigNum & operator = (const string &a){num = remove_zero(a);return *this;}
    BigNum & operator += (const BigNum &a){*this = *this + a;return *this;}
    BigNum & operator -= (const BigNum &a){*this = *this - a;return *this;}
    bool operator < (const BigNum &a) const{
        return (num.length()<a.num.length() ||
                (num.length()==a.num.length() &&
                 (num.compare(a.num)<0))) ? true:false;
    }
    bool operator <= (const BigNum &a) const {
        if (*this < a)
            return true;
        if (*this == a)
            return true;
        return false;
    }
    bool operator == (const BigNum &a) const {return num == a.num;}
    friend istream & operator >>(istream &in, BigNum &a);
    friend ostream & operator <<(ostream &out, const BigNum &a);
};

BigNum::BigNum(const string a){
    num = remove_zero(a);
}
string BigNum::remove_zero(const string &a){
    auto p = a.find_first_not_of('0');
    return (p == string::npos) ? "0" : a.substr(p);
}
BigNum BigNum::operator + (const BigNum &a){
    string re;
    string m = a.getNumStr();
    string n = getNumStr();
    int i = static_cast<int>(m.length()-1);
    int j = static_cast<int>(n.length()-1);
    int carry = 0;
    for(; i>=0||j>=0; i--,j--){
        int b1 = i>=0 ? m[i] - '0' : 0;
        int b2 = j>=0 ? n[j] - '0' : 0;
        int temp = b1+b2+carry;
        int bb = temp%10;
        carry = temp/10;
        re.insert(re.begin(), static_cast<char>('0'+bb));
    }
    if (carry == 1)
        re.insert(re.begin(),'1');
    return BigNum(re);
}
BigNum BigNum::operator - (const BigNum &a){
    bool minusFlag = false;
    string re;
    string m = getNumStr();
    string n = a.getNumStr();
    if(*this < a){
        minusFlag = true;
        m = a.getNumStr();
        n = getNumStr();
    }
    int i = static_cast<int>(m.length()-1);
    int j = static_cast<int>(n.length()-1);
    int carry = 0;
    for(; i>=0; i--,j--){
        int b1 = i>=0 ? m[i] - '0' : 0;
        int b2 = j>=0 ? n[j] - '0' : 0;
        int temp = b1-b2+carry;
        if(temp >= 0){
            carry = 0;
            re.insert(re.begin(), static_cast<char>('0'+temp));
        }
        else{
            carry = -1;
            re.insert(re.begin(), static_cast<char>(temp+10+'0'));
        }
    }
    re = remove_zero(re);
    if(minusFlag)
        re.insert(re.begin(), '-');
    return BigNum(re);
}

BigNum BigNum:: operator * (const BigNum &a){
    BigNum re("0");
    string m = getNumStr();
    string n = a.getNumStr();
    if(*this < a){
        m = a.getNumStr();
        n = getNumStr();
    }
    int i = static_cast<int>(m.length()-1);
    int j = static_cast<int>(n.length()-1);
    for(; j>=0; j--){
        string mid;
        int carry = 0;
        int b1 = n[j] - '0';
        for(int k=i; k>=0; k--){
            int b2 = m[k] - '0';
            int temp = b1*b2 + carry;
            mid.insert(mid.begin(), static_cast<char>('0'+temp%10));
            carry = temp/10;
        }
        if(carry)
            mid.insert(mid.begin(), static_cast<char>('0'+carry));
        int how_many_zero_should_add = static_cast<int>(n.length() - j - 1);
        for(int i = 0; i<how_many_zero_should_add; i++){
            mid.insert(mid.end(), '0');
        }
        re += BigNum(mid);
    }
    return re;
}

BigNum BigNum:: operator / (const BigNum &a){
    if(a==BigNum("1"))
        return *this;
    if(*this < a)
        return BigNum("0");
    BigNum re("0");
    BigNum numM(*this),numN(a);
    while (a < numM){
        string res;
        string m = numM.getNumStr();
        string n = a.getNumStr();
        int i = static_cast<int>(m.length());
        int j = static_cast<int>(n.length());
        int k = i - j;
        for(int kk = k; kk>0; kk--){
            n.insert(n.end(), '0');
        }
        numN = BigNum(n);
        if(numM < numN){
            --k;
            n.erase(n.end()-1,n.end());
            numN = BigNum(n);
        }
        int count = 0;
        while (numN <= numM) {
            numM -= numN;
            ++count;
        }
        res.insert(res.end(), static_cast<char>('0'+count));
        for(int j = k; j>0; j--){
            res.insert(res.end(), '0');
        }
        re += res;
    };
    return re;
}

istream & operator >>(istream &in, BigNum &a){
    string s;
    in >> s;
    a.num = BigNum::remove_zero(s);
    return in;
}

ostream & operator <<(ostream &out, const BigNum &a){
    out << a.num;
    return out;
}

int main(){
    BigNum a,b;
    cin >> a;
    char o;
    cin >> o >> b;
    switch (o) {
        case '+':
            cout << a+b;
            break;
        case '-':
            cout << a-b;
            break;
        case '*':
            cout << a*b;
            break;
        case '/':
            cout << a/b;
            break;
        default:
            break;
    }
    return 0;
}
