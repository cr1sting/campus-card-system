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

int                         gWinNum;//��������
vector<SBox>   gBox;//��������Ե�����
vector<SBox *>   gBoxp;
SWinLog             gWinLog[100][60000];//ÿ�����ڵ����Ѽ�¼
SNode *              gLogHead=new SNode;//�ϲ���ļ�¼����ͷ
string CWindow::sm_Time="2022022818000000";//ʱ��

CWindow::CWindow()
{
    //���캯��
    m_Index=gWinNum++;
    m_Total=m_Times=0;
}

bool CWindow::processApplication(CCard & card,double num,int pattern,string set_time=CWindow::sm_Time)
{
    /************************
    * Input��  Ŀ�꿨Ƭ
                    ���ѽ��
                    ģʽ���û�ģʽ�����Աģʽ
                    ����ʱ��
    *Output���޸Ŀ�Ƭ���
                    ���������Ѵ�������
                    ���������ѽ��仯
    *Return��success or not
    ************************/
    string log;
    string code;
    //��Ƭ��ʧʱ�޷�ʹ��
    if(card.freezeCard(0))
    {
        cout<<"��Ƭ���ڹ�ʧ״̬���޷�ʹ��"<<endl;
        return false;
    }
    //����ʱʹ��ʧ��
    if(card.getBalance()-num<0)
    {
        cout<<"����"<<endl;
        return false;
    }
    //�����û������ʱ�����¼ʱ��Ƚϣ���ͬ���滻�ҽ���ǰ���Ѷ���0
    if(!MatchTime(card.m_Sum.s_Period,set_time))
    {
        card.m_Sum.s_Period=set_time;
        card.m_Sum.fCharge=0;
    }
    if(pattern==1){
        if(card.m_Sum.fCharge>upnum||card.m_Sum.fCharge+num>upnum)
        {
            cout<<"�����뿨Ƭ"<<card.getMessage(IDINDEX)<<"������"<<endl;
            cin>>code;
            //������������ʧ��
            if(!card.verifyPasswd(code))
            {
                cout<<"�������"<<endl;
                return false;
            }
            else
                card.m_Sum.fCharge=0;
        }
        else
            card.m_Sum.fCharge+=num;
    }
    card.payCard(num);
    //�������Ѽ�¼���������Ѵ����������Ѷ�
    m_Total+=num;
    m_Times++;
    //log��ʽ��ʱ�䣬ѧ�ţ����������
    log=set_time+","+card.getMessage(STUIDINDEX)+","+card.getMessage(NAMEINDEX)+","+to_string(num);
    loadWinLog(log);
    return true;
}

void CWindow::loadTimes(int num)//�޸Ĵ��������Ѵ���
{
   
    m_Times=num;
}

double CWindow::getMessage(int order)//���ں����ѽ�����         
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

void CWindow::loadWinLog(string &log)//���Ѽ�¼����
{
    
    int j=m_Times%60000;
    strcpy(gWinLog[m_Index][j].sLogs,log.data());
    gWinLog[m_Index][j].nFlag=1;
}

void FormApplication(CWindow * wins,CCard * cards)//���ѳɹ�������ʾ
{
   
    double    num;
    int       win_index;
    int       card_index;
    string card_id,log;
    sort(cards,cards+GetUserMessage(2),compare3);
    while(true)
    {
        cout<<"�����봰�ںţ�����0�˳����򣩣�";
        cin>>win_index;
        if(win_index==0)
            break;
        //���ں�����������
        if(!(win_index>=1&&win_index<=99))
        {
            cout<<"���봰�ںŴ��󣬴��ڷ�ΧΪ1-99"<<endl;
            continue;
        }
        cout<<"�����뿨�ţ�";
        cin>>card_id;
        card_index=SearchCard(cards,card_id,IDINDEX);
        if(card_index==-1)
        {
            cout<<"�ÿ�Ƭ������"<<endl;
            continue;
        }
        cout<<"��������"<<endl;
        cin>>num;
        if(!wins[win_index].processApplication(cards[card_index],num,1))
        {
            log="����,"+cards[card_index].getMessage(IDINDEX)+","+to_string(num)+",ʧ��";
            WriteLog(log);
        }
        else cout<<"���ѳɹ������Ϊ"<<cards[card_index].getBalance()<<endl;
    }
    sort(cards,cards+GetUserMessage(2),compare2);
}

SNode* FormApplications(CWindow * wins,CCard * cards,SNode * cur,string & due_time)//�������ѳɹ���ʧ�ܵĴ���
{
    
    SNode * temp=cur;
    if(cur==nullptr)
        return temp;

    int winindex=0;
    static int flag[2]= {};
    string log;

    //���в��������ļ��ж�ȡ��������
    double price=0;
    string cardid;//��ȡ����
    string time;//��ȡʱ��
    string win_index;
    string num;//��ȡ���ѽ��
    sort(cards,cards+GetUserMessage(2),compare3);
    while(temp!=nullptr)
    {
        //��ȡ����Ϊ���Ѽ�¼�����
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
            log="����,"+cards[index].getMessage(IDINDEX)+","+num+",ʧ��";
            WriteLog(log);
            flag[1]++;
        }
        else flag[0]++;
        //ת�Ƶ���һ��ָ��
        temp=temp->next;
    }
    log="Ŀǰ��������"+to_string(flag[0]+flag[1])+"�����ѣ��ɹ�"+to_string(flag[0])+"�Σ�ʧ��"+to_string(flag[1])+"��";
    cout<<log<<endl;
    WriteLog(log);
    sort(cards,cards+GetUserMessage(2),compare2);
    return temp;
}

void LoadPreTimes(CWindow * wins)//��ȡ�ļ���������
{
    
    int index=1;
    //���в��������ļ��ж�ȡ��������
    char num[8];
    ifstream fin("wz003.txt");
    //�ļ�����״̬�ж�
    if(!fin.is_open())
    {
        cout<<"�ļ���������"<<endl;
        return;
    }
    //��ȡ�ļ���һ���ж��Ƿ�Ϊ���Ѽ�¼�ļ�
    fin.getline(num,3);
    if(strcmp(num,"WZ")!=0)
    {
        cout<<"�ļ���ȡ����"<<endl;
        return;
    }
    while(fin.peek()!=EOF)
    {
        fin.getline(num,3,',');
        fin.getline(num,8,';');
        fin.get();
        wins[index++].loadTimes(atoi(num));
    }
    cout<<"������Ϣ�������"<<endl;
    return;
}
void ManageWindow(CWindow * wins)//��ָ�����ڽ���ָ�������Ľ��
{
   
    int index;
    int order;
    cout<<"/**���ڹ������**/"<<endl;
    cout<<"������Ҫ����Ĵ��ںţ�"<<endl;
    cin>>index;
    if(index<1||index>99)
    {
        cout<<"���ںŲ�����"<<endl;
        return;
    }
    CWindow & win=wins[index];
    system("cls");
    while(true)
    {
        cout<<"0���˳�"<<endl;
        cout<<"1����ѯ�����������"<<endl;
        cout<<"2����ѯ������־"<<endl;
        cout<<"������ָ�"<<endl;
        cin>>order;
        if(order==0)
            break;
        switch(order)
        {
        case 1:
            cout<<"�������Ѵ���Ϊ��"<<win.getMessage(TIMESINDEX)<<endl;
            cout<<"���������ѽ��Ϊ��"<<win.getMessage(TOTALINDEX)<<endl;
            break;
        case 2:
            win.chkWinLog();
            break;
        default:
            cout<<"ָ�����"<<endl;
        }
        system("pause");
        system("cls");
    }
}

void CWindow::chkWinLog()//��ѯ�������Ѽ�¼����¼�����������ܶ�
{
    int start;
    int step;
    int num=0;
    double money=0;//�洢��������
    string key;
    string temp;
    cout<<"�������ѯ���ڣ�"<<endl;
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
    cout<<"���Ѽ�¼����"<<num<<endl;
    cout<<"�����ܶ"<<money<<endl;
    return;
}

void ChangeTime()//����ʱ��
{
  
    string set_time;
    cout<<"������ʱ�䣺"<<endl;
    cin>>set_time;
    if(set_time.length()!=16)
    {cout<<"��ʽ���������ñ�׼ʮ��λʱ��"<<endl;return;}
    CWindow::sm_Time=set_time;
    cout<<"���óɹ�"<<endl;
    return;
}

void MergeWinLogs(CWindow *wins,int pattern)//��ѯָ��ѧ�Ż����������Ѽ�¼
{

    SBox box[100];
    int cur=1;//��ǰλ��
    int nTotal=100;//����
    SNode * p=gLogHead;
    //���г�ʼ�������
    clock_t start_t,end_t;//��¼����ʱ��
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
        //��·�鲢����
        cout<<"���ڹ鲢��������"<<endl;
        sort(box+cur,box+nTotal,compare);
        while(cur<nTotal)
        {
            if(box[cur].str[0]=='\0'||box[cur].num>=60000)
            {
                cur++;
                continue;
            }
            //����������
            SNode * temp_node=new SNode;
            strcpy(temp_node->str,box[cur].str);
            p->next=temp_node;
            temp_node->last=p;
            p=temp_node;
            //������һ������
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
    //�����ܼ�¼д���ļ��������ɰ�ȫ��
    SafeStore(gLogHead);
    //������������ʱ��
    cout<<"��������ʱ��Ϊ��"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
    system("pause");
    //����ģ�����ң�����ָ��ѧ��ѧ����ָ��ʱ����ڵ���������
    string time1,time2,keys;//ʱ�����ؼ���
    int order;//ָ��
    while(true)
    {
        system("cls");
        p=gLogHead->next;
        cout<<"0���˳�"<<endl;
        cout<<"1��ģ���������Ѽ�¼"<<endl;
        cout<<"2����ѯָ��ѧ���˻���һ��ʱ��ε���������"<<endl;
        cout<<"3������ָ��ѧ�Ź�����"<<endl;
        cout<<"������ָ�"<<endl;
        cin>>order;
        if(order==0)
            break;
        if(order==1)
        {
            int num=0;
            cout<<"������ѧ�Ż�������";
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
            cout<<"����ʱ��Ϊ��"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
            cout<<"����ѯ��"<<num<<"����¼"<<endl;
        }
        if(order==2)
        {
            double money=0;//��������
            string temp;
            cout<<"������ѧ��ѧ�Ż�������"<<endl;
            cin>>keys;
            keys="*"+keys+"*";
            cout<<"�����뿪ʼ���ڣ�"<<endl;
            cin>>time1;
            cout<<"�������ֹ���ڣ�����������"<<endl;
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
            cout<<"����ʱ��Ϊ��"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
            cout<<"�ڸ�ʱ�����������Ϊ��"<<money<<"Ԫ"<<endl;
        }
        if(order==3)
        {
            cout<<"������ѧ�ţ�";
            cin>>keys;
            start_t=clock();
            FindConnection(keys);
            end_t=clock();
            cout<<"����ʱ��Ϊ��"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
        }
        system("pause");
    }
    //�ͷ�����ռ�ÿռ�
    SNode *q=gLogHead->next;
    p=gLogHead->next;
    while(q!=nullptr)
    {
        q=p->next;
        free(p);
        p=q;
    }
}

void FindConnection(string & id)//�����û���ѧ��������ֵĴ���
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
            //��ȡĿ��ǰ��λ������
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
            //��ȡĿ�����λ������
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
