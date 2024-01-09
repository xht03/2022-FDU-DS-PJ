#include<iostream>
#include<cstdio>
#include<algorithm>
#include<fstream>
#include<vector>
#include<cmath>
#include<queue>
using namespace std;

class User;

struct Friends{
    User* frd_1;
    User* frd_2;
    User* frd_3;
};      //用于recommend()返回推荐的三个用户

class User
{
    public:
        int get_ID();                                   //获取用户ID
        int friend_number();                            //用户的朋友数量
        int indirect_number();                          //间接朋友数量
        bool whether_friend(User* him);                 //判断是否是朋友
        int distance(User* him);                        //计算社交距离 
        Friends recommend();                              //推荐新朋友          
        void make_friend(User* him);                    //与him交朋友
        void set_ID(int ID);                            //设置新用户ID
        User(){ list.clear(); indirect.clear(); }       //构造函数
                             
    private:
        int number;                         //用户ID
        vector<User*> list;                 //好友列表
        vector<User*> indirect;             //间接好友列表
};

User users[5000];       //用户群体、及其大小
int group_size = 0;


int User::get_ID()
{
    return number;
}

int User::friend_number()
{
    return list.size();
}

int User::indirect_number()
{
    //间接朋友列表大小
    int num = 0;

    if(list.empty()) return 0;      //如果孤寡，就没有间接朋友

    int size1 = list.size();
    for(int i = 0; i < size1; i++)
    {
        User* direct = list[i];     //直接朋友
        int size2 = direct->list.size();
        for(int j = 0; j < size2; j++)
        {
            User* possible = direct->list[j];       //可能的间接朋友
            bool in = false;
            
            for(int k = 0; k < num; k++)            
            {
                if(this->indirect[k] == possible)
                {
                    in = true;
                    break;
                }
            }   
            for(int k = 0; k < size1; k++)          //他不能是直接朋友
            {
                if(possible == list[k])
                {
                    in = true;
                    break;
                }
            }   
            if(possible == this)                    //也不能是自己
            {
                in = true;
            }
            if(!in)
            {
                indirect.push_back(possible);
                num++;
            }
        }
    }

    return num;
}


bool User::whether_friend(User* him)
{
    int list_size = list.size();
    for(int i = 0; i < list_size; i++)
    {
        if(list[i] == him)
        {
            return true;
        }
    }
    return false;
}

void User::make_friend(User* him)
{
    list.push_back(him);
}

void User::set_ID(int ID)
{
    number = ID;
}

int User::distance(User* him)
{
    //采用BFS搜索
    vector<User*> que;  //que前半部分存已经访问过的用户，后面的是待访问的队列
    vector<int> dis;
    que.clear();
    dis.clear();
    que.push_back(this);
    dis.push_back(0);

    int num = 0;    //记录已经访问过的用户数量，也是队列开始的指针

    while(num < que.size())
    {
        User* tmp = que[num];
        int tmp_dis = dis[num];
        num++;

        if(tmp == him)
        {
            return tmp_dis;
        }
        if(tmp_dis > 5000)
        {
            return -1;
        }

        int tmp_size = tmp->list.size();
        for(int i = 0; i < tmp_size; i++)
        {
            bool been = false;
            int que_size = que.size();
            for(int j = 0; j < que_size; j++)
            {
                //只要是之前入队了的，之前的路径肯定更短
                if(tmp->list[i] == que[j])
                {
                    been = true;
                    break;
                }   
            }
            if(!been)
            {
                que.push_back(tmp->list[i]);
                dis.push_back(tmp_dis + 1);
            }
        }
    }

    return -1;
}



Friends User::recommend()
{
    Friends new_frd;
    int max[4] = {0};
    new_frd.frd_1 = nullptr;
    new_frd.frd_2 = nullptr;
    new_frd.frd_3 = nullptr;

    int num = indirect.size();
    for(int i = 0; i < num; i++)
    {
        int ls_size = indirect[i]->list.size();     //候选人的列表大小
        int co_num = 0;                              //共同朋友数量

        //查看候选人的列表里有几位也是当前用户的朋友
        for(int j = 0; j < ls_size; j++)
        {
            if(whether_friend(indirect[i]->list[j]))
            {
                co_num++;
            }
        }
        
        if(co_num > max[1])
        {
            new_frd.frd_1 = indirect[i];
            max[1] = co_num;
        }
        else if(co_num > max[2])
        {
            new_frd.frd_2 = indirect[i];
            max[2] = co_num;
        }
        else if(co_num > max[3])
        {
            new_frd.frd_3 = indirect[i];
            max[3] = co_num;
        }
    }
    return new_frd;
}

User* super_connect(User network[], int size)
{
    int max = 0;
    User* sup = nullptr;
    for(int i = 0; i <= size; i++)
    {
        if(network[i].friend_number() > max)
        {
            max = network[i].friend_number();
            sup = &network[i];
        }
    }
    return sup;
}


int main()
{
    
    fstream fin1;
    fstream fin2;
    fstream fout;

    fin1.open("data.txt");
    fin2.open("sample.txt");
    fout.open("output.txt");

    if ((!fin1.is_open()) || (!fin2.is_open())) {
        cerr << "Error opening file for input" << endl;
        return 1; // 返回错误代码
    }

    //从文件中读取每行两个整数
    int num1, num2;
    while (fin1 >> num1 >> num2) {

        users[num1].make_friend(&users[num2]);
        users[num2].make_friend(&users[num1]);

        if(num1 > group_size && num1 >= num2) group_size = num1;
        if(num2 > group_size && num2 > num1) group_size = num2;
    }

    for(int i = 0; i <= group_size; i++)
    {
        users[i].set_ID(i);
    }
    

    int que1 = 0;   //询问直接和间接朋友
    int que2 = 0;   //询问最短社交距离
    int que4 = 0;   //推荐好友

    fin2 >> que1;
    for(int i = 0; i < que1; i++)
    {
        int u = 0;
        fin2 >> u;

        fout <<"The direct friend number of User "<< u << " : " << users[u].friend_number() << ".   ";
        fout <<"The indirect friend number of User "<< u << " : " << users[u].indirect_number() << '.' << endl;
    }
    fout << endl;

    fin2 >> que2;
    for(int i = 0; i < que2; i++)
    {
        int u, v;
        fin2 >> u >> v;
        //que.push(&users[u]);
        //dis.push(0);
        fout << "The distance between User "<< u << " and User " << v << " : " << users[u].distance(&users[v]) << "." << endl;
    }
    fout << endl;


    fout << "The super connecter is : User " << super_connect(users, group_size)->get_ID() << '.' << endl;

    fout << endl;
    fin2 >> que4;
    for(int i = 0; i < que4; i++)
    {
        int u = 0;
        fin2 >> u;
        //确保该用户的indirect向量已经计算过
        int temp = users[u].indirect_number();
        Friends rmd = users[u].recommend();
        fout <<"The recommend friend of User "<< u << " : " << rmd.frd_1->get_ID() << ' ' << rmd.frd_2->get_ID() << ' ' << rmd.frd_3->get_ID() << '.' << endl;
    }

    fin1.close();   //关闭文件
    fin2.close();
    fout.close();
    return 0;
}