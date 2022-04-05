#ifndef ANALYSIS_H_INCLUDED
#define ANALYSIS_H_INCLUDED

#include "users.h"
#include "windows.h"
#include<iostream>
#include<string>
#include<vector>
//���ܷ���



void        SortApplications();//�������ļ����ж�·�鲢���������µ��ļ�
void        FindUnexpect();//���������в�������ѡ��
void        WriteLog(string log);//����־�ļ�����д�ļ�
void        EncryptApplications();//��ȫ�����ѳ�ֵ��������
void        ShowUserMessage();//��ʾ��ǰѧ�������뿨Ƭ����
bool        compare(SBox a,SBox b);//�ַ�����С�Ƚ�
bool        compare1(SBox a,SBox b);//�ַ�����С�Ƚ�
bool        compare4(SBox *a,SBox *b);
void        ClearLogs(string filename);//����ļ�����
void        ReSortBox(SBox * box,int start,int until);//���ܷ����������
bool        compare2(CCard &card1,CCard &card2);//��������򣬽���һ��λ�õ����ݷŵ����ʵ�λ��
bool        compare3(CCard &card1,CCard &card2);//�ַ�����С�Ƚ�
void        OperateS(CWindow * wins,CCard * cards);
void        ReshapeApplications(string & str,string num);//������������
bool        PutInVector(vector<SBox> &box,string id);//�����Ѽ�¼��������
void        ManageAnalysis(CWindow *wins,CCard *cards);//�ж�ָ���ַ����Ƿ������������
string      FormSafeCode(string log);//�������ܵĹ���
void SafeStore(SNode * head);//У�������ɺ���
#endif // ANALYSIS_H_INCLUDED
