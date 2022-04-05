#ifndef ANALYSIS_H_INCLUDED
#define ANALYSIS_H_INCLUDED

#include "users.h"
#include "windows.h"
#include<iostream>
#include<string>
#include<vector>
//汇总分析



void        SortApplications();//对消费文件进行多路归并排序并生成新的文件
void        FindUnexpect();//查找数据中不正常的选项
void        WriteLog(string log);//向日志文件中书写文件
void        EncryptApplications();//安全的消费充值申请生成
void        ShowUserMessage();//显示当前学生总数与卡片总数
bool        compare(SBox a,SBox b);//字符串大小比较
bool        compare1(SBox a,SBox b);//字符串大小比较
bool        compare4(SBox *a,SBox *b);
void        ClearLogs(string filename);//清空文件内容
void        ReSortBox(SBox * box,int start,int until);//汇总分析管理程序
bool        compare2(CCard &card1,CCard &card2);//针对性排序，将第一个位置的数据放到合适的位置
bool        compare3(CCard &card1,CCard &card2);//字符串大小比较
void        OperateS(CWindow * wins,CCard * cards);
void        ReshapeApplications(string & str,string num);//批量操作管理
bool        PutInVector(vector<SBox> &box,string id);//对消费记录进行重塑
void        ManageAnalysis(CWindow *wins,CCard *cards);//判断指定字符串是否存在于容器中
string      FormSafeCode(string log);//分析功能的管理
void SafeStore(SNode * head);//校验码生成函数
#endif // ANALYSIS_H_INCLUDED
