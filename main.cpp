#include <iostream>
#include <list>
#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<string>
#include <unordered_map>
#include<iterator>
#include<fstream>
using namespace std;
//stim ca elementele din sir2 se gasesc in sir1
string sterg(string sir1, string sir2)
{
    char  s2[10];
    strcpy(s2,sir2.c_str());

    for(int i=0; i<sir2.length();i++)
    {
        int pozitie = sir1.find(s2[i]);

        if (pozitie == 0)
            sir1 = sir1.substr(pozitie+1);
        else
            {
               string news = sir1;
               string sub1 = news.substr(0,pozitie);
               string  sub2 = news.substr((pozitie+1),sir1.length());
               sir1 = sub1+sub2;
            }
    }

    return sir1;

}
string numerical = "0123456789";
string alphabetical = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
string symbols = "~!@#$%^&*()_+-={}[]|\:\";<>?,./\' \n\r\0";
string A = numerical + alphabetical + symbols;
string whiteSpaces = " \n\t\0";
string operators = "%!*=^";
string separators = "(){}[];";
string fara0 = sterg(numerical,"0");
string cuMinus = numerical+"-";
string identf = alphabetical+numerical+"_";
string hexa = "abcdefABCDEF0123456789";
string zerosapte = "01234567";
string AfaraStar = sterg(A,"*");
string AfaraSlesStar = sterg(A,"*/");
string AfaraN= sterg(A,"\n");



class Token{
public:
    // 1 - identificator; 2 - int; 3 - float; 4 - caracter; 5 - comentariu; 6 - separator; 7 - operator; 8 - whiteSpaces;
    // 9 - string; 10 - keyword; 11 - OtherBase
     int type;          // un int de la 1 - 11, avand semnificatia de mai sus
     int value;         //pozitia din tabela de string-uri
};

class Automat
{
public:
    //lista cu tranzitii
    unordered_map<char,int> transitions[100];
    //stari finale - am un vectro de liste pentru ca pentru ca in v[1] o sa am toate
    //starile finale ale identificatorilor; e mai usor sa aflu tipul de token
    vector <int> finalStates[12];
    vector <Token> stringTable;
    void loadTransitions(Automat *a, string s);
    void loadFinalStates();
    char keywords[32][10] = {"auto","break","case","char","const","continue","default",
							"do","double","else","enum","extern","float","for","goto",
							"if","int","long","register","return","short","signed",
							"sizeof","static","struct","switch","typedef","union",
							"unsigned","void","volatile","while"};
//private:
    void gettoken(string numeFisier);
};

void Automat::loadFinalStates()
{
    finalStates[1].push_back(46); //identificator
    finalStates[1].push_back(1);

    finalStates[2].push_back(2); //int
    finalStates[2].push_back(10);
    finalStates[2].push_back(50);

    finalStates[3].push_back(9);  //float
    finalStates[3].push_back(51);

    finalStates[4].push_back(45);//caracter

    finalStates[5].push_back(15); //comentariu
    finalStates[5].push_back(47);

    finalStates[6].push_back(43); //separator

    finalStates[7].push_back(21); //operator
    finalStates[7].push_back(52);
    finalStates[7].push_back(48);
    finalStates[7].push_back(32);
    finalStates[7].push_back(33);

    finalStates[7].push_back(34);
    finalStates[7].push_back(35);
    finalStates[7].push_back(36);
    finalStates[7].push_back(37);
    finalStates[7].push_back(38);
    finalStates[7].push_back(39);
    finalStates[7].push_back(40);
    finalStates[7].push_back(41);
    finalStates[7].push_back(42);
    finalStates[7].push_back(28);
    finalStates[7].push_back(29);
    finalStates[7].push_back(30);
    finalStates[7].push_back(31);
    finalStates[7].push_back(26);
    finalStates[7].push_back(27);
    finalStates[7].push_back(24);
    finalStates[7].push_back(25);
    finalStates[7].push_back(22);
    finalStates[7].push_back(23);

    finalStates[8].push_back(20);//whitespaces

    finalStates[9].push_back(19); //string

    finalStates[10].push_back(1); //keyword

    finalStates[11].push_back(7);//otherbase

/*
    cout<<"finalStates: ";
    for(int i=1; i<=11;i++)
    {   cout<<"starile finale pt "<<i<<" ";

        for(auto j = finalStates[i].begin(); j<finalStates[i].end(); ++j)
            cout<<*j<<" ";
        cout<<endl;
    }
*/
}

 void Automat::loadTransitions(Automat *a, string numeFisier)
{
     char y; int x,z;
    ifstream read(numeFisier);

    while(read>>x>>y>>z)
    {
        //cout<<x<<y<<z;
        (*a).transitions[x][y] = z;

    }

    read.close();

    (*a).transitions[16]['\n'] = 47;    //16->47

    (*a).transitions[0]['\t'] = 20;    //0->20
    (*a).transitions[0]['\n'] = 20;    //0->20
    (*a).transitions[0][' '] = 20;    //0->20
    (*a).transitions[20]['\t'] = 20;    //20->20
    (*a).transitions[20]['\n'] = 20;    //20->20
    (*a).transitions[20][' '] = 20;    //20->20

    for(int j = 0; j <= fara0.length(); j++)
        (*a).transitions[0][fara0[j]] = 2;    //0->2

    for(int j = 0; j <= identf.length(); j++)
    {
        (*a).transitions[1][identf[j]] = 46;    //1->46
        (*a).transitions[46][identf[j]] = 46;    //46->46
    }

    for(int j = 0; j <= hexa.length(); j++)
    {
        (*a).transitions[5][hexa[j]] = 6;    //5->6
        (*a).transitions[6][hexa[j]] = 6;    //6->6
    }

    for(int j = 0; j <= zerosapte.length(); j++)
    {
        (*a).transitions[4][zerosapte[j]] = 53;    //4->53
        (*a).transitions[53][zerosapte[j]] = 54;    //53->54
        (*a).transitions[54][zerosapte[j]] = 7;    //54->7
    }

    for(int j = 0; j <= numerical.length(); j++)
      {
        (*a).transitions[2][numerical[j]] = 2;    //2->2
        (*a).transitions[11][numerical[j]] = 50;  //11->50
        (*a).transitions[50][numerical[j]] = 50;  //2->2
        (*a).transitions[8][numerical[j]] = 8;    //8->8
        (*a).transitions[9][numerical[j]] = 9;    //9->9
        (*a).transitions[49][numerical[j]] = 51;  //49->51
        (*a).transitions[51][numerical[j]] = 51;  //51->51
        (*a).transitions[8][numerical[j]] = 8;    //8->8
        (*a).transitions[8][numerical[j]] = 8;    //8->8
        }

    for(int j = 0; j <= cuMinus.length(); j++)
        (*a).transitions[10][cuMinus[j]] = 11;    //10->11

    for(int j = 0; j <= alphabetical.length(); j++)
    {
        (*a).transitions[0][alphabetical[j]] = 1;    //0->1
        (*a).transitions[3][alphabetical[j]] = 4;   //3->4
    }

    for(int j = 0; j <= AfaraSlesStar.length(); j++)
        (*a).transitions[14][AfaraSlesStar[j]] = 43;    //14->43

    for(int j = 0; j <= AfaraStar.length(); j++)
        (*a).transitions[43][AfaraStar[j]] = 43;    //43->43

    for(int j = 0; j <= fara0.length(); j++)
        (*a).transitions[1][fara0[j]] = 1;    //1->1

    for(int j = 0; j <= operators.length(); j++)
        (*a).transitions[0][operators[j]] = 22;    //0->22

    for(int j = 0; j <= separators.length(); j++)
        (*a).transitions[0][separators[j]] = 43;    //0->43

    for(int j = 0; j <= A.length(); j++)
        (*a).transitions[17][A[j]] = 18;    //17->18

    for(int j = 0; j <= AfaraN.length(); j++)
        (*a).transitions[16][AfaraN[j]] = 16;    //16->16

/*
int position(int stare)
{
    for(int i=1; i<=11;i++)
    {   //cout<<"starile finale pt "<<i<<" ";

        for(auto j = finalStates[i].begin(); j<finalStates[i].end(); ++j)
        {
            if( *j == stare)
            {
                pozitieIdentificator = *j;
                return pozitieIdentificator;
            }
        }
    }
}
*/
}
void Automat::gettoken(string numeFisier)
{
    char x;
    string buffer = "";           //buffer in care o sa tin minte ce citesc din fisier
    int k=0;                 //pozitia din fisier
    ifstream fin(numeFisier);//fisierul din care citesc
    ofstream fout("write.txt");//fisierul in care afisez tokenul
    Token token;
    int stare = 0;

    while(fin>>noskipws>>x) //citesc caracter cu caracter
    {
        if(transitions[stare][x]!=0)
        {
            buffer +=x;
            stare = transitions[stare][x];
        }
        else {
                stare = 0;
                cout << buffer << "\n";
                buffer = x;
                stare = transitions[stare][x];
            }
    }
    cout << buffer << "\n";
    fout.close();
}

bool contains (string sir1, string sir2)//verifica daca sirul2 apare in sirul1
{
    for(int i=0;i<sir1.length();i++)
    {
        if(sir1[i]==sir2[0])
            return true;
    }

    return false;
}

int main()
{
    Automat afd;


    string numeFisier;
    int number;

    cout<<"Tasta 1 - aici.txt, 2 - test_c1, 3 - test_c2, 4 - altceva.txt : ";
    cin>>number;

    switch (number){
    case 1:
        numeFisier = "aici.txt";
        break;
    case 2:
        numeFisier = "test_c1";
        break;
    case 3:
        numeFisier = "test_c2";
        break;
    case 4:
        numeFisier = "altceva.txt";
        break;
    }

    afd.loadTransitions(&afd,"tranzitii.txt");
    afd.loadFinalStates();
    afd.gettoken(numeFisier);
/*
    for(int i = 0; i<= 60; i++)
    {
        for(auto it = afd.transitions[i].begin(); it != afd.transitions[i].end(); ++it)
            cout<<i<<" "<<it->first<<" "<<it->second;
    }
*/
    return 0;
}
