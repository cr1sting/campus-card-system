#ifndef USERS_H_INCLUDED
#define USERS_H_INCLUDED

#include <iostream>
#include <string>
using namespace std;

#define lose_times 99
#define default_code "8888"
#define IDINDEX             0 //卡片信息中卡号的次序
#define NAMEINDEX     1 //卡片信息中卡号的次序
#define STUIDINDEX     2 //卡片信息中卡号的次序
#define MAXNUM          10001


typedef struct SCardId
{
    //参数为卡号以及卡的状态
    string      s_Id;
    bool        bLost = false;
    struct     SCardId* next = nullptr;

}SCardId;

typedef struct SPeriodCharge
{
    //参数为时间段以及此时间段的消费金额
    string     s_Period;
    double   fCharge=0;
}SPeriodCharge;

class CCard   //个人账户
{
private:

    SCardId *   ID;//卡号以及卡片的挂失情况
    string          m_Name;//姓名
    string          m_SchoolNum;//学号
    string          m_DueDate;//使用期限
    string          m_Passwd;//密码
    double        m_Balance;//余额
    int                m_LeftTimes;//剩余补卡次数
public:

    SPeriodCharge m_Sum;//当前时间段的消费金额
    CCard();
    ~CCard();
    double   getBalance();//显示余额
    bool        payCard(double num);//消费
    bool        freezeCard(int order);//冻结挂失或解挂
    string     getMessage(int order);//显示基本信息
    void        chargeCard(double amount);//充值
    bool        verifyPasswd(string input);//验证密码是否正确
    bool        reopenAccount(string newid);//补卡
    void        changePasswd(string newcode);//修改密码
    void        createCard(string _id,string _name,string _stu_id);//开户
};
//系统函数

void        ExpandStr(string & str);//字符串扩展功能，当匹配汉字串时要将?与*字符匹配两次
string      CreateCardNumber(int i);//生成卡号
void        ManageCard(CCard *cards);//单一卡片管理
int         GetUserMessage(int choice);//返回用户信息
void        BuildAccount(CCard * cards);//单一卡片开户
void        FuzzySearch(CCard * cards);//模糊查找功能
void        BuildAccounts(CCard * cards,int maxnum);//批量开户
int           SearchCard(CCard * cards,string id,int index);//查找学号对应的卡片信息
bool        MatchTime(string const &time1,string const &time2);//判断两个时间是否处于同一时间段
int           ManageCard(CCard &card,int order,int x,CCard * cards);//单一卡片管理函数重载
void        ManageCards(CCard *cards,ifstream & fin,string & due_time);//批量卡片管理
bool        FuzzyMatch(string const &str,string const &key,int start1,int start2);//模糊匹配功能
#endif // USERS_H_INCLUDED
