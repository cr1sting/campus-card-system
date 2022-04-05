#include "analysis.h"
#include "windows.h"
#include <fstream>
#include <ctime>
#include <string.h>
#include <algorithm>
using namespace std;


SNode *gAppHead=new SNode;
int          gFlow;

void ManageAnalysis(CWindow *wins,CCard *cards)
{
   
    int order;
    system("cls");
    while(true)
    {
        cout<<"0、退出程序"<<endl;
        cout<<"1、显示用户信息"<<endl;
        cout<<"2、清空操作日志"<<endl;
      
        cout<<"3、修改静态时间"<<endl;
        cout<<"请输入指令：";
        cin>>order;
        if(order==0) break;
        switch(order)
        {
        case 1:
            ShowUserMessage();
            break;
        case 2:
            ClearLogs("opr_log.txt");
            break;
        /*case 3:
            SortApplications();
            break;*/
        case 3:
            ChangeTime();
            break;
        default:
            cout<<"指令错误"<<endl;
        }
        system("pause");
        system("cls");
    }
}

void WriteLog(string log)//将操作日志写入log.txt文件
{
    
    
    fstream fout("opr_log.txt",ios_base::out|ios_base::app);
    if(!fout.is_open())
    {
        //cout<<"日志文件开启错误"<<endl;
        return;
    }
    fout<<log<<"\n";
    fout.close();
}

void ClearLogs(string filename)//清空文件
{
    ofstream file_writer(filename, ios_base::out);
}

void ShowUserMessage()
{
    cout<<"当前开卡总户数："<<GetUserMessage(2)<<endl;
    cout<<"目前发卡总卡数："<<GetUserMessage(1)<<endl;
    return;
}

void OperateS(CWindow * wins,CCard * cards)//消费申请的成功次数与失败次数
{
   
    char        check_code[3];
    clock_t     start_t,end_t;
    SNode *  cur=gAppHead;
    ifstream  fin("cz002.txt");
    string      time1,time2;

    //fin_opr开启文件状态判断
    if(!fin.is_open())
    {
        cout<<"文件开启错误"<<endl;
        return;
    }
    fin.getline(check_code,3);
    if(strcmp(check_code,"CZ")!=0)
    {
        cout<<"文件读取错误"<<endl;
        return;
    }
    if(cur->next==nullptr)
    {
        cout<<"消费数据未导入"<<endl;
        return ;
    }
    //分段操作
    start_t=clock();
    time1="20210906";
    time2="20210927";
    ManageCards(cards,fin,time1);
    cur=FormApplications(wins,cards,cur->next,time2);
    time1="20210907";
    time2="20211014";
    ManageCards(cards,fin,time1);
    cur=FormApplications(wins,cards,cur,time2);
    time1="20211015";
    time2="20211104";
    ManageCards(cards,fin,time1);
    cur=FormApplications(wins,cards,cur,time2);
    time1="20211105";
    time2="20211125";
    ManageCards(cards,fin,time1);
    cur=FormApplications(wins,cards,cur,time2);
    time1="20211126";
    time2="20211216";
    ManageCards(cards,fin,time1);
    cur=FormApplications(wins,cards,cur,time2);
    time1="20211217";
    time2="20211232";
    ManageCards(cards,fin,time1);
    cur=FormApplications(wins,cards,cur,time2);
    end_t=clock();
    cout<<"操作共花费时间："<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
    //操作结束
    fin.close();
    //释放链表占用空间
    SNode *p=gAppHead->next;
    cur=gAppHead->next;
    while(cur!=nullptr&&cur->next!=nullptr)
    {
        cur=p->next;
        free(p);
        p=cur;
    }
}

void SortApplications()//将大文件分成小文件
{
   
    int             gTotalCards=0;//子文件数目
    char         num[4];//存储窗口号
    char         temp[35];//临时储存
    string       filename="win";//文件名
    ifstream  fin("xf014.txt");
    ofstream fout;
    vector<string> files;//存储子文件名容器
    //将大文件分割为子文件并修改记录格式，记录更加人性化
    fin.getline(temp,4);//读取文件头
    if(strcmp(temp,"XF")!=0)
    {
        cout<<"文件读取错误"<<endl;
        return;
    }
    while(fin.peek()!=EOF)
    {
        //读取窗口代号，由此创建子文件名
        if(fin.peek()=='W')
        {
            gTotalCards++;
            //读取内容为窗口号的情况
            fout.close();
            filename="win";
            fin.getline(num,4);
            filename+=num+1;
            filename+=".txt";
            files.push_back(filename);
            fout.open(filename,ios::app|ios::out);
            if(fout.fail())
                cout<<"失败"<<endl;
        }
        else
        {
            fin.getline(temp,35);
            string tempstr(temp);//临时存储
            ReshapeApplications(tempstr,num+1);
            fout<<tempstr<<"\n";
        }
    }
    cout<<"文件分装完毕，正在多路归并排序..."<<endl;
    fin.close();
    fout.close();

    SNode * p=gAppHead;
    //对每一个文件建立流指针
    ifstream fins[1000];
    for(int i=0; i<gTotalCards; i++)
        fins[i].open(files[i]);
    SBox m[1000];
    //通过流读入内容
    for(int i=0; i<gTotalCards; i++)
    {
        m[i].index=i;
        fins[i].getline(m[i].str,40);
    }
    int cur=0;
    //多路归并排序
    sort(m+cur,m+gTotalCards,compare);
    while(cur<gTotalCards)
    {
        if(m[cur].str[0]=='\0')
        {
            cur++;
            continue;
        }
        //建立链表结点
        SNode * temp_node=new SNode;
        strcpy(temp_node->str,m[cur].str);
        p->next=temp_node;
        p=temp_node;
        //载入下一条数据
        fins[m[cur].index].getline(m[cur].str,40);
        if(gTotalCards-cur>1)
            ReSortBox(m,cur,gTotalCards);
    }
    //删除临时文件
    cout<<"读入完毕"<<endl;
    for(int i=0; i<gTotalCards; i++)
    {
        fins[i].close();
        remove(files[i].data());
    }
}

bool compare(SBox a,SBox b)//比较字符串大小
{
    
    if(strcmp(a.str,b.str)<0)
        return true;
    else return false;
}

bool compare1(SBox a,SBox b)
{
    if(a.num<b.num)
        return false;
    else return true;
}

bool compare2(CCard &card1,CCard &card2) //字符串比较，按学号进行比较
{
   
    if(card1.getMessage(STUIDINDEX)<card2.getMessage(STUIDINDEX))
        return true;
    else return false;
}

bool compare3(CCard &card1,CCard &card2)//字符串比较，按卡号进行比较
{
    
    if(card1.getMessage(IDINDEX)<card2.getMessage(IDINDEX))
        return true;
    else return false;
}

bool compare4(SBox* a,SBox* b)
{
    if(strcmp(a->str,b->str)<0)
        return true;
    else return false;
}
void ReshapeApplications(string & str,string  num)//批量操作管理
{
    string temp;
    temp=str.substr(8,8)+str.substr(17,9)+str.substr(0,8)+num+str.substr(25);
    str=temp;
}

void ReSortBox(SBox * box,int start,int until)
{
    
    while(start<until-1&&strcmp(box[start].str,box[start+1].str)>0)
    {
        swap(box[start],box[start+1]);
        start++;
    }
    return;
}

bool PutInVector(vector<SBox> &box,string id)
{
    
    int len=box.size();
    for(int i=0; i<len; i++)
    {
        if(strcmp(box[i].str,id.data())==0)
        {
            box[i].num++;
            return true;
        }
    }
    return false;
}

void EncryptApplications()//将安全申请放入文件
{
   
    ifstream fin("secure.txt");
    char flow[10];
    char log[48];
    char safe[5];
    int lines=1;
    int step=0;
    while(fin.peek()!=EOF)
    {
        fin.getline(flow,10,',');
        fin.getline(log,48,';');
        fin.getline(safe,5);
        if(strcmp(safe,FormSafeCode(log).data())!=0)
            cout<<"第"<<lines<<"行数据错误，发生篡改"<<endl;
        if(atoi(flow)!=step+1)
        {
            cout<<"第"<<lines-1<<"行数据与第"<<lines<<"行数据之间发生不正常跳跃，请检查"<<endl;
            step=atoi(flow);
            lines++;
            continue;
        }
        lines++;
        step++;
    }
}

string FormSafeCode(string log)//记录四位校验码
{
    
    string code="!!!!";
    int i=0,j=0,len=log.length();
    for(i=0;i<len;)
        for(j=0;j<4&&i<len;j++,i++)
    {
        if(log[i]<33)
            continue;
        code[j]=code[j]^log[i];
    }
    for(i=0;i<4;i++)
        code[i]=code[i]%84+33;
    return code;
}

void SafeStore(SNode * head)
{
    ofstream fout("secure.txt",ios::out);
    int i=1;
    SNode * p=head->next;
    while(p!=nullptr)
    {
        fout<<i<<","<<p->str<<";"<<FormSafeCode(p->str)<<"\n";
        i++;
        p=p->next;
    }
    fout.close();
}
