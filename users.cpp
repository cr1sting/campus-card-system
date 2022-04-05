#include <iomanip>
#include <string.h>
#include <fstream>
#include <algorithm>
#include "users.h"
#include "analysis.h"
using namespace std;


int gTotalCards;//卡片总数
int gTotalStu;//学生总数

CCard::CCard() //默认构造函数
{
   
    ID = new SCardId();
    m_Passwd=default_code;
    m_LeftTimes=lose_times;
    m_Balance=0;
    m_DueDate="20240900";
}

CCard::~CCard()
{
    
    
}

void CCard::createCard(string _id,string _name,string _stu_id) //开户函数
{
    
   
    string log;
    ID->s_Id=_id;
    m_Name=_name;
    m_SchoolNum=_stu_id;
    log=m_SchoolNum+","+m_Name+","+"开户,"+"成功";
    WriteLog(log);
}

double CCard::getBalance() //返回卡内余额
{
   
    return m_Balance;
}

bool CCard::payCard(double num)
{
    
    //消费，扣除卡内余额
    //余额不足情况
    string log;
    if(m_Balance-num<0)
        return false;
    else m_Balance-=num;
    log="消费,"+ID->s_Id+","+to_string(m_Balance)+","+to_string(m_Balance+num)+","+"成功";
    WriteLog(log);
    return true;
}

bool CCard::freezeCard(int order) //挂失或者解挂功能
{
   
    //1表示挂失，2表示解除挂失状态，0不做操作，返回卡的状态
    string log;
    if(order==0) return ID->bLost;
    if(order==1)
    {
        if(ID->bLost!=true)
        {
            ID->bLost=true;
            log=ID->s_Id+","+m_SchoolNum+","+m_Name+","+"挂失,"+"成功";
            WriteLog(log);
        }
    }
    else if(order==2)
    {
        if(ID->bLost!=false)
        {
            ID->bLost=false;
            log=ID->s_Id+","+m_SchoolNum+","+m_Name+","+"解挂,"+"成功";
            WriteLog(log);
        }
    }
    else cout<<"指令错误"<<endl;
    return ID->bLost;
}

bool CCard::reopenAccount(string newid)//补卡功能,将从前的卡号放置链尾，新卡号始终处于链首
{
    string log;
    if(m_LeftTimes<=0)
    {
        log=ID->s_Id+","+m_SchoolNum+","+m_Name+","+"补卡,"+"失败";
        WriteLog(log);
        return false;
    }
    else
    {
        m_LeftTimes--;
        SCardId *new_id=new SCardId;
        SCardId *temp=ID;
        new_id->s_Id=newid;
        new_id->next=temp;
        ID=new_id;
        log=ID->s_Id+","+m_SchoolNum+","+m_Name+","+"补卡,"+"成功";
    }
    WriteLog(log);
    return true;
}

string CCard::getMessage(int order) //0返回卡号，1返回姓名，2返回学号
{
   
    switch(order)
    {
    case IDINDEX:
        return ID->s_Id;
    case NAMEINDEX:
        return m_Name;
    case STUIDINDEX:
        return m_SchoolNum;
    default:
        return "what?";
    }
}

void CCard::chargeCard(double amount)//充值功能
{
    string log;
    m_Balance+=amount;
    log=m_SchoolNum+","+m_Name+","+to_string(m_Balance-amount)+","+to_string(m_Balance)+","+"充值,"+"成功";
    WriteLog(log);
}

void CCard::changePasswd(string newcode)//修改密码功能实现
{
    
    
    m_Passwd=newcode;
    return;
}

bool CCard::verifyPasswd(string input) //验证密码是否正确功能实现
{
   
   
    if(input==m_Passwd)
        return true;
    else return false;
}

void ManageCard(CCard *cards)//卡片功能管理
{
   
    int order=-2;
    string stuid;

    cout<<"请输入学号"<<endl;
    cin>>stuid;
    int index=SearchCard(cards,stuid,STUIDINDEX);
    if(index==-1)
    {
        cout<<"该学号不存在"<<endl;
        return;
    }
    while(order!=-1)
    {
        system("pause");
        system("cls");
        cout<<"**卡片基本信息**"<<endl;
        cout<<"卡号："<<cards[index].getMessage(0)<<endl;
        cout<<"姓名："<<cards[index].getMessage(1)<<endl;
        cout<<"余额："<<cards[index].getBalance()<<endl;
        cout<<"/*********功能表**********/"<<endl;
        cout<<"-1.退出"<<endl;
        cout<<"0.账户充值"<<endl;
        cout<<"1.密码修改"<<endl;
        cout<<"2.挂失与解挂"<<endl;
        cout<<"3.补卡"<<endl;
        cout<<"4.销户"<<endl;
        cout<<"请输入指令"<<endl;
        cin>>order;
        //充值功能管理
        if(order==0)
        {
            double amount;
            string input;
            string log=cards[index].getMessage(2)+","+cards[index].getMessage(1)+","+"充值,"+"失败";

            cout<<"欢迎进入充值界面"<<endl;
            //判断卡片是否挂失
            if(cards[index].freezeCard(0))
            {
                cout<<"卡片处于挂失状态，无法进行充值"<<endl;
                WriteLog(log);
                continue;
            }
            cout<<"请输入密码"<<endl;
            cin>>input;
            if(!cards[index].verifyPasswd(input))
            {
                cout<<"密码输入错误"<<endl;
                WriteLog(log);
                continue;
            }
            if(cards[index].getBalance()==999.99)
            {
                cout<<"您的卡余额已满，无需充值"<<endl;
                WriteLog(log);
                continue;
            }
            do
            {
                cout<<"请输入金额"<<endl;
                cin>>amount;
                if(cards[index].getBalance()+amount>999.99)
                    cout<<"该操作会造成超额，充值无效，请重新输入金额"<<endl;
            }
            while(cards[index].getBalance()+amount>999.99);
            cards[index].chargeCard(amount);
            cout<<"充值成功！"<<endl;
            cout<<"您卡内的余额为"<<cards[index].getBalance()<<endl;
        }
        //密码修改管理
        //先进行原密码验证，再输入新密码修改
        if(order==1)
        {
            cout<<"欢迎进入密码修改界面"<<endl;
            string newcode,judgecode,input;
            cout<<"请输入原始密码"<<endl;
            cin>>input;
            if(!cards[index].verifyPasswd(input))
            {
                cout<<"密码输入错误"<<endl;
                continue;
            }
            do
            {
                cout<<"请输入新的密码"<<endl;
                cin>>newcode;
                cout<<"请再次确认密码"<<endl;
                cin>>judgecode;
                if(newcode==judgecode)
                    cards[index].changePasswd(newcode);
                else cout<<"两次输入不相符，请重新设置密码"<<endl;
            }
            while(newcode!=judgecode);
            cout<<"密码修改成功"<<endl;
        }
        //挂失与解挂
        if(order==2)
        {
            int yes_no;//指令
            cout<<"欢迎进入卡片挂失与解挂界面"<<endl;
            cout<<"请确认基本信息"<<endl;
            cout<<"卡号："<<cards[index].getMessage(0)<<endl;
            cout<<"姓名："<<cards[index].getMessage(1)<<endl;
            if(cards[index].freezeCard(0))
                cout<<"当前卡片处于挂失状态"<<endl;
            else cout<<"当前卡片处于正常状态"<<endl;
            cout<<"请输入指令，1进行挂失，2解挂，其他退出"<<endl;
            cin>>yes_no;
            switch(yes_no)
            {
            case 1:
                cards[index].freezeCard(1);
                cout<<"成功挂失，卡片"<<cards[index].getMessage(0)<<"消费功能将被封锁"<<endl;
                break;
            case 2:
                cards[index].freezeCard(2);
                cout<<"成功解挂，卡片"<<cards[index].getMessage(0)<<"消费功能将恢复"<<endl;
                break;
            default:
                break;
            }
        }
        //补卡功能管理
        if(order==3)
        {
            cout<<"欢迎进入补卡界面"<<endl;
            cout<<"已对您卡号为"<<cards[index].getMessage(0)<<"的卡片进行挂失操作"<<endl;
            cards[index].freezeCard(1);
            cout<<"即将为您分配新的卡号…………"<<endl;
            if(cards[index].reopenAccount(CreateCardNumber(gTotalCards)))
            {
                gTotalCards++;
                cout<<"您的新卡卡号为"<<cards[index].getMessage(0)<<endl;
            }
            else cout<<"补卡次数已经耗尽"<<endl;
        }
        //销户管理
        if(order==4)
        {
            string log;
            cout<<"请确认基本信息"<<endl;
            cout<<"学号："<<cards[index].getMessage(2)<<endl;
            cout<<"姓名："<<cards[index].getMessage(1)<<endl;
            cout<<"是否进行销户，确认请按y/Y，否则将退出"<<endl;
            char y_Y;
            cin>>y_Y;
            if(y_Y=='y'||y_Y=='Y')
            {
                for(int i=index; i<gTotalStu; i++)
                    cards[i]=cards[i+1];
                gTotalStu--;
                cout<<"销户成功"<<endl;
                log=stuid+","+"销户,"+"成功";
            }
            WriteLog(log);
            break;
        }
    }
}

int ManageCard(CCard &card,int order,int x=0,CCard * cards=nullptr)//个人功能目录
{
    
    if(order==0)
    {
        string log=card.getMessage(2)+","+card.getMessage(1)+","+"充值,"+"失败";
        //判断卡片是否挂失
        if(card.freezeCard(0)||card.getBalance()==999.99||card.getBalance()+x>999.99)
        {
            WriteLog(log);
            return 0;
        }
        card.chargeCard(x);
    }
    if(order==1)
    {
        if(!card.freezeCard(0))
            card.freezeCard(1);
    }
    if(order==2)
    {
        if(card.freezeCard(0))
            card.freezeCard(2);
    }
    if(order==3)
    {
        card.freezeCard(1);
        card.reopenAccount(CreateCardNumber(gTotalCards++));
    }
    if(order==4)
    {
        for(int i=x; i<gTotalStu; i++)
            cards[i]=cards[i+1];
        gTotalStu--;
    }
    return 1;
}

void BuildAccount(CCard * cards)//单一卡片开户程序
{
    string stuid;
    string name;
    string log;
    if(gTotalStu==MAXNUM-1)
    {cout<<"满载！！！"<<endl;return;}
    cout<<"欢迎进入开户界面"<<endl;
    cout<<"请输入学号"<<endl;
    cin>>stuid;
    int i=SearchCard(cards,stuid,STUIDINDEX);
    if(i!=-1)
    {
        cout<<"该学号已进行开户操作"<<endl;
        WriteLog(stuid+","+"开户,"+"失败");
        return;
    }
    cout<<"请输入姓名"<<endl;
    cin>>name;
    cards[gTotalStu++].createCard(CreateCardNumber(gTotalCards++),name,stuid);
    cout<<"开户成功，您的卡号为"<<cards[gTotalStu-1].getMessage(0)<<endl;
    //按学号从低到高排序
    sort(cards,cards+gTotalStu,compare2);
    return;
}

void BuildAccounts(CCard * cards,int maxnum)//批量开户程序
{
      
    char stuid[11];
    char name[9];
    ifstream fin("kh001.txt");
    if(!fin.is_open())
    {
        cout<<"文件开启错误"<<endl;
        return;
    }
    fin.getline(stuid,3);
    if(strcmp(stuid,"KH")!=0)
    {
        cout<<"文件读取错误"<<endl;
        return;
    }
    for(int i=0; i<maxnum&&fin.peek()!=EOF; i++)
    {
        fin.getline(stuid,11,',');
        fin.getline(name,9,';');
        fin.get();
        cards[i].createCard(CreateCardNumber(gTotalCards++),name,stuid);
        gTotalStu++;
    }
    fin.close();
    //排序
    sort(cards,cards+gTotalStu,compare2);
    cout<<"此次批量开户完成，共发出"<<gTotalCards<<"张卡片，用户载入完毕"<<endl;
}

void ManageCards(CCard *cards,ifstream & fin,string & due_time) //批量卡片管理程序
{
    int         x=0;//充值金额
    int         times[5]= {}; //记录每种操作的次数
    char     stuid[11];//读取学号
    char     time[17];//读取操作时间
    char     order[5];//读取操作类型
    string   log;
    //读取文件内容
    while(fin.peek()!=EOF)
    {
        char to_x[4];//读取充值金额
        fin.getline(time,17,',');//读取时间
        //当超过指定时间时操作结束
        if(strcmp(time,due_time.data())>=0)
        {
            fin.seekg(-17,ios::cur);
            break;
        }
        fin.getline(order,5,',');//读取命令
        if(strcmp(order,"充值")==0)
        {
            //如果指令为充值，读取方式会有所不同
            fin.getline(stuid,11,',');
            fin.getline(to_x,4,';');
            x=atoi(to_x);
        }
        else fin.getline(stuid,11,';');
        fin.get();
        int index=SearchCard(cards,stuid,STUIDINDEX);
        if(index==-1)
            continue;

        if(strcmp(order,"充值")==0)
            times[0]+=ManageCard(cards[index],0,x);
        if(strcmp(order,"挂失")==0)
            times[1]+=ManageCard(cards[index],1);
        if(strcmp(order,"解挂")==0)
            times[2]+=ManageCard(cards[index],2);
        if(strcmp(order,"补卡")==0)
            times[3]+=ManageCard(cards[index],3);
        if(strcmp(order,"销户")==0)
            times[4]+=ManageCard(cards[index],4,index,cards);
    }
    log="进行充值"+to_string(times[0])+"次,"+"进行挂失"+to_string(times[1])+"次,"+"进行解挂"+to_string(times[2])+"次,"+"进行补卡"+to_string(times[3])+"次,"+"进行销户"+to_string(times[4])+"次.";
    WriteLog(log);
    cout<<log<<endl;
}

string CreateCardNumber(int i)
{
    /************************
    * Input：  发的第几张卡
    *Output：null
    *Return：返回新的卡号
    ************************/
    //生成卡号程序
    int     left;
    int     sum=3;
    int     nFlow=12346+i;
    char id[8];
    id[0]='3';
    for(int j=0; j<5; j++)
    {
        left=nFlow%10;
        id[5-j]='0'+left;
        nFlow/=10;
        sum+=left;
    }
    id[6]='0'+9-sum%10;
    id[7] = '\0';
    return id;
}

int SearchCard(CCard * cards,string id,int index) //使用二分法查询学号对应的卡
{      
    int low=0;
    int high=gTotalStu-1;
    int mid;
    int flag;
    while(low<=high)
    {
        mid=(low+high)/2;
        flag=strcmp(cards[mid].getMessage(index).data(),id.data());
        if(flag==0)
            return mid;
        if(flag>0)
            high=mid-1;
        else low=mid+1;
    }
    return -1;
}

bool MatchTime(string const &time1,string const &time2)//判断两个时间是否处于同一时间段
{    
    string a=time1.substr(0,8);
    string b=time2.substr(0,8);
    if(a==b&&abs(time1[10]-time2[10])<=1)
        return  true;
    else return false;
}

void FuzzySearch(CCard * cards)//模糊查找
{  
    int         order,nums;
    string   key;
    while(true)
    {
        nums=0;
        cout<<"0、按卡号查找"<<endl;
        cout<<"1、按姓名查找"<<endl;
        cout<<"2、按学号查找"<<endl;
        cout<<"3、退出"<<endl;
        cout<<"请输入指令：";
        cin>>order;
        if(order==3)
            break;
        if(order<0||order>3)
        {
            cout<<"指令错误"<<endl;
            continue;
        }
        cout<<"请输入关键字：";
        cin>>key;
        if(order==NAMEINDEX)
            ExpandStr(key);
        for(int i=0; i<gTotalStu; i++)
        {
            string temp=cards[i].getMessage(order);
            if(FuzzyMatch(temp,key,0,0))
            {
                cout<<cards[i].getMessage(STUIDINDEX)<<" "<<cards[i].getMessage(IDINDEX)<<" "<<cards[i].getMessage(NAMEINDEX)<<endl;
                nums++;
            }
        }
        cout<<"共查找到"<<nums<<"组匹配数据"<<endl;
    }
}
bool FuzzyMatch(string const &str,string const &key,int start1,int start2) //模糊匹配功能，不完善
{

    int i=start1;
    int j=start2;
    int len1=str.size();
    int len2=key.size();
    bool truth=false;
    //为空串时不匹配
    if(len1==0)
        return false;
    if(i==len1)
        return true;
    while(i<len1&&j<len2)
    {
        //字符为?时满足任何匹配
        if(key[j]=='?')
        {
            i++;
            j++;
            continue;
        }
        //字符为*时采用递归
        if(key[j]!='*'&&!(key[j]==str[i]))
            return false;
        if(key[j]=='*')
        {
            //当*为最后一个字符时，满足匹配
            //当同时有多个*并列时要合并
            while(key[j]=='*') j++;
            if(j==len2)
                return true;
            else
                for(int m=0; m<len1-i; m++)
                    if(str[i+m]==key[j]||key[j]=='?')
                        truth=truth||FuzzyMatch(str,key,i+m,j);
            return truth;
        }
        //其他情况若两字符不同，则不满足匹配
        i++;
        j++;
    }
    //考虑关键字串最后一位为*的情况
    while(key[j]=='*')
        j++;
    if(i<len1||j<len2)
        return false;
    else return true;
}

void ExpandStr(string & str)//字符串扩展
{
    
    
    int len=str.size();
    int ex_len=len;
    for(int i=0; i<len; i++)
        if(str[i]=='*'||str[i]=='?')
            ex_len++;
    //当不存在?或*字符时，直接返回
    if(ex_len==len)
        return;
    char temp[10];
    /*while (ex_len>0)
    {
        ex_len--;
        len--;
        
    }*/
    str=temp;
    return;
}

int GetUserMessage(int choice)//用户的大体信息统计
{  
    switch(choice)
    {
    case 1:
        return gTotalCards;
    case 2:
        return gTotalStu;
    default:
        return -1;
    }
}
