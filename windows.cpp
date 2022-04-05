#include <string.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <vector>
#include "users.h"
#include "windows.h"
#include "analysis.h"
using namespace std;

int                         gWinNum;//窗口数量
vector<SBox>   gBox;//查找相关性的容器
vector<SBox *>   gBoxp;
SWinLog             gWinLog[100][60000];//每个窗口的消费记录
SNode *              gLogHead=new SNode;//合并后的记录链表头
string CWindow::sm_Time="2022022818000000";//时间

CWindow::CWindow()
{
    //构造函数
    m_Index=gWinNum++;
    m_Total=m_Times=0;
}

bool CWindow::processApplication(CCard & card,double num,int pattern,string set_time=CWindow::sm_Time)
{
    /************************
    * Input：  目标卡片
                    消费金额
                    模式，用户模式或管理员模式
                    消费时间
    *Output：修改卡片余额
                    窗口总消费次数增加
                    窗口总消费金额变化
    *Return：success or not
    ************************/
    string log;
    string code;
    //卡片挂失时无法使用
    if(card.freezeCard(0))
    {
        cout<<"卡片处于挂失状态，无法使用"<<endl;
        return false;
    }
    //余额不足时使用失败
    if(card.getBalance()-num<0)
    {
        cout<<"余额不足"<<endl;
        return false;
    }
    //将设置或输入的时间与记录时间比较，不同则替换且将当前消费额置0
    if(!MatchTime(card.m_Sum.s_Period,set_time))
    {
        card.m_Sum.s_Period=set_time;
        card.m_Sum.fCharge=0;
    }
    if(pattern==1){
        if(card.m_Sum.fCharge>upnum||card.m_Sum.fCharge+num>upnum)
        {
            cout<<"请输入卡片"<<card.getMessage(IDINDEX)<<"的密码"<<endl;
            cin>>code;
            //消费密码输入失败
            if(!card.verifyPasswd(code))
            {
                cout<<"密码错误"<<endl;
                return false;
            }
            else
                card.m_Sum.fCharge=0;
        }
        else
            card.m_Sum.fCharge+=num;
    }
    card.payCard(num);
    //窗口消费记录，增加消费次数与总消费额
    m_Total+=num;
    m_Times++;
    //log格式：时间，学号，姓名，额度
    log=set_time+","+card.getMessage(STUIDINDEX)+","+card.getMessage(NAMEINDEX)+","+to_string(num);
    loadWinLog(log);
    return true;
}

void CWindow::loadTimes(int num)//修改窗口总消费次数
{
   
    m_Times=num;
}

double CWindow::getMessage(int order)//窗口号消费金额次数         
{ 
    
    switch(order)
    {
    case INDEX:
        return m_Index;
    case TOTALINDEX:
        return m_Total;
    case TIMESINDEX:
        return m_Times;
    default:
        return -1;
    }
}

void CWindow::loadWinLog(string &log)//消费记录载入
{
    
    int j=m_Times%60000;
    strcpy(gWinLog[m_Index][j].sLogs,log.data());
    gWinLog[m_Index][j].nFlag=1;
}

void FormApplication(CWindow * wins,CCard * cards)//消费成功与否的提示
{
   
    double    num;
    int       win_index;
    int       card_index;
    string card_id,log;
    sort(cards,cards+GetUserMessage(2),compare3);
    while(true)
    {
        cout<<"请输入窗口号（输入0退出程序）：";
        cin>>win_index;
        if(win_index==0)
            break;
        //窗口号输入错误情况
        if(!(win_index>=1&&win_index<=99))
        {
            cout<<"输入窗口号错误，窗口范围为1-99"<<endl;
            continue;
        }
        cout<<"请输入卡号：";
        cin>>card_id;
        card_index=SearchCard(cards,card_id,IDINDEX);
        if(card_index==-1)
        {
            cout<<"该卡片不存在"<<endl;
            continue;
        }
        cout<<"请输入金额"<<endl;
        cin>>num;
        if(!wins[win_index].processApplication(cards[card_index],num,1))
        {
            log="消费,"+cards[card_index].getMessage(IDINDEX)+","+to_string(num)+",失败";
            WriteLog(log);
        }
        else cout<<"消费成功，余额为"<<cards[card_index].getBalance()<<endl;
    }
    sort(cards,cards+GetUserMessage(2),compare2);
}

SNode* FormApplications(CWindow * wins,CCard * cards,SNode * cur,string & due_time)//批量消费成功与失败的次数
{
    
    SNode * temp=cur;
    if(cur==nullptr)
        return temp;

    int winindex=0;
    static int flag[2]= {};
    string log;

    //下列参数保存文件中读取出的数据
    double price=0;
    string cardid;//读取卡号
    string time;//读取时间
    string win_index;
    string num;//读取消费金额
    sort(cards,cards+GetUserMessage(2),compare3);
    while(temp!=nullptr)
    {
        //读取内容为消费记录的情况
        stringstream ss(temp->str);
        getline(ss,time,',');
        if(time>=due_time)
            break;
        getline(ss,cardid,',');
        getline(ss,win_index,',');
        getline(ss,num,';');
        price=atof(num.data());
        winindex=atoi(win_index.data());
        int index=SearchCard(cards,cardid,IDINDEX);
        if(index==-1)
        {
            flag[1]++;
            temp=temp->next;
            continue;
        }
        if(!wins[winindex].processApplication(cards[index],price,0,time))
        {
            log="消费,"+cards[index].getMessage(IDINDEX)+","+num+",失败";
            WriteLog(log);
            flag[1]++;
        }
        else flag[0]++;
        //转移到下一条指令
        temp=temp->next;
    }
    log="目前共进行了"+to_string(flag[0]+flag[1])+"次消费，成功"+to_string(flag[0])+"次，失败"+to_string(flag[1])+"次";
    cout<<log<<endl;
    WriteLog(log);
    sort(cards,cards+GetUserMessage(2),compare2);
    return temp;
}

void LoadPreTimes(CWindow * wins)//读取文件进行载入
{
    
    int index=1;
    //下列参数保存文件中读取出的数据
    char num[8];
    ifstream fin("wz003.txt");
    //文件开启状态判断
    if(!fin.is_open())
    {
        cout<<"文件开启错误"<<endl;
        return;
    }
    //读取文件第一行判断是否为消费记录文件
    fin.getline(num,3);
    if(strcmp(num,"WZ")!=0)
    {
        cout<<"文件读取错误"<<endl;
        return;
    }
    while(fin.peek()!=EOF)
    {
        fin.getline(num,3,',');
        fin.getline(num,8,';');
        fin.get();
        wins[index++].loadTimes(atoi(num));
    }
    cout<<"窗口信息载入完毕"<<endl;
    return;
}
void ManageWindow(CWindow * wins)//对指定窗口进行指定操作的结果
{
   
    int index;
    int order;
    cout<<"/**窗口管理界面**/"<<endl;
    cout<<"请输入要管理的窗口号："<<endl;
    cin>>index;
    if(index<1||index>99)
    {
        cout<<"窗口号不存在"<<endl;
        return;
    }
    CWindow & win=wins[index];
    system("cls");
    while(true)
    {
        cout<<"0、退出"<<endl;
        cout<<"1、查询窗口消费情况"<<endl;
        cout<<"2、查询消费日志"<<endl;
        cout<<"请输入指令："<<endl;
        cin>>order;
        if(order==0)
            break;
        switch(order)
        {
        case 1:
            cout<<"窗口消费次数为："<<win.getMessage(TIMESINDEX)<<endl;
            cout<<"窗口总消费金额为："<<win.getMessage(TOTALINDEX)<<endl;
            break;
        case 2:
            win.chkWinLog();
            break;
        default:
            cout<<"指令错误"<<endl;
        }
        system("pause");
        system("cls");
    }
}

void CWindow::chkWinLog()//查询窗口消费记录，记录条数，消费总额
{
    int start;
    int step;
    int num=0;
    double money=0;//存储消费总量
    string key;
    string temp;
    cout<<"请输入查询日期："<<endl;
    cin>>key;
    key+="*";
    if(m_Times<=60000)
        start=0;
    else start=m_Times%60000;
    for(step=0; step<60000; step++)
    {
        if(gWinLog[m_Index][(start+step)%60000].nFlag==1&&FuzzyMatch(gWinLog[m_Index][(start+step)%60000].sLogs,key,0,0))
        {
            cout<<gWinLog[m_Index][(start+step)%60000].sLogs<<endl;
            stringstream ss(gWinLog[m_Index][(start+step)%60000].sLogs);
            getline(ss,temp,',');
            getline(ss,temp,',');
            getline(ss,temp,',');
            getline(ss,temp);
            money+=atof(temp.data());
            num++;
        }
    }
    cout<<"消费记录数："<<num<<endl;
    cout<<"消费总额："<<money<<endl;
    return;
}

void ChangeTime()//设置时间
{
  
    string set_time;
    cout<<"请设置时间："<<endl;
    cin>>set_time;
    if(set_time.length()!=16)
    {cout<<"格式错误，请设置标准十六位时间"<<endl;return;}
    CWindow::sm_Time=set_time;
    cout<<"设置成功"<<endl;
    return;
}

void MergeWinLogs(CWindow *wins,int pattern)//查询指定学号或姓名的消费记录
{

    SBox box[100];
    int cur=1;//当前位段
    int nTotal=100;//总量
    SNode * p=gLogHead;
    //进行初始化与填充
    clock_t start_t,end_t;//记录运行时间
    start_t=clock();
    if(pattern==0)
    {
        for(int i=1; i<100; i++)
        {
            int temptimes=wins[i].getMessage(TIMESINDEX);
            if(temptimes==0)
                continue;
            int start=temptimes;
            start=(start<60000)?1:start%60000+1;
            while(!(gWinLog[i][start%60000].nFlag==1||start>60000))
                start++;
            box[i].index=i;
            box[i].now=start+1;
            strcpy(box[i].str,gWinLog[i][start].sLogs);
        }
        //多路归并排序
        cout<<"正在归并消费数据"<<endl;
        sort(box+cur,box+nTotal,compare);
        while(cur<nTotal)
        {
            if(box[cur].str[0]=='\0'||box[cur].num>=60000)
            {
                cur++;
                continue;
            }
            //建立链表结点
            SNode * temp_node=new SNode;
            strcpy(temp_node->str,box[cur].str);
            p->next=temp_node;
            temp_node->last=p;
            p=temp_node;
            //载入下一条数据
            strcpy(box[cur].str,gWinLog[box[cur].index][box[cur].now%60000].sLogs);
            box[cur].now++;
            box[cur].num++;
            if(nTotal-cur>1)
                ReSortBox(box,cur,nTotal);
        }
    }
    else if(pattern==1)
        sorts();
    end_t=clock();
    //将汇总记录写入文件，并生成安全码
    SafeStore(gLogHead);
    //计算排序消耗时间
    cout<<"排序消耗时间为："<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
    system("pause");
    //进行模糊查找，查找指定学号学生在指定时间段内的消费总量
    string time1,time2,keys;//时间段与关键字
    int order;//指令
    while(true)
    {
        system("cls");
        p=gLogHead->next;
        cout<<"0、退出"<<endl;
        cout<<"1、模糊查找消费记录"<<endl;
        cout<<"2、查询指定学号账户在一定时间段的消费总量"<<endl;
        cout<<"3、查找指定学号关联性"<<endl;
        cout<<"请输入指令："<<endl;
        cin>>order;
        if(order==0)
            break;
        if(order==1)
        {
            int num=0;
            cout<<"请输入学号或姓名：";
            cin>>keys;
            keys="*"+keys+"*";
            start_t=clock();
            while(p!=nullptr)
            {
                if(FuzzyMatch(p->str,keys,17,0))
                {
                    cout<<p->str<<endl;
                    num++;
                }
                p=p->next;
            }
            end_t=clock();
            cout<<"消耗时间为："<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
            cout<<"共查询到"<<num<<"条记录"<<endl;
        }
        if(order==2)
        {
            double money=0;//消费总量
            string temp;
            cout<<"请输入学生学号或姓名："<<endl;
            cin>>keys;
            keys="*"+keys+"*";
            cout<<"请输入开始日期："<<endl;
            cin>>time1;
            cout<<"请输入截止日期（不包含）："<<endl;
            cin>>time2;
            start_t=clock();
            while(p!=nullptr&&p->str<time1) p=p->next;
            while(p!=nullptr&&p->str<time2)
            {
                if(FuzzyMatch(p->str,keys,17,0))
                {
                    cout<<p->str<<endl;
                    stringstream ss(p->str);
                    getline(ss,temp,',');
                    getline(ss,temp,',');
                    getline(ss,temp,',');
                    getline(ss,temp);
                    money+=atof(temp.data());
                }
                p=p->next;
            }
            end_t=clock();
            cout<<"消耗时间为："<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
            cout<<"在该时间段消费总数为："<<money<<"元"<<endl;
        }
        if(order==3)
        {
            cout<<"请输入学号：";
            cin>>keys;
            start_t=clock();
            FindConnection(keys);
            end_t=clock();
            cout<<"消耗时间为："<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        }
        system("pause");
    }
    //释放链表占用空间
    SNode *q=gLogHead->next;
    p=gLogHead->next;
    while(q!=nullptr)
    {
        q=p->next;
        free(p);
        p=q;
    }
}

void FindConnection(string & id)//关联用户的学号与其出现的次数
{
  
    string temp;
    SNode * p=gLogHead->next;
    SNode * q;
    while(p!=nullptr)
    {
        temp=p->str;
        if(temp.substr(17,10)==id)
        {
            q=p->last;
            string temptime=temp.substr(0,16);
            string tempstr=q->str;
            //截取目标前四位消费者
            for(int i=0; i<RANGE&&q!=nullptr&&MatchTime(temptime,tempstr.substr(0,16)); i++,q=q->last,tempstr=q->str)
            {
                string tempid=tempstr.substr(17,10);
                if(!PutInVector(gBox,tempid))
                {
                    SBox newone;
                    strcpy(newone.str,tempid.data());
                    newone.num=1;
                    gBox.push_back(newone);
                }
            }
            q=p->next;
            tempstr=q->str;
            //截取目标后四位消费者
            for(int i=0; i<RANGE&&q!=nullptr&&MatchTime(temptime,tempstr.substr(0,16)); i++,q=q->next,tempstr=q->str)
            {
                string tempid=tempstr.substr(17,10);
                if(!PutInVector(gBox,tempid))
                {
                    SBox newone;
                    strcpy(newone.str,tempid.data());
                    newone.num=1;
                    gBox.push_back(newone);
                }
            }
        }
        p=p->next;
    }
    sort(gBox.begin(),gBox.end(),compare1);
    int size_b=gBox.size();
    int content=min(size_b,8);
    for(int i=0; i<content; i++)
        cout<<gBox[i].str<<","<<gBox[i].num<<endl;
    gBox.clear();
}
void sorts()
{
    for(int i=1;i<100;i++)
    {
        for(int j=0;j<60000;j++)
        {
            if(gWinLog[i][j].nFlag!=0)
            {
                SBox * temp=new SBox;
                strcpy(temp->str,gWinLog[i][j].sLogs);
                gBoxp.push_back(temp);
            }
        }
    }
    sort(gBoxp.begin(),gBoxp.end(),compare4);
    int len=gBoxp.size();
    SNode *p=new SNode;
    SNode *q;
    strcpy(p->str,gBoxp[0]->str);
    gLogHead->next=p;
    for(int i=1;i<len;i++)
    {
        q=new SNode;
        strcpy(q->str,gBoxp[i]->str);
        p->next=q;
        q->last=p;
        p=q;
    }
    gBoxp.clear();
}
