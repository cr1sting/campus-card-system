#include <iomanip>
#include <string.h>
#include <fstream>
#include <algorithm>
#include "users.h"
#include "analysis.h"
using namespace std;


int gTotalCards;//��Ƭ����
int gTotalStu;//ѧ������

CCard::CCard() //Ĭ�Ϲ��캯��
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

void CCard::createCard(string _id,string _name,string _stu_id) //��������
{
    
   
    string log;
    ID->s_Id=_id;
    m_Name=_name;
    m_SchoolNum=_stu_id;
    log=m_SchoolNum+","+m_Name+","+"����,"+"�ɹ�";
    WriteLog(log);
}

double CCard::getBalance() //���ؿ������
{
   
    return m_Balance;
}

bool CCard::payCard(double num)
{
    
    //���ѣ��۳��������
    //�������
    string log;
    if(m_Balance-num<0)
        return false;
    else m_Balance-=num;
    log="����,"+ID->s_Id+","+to_string(m_Balance)+","+to_string(m_Balance+num)+","+"�ɹ�";
    WriteLog(log);
    return true;
}

bool CCard::freezeCard(int order) //��ʧ���߽�ҹ���
{
   
    //1��ʾ��ʧ��2��ʾ�����ʧ״̬��0�������������ؿ���״̬
    string log;
    if(order==0) return ID->bLost;
    if(order==1)
    {
        if(ID->bLost!=true)
        {
            ID->bLost=true;
            log=ID->s_Id+","+m_SchoolNum+","+m_Name+","+"��ʧ,"+"�ɹ�";
            WriteLog(log);
        }
    }
    else if(order==2)
    {
        if(ID->bLost!=false)
        {
            ID->bLost=false;
            log=ID->s_Id+","+m_SchoolNum+","+m_Name+","+"���,"+"�ɹ�";
            WriteLog(log);
        }
    }
    else cout<<"ָ�����"<<endl;
    return ID->bLost;
}

bool CCard::reopenAccount(string newid)//��������,����ǰ�Ŀ��ŷ�����β���¿���ʼ�մ�������
{
    string log;
    if(m_LeftTimes<=0)
    {
        log=ID->s_Id+","+m_SchoolNum+","+m_Name+","+"����,"+"ʧ��";
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
        log=ID->s_Id+","+m_SchoolNum+","+m_Name+","+"����,"+"�ɹ�";
    }
    WriteLog(log);
    return true;
}

string CCard::getMessage(int order) //0���ؿ��ţ�1����������2����ѧ��
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

void CCard::chargeCard(double amount)//��ֵ����
{
    string log;
    m_Balance+=amount;
    log=m_SchoolNum+","+m_Name+","+to_string(m_Balance-amount)+","+to_string(m_Balance)+","+"��ֵ,"+"�ɹ�";
    WriteLog(log);
}

void CCard::changePasswd(string newcode)//�޸����빦��ʵ��
{
    
    
    m_Passwd=newcode;
    return;
}

bool CCard::verifyPasswd(string input) //��֤�����Ƿ���ȷ����ʵ��
{
   
   
    if(input==m_Passwd)
        return true;
    else return false;
}

void ManageCard(CCard *cards)//��Ƭ���ܹ���
{
   
    int order=-2;
    string stuid;

    cout<<"������ѧ��"<<endl;
    cin>>stuid;
    int index=SearchCard(cards,stuid,STUIDINDEX);
    if(index==-1)
    {
        cout<<"��ѧ�Ų�����"<<endl;
        return;
    }
    while(order!=-1)
    {
        system("pause");
        system("cls");
        cout<<"**��Ƭ������Ϣ**"<<endl;
        cout<<"���ţ�"<<cards[index].getMessage(0)<<endl;
        cout<<"������"<<cards[index].getMessage(1)<<endl;
        cout<<"��"<<cards[index].getBalance()<<endl;
        cout<<"/*********���ܱ�**********/"<<endl;
        cout<<"-1.�˳�"<<endl;
        cout<<"0.�˻���ֵ"<<endl;
        cout<<"1.�����޸�"<<endl;
        cout<<"2.��ʧ����"<<endl;
        cout<<"3.����"<<endl;
        cout<<"4.����"<<endl;
        cout<<"������ָ��"<<endl;
        cin>>order;
        //��ֵ���ܹ���
        if(order==0)
        {
            double amount;
            string input;
            string log=cards[index].getMessage(2)+","+cards[index].getMessage(1)+","+"��ֵ,"+"ʧ��";

            cout<<"��ӭ�����ֵ����"<<endl;
            //�жϿ�Ƭ�Ƿ��ʧ
            if(cards[index].freezeCard(0))
            {
                cout<<"��Ƭ���ڹ�ʧ״̬���޷����г�ֵ"<<endl;
                WriteLog(log);
                continue;
            }
            cout<<"����������"<<endl;
            cin>>input;
            if(!cards[index].verifyPasswd(input))
            {
                cout<<"�����������"<<endl;
                WriteLog(log);
                continue;
            }
            if(cards[index].getBalance()==999.99)
            {
                cout<<"���Ŀ���������������ֵ"<<endl;
                WriteLog(log);
                continue;
            }
            do
            {
                cout<<"��������"<<endl;
                cin>>amount;
                if(cards[index].getBalance()+amount>999.99)
                    cout<<"�ò�������ɳ����ֵ��Ч��������������"<<endl;
            }
            while(cards[index].getBalance()+amount>999.99);
            cards[index].chargeCard(amount);
            cout<<"��ֵ�ɹ���"<<endl;
            cout<<"�����ڵ����Ϊ"<<cards[index].getBalance()<<endl;
        }
        //�����޸Ĺ���
        //�Ƚ���ԭ������֤���������������޸�
        if(order==1)
        {
            cout<<"��ӭ���������޸Ľ���"<<endl;
            string newcode,judgecode,input;
            cout<<"������ԭʼ����"<<endl;
            cin>>input;
            if(!cards[index].verifyPasswd(input))
            {
                cout<<"�����������"<<endl;
                continue;
            }
            do
            {
                cout<<"�������µ�����"<<endl;
                cin>>newcode;
                cout<<"���ٴ�ȷ������"<<endl;
                cin>>judgecode;
                if(newcode==judgecode)
                    cards[index].changePasswd(newcode);
                else cout<<"�������벻�������������������"<<endl;
            }
            while(newcode!=judgecode);
            cout<<"�����޸ĳɹ�"<<endl;
        }
        //��ʧ����
        if(order==2)
        {
            int yes_no;//ָ��
            cout<<"��ӭ���뿨Ƭ��ʧ���ҽ���"<<endl;
            cout<<"��ȷ�ϻ�����Ϣ"<<endl;
            cout<<"���ţ�"<<cards[index].getMessage(0)<<endl;
            cout<<"������"<<cards[index].getMessage(1)<<endl;
            if(cards[index].freezeCard(0))
                cout<<"��ǰ��Ƭ���ڹ�ʧ״̬"<<endl;
            else cout<<"��ǰ��Ƭ��������״̬"<<endl;
            cout<<"������ָ�1���й�ʧ��2��ң������˳�"<<endl;
            cin>>yes_no;
            switch(yes_no)
            {
            case 1:
                cards[index].freezeCard(1);
                cout<<"�ɹ���ʧ����Ƭ"<<cards[index].getMessage(0)<<"���ѹ��ܽ�������"<<endl;
                break;
            case 2:
                cards[index].freezeCard(2);
                cout<<"�ɹ���ң���Ƭ"<<cards[index].getMessage(0)<<"���ѹ��ܽ��ָ�"<<endl;
                break;
            default:
                break;
            }
        }
        //�������ܹ���
        if(order==3)
        {
            cout<<"��ӭ���벹������"<<endl;
            cout<<"�Ѷ�������Ϊ"<<cards[index].getMessage(0)<<"�Ŀ�Ƭ���й�ʧ����"<<endl;
            cards[index].freezeCard(1);
            cout<<"����Ϊ�������µĿ��š�������"<<endl;
            if(cards[index].reopenAccount(CreateCardNumber(gTotalCards)))
            {
                gTotalCards++;
                cout<<"�����¿�����Ϊ"<<cards[index].getMessage(0)<<endl;
            }
            else cout<<"���������Ѿ��ľ�"<<endl;
        }
        //��������
        if(order==4)
        {
            string log;
            cout<<"��ȷ�ϻ�����Ϣ"<<endl;
            cout<<"ѧ�ţ�"<<cards[index].getMessage(2)<<endl;
            cout<<"������"<<cards[index].getMessage(1)<<endl;
            cout<<"�Ƿ����������ȷ���밴y/Y�������˳�"<<endl;
            char y_Y;
            cin>>y_Y;
            if(y_Y=='y'||y_Y=='Y')
            {
                for(int i=index; i<gTotalStu; i++)
                    cards[i]=cards[i+1];
                gTotalStu--;
                cout<<"�����ɹ�"<<endl;
                log=stuid+","+"����,"+"�ɹ�";
            }
            WriteLog(log);
            break;
        }
    }
}

int ManageCard(CCard &card,int order,int x=0,CCard * cards=nullptr)//���˹���Ŀ¼
{
    
    if(order==0)
    {
        string log=card.getMessage(2)+","+card.getMessage(1)+","+"��ֵ,"+"ʧ��";
        //�жϿ�Ƭ�Ƿ��ʧ
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

void BuildAccount(CCard * cards)//��һ��Ƭ��������
{
    string stuid;
    string name;
    string log;
    if(gTotalStu==MAXNUM-1)
    {cout<<"���أ�����"<<endl;return;}
    cout<<"��ӭ���뿪������"<<endl;
    cout<<"������ѧ��"<<endl;
    cin>>stuid;
    int i=SearchCard(cards,stuid,STUIDINDEX);
    if(i!=-1)
    {
        cout<<"��ѧ���ѽ��п�������"<<endl;
        WriteLog(stuid+","+"����,"+"ʧ��");
        return;
    }
    cout<<"����������"<<endl;
    cin>>name;
    cards[gTotalStu++].createCard(CreateCardNumber(gTotalCards++),name,stuid);
    cout<<"�����ɹ������Ŀ���Ϊ"<<cards[gTotalStu-1].getMessage(0)<<endl;
    //��ѧ�Ŵӵ͵�������
    sort(cards,cards+gTotalStu,compare2);
    return;
}

void BuildAccounts(CCard * cards,int maxnum)//������������
{
      
    char stuid[11];
    char name[9];
    ifstream fin("kh001.txt");
    if(!fin.is_open())
    {
        cout<<"�ļ���������"<<endl;
        return;
    }
    fin.getline(stuid,3);
    if(strcmp(stuid,"KH")!=0)
    {
        cout<<"�ļ���ȡ����"<<endl;
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
    //����
    sort(cards,cards+gTotalStu,compare2);
    cout<<"�˴�����������ɣ�������"<<gTotalCards<<"�ſ�Ƭ���û��������"<<endl;
}

void ManageCards(CCard *cards,ifstream & fin,string & due_time) //������Ƭ�������
{
    int         x=0;//��ֵ���
    int         times[5]= {}; //��¼ÿ�ֲ����Ĵ���
    char     stuid[11];//��ȡѧ��
    char     time[17];//��ȡ����ʱ��
    char     order[5];//��ȡ��������
    string   log;
    //��ȡ�ļ�����
    while(fin.peek()!=EOF)
    {
        char to_x[4];//��ȡ��ֵ���
        fin.getline(time,17,',');//��ȡʱ��
        //������ָ��ʱ��ʱ��������
        if(strcmp(time,due_time.data())>=0)
        {
            fin.seekg(-17,ios::cur);
            break;
        }
        fin.getline(order,5,',');//��ȡ����
        if(strcmp(order,"��ֵ")==0)
        {
            //���ָ��Ϊ��ֵ����ȡ��ʽ��������ͬ
            fin.getline(stuid,11,',');
            fin.getline(to_x,4,';');
            x=atoi(to_x);
        }
        else fin.getline(stuid,11,';');
        fin.get();
        int index=SearchCard(cards,stuid,STUIDINDEX);
        if(index==-1)
            continue;

        if(strcmp(order,"��ֵ")==0)
            times[0]+=ManageCard(cards[index],0,x);
        if(strcmp(order,"��ʧ")==0)
            times[1]+=ManageCard(cards[index],1);
        if(strcmp(order,"���")==0)
            times[2]+=ManageCard(cards[index],2);
        if(strcmp(order,"����")==0)
            times[3]+=ManageCard(cards[index],3);
        if(strcmp(order,"����")==0)
            times[4]+=ManageCard(cards[index],4,index,cards);
    }
    log="���г�ֵ"+to_string(times[0])+"��,"+"���й�ʧ"+to_string(times[1])+"��,"+"���н��"+to_string(times[2])+"��,"+"���в���"+to_string(times[3])+"��,"+"��������"+to_string(times[4])+"��.";
    WriteLog(log);
    cout<<log<<endl;
}

string CreateCardNumber(int i)
{
    /************************
    * Input��  ���ĵڼ��ſ�
    *Output��null
    *Return�������µĿ���
    ************************/
    //���ɿ��ų���
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

int SearchCard(CCard * cards,string id,int index) //ʹ�ö��ַ���ѯѧ�Ŷ�Ӧ�Ŀ�
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

bool MatchTime(string const &time1,string const &time2)//�ж�����ʱ���Ƿ���ͬһʱ���
{    
    string a=time1.substr(0,8);
    string b=time2.substr(0,8);
    if(a==b&&abs(time1[10]-time2[10])<=1)
        return  true;
    else return false;
}

void FuzzySearch(CCard * cards)//ģ������
{  
    int         order,nums;
    string   key;
    while(true)
    {
        nums=0;
        cout<<"0�������Ų���"<<endl;
        cout<<"1������������"<<endl;
        cout<<"2����ѧ�Ų���"<<endl;
        cout<<"3���˳�"<<endl;
        cout<<"������ָ�";
        cin>>order;
        if(order==3)
            break;
        if(order<0||order>3)
        {
            cout<<"ָ�����"<<endl;
            continue;
        }
        cout<<"������ؼ��֣�";
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
        cout<<"�����ҵ�"<<nums<<"��ƥ������"<<endl;
    }
}
bool FuzzyMatch(string const &str,string const &key,int start1,int start2) //ģ��ƥ�书�ܣ�������
{

    int i=start1;
    int j=start2;
    int len1=str.size();
    int len2=key.size();
    bool truth=false;
    //Ϊ�մ�ʱ��ƥ��
    if(len1==0)
        return false;
    if(i==len1)
        return true;
    while(i<len1&&j<len2)
    {
        //�ַ�Ϊ?ʱ�����κ�ƥ��
        if(key[j]=='?')
        {
            i++;
            j++;
            continue;
        }
        //�ַ�Ϊ*ʱ���õݹ�
        if(key[j]!='*'&&!(key[j]==str[i]))
            return false;
        if(key[j]=='*')
        {
            //��*Ϊ���һ���ַ�ʱ������ƥ��
            //��ͬʱ�ж��*����ʱҪ�ϲ�
            while(key[j]=='*') j++;
            if(j==len2)
                return true;
            else
                for(int m=0; m<len1-i; m++)
                    if(str[i+m]==key[j]||key[j]=='?')
                        truth=truth||FuzzyMatch(str,key,i+m,j);
            return truth;
        }
        //������������ַ���ͬ��������ƥ��
        i++;
        j++;
    }
    //���ǹؼ��ִ����һλΪ*�����
    while(key[j]=='*')
        j++;
    if(i<len1||j<len2)
        return false;
    else return true;
}

void ExpandStr(string & str)//�ַ�����չ
{
    
    
    int len=str.size();
    int ex_len=len;
    for(int i=0; i<len; i++)
        if(str[i]=='*'||str[i]=='?')
            ex_len++;
    //��������?��*�ַ�ʱ��ֱ�ӷ���
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

int GetUserMessage(int choice)//�û��Ĵ�����Ϣͳ��
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
