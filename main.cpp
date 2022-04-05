#include "analysis.h"
#include "users.h"
#include "windows.h"
using namespace std;

int main()
{
    
    int                  nOrder;
    CCard           cards[MAXNUM];
    CWindow      wins[100];
    cout << "系统正在初始化，稍等..." << endl;

    BuildAccounts(cards, MAXNUM - 1);
    LoadPreTimes(wins);
    system("pause");
    system("cls");
    while (true)
    {
        cout << "时间:" << CWindow::sm_Time << endl;
        cout << R"(
                 /欢迎使用校园卡管理系统/
                    请选择使用功能
                    0、退出系统
                    1、单独开户
                    2、个人信息管理
                    3、食堂单独消费 
                    4、模糊信息查询
                    5、批量操作读取
                    6、多路归并消费记录并查询
                    7、快排消费记录并查询
                    8、窗口管理
                    9、系统设置与批量导入文件
                    10、校验码
                 /**************************/)" << endl;
        cout << "请输入指令:";
        cin >> nOrder;
        if (nOrder == 0) break;
        switch (nOrder)
        {
        case 1:
            BuildAccount(cards);
            break;
        case 2:
            ManageCard(cards);
            break;
        case 3:
            FormApplication(wins, cards);
            break;
        case 4:
            FuzzySearch(cards);
            break;
        case 5:
            SortApplications();
            OperateS(wins, cards);
            break;
        case 6:
            MergeWinLogs(wins, 0);
            break;
        case 7:
            MergeWinLogs(wins, 1);
            break;
        case 8:
            ManageWindow(wins);
            break;
        case 9:
            ManageAnalysis(wins, cards);
            break;
        case 10:
            EncryptApplications();
            break;
        default:
            cout << "指令错误" << endl;
        }
    }
    system("pause");
    system("cls");
    return 0;

}
