#pragma once;
#include "stdafx.h"
#include <math.h>
#include <iostream>

const double _PI_ = 3.14159265;
const double Rc = 6378137.0;  // 赤道半径
const double Rj = 6356725.0;  // 极半径


using namespace std;

namespace GisCommonUtil{

	
	class LonLat
	{
		public:
			double m_LoDeg, m_LoMin, m_LoSec;  // longtitude 经度
			double m_LaDeg, m_LaMin, m_LaSec;
			double m_Longitude, m_Latitude;
			double m_RadLo, m_RadLa;
			double Ec;//半径
			double Ed;//距离赤道切面的高度
		public:
		// 构造函数, 经度: loDeg 度, loMin 分, loSec 秒;  纬度: laDeg 度, laMin 分, laSec秒
			LonLat(double loDeg, double loMin, double loSec, double laDeg, double laMin, double laSec)
			{
				m_LoDeg=loDeg; m_LoMin=loMin; m_LoSec=loSec; m_LaDeg=laDeg; m_LaMin=laMin; m_LaSec=laSec;
				m_Longitude = m_LoDeg + m_LoMin / 60 + m_LoSec / 3600;
				m_Latitude = m_LaDeg + m_LaMin / 60 + m_LaSec / 3600;
				m_RadLo  = m_Longitude * _PI_ / 180.;
				m_RadLa  = m_Latitude * _PI_ / 180.;
				Ec = Rj + (Rc - Rj) * (90.- m_Latitude) / 90.;
				Ed = Ec * cos(m_RadLa);
			}

			//！
			LonLat(double longitude, double latitude)
			{
				m_LoDeg = int(longitude);
				m_LoMin = int((longitude - m_LoDeg)*60);
				m_LoSec = (longitude - m_LoDeg - m_LoMin/60.)*3600;

				m_LaDeg = int(latitude);
				m_LaMin = int((latitude - m_LaDeg)*60);
				m_LaSec = (latitude - m_LaDeg - m_LaMin/60.)*3600;

				m_Longitude = longitude;
				m_Latitude = latitude;
				m_RadLo = longitude * _PI_/180.;
				m_RadLa = latitude * _PI_/180.;
				Ec = Rj + (Rc - Rj) * (90.-m_Latitude) / 90.;
				Ed = Ec * cos(m_RadLa);
			}
	};


	class CLonLatHelper
	{
		public:
			CLonLatHelper() {};
			~CLonLatHelper() {};


		//! 计算点A 和 点B的经纬度，求他们的距离和点B相对于点A的方位
		/*! 
		  * /param A A点经纬度
		  * /param B B点经纬度
		  * /param angle B相对于A的方位, 不需要返回该值，则将其设为空
		  * /return A点B点的距离
		  */
		static double distance(LonLat A, LonLat B, double *angle)
		{
		  double dx = (B.m_RadLo - A.m_RadLo) * A.Ed;
		  double dy = (B.m_RadLa - A.m_RadLa) * A.Ec;
		  double out = sqrt(dx * dx + dy * dy);
		  
		  if( angle != NULL)
		  {
				*angle = atan(fabs(dx/dy))*180./_PI_;
				// 判断象限
				double dLo = B.m_Longitude - A.m_Longitude;
				double dLa = B.m_Latitude - A.m_Latitude;

				if(dLo > 0 && dLa <= 0) {
					*angle = (90. - *angle) + 90.;
				}
				else if(dLo <= 0 && dLa < 0) {
					*angle = *angle + 180.;
				}
				else if(dLo < 0 && dLa >= 0) {
					*angle = (90. - *angle) + 270;
				}
		}


		return out/1000;
	}


	//! 计算点A 和 点B的经纬度，求他们的距离和点B相对于点A的方位
	/*! 
	  * /param longitude1 A点经度
	  * /param latitude1 A点纬度
	  * /param longitude2 B点经度
	  * /param latitude2 B点纬度
	  * /param angle B相对于A的方位, 不需要返回该值，则将其设为空
	  * /return A点B点的距离
	  */
	static double distance(
	  double longitude1, double latitude1,
	  double longitude2, double latitude2, 
	  double *angle)
	{
	  LonLat A(longitude1,latitude1);
	  LonLat B(longitude2,latitude2);


	  return distance(A, B, angle);
	}


	//! 已知点A经纬度，根据B点据A点的距离，和方位，求B点的经纬度
	/*!
	  * /param A 已知点A
	  * /param distance B点到A点的距离 
	  * /param angle B点相对于A点的方位
	  * /return B点的经纬度坐标
	  */
	static LonLat GetLonLatB(LonLat A, double distance, double angle)
	{ 
	  double dx = distance*1000 * sin(angle * _PI_ /180.);
	  double dy = distance*1000 * cos(angle * _PI_ /180.);
	  
	  //double dx = (B.m_RadLo - A.m_RadLo) * A.Ed;
	  //double dy = (B.m_RadLa - A.m_RadLa) * A.Ec;


	  double BJD = (dx/A.Ed + A.m_RadLo) * 180./_PI_;
	  double BWD = (dy/A.Ec + A.m_RadLa) * 180./_PI_;
	  LonLat B(BJD, BWD);
	  return B; 
	}


	//! 已知点A经纬度，根据B点据A点的距离，和方位，求B点的经纬度
	/*!
	  * /param longitude 已知点A经度
	  * /param latitude 已知点A纬度
	  * /param distance B点到A点的距离 
	  * /param angle B点相对于A点的方位
	  * /return B点的经纬度坐标
	  */
	static LonLat GetLonLatB(double longitude, double latitude, double distance, double angle)
	{ 
		LonLat A(longitude,latitude);
		return GetLonLatB(A, distance, angle);
	}

	};
}
