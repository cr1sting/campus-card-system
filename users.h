#ifndef USERS_H_INCLUDED
#define USERS_H_INCLUDED

#include <iostream>
#include <string>
using namespace std;

#define lose_times 99
#define default_code "8888"
#define IDINDEX             0 //��Ƭ��Ϣ�п��ŵĴ���
#define NAMEINDEX     1 //��Ƭ��Ϣ�п��ŵĴ���
#define STUIDINDEX     2 //��Ƭ��Ϣ�п��ŵĴ���
#define MAXNUM          10001


typedef struct SCardId
{
    //����Ϊ�����Լ�����״̬
    string      s_Id;
    bool        bLost = false;
    struct     SCardId* next = nullptr;

}SCardId;

typedef struct SPeriodCharge
{
    //����Ϊʱ����Լ���ʱ��ε����ѽ��
    string     s_Period;
    double   fCharge=0;
}SPeriodCharge;

class CCard   //�����˻�
{
private:

    SCardId *   ID;//�����Լ���Ƭ�Ĺ�ʧ���
    string          m_Name;//����
    string          m_SchoolNum;//ѧ��
    string          m_DueDate;//ʹ������
    string          m_Passwd;//����
    double        m_Balance;//���
    int                m_LeftTimes;//ʣ�ಹ������
public:

    SPeriodCharge m_Sum;//��ǰʱ��ε����ѽ��
    CCard();
    ~CCard();
    double   getBalance();//��ʾ���
    bool        payCard(double num);//����
    bool        freezeCard(int order);//�����ʧ����
    string     getMessage(int order);//��ʾ������Ϣ
    void        chargeCard(double amount);//��ֵ
    bool        verifyPasswd(string input);//��֤�����Ƿ���ȷ
    bool        reopenAccount(string newid);//����
    void        changePasswd(string newcode);//�޸�����
    void        createCard(string _id,string _name,string _stu_id);//����
};
//ϵͳ����

void        ExpandStr(string & str);//�ַ�����չ���ܣ���ƥ�人�ִ�ʱҪ��?��*�ַ�ƥ������
string      CreateCardNumber(int i);//���ɿ���
void        ManageCard(CCard *cards);//��һ��Ƭ����
int         GetUserMessage(int choice);//�����û���Ϣ
void        BuildAccount(CCard * cards);//��һ��Ƭ����
void        FuzzySearch(CCard * cards);//ģ�����ҹ���
void        BuildAccounts(CCard * cards,int maxnum);//��������
int           SearchCard(CCard * cards,string id,int index);//����ѧ�Ŷ�Ӧ�Ŀ�Ƭ��Ϣ
bool        MatchTime(string const &time1,string const &time2);//�ж�����ʱ���Ƿ���ͬһʱ���
int           ManageCard(CCard &card,int order,int x,CCard * cards);//��һ��Ƭ����������
void        ManageCards(CCard *cards,ifstream & fin,string & due_time);//������Ƭ����
bool        FuzzyMatch(string const &str,string const &key,int start1,int start2);//ģ��ƥ�书��
#endif // USERS_H_INCLUDED
