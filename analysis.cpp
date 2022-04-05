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
        cout<<"0���˳�����"<<endl;
        cout<<"1����ʾ�û���Ϣ"<<endl;
        cout<<"2����ղ�����־"<<endl;
      
        cout<<"3���޸ľ�̬ʱ��"<<endl;
        cout<<"������ָ�";
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
            cout<<"ָ�����"<<endl;
        }
        system("pause");
        system("cls");
    }
}

void WriteLog(string log)//��������־д��log.txt�ļ�
{
    
    
    fstream fout("opr_log.txt",ios_base::out|ios_base::app);
    if(!fout.is_open())
    {
        //cout<<"��־�ļ���������"<<endl;
        return;
    }
    fout<<log<<"\n";
    fout.close();
}

void ClearLogs(string filename)//����ļ�
{
    ofstream file_writer(filename, ios_base::out);
}

void ShowUserMessage()
{
    cout<<"��ǰ�����ܻ�����"<<GetUserMessage(2)<<endl;
    cout<<"Ŀǰ�����ܿ�����"<<GetUserMessage(1)<<endl;
    return;
}

void OperateS(CWindow * wins,CCard * cards)//��������ĳɹ�������ʧ�ܴ���
{
   
    char        check_code[3];
    clock_t     start_t,end_t;
    SNode *  cur=gAppHead;
    ifstream  fin("cz002.txt");
    string      time1,time2;

    //fin_opr�����ļ�״̬�ж�
    if(!fin.is_open())
    {
        cout<<"�ļ���������"<<endl;
        return;
    }
    fin.getline(check_code,3);
    if(strcmp(check_code,"CZ")!=0)
    {
        cout<<"�ļ���ȡ����"<<endl;
        return;
    }
    if(cur->next==nullptr)
    {
        cout<<"��������δ����"<<endl;
        return ;
    }
    //�ֶβ���
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
    cout<<"����������ʱ�䣺"<<(double)(end_t-start_t)/CLOCKS_PER_SEC<<endl;
    //��������
    fin.close();
    //�ͷ�����ռ�ÿռ�
    SNode *p=gAppHead->next;
    cur=gAppHead->next;
    while(cur!=nullptr&&cur->next!=nullptr)
    {
        cur=p->next;
        free(p);
        p=cur;
    }
}

void SortApplications()//�����ļ��ֳ�С�ļ�
{
   
    int             gTotalCards=0;//���ļ���Ŀ
    char         num[4];//�洢���ں�
    char         temp[35];//��ʱ����
    string       filename="win";//�ļ���
    ifstream  fin("xf014.txt");
    ofstream fout;
    vector<string> files;//�洢���ļ�������
    //�����ļ��ָ�Ϊ���ļ����޸ļ�¼��ʽ����¼�������Ի�
    fin.getline(temp,4);//��ȡ�ļ�ͷ
    if(strcmp(temp,"XF")!=0)
    {
        cout<<"�ļ���ȡ����"<<endl;
        return;
    }
    while(fin.peek()!=EOF)
    {
        //��ȡ���ڴ��ţ��ɴ˴������ļ���
        if(fin.peek()=='W')
        {
            gTotalCards++;
            //��ȡ����Ϊ���ںŵ����
            fout.close();
            filename="win";
            fin.getline(num,4);
            filename+=num+1;
            filename+=".txt";
            files.push_back(filename);
            fout.open(filename,ios::app|ios::out);
            if(fout.fail())
                cout<<"ʧ��"<<endl;
        }
        else
        {
            fin.getline(temp,35);
            string tempstr(temp);//��ʱ�洢
            ReshapeApplications(tempstr,num+1);
            fout<<tempstr<<"\n";
        }
    }
    cout<<"�ļ���װ��ϣ����ڶ�·�鲢����..."<<endl;
    fin.close();
    fout.close();

    SNode * p=gAppHead;
    //��ÿһ���ļ�������ָ��
    ifstream fins[1000];
    for(int i=0; i<gTotalCards; i++)
        fins[i].open(files[i]);
    SBox m[1000];
    //ͨ������������
    for(int i=0; i<gTotalCards; i++)
    {
        m[i].index=i;
        fins[i].getline(m[i].str,40);
    }
    int cur=0;
    //��·�鲢����
    sort(m+cur,m+gTotalCards,compare);
    while(cur<gTotalCards)
    {
        if(m[cur].str[0]=='\0')
        {
            cur++;
            continue;
        }
        //����������
        SNode * temp_node=new SNode;
        strcpy(temp_node->str,m[cur].str);
        p->next=temp_node;
        p=temp_node;
        //������һ������
        fins[m[cur].index].getline(m[cur].str,40);
        if(gTotalCards-cur>1)
            ReSortBox(m,cur,gTotalCards);
    }
    //ɾ����ʱ�ļ�
    cout<<"�������"<<endl;
    for(int i=0; i<gTotalCards; i++)
    {
        fins[i].close();
        remove(files[i].data());
    }
}

bool compare(SBox a,SBox b)//�Ƚ��ַ�����С
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

bool compare2(CCard &card1,CCard &card2) //�ַ����Ƚϣ���ѧ�Ž��бȽ�
{
   
    if(card1.getMessage(STUIDINDEX)<card2.getMessage(STUIDINDEX))
        return true;
    else return false;
}

bool compare3(CCard &card1,CCard &card2)//�ַ����Ƚϣ������Ž��бȽ�
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
void ReshapeApplications(string & str,string  num)//������������
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

void EncryptApplications()//����ȫ��������ļ�
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
            cout<<"��"<<lines<<"�����ݴ��󣬷����۸�"<<endl;
        if(atoi(flow)!=step+1)
        {
            cout<<"��"<<lines-1<<"���������"<<lines<<"������֮�䷢����������Ծ������"<<endl;
            step=atoi(flow);
            lines++;
            continue;
        }
        lines++;
        step++;
    }
}

string FormSafeCode(string log)//��¼��λУ����
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
