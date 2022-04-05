#ifndef WINDOWS_H_INCLUDED
#define WINDOWS_H_INCLUDED

#include "users.h"

#define upnum 20 //һ�������������������
#define RANGE 20//��������Բ���ʱ�ķ�Χ
#define INDEX 0
#define TOTALINDEX 1
#define TIMESINDEX 2

typedef struct SBox
{
    //�洢ָ���ԡ�����Լ�λ�Σ��Լ���¼����
    int      index;
    int      now;
    int      num=0;
    char  str[48];
} SBox;

typedef struct SWinLog
{
    //flag��ʾ�Ƿ�Ϊ�գ�logs��¼log����
    int     nFlag;
    char sLogs[48];
} SWinLog;

typedef struct SNode
{
    //�����������ݵĴ洢
    char    str[48];
    struct SNode *next=nullptr;
    struct SNode *last;
} SNode;

class CWindow
{
private:

//���ں�

    int           m_Index;//�ܹ����ѽ��
    double   m_Total;//���Ѵ���
    int           m_Times;//ʱ�䣬���������Լ�ʱ��
public:

    static string sm_Time;//ʱ�䣬Ϊ�����๲��
    CWindow();//�������캯��
    void        chkWinLog();//�鿴�������ڵ����Ѽ�¼������������
    void        loadTimes(int num);//�������Ѵ���
    double   getMessage(int order);//������Ϣ��ʾ����
    void        loadWinLog(string &log);//��¼���Ѽ�¼
    bool        processApplication(CCard & card,double num,int pattern,string set_time);//������������
};

void            ChangeTime();//�޸�����ʱ��
void            FindConnection(string & id);//����������˻�
void            LoadPreTimes(CWindow * wins);//����wz�ļ�
void            ManageWindow(CWindow * wins);//���ڹ���
void            MergeWinLogs(CWindow * wins,int pattern);//�ϲ����д������Ѽ�¼�����������ͳ�Ʋ���
void            FormApplication(CWindow * wins,CCard * cards);//���ɵ�����������
SNode*      FormApplications(CWindow * wins,CCard * cards,SNode * cur,string & due_time);//����������������

void sorts();
#endif // WINDOWS_H_INCLUDED
