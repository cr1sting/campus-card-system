#ifndef WINDOWS_H_INCLUDED
#define WINDOWS_H_INCLUDED

#include "users.h"

#define upnum 20 //一次密码输入的消费上限
#define RANGE 20//进行相关性查找时的范围
#define INDEX 0
#define TOTALINDEX 1
#define TIMESINDEX 2

typedef struct SBox
{
    //存储指令以、序号以及位次，以及记录条数
    int      index;
    int      now;
    int      num=0;
    char  str[48];
} SBox;

typedef struct SWinLog
{
    //flag表示是否为空，logs记录log内容
    int     nFlag;
    char sLogs[48];
} SWinLog;

typedef struct SNode
{
    //消费申请数据的存储
    char    str[48];
    struct SNode *next=nullptr;
    struct SNode *last;
} SNode;

class CWindow
{
private:

//窗口号

    int           m_Index;//总共消费金额
    double   m_Total;//消费次数
    int           m_Times;//时间，包括日期以及时钟
public:

    static string sm_Time;//时间，为所有类共享
    CWindow();//基本构造函数
    void        chkWinLog();//查看单个窗口的消费记录，六万条回绕
    void        loadTimes(int num);//载入消费次数
    double   getMessage(int order);//基本信息显示函数
    void        loadWinLog(string &log);//记录消费记录
    bool        processApplication(CCard & card,double num,int pattern,string set_time);//处理消费申请
};

void            ChangeTime();//修改整体时间
void            FindConnection(string & id);//查找相关联账户
void            LoadPreTimes(CWindow * wins);//载入wz文件
void            ManageWindow(CWindow * wins);//窗口管理
void            MergeWinLogs(CWindow * wins,int pattern);//合并所有窗口消费记录，并管理后续统计操作
void            FormApplication(CWindow * wins,CCard * cards);//生成单次消费申请
SNode*      FormApplications(CWindow * wins,CCard * cards,SNode * cur,string & due_time);//生成批量消费申请

void sorts();
#endif // WINDOWS_H_INCLUDED
