#include "stdafx.h"
#include <math.h>
#include <iostream>
#include "GisCommonUtil.h"

using namespace std;
using namespace GisCommonUtil;


double jd1 = 104.740999999;
double wd1 = 31.4337;// 成都
double jd2 = 104.01;
double wd2 = 30.40; 

int main(int argc, char* argv[])
{
	double Rc = 6378137;  // 赤道半径
	double Rj = 6356725;  // 极半径// 绵阳
	double angle = 0;
	cout << "A(绵阳): JD = " << jd1 << "  WD = " << wd1 << endl;
	cout << "B(成都): JD = " << jd2 << "  WD = " << wd2 << endl; 
	cout << "--------------------" << endl;
	//cout << D_jw(wd1,jd1,wd2,jd2, angle) << endl;
	cout << "angle: " << angle <<endl;
	cout << "==============" <<endl;
	JWD A(jd1,wd1),B(jd2,wd2);
	double distance = CJWDHelper::distance(jd1,wd1,jd2,wd2, &angle);
	//cout << CJWDHelper::distance(A,B, &angle) << endl;
	cout << distance << endl;
	cout << "angle: " << angle <<endl; 
	cout << "==============" <<endl;
	JWD C = CJWDHelper::GetJWDB(A, distance, angle);
	cout << "JD = " << C.m_Longitude << "  WD = " << C.m_Latitude << endl;
	cout << "==============" <<endl; 
	cout << A.m_LoDeg << " " << A.m_LoMin << " " << A.m_LoSec << endl; 
	return 0;
}
