#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "CustomMdSpi.h"

using namespace std;

// 链接库
#pragma comment (lib, "thostmduserapi.lib")
#pragma comment (lib, "thosttraderapi.lib")

// ---- 全局变量 ---- //
// 公共参数
TThostFtdcBrokerIDType gBrokerID = "9999";                         // 模拟经纪商代码
TThostFtdcInvestorIDType gInvesterID = "107064";                         // 投资者账户名
TThostFtdcPasswordType gInvesterPassword = "BestHF2020";                     // 投资者密码

																			   // 行情参数
CThostFtdcMdApi *g_pMdUserApi = nullptr;                           // 行情指针
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";               // 模拟行情前置地址
char *g_pInstrumentID[] = {"j1805", "rb1805", "T1803", "IC1712", "i1805"}; // 行情合约代码列表，中、上、大、郑交易所各选一种
int instrumentNum = 5;                                             // 行情合约订阅数量

int main()
{
	// 账号密码
	// cout << "请输入账号： ";
	// scanf("%s", gInvesterID);
	// cout << "请输入密码： ";
	// scanf("%s", gInvesterPassword);

	// 初始化行情线程
	cout << "初始化行情..." << endl;
	g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi();   // 创建行情实例
	CThostFtdcMdSpi *pMdUserSpi = new CustomMdSpi;       // 创建行情回调实例
	g_pMdUserApi->RegisterSpi(pMdUserSpi);               // 注册事件类
	g_pMdUserApi->RegisterFront(gMdFrontAddr);           // 设置行情前置地址
	g_pMdUserApi->Init();                                // 连接运行

																  // 等到线程退出
	g_pMdUserApi->Join();
	delete pMdUserSpi;
	g_pMdUserApi->Release();

	// 转换本地k线数据
	//TickToKlineHelper tickToKlineHelper;
	//tickToKlineHelper.KLineFromLocalData("market_data.csv", "K_line_data.csv");

	getchar();
	return 0;
}