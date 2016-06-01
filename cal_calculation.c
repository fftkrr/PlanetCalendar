//
//  cal_calculation.c.h
//  CAL20000
//
//  Created by YOUNGCHANG KOH(fftkrr@gmail.com)
//
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define Logi(...) ((void)printf(__VA_ARGS__))

#include "cal20000_calculation_type.h"
#include "cal20000_calculation_value.h"



int lang_mode=0;
int internal_calc_planet_current;

date_cal calc_1, calc_0, calc_prev;
BOOL Planet_event_process[9];
BOOL Planet_event_process_event[14];

Planet_Position_instance_record calc_planet_position_calc[9];
Planet_Position_instance_record planet_position[9][3];
double moon_node[3],moon_perigee[3],moon_apogee[3],moon_distance[3];


#define save_count 2048

int count_calc_current_planet_position;
Planet_Position_instance_record internal_calc_planet_position_calc[save_count][9];
double internal_moon_node[save_count],internal_moon_perigee[save_count],internal_moon_apogee[save_count],internal_moon_distance[save_count];

int count_calc_current_planet_position_in_cache;
double g_earth_ecl;

#define devent_result_cap 3000

int planet_event_result[devent_result_cap];
int planet_event_result_planet1[devent_result_cap];
int planet_event_result_planet2[devent_result_cap];
Planet_Position_instance_record planet_event_result_position[devent_result_cap][9];
int event_result_count;
bool event_result_count_over;


int event_sum;




int depth_count;
int execute_check_planet_event_count;

bool chk_stationary_and_great_elong;
bool chk_planet_events_not_stat_greatelong;



char* get_string_time_local(date_cal time,int time_off);


double degreelow(double d)
{
    if (d<0)
    {
        double A=-1*d;
        double B=(A/360.0);
        double C=A-360*(int)B;
        C=-1*C;
        C=C+360;
        return C;
    }
    else
    {
        double A=(d/360.0);
        double B= d-360*(int)A;
        return B;
    }
}

double arctan2(double y, double x)
{
    double arctan2=0;
    if (x==0.0)
    {
        if (y==0.0) {

        }else if (y>0.0) arctan2=M_PI/2;
        else arctan2=-1*M_PI/2;
    }
    else
    {
        if (x>0.0)
        {
            arctan2=atan(y/x);
        }
        else
        {
            if (y>=0.0) arctan2=atan(y/x)+M_PI;
            else arctan2=atan(y/x)-M_PI;
        }
    }
    return arctan2;
}

double get_mean_sun_lon(double day)
{  //1995-12-31 09:00 기준
	return degreelow( day*0.98564736+278.956807);
}


double get_sun_nearest_lon( double day)
{  //1995-12-31 09:00 기준
	return degreelow( 282.869498+0.00004708*day);
}

double moonsundegree(double day, int mode)
{
		double ret;
		double sl,smin,sminangle,sd,sreal,ml,mmin,mminangle,msangle,msdangle,md,mreal,ir;
		double DD,M,FF;

		sl=get_mean_sun_lon(day); //{ 평균 황경 }
	    smin=get_sun_nearest_lon(day); //{근일점 황경 }

		sminangle = M_PI*degreelow(((sl-smin)/180));//self degreelow:((sl-smin)/180)] ; //{근점이각 }

		sd = 1.919*sin(sminangle)+0.02*sin(2*sminangle); //{ 황경차 }
		sreal = degreelow((sl + sd)) ; //{ 진황경 }

		ml = 27.836584+13.17639648*day; //{ 평균황경 }
		mmin =280.425774+0.11140356*day; //{ 근지점 황경 }
		mminangle =M_PI*degreelow(((ml-mmin)/180)); //{ 근점이각 }

		msangle = 202.489407-0.05295377*day; //{ 교점황경 }

		msdangle=M_PI*degreelow(((ml-msangle)/180));

		md = 5.068889*sin(mminangle)+0.146111*sin(2*mminangle)+0.01*sin(3*mminangle)
		-0.238056*sin(sminangle)-0.087778*sin(mminangle+sminangle)  //{ 황경차 }
		+0.048889*sin(mminangle-sminangle)-0.129722*sin(2*msdangle)
		-0.011111*sin(2*msdangle-mminangle)-0.012778*sin(2*msdangle+mminangle);
		mreal = degreelow((ml + md)) ; //{ 진황경 }

		DD=M_PI*degreelow(((108.879777+12.19074912*day)/180)); //달과 태양의 평균이각

		M=sminangle;
		FF=msdangle;

		ret=0;

		if (mode==T_Moon_Sol_lat)
		{
			ret=sreal;
		}

		if (mode==T_Moon_lunar_lat)
		{
			ret=mreal;
		}



		if (mode==T_Moon_Jaki)
		{
			//자기 주기 28*365.2564(1항성년)=10227.1792
			//10227.1792일에 한바퀴 회전, 하루 이동값  360/10227.1792=0.0352003219030327

			//시작점, 1280년 12월 동지(己未일)에 지후책이 1256.5224 //칠정산 내편 사여성 자기 참조
			//동지일로부터 1256.5224일전
			//> 1256.5224/10227.1792=>0.1228610
			//0.1228610*360=> 44.22999도
			//동지점에서의 자기경도가 44.2299도
			//x+0.03520*d (d=동지일때) 자기경도 44.2299도  -> x=237.038993
			//자기 경도 0 => 1227.7.14일(癸亥일) 8시 9분

			double Jaki=237.038993+0.0352003219030327*day;
			ret=degreelow((mreal-Jaki));
		}

		if (mode==T_Moon_Real)
		{
			ret=degreelow((mreal-sreal));
		}

		if (mode==T_Moon_Junction)
		{
			ret=degreelow((mreal-msangle));
		}

		if ((mode==T_Moon_perigee)||(mode==T_Moon_apogee))  //if (mode==T_Moon_perigee)
		{
			ret=degreelow((mreal-mmin));
		}

		if (mode==T_Moon_Junction_distance)
		{
			ir=degreelow((mreal-msangle));
			if (ir > (double)90) ir = ir-(double)180;
			if (ir > (double)90) ir = ir-(double)180;
			ret=ir;
		}

		if (mode==T_Moon_perigee_correct)
		{

			ret= -1.6769*sin(2*DD)+0.4589*sin(4*DD)-0.1856*sin(6*DD)+0.0883*sin(8*DD)
			-0.0773*sin(2*DD-M)+0.0502*sin(M)-0.0460*sin(10*DD)+0.0422*sin(4*DD-M)
			-0.0256*sin(6*DD-M)+0.0253*sin(12*DD)+0.0237*sin(DD)+0.0162*sin(8*DD-M)
			-0.0145*sin(14*DD)+0.0129*sin(2*FF)-0.0112*sin(3*DD)-0.0104*sin(10*DD-M)
			+0.0086*sin(16*DD)+0.0069*sin(12*DD-M)+0.0066*sin(5*DD)-0.0053*sin(2*DD+2*FF)
			-0.0052*sin(18*DD)-0.0046*sin(14*DD-M)-0.0041*sin(7*DD)+0.0040*sin(2*DD+M)
			+0.0032*sin(20*DD)-0.0032*sin(DD+M)+0.0031*sin(16*DD-M)-0.0029*sin(4*DD+M)
			+0.0027*sin(9*DD)+0.0027*sin(4*DD+2*FF)-0.0027*sin(2*DD-2*M)+0.0024*sin(4*DD-2*M)
			-0.0021*sin(6*DD-2*M)-0.0021*sin(22*DD);

		}

		if (mode==T_Moon_apogee_correct)
		{
			ret=0.4392*sin(2*DD)+0.0684*sin(4*DD)+0.0456*sin(M)
			+0.0426*sin(2*DD-M)
			+0.0212*sin(2*FF)-0.0189*sin(DD)+0.0144*sin(6*DD)+0.0113*sin(4*DD-M)+0.0047*sin(2*DD+2*FF)
			+0.0036*sin(DD+M)+0.0035*sin(8*DD)+0.0034*sin(6*DD-M)-0.0034*sin(2*DD-2*FF)+0.0022*sin(2*DD-2*M)
			-0.0017*sin(3*DD);

		}

		if (mode==T_Moon_distance)
		{

			ret=385000.52719;
			ret=ret-20905.355*cos(                 mminangle);
			ret=ret- 3699.111*cos(2*DD             -mminangle);
			ret=ret- 2955.968*cos(2*DD);
			ret=ret-  569.925*cos(               2*mminangle);
			ret=ret+   48.888*cos(    sminangle);
			ret=ret-    3.149*cos(                           2*msdangle);
			ret=ret+  246.158*cos(2*DD           -2*mminangle);
			ret=ret-  152.138*cos(2*DD-sminangle   -mminangle);
			ret=ret-  170.733*cos(2*DD             +mminangle);
			ret=ret-  204.568*cos(2*DD-sminangle);
			ret=ret-  129.620*cos(    sminangle   -mminangle);
			ret=ret+  108.743*cos(DD);
			ret=ret+  104.755*cos(    sminangle   +mminangle);
			ret=ret+   10.321*cos(2*DD                       -2*msdangle);
			ret=ret+   79.661*cos(                 mminangle-2*msdangle);
			ret=ret-   34.782*cos(4*DD             -mminangle);
			ret=ret-   23.210*cos(               3*mminangle);
			ret=ret-   21.636*cos(4*DD           -2*mminangle);
			ret=ret+   24.208*cos(2*DD+sminangle   -mminangle);
			ret=ret+   30.824*cos(2*DD+sminangle);
			ret=ret-    8.379*cos(DD               -mminangle);
			ret=ret-   16.675*cos(DD+sminangle);
			ret=ret-   12.831*cos(2*DD-sminangle+mminangle);
			ret=ret-   10.445*cos(2*DD          -2*mminangle);
			ret=ret-   11.650*cos(4*DD);
			ret=ret+   14.403*cos(2*DD-3*mminangle);
			ret=ret-    7.003*cos(    sminangle-2*mminangle);
			ret=ret-   10.056*cos(2*DD-sminangle-2*mminangle);
			ret=ret+    6.322*cos(DD              +mminangle);
			ret=ret-    9.884*cos(2*DD-2*sminangle);
			ret=ret+    8.752*cos(2*DD            -mminangle-2*msdangle);
		}


		return ret;


}

double interpointdisp(double day)
{

	 return moonsundegree(day, T_Moon_Junction_distance);
}

double moon_distance_double(double day)
{
	return moonsundegree(day, T_Moon_distance);
}

int moon_distance_rate(int distance)
{
	double d=((double)distance-385000+29000)/50800*100;
	int ret=(int)d;
	return ret;
}




short disptimeday( int year, int month, int day)
/* year의 1월 1일부터 year의 month월, day일까지의 날수계산 */
{
  int i; int e;

  e = 0;
  {
	  long long i_end = month ;
	  for( i = 1 ; i < i_end ; ++i )
	  {
			e = e + 31;
			if(  ( i==2 ) || ( i==4 ) || ( i==6 ) || ( i==9 ) || ( i==11 ) ) e =e-1;
			if(  i==2 )
			{
			  e =e-2;
			  if(  year % 4 == 0 ) e = e + 1;
			  if(  year % 100 == 0 ) e = e - 1;
			  if(  year % 400 == 0 ) e = e + 1;
			  if(  year % 4000 == 0 ) e = e - 1;
			}

	  }
  }

  e = e + day;
  return e;
}

int disp2days( int y1, int m1, int d1, int y2, int m2, int d2)
/*y1,m1,d1일부터 y2,m2,d2까지의 일수 계산 */
{

	int p1; int p2; int p1n; int pp1; int pp2; int pr; int i; int dis; int ppp1; int ppp2;

  if(  y2 > y1 )
  {
    p2 = disptimeday( y2, m2, d2 );
    p1 = disptimeday( y1, m1, d1 );
    p1n = disptimeday( y1, 12, 31 );
    pp1 = y1; pp2 = y2;
    pr = -1;}
  else
  {
    p1 = disptimeday( y2, m2, d2 );
    p1n = disptimeday( y2, 12, 31 );
    p2 = disptimeday( y1, m1, d1 );
    pp1 = y2; pp2 = y1;
    pr = +1;}
  if(  y2 == y1 ) dis = p2-p1;
  else
  {
    dis = p1n-p1;
    ppp1 = pp1 + 1;
    ppp2 = pp2 - 1;

    i =dis;
/*  ppp2 := 1990 ;
    i := 0;   */

/*  for k := ppp1 to ppp2 do  { 속도개선부분 }
    begin
      dis := dis + disptimeday(k,12,31);
{     i :=  i + disptimeday(k,12,31); }
    end;  */

    while(  ppp1 <= ppp2 )
    {

      if(  ( ppp1==-9000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 4014377;}
      if(  ( ppp1==-8000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 3649135;}
      if(  ( ppp1==-7000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 3283893;}
      if(  ( ppp1==-6000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 2918651;}
      if(  ( ppp1==-5000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 2553408;}
      if(  ( ppp1==-4000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 2188166;}
      if(  ( ppp1==-3000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 1822924;}


      if(  ( ppp1==-2000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 1457682;}
      if(  ( ppp1==-1750 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 1366371;}
      if(  ( ppp1==-1500 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 1275060;}
      if(  ( ppp1==-1250 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 1183750;}
      if(  ( ppp1==-1000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 1092439;}
      if(  ( ppp1==-750 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 1001128;}
      if(  ( ppp1==-500 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 909818;}
      if(  ( ppp1==-250 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 818507;}
      if(  ( ppp1==0 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 727197;}
      if(  ( ppp1==250 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 635887;}
      if(  ( ppp1==500 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 544576;}
      if(  ( ppp1==750 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 453266;}
      if(  ( ppp1==1000 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 361955;}
      if(  ( ppp1==1250 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 270644;}
      if(  ( ppp1==1500 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 179334;}
      if(  ( ppp1==1750 ) && ( ppp2>1990 ) )
      {
        ppp1 = 1991;
        i = i + 88023;}

      i = i + disptimeday( ppp1, 12, 31 );
      ppp1 = ppp1 + 1;}
    dis = i;

    dis = dis + p2;
    dis = dis * pr;}

  return dis;
}



long long getminbytime_pre( int  uy, int  umm, int  ud, int  uh, int  umin,  int  y1,   int  mo1, int  d1, int  h1, int  mm1)
{
  long long dispday,t;

  dispday =disp2days( uy, umm, ud, y1, mo1, d1 );
  t = dispday * 24 * 60 + ( uh-h1 )* 60 + ( umin-mm1 );

  return t;
}

long long getminbytime( int uy, int umm, int ud, int uh, int umin,int y1, int mo1, int d1, int h1, int mm1)
{
	return getminbytime_pre(uy,umm,ud,uh,umin,y1,mo1,d1,h1,mm1);
}


void getdatebymin_pre( long long tmin,
                   int uyear,  int umonth, int uday, int uhour, int umin,
                   int *y1,  int *mo1, int *d1, int *h1, int *mi1)
/*1996년 2월 4일 22시 8분부터 tmin분 떨어진 날자와 시간을 구하는 프로시져
 특정시점(udate)으로부터 tmin분 떨어진 날짜를 구하는 프로시져 */
{
  long long t;

  *y1 = uyear - tmin / 525949;
  if(  tmin >= 0 )
  {
    *y1 = *y1 + 2;
    do {
      *y1 = *y1 - 1;
      t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, 1, 1, 0, 0 );
    } while(!(  t >= tmin ));
    *mo1 = 13;
    do {
      *mo1 = *mo1 - 1;
      t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, *mo1, 1, 0, 0 );
    } while(!(  t >= tmin ));
    *d1 = 32;
    do {
      *d1 = *d1 - 1;
      t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, *mo1, *d1, 0, 0 );
    } while(!(  t >= tmin ));
    *h1 = 24;
    do {
      *h1 = *h1 - 1;
      t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, *mo1, *d1, *h1, 0 );
    } while(!(  t >= tmin ));
    t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, *mo1, *d1, *h1, 0 );
    *mi1 =  t - tmin;
  }
  else
  {
    *y1 = *y1 - 2;
    do {
      *y1 = *y1 + 1;
      t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, 1, 1, 0, 0 );
    } while(!(  t < tmin ));
    *y1 = *y1 - 1;
    *mo1 = 0;
    do {
      *mo1 = *mo1 + 1;
      t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, *mo1, 1, 0, 0 );
    } while(!(  t < tmin ));
    *mo1 = *mo1 - 1;
    *d1 = 0;
    do {
      *d1 = *d1 + 1;
      t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, *mo1, *d1, 0, 0 );
    } while(!(  t < tmin ));
    *d1 = *d1 - 1;
    *h1 = -1;
    do {
      *h1 = *h1 + 1;
      t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, *mo1, *d1, *h1, 0 );
    } while(!(  t < tmin ));
    *h1 = *h1 - 1;
    t = getminbytime_pre( uyear, umonth, uday, uhour, umin, *y1, *mo1, *d1, *h1, 0 );
    *mi1 = t - tmin;
  }
}


date_cal getdatebymin(long long dmin,date_cal indate)
{
	date_cal ret;

	getdatebymin_pre(dmin,indate.year,indate.month,indate.day,indate.hour,indate.min,&ret.year, &ret.month, &ret.day, &ret.hour,&ret.min);

	return ret;

}

double get_mean_moon_lon( double day)
{
	//1995-12-31 09:00 기준d
	return degreelow( 27.836584+13.17639648*day);
};


double get_moon_nearest_lon( double day)
{
	//1995-12-31 09:00 기준d
	return degreelow( 280.425774+0.11140356*day);
};


double get_moon_node_lon( double day)
{
	//1995-12-31 09:00 기준d
	return degreelow( 202.489407-0.05295377*day);
};



void moon_perigee_calc( double day,double *degree, double *correct_day)
{
    double sl,smin,sminangle,ml,mmin,mminangle,msangle,msdangle,md,mreal,D,M,F;

    // 평균 황경
    sl=get_mean_sun_lon(day);

    //근일점 황경
    smin=get_sun_nearest_lon(day);

    //근점이각
    sminangle=M_PI*(sl-smin)/180;

    // 평균황경
    ml=get_mean_moon_lon(day);

    // 근지점 황경
    mmin=get_moon_nearest_lon(day);

    // 근점이각
    mminangle=M_PI*(ml-mmin)/180;

    // 교점황경
    msangle=get_moon_node_lon(day);

    // 교점이각
    msdangle = M_PI*(ml-msangle)/180;

    //황경차
    md=5.068889*sin(mminangle)+0.146111*sin(2*mminangle)+0.01*sin(3*mminangle)
    -0.238056*sin(sminangle)-0.087778*sin(mminangle+sminangle)
    +0.048889*sin(mminangle-sminangle)-0.129722*sin(2*msdangle)
    -0.011111*sin(2*msdangle-mminangle)-0.012778*sin(2*msdangle+mminangle);


    mreal=degreelow( ml+md);//진황경





    //달과 태양의 평균이각
    D=M_PI*degreelow( 108.879777+12.19074912*day)/180;
    M=sminangle;
    F=msdangle;

    *degree = degreelow( mreal-mmin);

    *correct_day =
    -1.6769*sin(2*D)+0.4589*sin(4*D)-0.1856*sin(6*D)+0.0883*sin(8*D)
    -0.0773*sin(2*D-M)+0.0502*sin(M)-0.0460*sin(10*D)+0.0422*sin(4*D-M)
    -0.0256*sin(6*D-M)+0.0253*sin(12*D)+0.0237*sin(D)+0.0162*sin(8*D-M)
    -0.0145*sin(14*D)+0.0129*sin(2*F)-0.0112*sin(3*D)-0.0104*sin(10*D-M)
    +0.0086*sin(16*D)+0.0069*sin(12*D-M)+0.0066*sin(5*D)-0.0053*sin(2*D+2*F)
    -0.0052*sin(18*D)-0.0046*sin(14*D-M)-0.0041*sin(7*D)+0.0040*sin(2*D+M)
    +0.0032*sin(20*D)-0.0032*sin(D+M)+0.0031*sin(16*D-M)-0.0029*sin(4*D+M)
    +0.0027*sin(9*D)+0.0027*sin(4*D+2*F)-0.0027*sin(2*D-2*M)+0.0024*sin(4*D-2*M)
    -0.0021*sin(6*D-2*M)-0.0021*sin(22*D);


}


double perigee_point( double day)
{
    int loopcount=0;
    double perigee_lon, corr_day;
    double nday,ddd;

    double chk;
    moon_perigee_calc( day,&perigee_lon,&corr_day);
    nday=day;

    do
    {
        loopcount++;
        if (loopcount>5000) break;

        ddd=day-(nday+corr_day);
        if (ddd>0)
        {
            if (ddd>(20.0/1440.0)) nday=nday+ddd/2;
            else nday=nday+(double)(1.0/1440.0);
        }
        else
        {
            if (fabs(ddd)>(double)(20.0/1440.0)) nday=nday+ddd/2;
            else nday=nday-((double)1.0/1440.0);

        }

        moon_perigee_calc( nday,&perigee_lon,&corr_day);

        chk=fabs(day-(nday+corr_day));
    } while (chk>(2.0/1440.0));

    if (loopcount>5000)
    {
    	Logi("!!!!!perigee_point loop overflow");
    }

    return perigee_lon;


}


void moon_apogee_calc( double day, double *degree, double *correct_day)
{
    double sl,smin,sminangle,ml,mmin,mminangle,msangle,msdangle,md,mreal,D,M,F;

    // 평균 황경
	sl=get_mean_sun_lon(day);

	//근일점 황경
	smin=get_sun_nearest_lon(day);

	//근점이각
	sminangle=M_PI*(sl-smin)/180;

	// 평균황경
	ml=get_mean_moon_lon(day);

	// 근지점 황경
	mmin=get_moon_nearest_lon(day);


	// 근점이각
	mminangle=M_PI*(ml-mmin)/180;

	// 교점황경
	msangle=get_moon_node_lon(day);

    // 교점이각
    msdangle = M_PI*(ml-msangle)/180;

    //황경차
    md=5.068889*sin(mminangle)+0.146111*sin(2*mminangle)+0.01*sin(3*mminangle)
    -0.238056*sin(sminangle)-0.087778*sin(mminangle+sminangle)
    +0.048889*sin(mminangle-sminangle)-0.129722*sin(2*msdangle)
    -0.011111*sin(2*msdangle-mminangle)-0.012778*sin(2*msdangle+mminangle);


    mreal=degreelow( ml+md);//진황경

    //달과 태양의 평균이각
    D=M_PI*degreelow( 108.879777+12.19074912*day)/180;
    M=sminangle;
    F=msdangle;

    *degree = degreelow( mreal-mmin+180);

    *correct_day =
     0.4392*sin(2*D)+0.0684*sin(4*D)+0.0456*sin(M)
    +0.0426*sin(2*D-M)
    +0.0212*sin(2*F)-0.0189*sin(D)+0.0144*sin(6*D)+0.0113*sin(4*D-M)+0.0047*sin(2*D+2*F)
    +0.0036*sin(D+M)+0.0035*sin(8*D)+0.0034*sin(6*D-M)-0.0034*sin(2*D-2*F)+0.0022*sin(2*D-2*M)
    -0.0017*sin(3*D);


}

double apogee_point( double day)
{
    int loopcount=0;
    double apogee_lon, corr_day;
    double nday,ddd;

    double chk;

    moon_apogee_calc(day,&apogee_lon,&corr_day);

    nday=day;

    do
    {
        loopcount++;
        if (loopcount>5000) break;

        ddd=day-(nday+corr_day);
        if (ddd>0)
        {
            if (ddd>(double)20.0/1440.0) nday=nday+ddd/2;
            else nday=nday+(1.0/1440.0);
        }
        else
        {
            if (fabs(ddd)>(double)20.0/1440.0) nday=nday+ddd/2;
            else nday=nday-(1.0/1440.0);

        }

        moon_apogee_calc(nday,&apogee_lon,&corr_day);

        chk=fabs(day-(nday+corr_day));
    } while (chk>(2.0/1440.0));

    if (loopcount>5000)
    {
    	Logi("!!!!!apogee_point loop overflow");
    }

    return apogee_lon;


}

void moon_position(date_cal utime, double *moon_lat, double *moon_lon, double *ip, double *perigee, double *apogee, double *moon_distance)
{
	long long min;
    double msangle,day;

	min=getminbytime(utime.year,utime.month,utime.day,utime.hour,utime.min,1995,12,31,9,0);

    day=(double)min/1440;

    msangle=get_moon_node_lon(day);

    *moon_lon=moonsundegree(day,T_Moon_lunar_lat);
    *moon_lat=5.1566898*sin(M_PI*(*moon_lon-msangle)/180);

    *ip=degreelow( interpointdisp(day));
    *moon_distance=moon_distance_double(day);

    *perigee=perigee_point(day);
    *apogee=apogee_point(day);;

}

void pr_planet_position(
		int Planet_number, date_cal utime,
        double *planet_lat, double *planet_lon, double *planet_radius,
          double *planet_x, double *planet_y, double *planet_z, double *planet_true_anomaly, double *earth_ecl,
          char **Name_Kor, char **Name_Han, char **Name_Eng,
           int *P_Color, int *loop_count
		)

{

    long long tmin;
    double  day,
        N,// : extended ; //승교점 위치 longitude of ascending node at date of elements
        i,// : extended ; //기울기(황도과 해당 행성의 궤도면 기울기  inclination
        w,// : extended ; //승교점에서 근일점까지의 각 longitude of perihelion at date of elements
        a,// : extended ; //장반경? 평균거리? mean distance (au)
        e,// : extended ; //이심률 eccentricity of orbit
        M,// : extended ; //평균 근점이각
        E0,E1,Eccentric, xv,yv,zv,v,r, //Mean_lon
        Mj,Ms,Mu,M_perturbations,L_perturbations,R_perturbations,
    ws,Ls,Lm,D,F,ecl;

    int loop_i;

    tmin=getminbytime(utime.year,utime.month,utime.day,utime.hour,utime.min,1999,12,31,9,0);

    day=(double)tmin/1440;

	N = degreelow( Planet_Orbit[Planet_number]->longitude_ascending_node_initial_degree + Planet_Orbit[Planet_number]->longitude_ascending_node_period*day);

	i= degreelow( Planet_Orbit[Planet_number]->inclination_initial_degree+Planet_Orbit[Planet_number]->inclination_period*day);

	w = degreelow( Planet_Orbit[Planet_number]->longitude_perihelion_initial_degree+Planet_Orbit[Planet_number]->longitude_perihelion_period*day);
    a = Planet_Orbit[Planet_number]->mean_distance;

    e= degreelow( Planet_Orbit[Planet_number]->eccentricity_initial_degree+Planet_Orbit[Planet_number]->eccentricity_period*day);

    if ((e>1)||(e<0)) e=Planet_Orbit[Planet_number]->eccentricity_initial_degree;

    M = degreelow( Planet_Orbit[Planet_number]->mean_anomaly_initial_degree+Planet_Orbit[Planet_number]->mean_anomaly_period*day);

    ecl = degreelow( 23.4393 - 3.563E-7 * day);

    Eccentric=M+e*(180/M_PI)*sin(M_PI*M/180)*(1.0+e*cos(M_PI*M/180));

    E0=Eccentric;

    if (Planet_number==0)
    {

    }
    else
    {
        for (loop_i=1; loop_i<330; loop_i++) {
            *loop_count=loop_i;
            E1=E0-(E0-e*(180/M_PI)*sin(M_PI*E0/180)-M)/(1-e*cos(M_PI*E0/180));
            if (fabs(E1-E0)<0.00000000000001) break;
            E0=E1;
        }
        E0=E1;
    }




    xv=a*(cos(M_PI*E0/180)-e);
    yv=a*(sqrt(1.0-e*e)*sin(M_PI*E0/180));

    v=arctan2(yv,xv)*180/M_PI;

    *planet_true_anomaly=v;
    r=sqrt(xv*xv+yv*yv);

    if (Planet_number==0)
    {

    	*planet_lon=degreelow( v+w);
        *planet_x=r*cos(*planet_lon*M_PI/180);
        *planet_y=r*sin(*planet_lon*M_PI/180)*cos(ecl*M_PI/180);
        *planet_z=r*sin(*planet_lon*M_PI/180)*sin(ecl*M_PI/180);

        *planet_lon=degreelow( arctan2(*planet_y,*planet_x)*180/M_PI);

        *planet_lat=arctan2(*planet_z, sqrt(*planet_x*(*planet_x)+(*planet_y)*(*planet_y)))*180/M_PI;
        *planet_radius=r;

        *planet_y=r*sin(degreelow( v+w)*M_PI/180);
        *planet_z=0;

    }
    else
    {
        *planet_x=r*(cos(M_PI*N/180)*cos((v+w)*M_PI/180)-sin(M_PI*N/180)*sin((v+w)*M_PI/180)*cos(i*M_PI/180));
        *planet_y=r*(sin(M_PI*N/180)*cos((v+w)*M_PI/180)+cos(M_PI*N/180)*sin((v+w)*M_PI/180)*cos(i*M_PI/180));
        *planet_z=r*(sin((v+w)*M_PI/180)*sin(i*M_PI/180));

        *planet_lon=degreelow(arctan2(*planet_y,*planet_x)*180/M_PI);

        *planet_lat=arctan2(*planet_z,sqrt(*planet_x*(*planet_x)+(*planet_y)*(*planet_y)) )*180/M_PI;
        *planet_radius=r;
    };



    //For perturbations
    M_perturbations=0;
    L_perturbations=0;
    R_perturbations=0;

    if (Planet_Orbit[Planet_number]->Number==1)  //moon
    {

    	Ms= degreelow( Planet_Orbit[0]->mean_anomaly_initial_degree+Planet_Orbit[0]->mean_anomaly_period*day);

    	ws= degreelow( Planet_Orbit[0]->longitude_perihelion_initial_degree+Planet_Orbit[0]->longitude_perihelion_period*day);
        Ls=Ms+ws;
        Lm=M+w+N;
        D=Lm-Ls;
        F=Lm-N;

        M_perturbations=
            -1.274 * sin((M - 2*D)*M_PI/180)
            +0.658 * sin((2*D)*M_PI/180)
            -0.186 * sin((Ms)*M_PI/180)
            -0.059 * sin((2*M - 2*D)*M_PI/180)
            -0.057 * sin((M - 2*D + Ms)*M_PI/180)
            +0.053 * sin((M + 2*D)*M_PI/180)
            +0.046 * sin((2*D - Ms)*M_PI/180)
            +0.041 * sin((M - Ms)*M_PI/180)
            -0.035 * sin((D)*M_PI/180)
            -0.031 * sin((M + Ms)*M_PI/180)
            -0.015 * sin((2*F - 2*D)*M_PI/180)
            +0.011 * sin((M - 4*D)*M_PI/180);

        L_perturbations=
            -0.173 * sin((F - 2*D)*M_PI/180)
            -0.055 * sin((M - F - 2*D)*M_PI/180)
            -0.046 * sin((M + F - 2*D)*M_PI/180)
            +0.033 * sin((F + 2*D)*M_PI/180)
            +0.017 * sin((2*M + F)*M_PI/180);

        R_perturbations=
            -0.58 * cos((M - 2*D)*M_PI/180)
            -0.46 * cos((2*D)*M_PI/180);
    };


    if (Planet_Orbit[Planet_number]->Number==5)  //JUPITER
    {

    	Mj=  degreelow( Planet_Orbit[5]->mean_anomaly_initial_degree+Planet_Orbit[5]->mean_anomaly_period*day);

    	Ms= degreelow( Planet_Orbit[6]->mean_anomaly_initial_degree+Planet_Orbit[6]->mean_anomaly_period*day);

        M_perturbations = -0.332*sin((2*Mj-5*Ms-67.6)*M_PI/180)
            -0.056*sin((2*Mj-2*Ms+21)*M_PI/180)
            +0.042*sin((3*Mj-5*Ms+21)*M_PI/180)
            -0.036*sin((Mj-2*Ms)*M_PI/180)
            +0.022*cos((Mj-Ms)*M_PI/180)
            +0.023*sin((2*Mj-3*Ms+52)*M_PI/180)
            -0.016*sin((Mj-5*Ms-69)*M_PI/180);
    };



    if (Planet_Orbit[Planet_number]->Number==6)  //SATURN
    {

    	Mj=  degreelow( Planet_Orbit[5]->mean_anomaly_initial_degree+Planet_Orbit[5]->mean_anomaly_period*day);

    	Ms= degreelow( Planet_Orbit[6]->mean_anomaly_initial_degree+Planet_Orbit[6]->mean_anomaly_period*day);

        M_perturbations = +0.812*sin((2*Mj-5*Ms-67.6)*M_PI/180)
            -0.229*cos((2*Mj-4*Ms-2)*M_PI/180)
            +0.119*sin((Mj-2*Ms-3)*M_PI/180)
            +0.046*sin((2*Mj-6*Ms-69)*M_PI/180)
            +0.014*sin((Mj-3*Ms+32)*M_PI/180);
        L_perturbations = -0.020*cos((2*Mj-4*Ms-2)*M_PI/180)
            +0.018*sin((2*Mj-6*Ms-49)*M_PI/180);
    };



    if (Planet_Orbit[Planet_number]->Number==7)  //URANUS
    {
    	Mj= degreelow( Planet_Orbit[5]->mean_anomaly_initial_degree+Planet_Orbit[5]->mean_anomaly_period*day);

    	Ms= degreelow( Planet_Orbit[6]->mean_anomaly_initial_degree+Planet_Orbit[6]->mean_anomaly_period*day);

    	Mu= degreelow( Planet_Orbit[7]->mean_anomaly_initial_degree+Planet_Orbit[7]->mean_anomaly_period*day);

        M_perturbations = +0.040*sin((Ms-2*Mu+6)*M_PI/180)
            +0.035*sin((Ms-3*Mu+33)*M_PI/180)
            -0.015*sin((Mj-Mu+20)*M_PI/180);

    };

    *planet_lon=*planet_lon+M_perturbations;
    *planet_lat=*planet_lat+L_perturbations;
    *planet_radius=*planet_radius+R_perturbations;



    if (Planet_Orbit[Planet_number]->Number==1)  //Moon
    {
        *planet_x=cos(*planet_lon*M_PI/180)*cos(*planet_lat*M_PI/180);
        *planet_y=sin(*planet_lon*M_PI/180)*cos(*planet_lat*M_PI/180);
        *planet_z=sin(*planet_lat*M_PI/180);

        xv=*planet_x;
        yv=*planet_y*cos(ecl*M_PI/180)-*planet_z*sin(ecl*M_PI/180);
        zv=*planet_y*sin(ecl*M_PI/180)+*planet_z*cos(ecl*M_PI/180);

        *planet_lon=degreelow( arctan2(yv,xv)*180/M_PI);
        *planet_lat=arctan2(zv,sqrt(xv*xv+yv*yv))*180/M_PI;

    };


    *earth_ecl=ecl;


    *Name_Kor=(char *)(Planet_Orbit[Planet_number]->Name_Kor);
    *Name_Han=(char *)(Planet_Orbit[Planet_number]->Name_Han);
    *Name_Eng=(char *)(Planet_Orbit[Planet_number]->Name_Eng);
    *P_Color =Planet_Orbit[Planet_number]->color ;

}


int compare_time_equal(date_cal time1, date_cal time2)
{
    if ((time1.year==time2.year)&&(time1.month==time2.month)&&(time1.day==time2.day)&&(time1.hour==time2.hour)&&(time1.min==time2.min)) return 1;
    return 0;
}


void get_LamdaBeta_from_RADec(double RA, double Dec, double ecl, double *Ramda, double *Beta)
{
    double r_RAD,d_RAD,ecl_RAD,t;
    int add_t;

    r_RAD=RA*M_PI/180;
    d_RAD=Dec*M_PI/180;
    ecl_RAD=ecl*M_PI/180;

    add_t=0;
    if (cos(r_RAD)<0) add_t=180;

    t=(sin(r_RAD)*cos(ecl_RAD)+tan(d_RAD)*sin(ecl_RAD))/cos(r_RAD);
    t=atan(t)*180/M_PI;
    *Ramda=degreelow( t+add_t);

    t=sin(d_RAD)*cos(ecl_RAD)-cos(d_RAD)*sin(ecl_RAD)*sin(r_RAD);
    *Beta=asin(t)*180/M_PI;
}

void get_RADec_from_RamdaBeta( double Ramda, double Beta, double ecl, double *RA, double *Dec)
{
    double r_RAD,b_RAD,ecl_RAD,t;
    int add_t;

    r_RAD = Ramda*M_PI/180;
    b_RAD = Beta*M_PI/180;
    ecl_RAD = ecl*M_PI/180;

    add_t=0;
    if (cos(r_RAD)<0) add_t=180;


    t=(sin(r_RAD)*cos(ecl_RAD)-tan(b_RAD)*sin(ecl_RAD))/cos(r_RAD);
    t=atan(t)*180/M_PI;

    *RA=degreelow( t+add_t);

    t=sin(b_RAD)*cos(ecl_RAD)+cos(b_RAD)*sin(ecl_RAD)*sin(r_RAD);
    *Dec=asin(t)*180/M_PI;

}


void sort_event_result()
{
	int i,j,k;
    if (event_result_count>5)
    {
        for (i=event_result_count-6;i<event_result_count;i++)
        {
            for(j=i;j<event_result_count;j++)
            {
                long long time=0;

                time=getminbytime(
                		planet_event_result_position[j][0].time.year,planet_event_result_position[j][0].time.month,planet_event_result_position[j][0].time.day,planet_event_result_position[j][0].time.hour,planet_event_result_position[j][0].time.min,
                		planet_event_result_position[i][0].time.year,planet_event_result_position[i][0].time.month,planet_event_result_position[i][0].time.day,planet_event_result_position[i][0].time.hour,planet_event_result_position[i][0].time.min);
                if (time<0)
                {
                        planet_event_result[event_result_count+1]=planet_event_result[i];
                        planet_event_result_planet1[event_result_count+1]=planet_event_result_planet1[i];
                        planet_event_result_planet2[event_result_count+1]=planet_event_result_planet2[i];

                        planet_event_result[i]=planet_event_result[j];
                        planet_event_result_planet1[i]=planet_event_result_planet1[j];
                        planet_event_result_planet2[i]=planet_event_result_planet2[j];

                        planet_event_result[j]=planet_event_result[event_result_count+1];
                        planet_event_result_planet1[j]=planet_event_result_planet1[event_result_count+1];
                        planet_event_result_planet2[j]=planet_event_result_planet2[event_result_count+1];


                        for(k=0;k<9;k++)
                        {
                            planet_event_result_position[event_result_count+1][k]=planet_event_result_position[i][k];
                            planet_event_result_position[i][k]=planet_event_result_position[j][k];
                            planet_event_result_position[j][k]=planet_event_result_position[event_result_count+1][k];
                        }
                }

            }
        }
    }

}



//void put_event_to_result( int Type, int planet1, int planet2, Planet_Position_instance_record *position)
void put_event_to_result( int Type, int planet1, int planet2, Planet_Position_instance_record position[])
{

	if ((Type==en_stationary_east_to_west)&&(Planet_event_process_event[en_ck_stationary_east_to_west]==FALSE)) return;

    if ((Type==en_stationary_east_to_west)&&(Planet_event_process_event[en_ck_stationary_east_to_west]==FALSE)) return;

    if ((Type==en_stationary_west_to_east)&&(Planet_event_process_event[en_ck_stationary_west_to_east]==FALSE)) return;

    if ((Type==en_great_elong_east)&&(Planet_event_process_event[en_ck_great_elong_east]==FALSE)) return;

    if ((Type==en_great_elong_west)&&(Planet_event_process_event[en_ck_great_elong_west]==FALSE)) return;

    if (((Type==en_ascending_node)||(Type==en_ascecliptic))&&(Planet_event_process_event[en_ck_ascending_node]==FALSE)) return;

    if (((Type==en_descending_node)||(Type==en_desecliptic))&&(Planet_event_process_event[en_ck_descending_node]==FALSE)) return;

    if ((Type==en_sun_perihelion)&&(Planet_event_process_event[en_ck_perigee_Perihelion]==FALSE)) return;

    if ((Type==en_sun_aphelion)&&(Planet_event_process_event[en_ck_apogee_Aphelion]==FALSE)) return;

    if ((planet1==2)||(planet1==3))
    {
        if (Planet_event_process_event[en_ck_conjunction_superior_new_moon]==FALSE)
        {
            if (Type==en_superior_conjunction) return;
        }
        if (Planet_event_process_event[en_ck_opposition_inferior_full_moon]==FALSE)
        {
            if (Type==en_inferior_conjuinction) return;
        }
    }
    else
    {
        if (Planet_event_process_event[en_ck_conjunction_superior_new_moon]==FALSE)
        {
            if (Type==en_conjunction) return;
            if (Type==en_new_moon) return;
        }

        if (Planet_event_process_event[en_ck_opposition_inferior_full_moon]==FALSE)
        {
            if (Type==en_opposition) return;
            if (Type==en_full_moon) return;
        }

    }


    if (event_result_count==devent_result_cap)
    {
    	Logi("!!!!!!!!!!event result count is over. curr_count=%d / cap=%d",event_result_count,devent_result_cap);
        event_result_count_over=TRUE;
        return; //Do nothing
    }

    if (Type==en_nothing) return;

    int i;
    for(i=0;i<event_result_count;i++)
    {
        if ((planet_event_result[i]==Type)
        	&&(planet_event_result_planet1[i]==planet1)
        	&&(planet_event_result_planet2[i]==planet2)
        	)
        {
        	int dd=getminbytime(position[0].time.year,position[0].time.month,position[0].time.day,position[0].time.hour,position[0].time.min,
        			planet_event_result_position[i][0].time.year,planet_event_result_position[i][0].time.month,planet_event_result_position[i][0].time.day,planet_event_result_position[i][0].time.hour,planet_event_result_position[i][0].time.min
        						);
        	if ((dd<3)&&(dd>-3))
        		return;
        }

    }

    planet_event_result[event_result_count]=Type;
    planet_event_result_planet1[event_result_count]=planet1;
    planet_event_result_planet2[event_result_count]=planet2;

    for(i=0;i<9;i++)
    {
        planet_event_result_position[event_result_count][i]=position[i];
    }

    //Logi("!!!!!!!!  put_event_to_result found an event time= %d-%d-%d %d:%d<br>\n",position[0].time.year,position[0].time.month,position[0].time.day,position[0].time.hour,position[0].time.min);

    event_result_count++;

	sort_event_result();
}






void sun_position(date_cal utime,
        double *earth_lat,double *earth_lon, double *earth_radius,
        double *earth_x, double *earth_y, double *earth_z,
        double *planet_true_anomaly,double *earth_ecl)
{
    double tmin,day,sl,smin,sminangle,sd,sreal,earth_r,earth_e,xv,yv;

    tmin=getminbytime(utime.year,utime.month,utime.day,utime.hour,utime.min,1999,12,31,9,0);
    day=(double)tmin/1440;

    *earth_ecl=degreelow( (23.4393 - 3.563E-7 * day));

    tmin=getminbytime(utime.year,utime.month,utime.day,utime.hour,utime.min,1995,12,31,9,0);

    day=(double)tmin/1440;

    sl=get_mean_sun_lon(day);
    smin=get_sun_nearest_lon(day);

    sminangle=M_PI*(sl-smin)/180;
    sd=1.919*sin(sminangle)+0.02*sin((2*sminangle));
    sreal=degreelow( sl+sd);

    *planet_true_anomaly=sreal-smin;

    *earth_lon=sreal;
    *earth_lat=*earth_ecl*sin(M_PI*sreal/180);

    sminangle=M_PI*(sreal-smin)/180;
    earth_e=0.0166967;

    xv=cos(sminangle)-earth_e;
    yv=sqrt(1.0-earth_e*earth_e)*sin(sminangle);

    earth_r=1.0 ;// 1AU
    *earth_radius=earth_r*sqrt(xv*xv+yv*yv);

    *earth_x = *earth_radius * cos( M_PI*(*earth_lon)/180 );
    *earth_y = *earth_radius * sin( M_PI*(*earth_lon)/180 )* cos(*earth_ecl*M_PI/180);;
    *earth_z = *earth_radius * sin( M_PI*(*earth_lon)/180 )* sin(*earth_ecl*M_PI/180);;;
}


void calc_current_planet_position( date_cal current_time, double *moon_node, double *perigee, double *apogee, double *moon_distance)
{

    count_calc_current_planet_position++;

    double planet_lat,planet_lon,plane_radius,planet_x,planet_y,planet_z,earth_ecl;//,earth_lon;
    double px1,px2,px3,py1,py2,py3,pz1,pz3;
    double sun_ramda,sun_beta,earth_radius,earth_x,earth_y,earth_z;
    double moon_beta, moon_ramda;
    int P_color,loop_count;
    double planet_true_anomaly;

    char *Name_kor;
    char *Name_Han;
    char *Name_eng;

    int i,j;
    for(i=0;i<save_count;i++)
    {

    	if (1==compare_time_equal(current_time,internal_calc_planet_position_calc[i][0].time))
        {

            for(j=0;j<9;j++)
                calc_planet_position_calc[j]=internal_calc_planet_position_calc[i][j];
            *moon_node=internal_moon_node[i];
            *perigee=internal_moon_perigee[i];
            *apogee=internal_moon_apogee[i];
            *moon_distance=internal_moon_distance[i];
            count_calc_current_planet_position_in_cache++;

            goto final_return;

        }
    }
    sun_ramda=0;
    moon_ramda=0;


    int planet_no;
    for (planet_no=0; planet_no<9; planet_no++)
    {
    	pr_planet_position( planet_no, current_time,
    			&planet_lat,&planet_lon,&plane_radius,&planet_x,&planet_y,&planet_z,&planet_true_anomaly,&earth_ecl,
    			&Name_kor,	&Name_Han,&Name_eng,&P_color,&loop_count);


        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].time=current_time;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_lat=planet_lat;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_lon=planet_lon;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_radius=plane_radius;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_x=planet_x;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_y=planet_y;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_z=planet_z;

        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].true_anomaly=degreelow( planet_true_anomaly);

        px1=internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_x;
        py1=internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_y;
        pz1=internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_z;

        px2=internal_calc_planet_position_calc[internal_calc_planet_current][0].planet_x;
        py2=internal_calc_planet_position_calc[internal_calc_planet_current][0].planet_y;

        px3=px1+px2;
        py3=py1+py2;
        pz3=pz1;

        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RA=degreelow( arctan2(py3*cos(earth_ecl*M_PI/180)-pz3*sin(earth_ecl*M_PI/180),px3 )*180/M_PI);
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].DEC
        	=arctan2(py3*sin(earth_ecl*M_PI/180)+pz3*cos(earth_ecl*M_PI/180),sqrt(px3*px3+(py3*cos(earth_ecl*M_PI/180)-pz3*sin(earth_ecl*M_PI/180))*(py3*cos(earth_ecl*M_PI/180)-pz3*sin(earth_ecl*M_PI/180))))*180/M_PI; //적위;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_distance=sqrt(px3*px3+py3*py3+pz3*pz3);
        if (planet_no==0) internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_distance=sqrt(px3*px3+py3*py3);

        if (planet_no>1)
        	get_LamdaBeta_from_RADec(
                            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RA,
                            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].DEC,
                            earth_ecl,
                            &internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RAMDA,
                            &internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].BETA
                                     );

        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].elongation
        	=degreelow( sun_ramda-internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RAMDA);

        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].elongation_moon
        	=degreelow( moon_ramda-internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RAMDA);

        if (planet_no==0) //SUN
        {
        	sun_position(current_time,
        					&sun_beta,&sun_ramda,&earth_radius,&earth_x,&earth_y,&earth_z,&planet_true_anomaly,&earth_ecl
        				 );

            sun_beta=0.0;
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RAMDA=sun_ramda;

            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].BETA=0;
            get_RADec_from_RamdaBeta(
                            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RAMDA,
                            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].BETA,
                            earth_ecl,
                            &internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RA,
                            &internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].DEC
                                     );

            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].true_anomaly //=degreelow(planet_true_anomaly);
            	=degreelow( planet_true_anomaly);

            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].elongation=0;

        }

        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].moon_node=0;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].moon_perigee=0;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].moon_apogee=0;
        internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].moon_distance_to_earth=0;

        if(planet_no==1) //MOON
        {

        	moon_position( current_time,&moon_beta,&moon_ramda,&internal_moon_node[internal_calc_planet_current],&internal_moon_perigee[internal_calc_planet_current],&internal_moon_apogee[internal_calc_planet_current],&internal_moon_distance[internal_calc_planet_current]);

            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RAMDA=moon_ramda;
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].BETA=moon_beta;
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].planet_distance=internal_moon_distance[internal_calc_planet_current]; //km

            get_RADec_from_RamdaBeta(
                            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RAMDA,
                            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].BETA,
                            earth_ecl,
                            &internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RA,
                            &internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].DEC
                                     );
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].elongation_moon=0;
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].elongation
            	=degreelow( sun_ramda-internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].RAMDA);
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].elongation_moon
            	=degreelow( -1*internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].elongation);

            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].moon_node=internal_moon_node[internal_calc_planet_current];
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].moon_perigee=internal_moon_perigee[internal_calc_planet_current];
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].moon_apogee=internal_moon_apogee[internal_calc_planet_current];
            internal_calc_planet_position_calc[internal_calc_planet_current][planet_no].moon_distance_to_earth=internal_moon_distance[internal_calc_planet_current];

        }

    }

    for(j=0;j<9;j++)
    {
        calc_planet_position_calc[j]=internal_calc_planet_position_calc[internal_calc_planet_current][j];
    }
    *moon_node=internal_moon_node[internal_calc_planet_current];
    *perigee=internal_moon_perigee[internal_calc_planet_current];
    *apogee=internal_moon_apogee[internal_calc_planet_current];
    *moon_distance=internal_moon_distance[internal_calc_planet_current];

    internal_calc_planet_current++;
    if (internal_calc_planet_current==save_count)
    {
    	internal_calc_planet_current=0;
    }

    g_earth_ecl=earth_ecl;


final_return: ;


}


void init_check_planet_event()
{

	event_result_count_over=false;
	event_result_count=0;

	int i,j;
	for(i=0;i<devent_result_cap;i++)
	{
		planet_event_result[i]=0;
		planet_event_result_planet1[i]=9;
		planet_event_result_planet2[i]=9;
		for(j=0;j<9;j++)
		{
			planet_event_result_position[i][j].time.year=0;
			planet_event_result_position[i][j].time.month=0;
			planet_event_result_position[i][j].time.day=0;
			planet_event_result_position[i][j].time.hour=0;
			planet_event_result_position[i][j].time.min=0;
		}
	}

}

int check_planet_event( int planet_start_no)
{
	depth_count++;

	int i,j;
    if (event_result_count_over) return 0;

    execute_check_planet_event_count++;


    Planet_Position_instance_record save_planet_position[9][3];
    Planet_Position_instance_record q1_planet_position[9];

    double save_moon_node[3],save_moon_perigee[3],save_moon_apogee[3],save_moon_distance[3];
    double q1_moon_node,q1_moon_perigee,q1_moon_apogee,q1_moon_distance;
    for(i=0;i<3;i++)
    {
        for(j=0;j<9;j++)
            save_planet_position[j][i]=planet_position[j][i];
        save_moon_node[i]=moon_node[i];
        save_moon_perigee[i]=moon_perigee[i];
        save_moon_apogee[i]=moon_apogee[i];
        save_moon_distance[i]=moon_distance[i];
    }
    Planet_Position_instance_record save_calc_planet_position_calc[9];
    for (i=0; i<9; i++)
    {
        save_calc_planet_position_calc[i]=calc_planet_position_calc[i];
    }

    int event_planet=0;
    date_cal mid_time;

    double mid_moon_node,mid_moon_perigee,mid_moon_apogee,mid_moon_distance;
    long long dmin=getminbytime(
    		planet_position[0][1].time.year,planet_position[0][1].time.month,planet_position[0][1].time.day,planet_position[0][1].time.hour,planet_position[0][1].time.min,
    		planet_position[0][0].time.year,planet_position[0][0].time.month,planet_position[0][0].time.day,planet_position[0][0].time.hour,planet_position[0][0].time.min);

    mid_time=getdatebymin( dmin/2,planet_position[0][0].time);


    calc_current_planet_position( mid_time, &q1_moon_node,&q1_moon_perigee,&q1_moon_apogee,&q1_moon_distance);



    for (i=0; i<9; i++)
    {
        q1_planet_position[i]=calc_planet_position_calc[i];
    }

    mid_time=getdatebymin( -dmin/2,	planet_position[0][0].time);


    calc_current_planet_position( mid_time, &mid_moon_node,&mid_moon_perigee,&mid_moon_apogee,&mid_moon_distance);


    BOOL BL_Moon_Node=FALSE;
    BOOL BL_Moon_perigee=FALSE;
    BOOL BL_Moon_apogee=FALSE;
    BOOL BL_MOON_First_Q=FALSE;
    BOOL BL_MOON_LAST_Q=FALSE;
    BOOL BL_SUN_JULGI=FALSE;
    BOOL BL_SUN_helion=FALSE;


    BOOL Stationary[9];
    BOOL Stationary_heading[9];
    BOOL great_elong[9];
    BOOL great_elong_heading[9];

    BOOL conjunction[9];
    BOOL opposition[9];
    BOOL right_east[9];
    BOOL right_west[9];
    BOOL planet_conjunction[9];
    BOOL Perihelion[9];
    BOOL Aphelion[9];
    BOOL ecliptic_ascdesc[9];

    for(i=0;i<9;i++)
    {
        Stationary[i] = FALSE;
        Stationary_heading[i] = FALSE;
        great_elong[i]=FALSE;
        great_elong_heading[i]=FALSE;

        conjunction[i]=FALSE;
        opposition[i]=FALSE;
        right_east[i]=FALSE;
        right_west[i]=FALSE;
        planet_conjunction[i]=FALSE;

        Perihelion[i]=FALSE;
        Aphelion[i]=FALSE;
        ecliptic_ascdesc[i]=FALSE;

    }

    int start_planet_no=planet_start_no;
    int end_planet_no=planet_start_no+1;

    if (planet_start_no==0)
    {
        start_planet_no=1;
        end_planet_no=9;
    }


    Planet_Position_instance_record result_calc_planet_position_calc[9];

    int planet_no;
    for(planet_no=start_planet_no;planet_no<end_planet_no;planet_no++)
    {
        if (event_result_count_over) return 0;
        if (Planet_event_process[planet_no]==FALSE) continue;

        for(i=0;i<3;i++)
        {
            for(j=0;j<9;j++)
                planet_position[j][i]=save_planet_position[j][i];
        }

        if (planet_no==1)
        {
            if ((moon_node[0]>270)&&(moon_node[1]<90))
            {
            	BL_Moon_Node=TRUE;
            }
            if ((moon_perigee[0]>270)&&(moon_perigee[1]<90))
                BL_Moon_perigee=TRUE;
            if ((moon_apogee[0]>270)&&(moon_apogee[1]<90))
                BL_Moon_apogee=TRUE;
            if ((planet_position[1][1].elongation<90)&&(planet_position[1][0].elongation>90))
                BL_MOON_First_Q=TRUE;
            if ((planet_position[1][1].elongation<270)&&(planet_position[1][0].elongation>270))
                BL_MOON_LAST_Q=TRUE;

            BL_SUN_helion=((planet_position[0][1].true_anomaly<5.0)&&(planet_position[0][1].true_anomaly>0.0))
                &&((planet_position[0][0].true_anomaly<360.0)&&(planet_position[0][0].true_anomaly>355.0));
            BL_SUN_helion=BL_SUN_helion||(((planet_position[0][1].true_anomaly<185.0)&&(planet_position[0][1].true_anomaly>180.0))
            &&((planet_position[0][0].true_anomaly<180.0)&&(planet_position[0][0].true_anomaly>175.0)));

            int jul;
            for(jul=0;jul<24;jul++)
            {
                /*
                double reframda=jul*15.0;
                double reframda2=reframda+2.0;
                if ((planet_position[0][1].RAMDA>reframda)&&(planet_position[0][1].RAMDA<reframda2)&&(planet_position[0][0].RAMDA<reframda))
                {
                    BL_SUN_JULGI=TRUE;
                    break;
                }
                */

                if (jul==0)
                {
                    if ((planet_position[0][0].RAMDA>359.0)&&(planet_position[0][1].RAMDA<359.0))
                    {
                        BL_SUN_JULGI=TRUE;
                        break;
                    }
                }
                else
                {
                    double reframda=jul*15.0;
                    double reframda2=reframda+2.0;
                    if ((planet_position[0][1].RAMDA>reframda)&&(planet_position[0][1].RAMDA<reframda2)&&(planet_position[0][0].RAMDA<reframda))
                    {
                        BL_SUN_JULGI=TRUE;
                        break;
                    }

                }

            }

        }


        opposition[planet_no]=(((planet_position[planet_no][0].elongation>180)&&(planet_position[planet_no][1].elongation<180))
                               ||((planet_position[planet_no][0].elongation<180)&&(planet_position[planet_no][1].elongation>180)&&((planet_position[planet_no][1].elongation>170)&& (planet_position[planet_no][1].elongation<190)))); //충
        conjunction[planet_no]=(((planet_position[planet_no][0].elongation>330)&&(planet_position[planet_no][1].elongation<30))
                          ||((planet_position[planet_no][0].elongation<30)&&(planet_position[planet_no][1].elongation>330))); //태양 합

        if (opposition[planet_no]&&conjunction[planet_no])
        {

            opposition[planet_no]=FALSE;
        }


        right_east[planet_no]=((planet_position[planet_no][1].elongation>90)&&(planet_position[planet_no][1].elongation<100)&&(planet_position[planet_no][0].elongation>80)&&(planet_position[planet_no][0].elongation<90))||((planet_position[planet_no][0].elongation>90)&&(planet_position[planet_no][0].elongation<100)&&(planet_position[planet_no][1].elongation>80)&&(planet_position[planet_no][1].elongation<90)); //동구

        right_west[planet_no]=((planet_position[planet_no][1].elongation>270)&&(planet_position[planet_no][1].elongation<280)&&(planet_position[planet_no][0].elongation>260)&&(planet_position[planet_no][0].elongation<270))||((planet_position[planet_no][0].elongation>270)&&(planet_position[planet_no][0].elongation<280)&&(planet_position[planet_no][1].elongation>260)&&(planet_position[planet_no][1].elongation<270)); //서구

        Perihelion[planet_no]=((planet_position[planet_no][1].true_anomaly<5.0)&&(planet_position[planet_no][1].true_anomaly>0.0))
                            &&((planet_position[planet_no][0].true_anomaly<360.0)&&(planet_position[planet_no][0].true_anomaly>355.0));
        Aphelion[planet_no]=((planet_position[planet_no][1].true_anomaly<185.0)&&(planet_position[planet_no][1].true_anomaly>180.0))
                            &&((planet_position[planet_no][0].true_anomaly<180.0)&&(planet_position[planet_no][0].true_anomaly>175.0));

        if ((planet_position[planet_no][1].BETA*planet_position[planet_no][0].BETA)<0)
            ecliptic_ascdesc[planet_no]=TRUE;

        planet_conjunction[planet_no]=FALSE;  //행성합
        int second_planet=0;
        int second_planet_count=0;
        int secone_planet_number[7];

        if (Planet_event_process_event[en_ck_planet_conjunction]==TRUE)
        {
        	int planet_no2;
            for(planet_no2=1;planet_no2<planet_no;planet_no2++)
            {
                if (Planet_event_process[planet_no2]==FALSE) continue;

                double in_low,in_upp,out_low,out_upp,temp_ramda;
                in_low=degreelow(planet_position[planet_no2][0].RAMDA);
                in_upp=degreelow(planet_position[planet_no2][1].RAMDA);
                out_low=degreelow(planet_position[planet_no][0].RAMDA);
                out_upp=degreelow(planet_position[planet_no][1].RAMDA);

                if ((in_low>330)||(in_low<30)||(out_low>330)||(out_low<30))
                {
                    in_low=degreelow(in_low+60);
                    in_upp=degreelow(in_upp+60);
                    out_low=degreelow(out_low+60);
                    out_upp=degreelow(out_upp+60);
                }

                if ((in_low>330)||(in_low<30)||(out_low>330)||(out_low<30))
                {
                    in_low=degreelow(in_low+60);
                    in_upp=degreelow(in_upp+60);
                    out_low=degreelow(out_low+60);
                    out_upp=degreelow(out_upp+60);
                }

                if (in_low>in_upp)
                {
                    temp_ramda=in_upp;
                    in_upp=in_low;
                    in_low=temp_ramda;
                }
                if (out_low>out_upp)
                {
                    temp_ramda=out_upp;
                    out_upp=out_low;
                    out_low=temp_ramda;
                }

                BOOL this_loop_overlap=FALSE;
                if ((in_low>out_low)&&(in_low<out_upp))
                {
                    planet_conjunction[planet_no]=true;
                    second_planet=planet_no2;
                    secone_planet_number[second_planet_count]=planet_no2;
                    second_planet_count++;
                    this_loop_overlap=TRUE;
                }
                if ((out_low>in_low)&&(in_upp>out_upp))
                {
                    planet_conjunction[planet_no]=true;
                    second_planet=planet_no2;
                    if (!this_loop_overlap)
                    {
                        secone_planet_number[second_planet_count]=planet_no2;
                        second_planet_count++;
                        this_loop_overlap=TRUE;
                    }
                }
                if ((in_upp>out_low)&&(out_upp>in_upp))
                {
                    planet_conjunction[planet_no]=true;
                    second_planet=planet_no2;
                    if (!this_loop_overlap)
                    {
                        secone_planet_number[second_planet_count]=planet_no2;
                        second_planet_count++;
                        this_loop_overlap=TRUE;
                    }
                }
                if ((out_upp>in_upp)&&(in_low>out_low))
                {
                    planet_conjunction[planet_no]=true;
                    second_planet=planet_no2;
                    if (!this_loop_overlap)
                    {
                        secone_planet_number[second_planet_count]=planet_no2;
                        second_planet_count++;
                    }
                }

            }
        }


        if (Planet_event_process_event[en_ck_stationary_east_to_west]||Planet_event_process_event[en_ck_stationary_west_to_east])
        {
            if (degreelow(planet_position[planet_no][1].RA-planet_position[planet_no][0].RA)<180)
            {
                if (degreelow(planet_position[planet_no][0].RA-planet_position[planet_no][prev_1].RA)>180)
                {
                    Stationary[planet_no]=TRUE;
                }
            }
            else
            {
                if (degreelow(planet_position[planet_no][0].RA-planet_position[planet_no][prev_1].RA)<180)
                {
                    Stationary[planet_no]=TRUE;
                    Stationary_heading[planet_no] = TRUE;
                }

            }
        }

        if (Planet_event_process_event[en_ck_great_elong_east]||Planet_event_process_event[en_ck_great_elong_west])
        {
            if (((planet_no==2)||(planet_no==3))&&(planet_position[planet_no][0].elongation>1)&&(planet_position[planet_no][0].elongation<359)) //내합 근처의 부호변경 제외
            {
                if ((planet_position[planet_no][1].elongation-planet_position[planet_no][0].elongation)>=0)
                {
                    if ((planet_position[planet_no][0].elongation-planet_position[planet_no][prev_1].elongation)<0)
                        great_elong[planet_no]=TRUE;
                }
                else
                {
                    if ((planet_position[planet_no][0].elongation-planet_position[planet_no][prev_1].elongation)>=0)
                    {
                        great_elong[planet_no]=TRUE;
                        great_elong_heading[planet_no]=TRUE;
                    }
                }
            }
        }

        if (compare_time_equal(planet_position[planet_no][prev_1].time,planet_position[planet_no][0].time))
        {
            Stationary[planet_no]=FALSE;
            great_elong[planet_no]=FALSE;
        }

        if ((Stationary[planet_no]||great_elong[planet_no])&&chk_stationary_and_great_elong)
        {
            int chkevent;//=5;
            chkevent=en_chk_event_stationary_or_great_elong; //=5

            if(dmin>2)
            {
                for(i=0;i<9;i++)
                {
                    planet_position[i][1]=planet_position[i][0];
                    planet_position[i][0]=q1_planet_position[i];
                }

                chk_planet_events_not_stat_greatelong=FALSE;
                event_planet=check_planet_event(planet_no); //REV_1 to 0
                chk_planet_events_not_stat_greatelong=TRUE;
                //if (event_planet==planet_no)
                if (((event_planet&0xff)==planet_no)&&(((event_planet>>8)&0xff)==chkevent))
                {
                }
                else
                {
                    for(i=0;i<9;i++)
                    {
                        planet_position[i][prev_1]=save_planet_position[i][0];
                        planet_position[i][0]=calc_planet_position_calc[i];
                        planet_position[i][1]=save_planet_position[i][1];
                    }
                    chk_planet_events_not_stat_greatelong=FALSE;
                    event_planet=check_planet_event(planet_no); //0 to 1
                    chk_planet_events_not_stat_greatelong=TRUE;

                    //if (event_planet==planet_no)
                    if (((event_planet&0xff)==planet_no)&&(((event_planet>>8)&0xff)==chkevent))
                    {
                    }
                    else
                    {
                        for(i=0;i<9;i++)
                        {
                            planet_position[i][prev_1]=q1_planet_position[i];
                            planet_position[i][0]=save_planet_position[i][0];
                            planet_position[i][1]=calc_planet_position_calc[i];
                        }
                        chk_planet_events_not_stat_greatelong=FALSE;
                        event_planet=check_planet_event(planet_no); //-mid to mid
                        chk_planet_events_not_stat_greatelong=TRUE;

                        //if (event_planet==planet_no)
                        if (((event_planet&0xff)==planet_no)&&(((event_planet>>8)&0xff)==chkevent))
                        {
                        }
                        else
                        {
                            /*
                            NSLog(@"fail occur  %@ check_planet_event depth_count=%d planet_no=%d dmin=%lld %@  %@ [prev_1].time=%@ [prev_1].RA=%3.15f [0].time=%@ [0].RA=%3.15f [1].time=%@ [1].RA=%3.15f",
                                  Stationary[planet_no]?@"stationary":@"Great_elong", depth_count, planet_no,dmin, @"유/최대이각", Planet_Orbit[planet_no]->Name_Kor,
                                  [self get_string_time:planet_position[planet_no][prev_1].time],planet_position[planet_no][prev_1].RA,
                                  [self get_string_time:planet_position[planet_no][0].time],planet_position[planet_no][0].RA,
                                  [self get_string_time:planet_position[planet_no][1].time],planet_position[planet_no][1].RA
                                  );
                            */
                        }
                    }

                }
            }
            else
            {
                BOOL Last_Stationary=FALSE;
                BOOL Last_great_elong=FALSE;

                event_planet=planet_no|(chkevent<<8);

                if (dmin==2)
                {
                	mid_time=getdatebymin( -1,planet_position[0][0].time);

                    calc_current_planet_position( mid_time, &q1_moon_node,&q1_moon_perigee,&q1_moon_apogee,&q1_moon_distance);


                    if (degreelow(planet_position[planet_no][1].RA-calc_planet_position_calc[planet_no].RA)<180)
                    {
                        if (degreelow(calc_planet_position_calc[planet_no].RA-planet_position[planet_no][0].RA)>180) Last_Stationary=TRUE;
                    }
                    else
                    {
                        if (degreelow(calc_planet_position_calc[planet_no].RA-planet_position[planet_no][0].RA)<180) Last_Stationary=TRUE;
                    }

                    if ((planet_position[planet_no][1].elongation-planet_position[planet_no][0].elongation)>=0)
                    {
                        if ((planet_position[planet_no][0].elongation-planet_position[planet_no][prev_1].elongation)<0) Last_great_elong=TRUE;
                    }
                    else
                    {
                        if ((planet_position[planet_no][0].elongation-planet_position[planet_no][prev_1].elongation)>=0) Last_great_elong=TRUE;
                    }

                }

                if (dmin==1)
                {
                    if (Stationary[planet_no])
                    {
                        if (Stationary_heading[planet_no])
                        {
                            put_event_to_result( en_stationary_east_to_west, planet_no, 0, calc_planet_position_calc);
                        }
                        else
                        {
                            put_event_to_result( en_stationary_west_to_east, planet_no, 0, calc_planet_position_calc);
                        }

                    }
                    if (great_elong[planet_no])
                    {
                        if (!great_elong_heading[planet_no])
                        {
                            put_event_to_result( en_great_elong_east, planet_no, 0, calc_planet_position_calc);
                        }
                        else
                        {
                            put_event_to_result( en_great_elong_west, planet_no, 0, calc_planet_position_calc);
                        }
                    }

                }
                else
                {
                	mid_time=getdatebymin( -1,planet_position[0][0].time);

                    calc_current_planet_position( mid_time, &q1_moon_node,&q1_moon_perigee,&q1_moon_apogee,&q1_moon_distance);


                    if (degreelow(planet_position[planet_no][1].RA-calc_planet_position_calc[planet_no].RA)<180)
                    {
                        if (degreelow(calc_planet_position_calc[planet_no].RA-planet_position[planet_no][0].RA)>180) Last_Stationary=TRUE;
                    }
                    else
                    {
                        if (degreelow(calc_planet_position_calc[planet_no].RA-planet_position[planet_no][0].RA)<180) Last_Stationary=TRUE;
                    }


                    if (Last_Stationary)
                    {
                        if (Stationary_heading[planet_no])
                        {
                            put_event_to_result( en_stationary_east_to_west, planet_no, 0, calc_planet_position_calc);
                        }
                        else
                        {
                        	put_event_to_result( en_stationary_west_to_east, planet_no, 0, calc_planet_position_calc);
                        }

                    }
                    else
                    {
                        if (!Last_great_elong)
                        {
                            if (Stationary_heading[planet_no])
                            {
                            	put_event_to_result( en_stationary_east_to_west, planet_no, 0, calc_planet_position_calc);
                            }
                            else
                            {
                            	put_event_to_result( en_stationary_west_to_east, planet_no, 0, calc_planet_position_calc);
                            }
                        }
                    }

                    if ((planet_position[planet_no][1].elongation-calc_planet_position_calc[planet_no].elongation)>=0)
                    {
                        if ((calc_planet_position_calc[planet_no].elongation-planet_position[planet_no][0].elongation)<0) Last_great_elong=TRUE;
                    }
                    else
                    {
                        if ((calc_planet_position_calc[planet_no].elongation-planet_position[planet_no][0].elongation)>=0) Last_great_elong=TRUE;
                    }

                    if (Last_great_elong)
                    {
                        if (!great_elong_heading[planet_no])
                        {
                            put_event_to_result( en_great_elong_east, planet_no, 0, calc_planet_position_calc);
                        }
                        else
                        {
                        	put_event_to_result( en_great_elong_west, planet_no, 0, calc_planet_position_calc);
                        }

                    }
                    else
                    {
                        //if ((!Last_Stationary)&&(!Stationary))
                        if ((!Last_Stationary)&&(!Stationary[planet_no]))
                        {
                            if (!great_elong_heading[planet_no])
                            {
                            	put_event_to_result( en_great_elong_east, planet_no, 0, calc_planet_position_calc);
                            }
                            else                            {
                            	put_event_to_result( en_great_elong_west, planet_no, 0, calc_planet_position_calc);
                            }

                        }
                    }
                }
            }
        }


        for(i=0;i<3;i++)
        {
            for(j=0;j<9;j++)
                planet_position[j][i]=save_planet_position[j][i];
        }

        if ((Planet_event_process_event[en_ck_ascending_node]==FALSE)&&(Planet_event_process_event[en_ck_descending_node]==FALSE))
        {
            BL_Moon_Node=FALSE;
            ecliptic_ascdesc[planet_no]=FALSE;
        }
        if (Planet_event_process_event[en_ck_perigee_Perihelion]==FALSE)
        {
            BL_Moon_perigee=FALSE;
            Perihelion[planet_no]=FALSE;
        }
        if (Planet_event_process_event[en_ck_apogee_Aphelion]==FALSE)
        {
            BL_Moon_apogee=FALSE;
            Aphelion[planet_no]=FALSE;
        }
        if ((Planet_event_process_event[en_ck_perigee_Perihelion]==FALSE)&&(Planet_event_process_event[en_ck_apogee_Aphelion]==FALSE))
            BL_SUN_helion=FALSE;

        if (Planet_event_process_event[en_ck_right_east_first_quarter]==FALSE)
        {
            right_west[planet_no]=FALSE;
            BL_MOON_LAST_Q=FALSE;

        }
        if (Planet_event_process_event[en_ck_right_west_last_quarter]==FALSE)
        {
            right_east[planet_no]=FALSE;

            BL_MOON_First_Q=FALSE;
        }
        if (Planet_event_process_event[en_ck_planet_conjunction]==FALSE) planet_conjunction[planet_no]=FALSE;

        if (Planet_event_process_event[en_ck_sun_julgi]==FALSE) BL_SUN_JULGI=FALSE;

        if (Planet_event_process_event[en_ck_opposition_inferior_full_moon]==FALSE) opposition[planet_no]=FALSE;
        if (Planet_event_process_event[en_ck_conjunction_superior_new_moon]==FALSE)
        {
            if ((planet_no!=2)&&(planet_no!=3))
                conjunction[planet_no]=FALSE;
        }

        if (Planet_event_process_event[en_ck_opposition_inferior_full_moon]==FALSE) opposition[planet_no]=FALSE;



        if ((((BL_Moon_Node||BL_Moon_perigee||BL_Moon_apogee||BL_MOON_First_Q||BL_MOON_LAST_Q||BL_SUN_JULGI||BL_SUN_helion)&&(planet_no==1))||(conjunction[planet_no] || opposition[planet_no] || right_east[planet_no] ||right_west[planet_no]||planet_conjunction[planet_no]||Perihelion[planet_no]||Aphelion[planet_no]||ecliptic_ascdesc[planet_no]))&&chk_planet_events_not_stat_greatelong)
        {
            int count_true=0;
            int check_event[3];

            if ((BL_Moon_perigee||BL_Moon_apogee)&&(planet_no==1))
            {
                check_event[count_true]=en_chk_event_perigee_apogee;//=1,
                count_true++;
            }
            if ((BL_Moon_Node)&&(planet_no==1))
            {
                check_event[count_true]=en_chk_event_Node;//=1,
                count_true++;
            }


            if ((BL_MOON_First_Q||BL_MOON_LAST_Q)&&(planet_no==1))
            {
                check_event[count_true]=en_chk_event_First_Q_LAST_Q_planet_no_1;//=2,
                count_true++;
            }

            if (BL_SUN_JULGI&&(planet_no==1))
            {
                check_event[count_true]=en_chk_event_sun_julgi;
                count_true++;
            }

            if (BL_SUN_helion&&(planet_no==1))
            {
                check_event[count_true]=en_chk_event_sun_helion;
                count_true++;
            }


            if (conjunction[planet_no] || opposition[planet_no] || right_east[planet_no] ||right_west[planet_no]||Perihelion[planet_no]||Aphelion[planet_no]||ecliptic_ascdesc[planet_no])
            {

                if ((planet_no==1)&&((BL_MOON_First_Q&&right_east[planet_no])||(BL_MOON_LAST_Q&&right_west[planet_no])))
                {

                }
                else
                {
                    check_event[count_true]=en_chk_event_conjunction_opposition_right_east_west;//=3,
                    count_true++;
                }
            }

            if (planet_conjunction[planet_no])
            {
            	int k;
                for (k=0;k<second_planet_count;k++)
                {
                    check_event[count_true]=en_chk_event_planet_conjunction | (secone_planet_number[k]<<8);//=4,
                    count_true++;
                }
            }

            int total_true=count_true;

            int event_result=en_nothing;
            BOOL requested_event=TRUE;
            do
            {
                if (dmin>1)
                {
                    for (i=0; i<9; i++)
                        planet_position[i][1]=calc_planet_position_calc[i];
                    moon_node[1]=mid_moon_node;
                    moon_perigee[1]=mid_moon_perigee;
                    moon_apogee[1]=mid_moon_apogee;
                    moon_distance[1]=mid_moon_distance;
                    chk_stationary_and_great_elong=FALSE;
                    event_planet=check_planet_event(planet_no); // 0 to mid
                    chk_stationary_and_great_elong=TRUE;
                    requested_event=FALSE;
                    if (total_true>1)
                    {
                        int get_event=(event_planet>>8)&0xff;
                        if (get_event==check_event[count_true]) requested_event=TRUE;
                        else requested_event=FALSE;
                    }
                    //if ((event_planet==planet_no)&&(requested_event))
                    if (((event_planet&0xff)==planet_no)&&(requested_event))
                    {

                    }
                    else
                    {
                        for (i=0; i<9; i++)
                        {
                            planet_position[i][0]=calc_planet_position_calc[i];
                            planet_position[i][1]=save_planet_position[i][1];
                        }

                        moon_node[0]=mid_moon_node;
                        moon_perigee[0]=mid_moon_perigee;
                        moon_apogee[0]=mid_moon_apogee;
                        moon_distance[0]=mid_moon_distance;

                        moon_node[1]=save_moon_node[1];
                        moon_perigee[1]=save_moon_perigee[1];
                        moon_apogee[1]=save_moon_apogee[1];
                        moon_distance[1]=save_moon_distance[1];
                        chk_stationary_and_great_elong=FALSE;
                        event_planet=check_planet_event(planet_no); // mid to 1
                        chk_stationary_and_great_elong=TRUE;
                        requested_event=FALSE;
                        if (total_true>1)
                        {
                            int get_event=(event_planet>>8)&0xff;
                            if (get_event==check_event[count_true]) requested_event=TRUE;
                            else requested_event=FALSE;
                        }
                        //if ((event_planet==planet_no)&&(requested_event))
                        if (((event_planet&0xff)==planet_no)&&(requested_event))
                        {
                        }
                        else
                        {
                        	/*
                            if ((!requested_event)&&(!planet_conjunction[planet_no])&&(planet_no!=1))
                            {
                            	Logi("<br>fail occur  check_planet_event depth_count=%d planet_no=%d dmin=%lld %s  %s  %s time=%d-%d-%d %d:%d <br>\n",
                            			depth_count, planet_no,dmin, "Moon/Planet",//달/행성 합충구 행성합,
                            			Planet_Orbit[planet_no]->Name_Kor,Planet_Orbit[second_planet]->Name_Kor,

                            			planet_position[0][1].time.year,planet_position[0][1].time.month,planet_position[0][1].time.day,planet_position[0][1].time.hour,planet_position[0][1].time.min
                            	);
                            }
							*/
                        }
                    }
                }
                else
                {
                    event_planet=planet_no;

                    if (conjunction[planet_no] || opposition[planet_no] || right_east[planet_no] ||right_west[planet_no]||Perihelion[planet_no]||Aphelion[planet_no]||ecliptic_ascdesc[planet_no])
                    {
                        int event_result=en_nothing;

                        if (conjunction[planet_no])
                        {
                            //합
                            event_result=en_conjunction;
                            if ((planet_no==2)||(planet_no==3))
                            {
                                //외합
                                event_result=en_superior_conjunction;
                                if (planet_position[planet_no][0].planet_distance <1)
                                {
                                    event_result=en_inferior_conjuinction;
                                }
                            }
                        }

                        if (opposition[planet_no])
                        {
                            event_result=en_opposition;
                        }
                        if (right_east[planet_no])
                        {
                            event_result=en_right_east;
                        }
                        if (right_west[planet_no])
                        {
                        		event_result=en_right_west;
                        }
                        if (Perihelion[planet_no])
                        {
                            event_result=en_perihelion;
                        }
                        if (Aphelion[planet_no])
                        {
                            event_result=en_Aphelion;
                        }

                        if (ecliptic_ascdesc[planet_no])
                        {
                            if (planet_position[planet_no][0].BETA<planet_position[planet_no][1].BETA)
                                 event_result=en_ascecliptic;
                            else
                                 event_result=en_desecliptic;
                        }


                        if (planet_no==1)
                        {

                            if (conjunction[planet_no]) event_result=en_new_moon;
                            if (opposition[planet_no]) event_result=en_full_moon;

                            if ((event_result==en_new_moon)||(event_result==en_full_moon))
                            {
                                for(j=0;j<9;j++)
                                    result_calc_planet_position_calc[j]=planet_position[j][1];
                                put_event_to_result( event_result, 1, 0, result_calc_planet_position_calc);
                            }
                        }
                        else
                        {
                            for(j=0;j<9;j++)
                                result_calc_planet_position_calc[j]=planet_position[j][1];
                            put_event_to_result( event_result, planet_no, 0, result_calc_planet_position_calc);
                        }


                    }
                    //for 행성합
                    if (planet_conjunction[planet_no])
                    {
                        for(j=0;j<9;j++)
                            result_calc_planet_position_calc[j]=planet_position[j][1];
                        put_event_to_result( en_planet_conjunction, planet_no, second_planet, result_calc_planet_position_calc);

                    }
                    //달의 위치
                    if ((BL_Moon_Node||BL_Moon_perigee||BL_Moon_apogee||BL_MOON_First_Q||BL_MOON_LAST_Q||BL_SUN_JULGI||BL_SUN_helion)&&(planet_no==1))
                    {

                        if (BL_Moon_apogee)
                        {
                            event_result=en_apogee;
                        }
                        if (BL_Moon_perigee)
                        {
                            event_result=en_perigee;
                        }

                        if (BL_Moon_Node)
                        {
                            event_result=en_descending_node;
                            if (planet_position[1][0].BETA<planet_position[1][1].BETA)
                            {
                                event_result=en_ascending_node;
                            }

                        }
                        if (BL_MOON_First_Q)
                        {
                            event_result=en_last_quarter;
                        }
                        if (BL_MOON_LAST_Q)
                        {
                            event_result=en_first_quarter;
                        }

                        if (BL_SUN_JULGI)
                        {
                            event_result=en_sun_julgi;
                        }

                        if (BL_SUN_helion)
                        {
                            if ((planet_position[0][1].true_anomaly>175)&&(planet_position[0][1].true_anomaly<185))
                                event_result=en_Aphelion;
                            else
                                event_result=en_perihelion;

                        }

                        if (event_result!=en_nothing)
                        {
                            for(j=0;j<9;j++)
                                result_calc_planet_position_calc[j]=planet_position[j][1];
                            if (BL_SUN_JULGI||BL_SUN_helion)
                            {
                                put_event_to_result( event_result, 0, 0, result_calc_planet_position_calc);
                            }
                            else if (BL_MOON_First_Q||BL_MOON_LAST_Q)
                            {
                                put_event_to_result( event_result, 1, 0, result_calc_planet_position_calc);
                            }
                            else
                            {
                                put_event_to_result( event_result, 1, 1, result_calc_planet_position_calc);
                            }
                        }

                    }

                }

                if (total_true>1)
                {
                    event_planet|=(check_event[count_true-1]<<8);

                }

                count_true--;
            } while (count_true!=0);
        }

    } //for(int planet_no=start_planet_no;planet_no<end_planet_no;planet_no++)



    for (i=0; i<9; i++)
    {
        calc_planet_position_calc[i]=save_calc_planet_position_calc[i];
    }

    for(i=0;i<3;i++)
    {
        for(j=0;j<9;j++)
            planet_position[j][i]=save_planet_position[j][i];
        moon_node[i]=save_moon_node[i];
        moon_perigee[i]=save_moon_perigee[i];
        moon_apogee[i]=save_moon_apogee[i];
        moon_distance[i]=save_moon_distance[i];
    }

    depth_count--;

    return event_planet;
}


double moon_apogee_correction(double day)
{
	return moonsundegree(day, T_Moon_apogee_correct);
}


double moon_perigee_correction(double day)
{
	return moonsundegree(day, T_Moon_perigee_correct);
}



//d에 이 가까운 합삭/망 시점 구하기,
double datefix(double d, int degree, int pn, int mode)
{
	double ret_d, dp, temp=0;
	double degree0, degree1;

	dp=d;

	degree0 = moonsundegree(dp,mode);

	if (degree==0)
	{
		if (pn==-1)  //(pn==-1)&&(degree==0) <- 이전합삭시점 구할때만 사용
		{
			if (degree0>180) {
				dp=dp-14;
				degree0 = moonsundegree(dp,mode);
			}
			temp=-9.666;
		}
		else
		{
			if (degree0<180) {
				dp=dp+14;
				degree0 = moonsundegree(dp,mode);
			}
			temp=+9.666 ;
		}
	}
	else
	{
		//pn=+1 ,
		//원지점,강교점,망,상현,하현 구할때 사용
		//temp=10; //13일부터 17일 사이
		if (mode==T_Moon_Real)
		{			//degree=90,180,270 상현/망/하련
			temp=5;
		}
		else
		{
			//180도 - 원지점,강교점
			if (degree0>180) {
				dp=dp+14;
				degree0 = moonsundegree(dp,mode);
			}
			temp=+9.666 ;

		}
	}

	double init_temp=temp;

	int i=0;
	while (i<30)
	{

		degree1 = moonsundegree(dp+temp,mode);

		if (pn==-1)
		{
			if (degree1>180) temp=temp-init_temp;
			else temp=temp+init_temp;
		}
		else
		{
			//pn==1
			if (degree==0)
			{
				if (degree1>180) temp=temp+init_temp;
				else temp=temp-init_temp;
			}
			else
			{
				if (degree==180)
				{
					if (degree1>180) temp=temp-init_temp;
					else temp=temp+init_temp;
				}
				else
				{
					if (degree==90)
					{
						if (degree1>90) temp=temp-init_temp;
						else temp=temp+init_temp;

					}
					else
					{	// degree==270
						if (degree1>270) temp=temp-init_temp;
						else temp=temp+init_temp;
					}
				}
			}
		}

		init_temp=init_temp/2;

		if (fabs(degree-degree0)<0.0001) break;

		degree0=degree1;
		i=i+1;

	}
	ret_d=dp+temp;

	if (mode==T_Moon_perigee)
	{
		ret_d=ret_d+moon_perigee_correction(ret_d);
	}

	if (mode==T_Moon_apogee)
	{
		ret_d=ret_d+moon_apogee_correction(ret_d);
	}

	return ret_d;
}

double getjuliandate_point_pre(int syear, int smonth, int sday, int shour, int smin)
{
	int d,dt;
	double df,tminreal;
	int tmin;

	d=disp2days(syear,smonth,sday,1899,12,31);

	d =d+2415020;
	tmin=(int)getminbytime(syear,smonth,sday,shour,smin,syear,smonth,sday,21,0);

	tminreal=((double)tmin/1440)*1000; //-362.5
	dt=(int)tminreal;;

	df = (double)d+(double)dt/1000;

	return df;
}

double getjuliandate_point(date_cal date)
{
	double df = getjuliandate_point_pre(date.year,date.month,date.day,date.hour,date.min);
	return df;
}


date_interpol calc_moon_position(double d,int degree,int pn, int mode)
{
	date_interpol ret;
	long long i;

	date_cal moon_udate;
	moon_udate.year=1995;
	moon_udate.month=12;
	moon_udate.day=31;
	moon_udate.hour=0;
	moon_udate.min=0;

	ret.calc_d=datefix(d, degree, pn,mode);   //입력 d-기준날짜, ret값 temp=해당 시점

	ret.ip=interpointdisp(ret.calc_d);

	ret.distance=moon_distance_double(ret.calc_d);

	ret.distance_rate=moon_distance_rate(ret.distance);

	ret.solor_lat=moonsundegree(ret.calc_d,T_Moon_Sol_lat);

	ret.lunar_lat=moonsundegree(ret.calc_d,T_Moon_lunar_lat);

	double temp=ret.calc_d+0.375;
	temp = temp*1440 ;
	i = -1*(long long)temp;

	ret.date=getdatebymin(i,moon_udate);

	ret.julian_day=getjuliandate_point_pre(ret.date.year,ret.date.month,ret.date.day,ret.date.hour,ret.date.min);

	ret.remark=-1;
	ret.eclipse=0;
	return ret;

}

moon_info getlunarfirst(int syear, int smonth, int sday)
{
	moon_info ret;
	/*
	date_cal moon_udate;
	moon_udate.year=1995;
	moon_udate.month=12;
	moon_udate.day=31;
	moon_udate.hour=0;
	moon_udate.min=0;
	*/

	double dm;
	double new_moon_d;

	ret.syear=syear;
	ret.smonth=smonth;
	ret.sday=sday;

	dm=disp2days(syear,smonth,sday,1995,12,31);

//New_moon
	ret.moon_new=calc_moon_position(dm,0,-1,T_Moon_Real);
	new_moon_d=ret.moon_new.calc_d;
	ret.moon_new.moon_age=0;
	ret.moon_new.remark=T_remark_date_interpol_moon_new;

//Next_new_moon
	ret.moon_next_new=calc_moon_position(dm,0,+1,T_Moon_Real);
	ret.moon_next_new.moon_age=ret.moon_next_new.calc_d-new_moon_d;
	ret.moon_next_new.remark=T_remark_date_interpol_moon_next_new;

	if ((smonth==ret.moon_next_new.date.month)&&(sday==ret.moon_next_new.date.day))
	{
		ret.moon_new=ret.moon_next_new;
		ret.moon_new.moon_age=0;
		ret.moon_new.remark=T_remark_date_interpol_moon_new;
		new_moon_d=ret.moon_new.calc_d;

		ret.moon_next_new=calc_moon_position((new_moon_d+25),0,+1,T_Moon_Real);
		ret.moon_next_new.moon_age=ret.moon_next_new.calc_d-new_moon_d;
		ret.moon_next_new.remark=T_remark_date_interpol_moon_next_new;
	}

	ret.moon_month_length_min=getminbytime( ret.moon_new.date.year,ret.moon_new.date.month,ret.moon_new.date.day,ret.moon_new.date.hour,ret.moon_new.date.min,
   											ret.moon_next_new.date.year,ret.moon_next_new.date.month,ret.moon_next_new.date.day,ret.moon_next_new.date.hour,ret.moon_next_new.date.min
			 	 	 	 	 	 	 	   );

	ret.moon_new.eclipse=0;
	if (fabs(ret.moon_new.ip)<15.3) ret.moon_new.eclipse=1;// " 日蝕 ";  // 日식 < 15.3
	else if (fabs(ret.moon_new.ip)<18.5) ret.moon_new.eclipse=2;  //日식가능 < 18.5

	ret.moon_next_new.eclipse=0;
	if (fabs(ret.moon_next_new.ip)<15.3) ret.moon_next_new.eclipse=1;// " 日蝕 ";  // 日식 < 15.3
	else if (fabs(ret.moon_next_new.ip)<18.5) ret.moon_next_new.eclipse=2;  //日식가능 < 18.5

	ret.moon_datefix_array[0]=ret.moon_new;
	ret.moon_datefix_array[4]=ret.moon_next_new;

//moon_full
	ret.moon_full=calc_moon_position((new_moon_d+12),180,+1,T_Moon_Real);
	ret.moon_full.moon_age=ret.moon_full.calc_d-new_moon_d;
	ret.moon_full.remark=T_remark_date_interpol_moon_full;

	ret.moon_full.eclipse=0;
	if (fabs(ret.moon_full.ip)<9.5) ret.moon_full.eclipse=3;// " 日蝕 ";  // 日식 < 15.3
	else if (fabs(ret.moon_full.ip)<12.2) ret.moon_full.eclipse=4;  //日식가능 < 18.5

	ret.moon_datefix_array[2]=ret.moon_full;

	//Islamic Month
	//int islamic_epoch=1948438; // JD 1948438=622.7.17 이슬람 달력 원기
	long long tmin_start_to_this=getminbytime(ret.moon_new.date.year,ret.moon_new.date.month,ret.moon_new.date.day,ret.moon_new.date.hour,ret.moon_new.date.min,622,7,17,15,35);

	if (tmin_start_to_this<0) {
		tmin_start_to_this=tmin_start_to_this-2880;//2일 추가 - 나눗셈에서 항상 크게나오게 하기위해
	} else
	{
		tmin_start_to_this=tmin_start_to_this+2880;//2일 추가 - 나눗셈에서 항상 크게나오게 하기위해
	}


	int  total_month=(int) (tmin_start_to_this/42524.04672);

	ret.islamic_year=total_month/12 ;

	ret.islamic_month=total_month%12 ;

	if (tmin_start_to_this<0) {
		ret.islamic_month=ret.islamic_month+12 ;
		ret.islamic_year=ret.islamic_year-1;
		if (ret.islamic_month==12)
		{
			ret.islamic_month=0 ;
			ret.islamic_year=ret.islamic_year+1;
		}

	}


    //유대력 구하기 jewish_year,jewish_month,jewish_day;
    ret.jewish_year=0;
    ret.jewish_month=0;
    ret.jewish_day=0;

    //1996.9.13 -> 5757.1.1 기점
    total_month=total_month-16996;
    ret.jewish_year=5757;
    if (total_month>0)
    {
        while(total_month>235)
        {
            total_month=total_month-235;
            ret.jewish_year+=19;
        }
    }
    else
    {
        while(total_month<1)
        {
            total_month=total_month+235;
            ret.jewish_year-=19;
        }
    }


    int jewish_mod=0;
    int jewish_mount_count=13;
    while (total_month>(jewish_mount_count-1))
    {
        total_month=total_month-jewish_mount_count;
        ret.jewish_year++;
        jewish_mod++;
        if ((jewish_mod==3)||(jewish_mod==6)||(jewish_mod==8)||(jewish_mod==11)||(jewish_mod==14)||(jewish_mod==17))
        {
            jewish_mount_count=13;
        }
        else
        {
            jewish_mount_count=12;
        }
    }


    if ((jewish_mod==0)||(jewish_mod==3)||(jewish_mod==6)||(jewish_mod==8)||(jewish_mod==11)||(jewish_mod==14)||(jewish_mod==17))
    {
        if (total_month<6)
        {
            ret.jewish_month=total_month;
        }
        else
        {
            if (total_month==6) ret.jewish_month=12;
            else ret.jewish_month=total_month-1;

        }
    }
    else
        ret.jewish_month=total_month;
    //유대력 jewish year,month 끝


//상현(90),하현(270) 구하기
	ret.moon_1st_quarter=calc_moon_position((new_moon_d+5),90,+1,T_Moon_Real);
	ret.moon_1st_quarter.moon_age=ret.moon_1st_quarter.calc_d-new_moon_d;
	ret.moon_1st_quarter.remark=T_remark_date_interpol_moon_1st_quarter;
	ret.moon_1st_quarter.eclipse=0;
	ret.moon_datefix_array[1]=ret.moon_1st_quarter;

	ret.moon_last_quarter=calc_moon_position((new_moon_d+20),270,+1,T_Moon_Real);
	ret.moon_last_quarter.moon_age=ret.moon_last_quarter.calc_d-new_moon_d;
	ret.moon_last_quarter.remark=T_remark_date_interpol_moon_last_quarter;
	ret.moon_last_quarter.eclipse=0;
	ret.moon_datefix_array[3]=ret.moon_last_quarter;


//승교점/강교점과 근점/원점 구하기
	ret.moon_junction_1=calc_moon_position((new_moon_d),0,+1,T_Moon_Junction);
	ret.moon_junction_1.moon_age=ret.moon_junction_1.calc_d-new_moon_d;
	ret.moon_junction_1.remark=T_remark_date_interpol_moon_junction_ascending;

	ret.moon_junction_2=calc_moon_position((new_moon_d),180,+1,T_Moon_Junction);
	ret.moon_junction_2.moon_age=ret.moon_junction_2.calc_d-new_moon_d;
	ret.moon_junction_2.remark=T_remark_date_interpol_moon_junction_descending;

	ret.moon_junction_3.remark=-1;

//두번째 승교점
	if ((ret.moon_junction_1.moon_age<4)||((ret.moon_junction_1.moon_age>25)&&(ret.moon_new.eclipse!=0)))
	{
		if (ret.moon_junction_1.moon_age<4)
		{
			ret.moon_junction_3=calc_moon_position(new_moon_d+ret.moon_junction_1.moon_age+25, 0, +1,T_Moon_Junction);
		}
		else
		{
			ret.moon_junction_3=calc_moon_position(new_moon_d-5, 0, +1,T_Moon_Junction);
		}
		ret.moon_junction_3.moon_age=ret.moon_junction_3.calc_d-new_moon_d;

		if (ret.moon_junction_3.moon_age<ret.moon_next_new.moon_age)
			ret.moon_junction_3.remark=T_remark_date_interpol_moon_junction_ascending;
		if (ret.moon_next_new.eclipse!=0)
			ret.moon_junction_3.remark=T_remark_date_interpol_moon_junction_ascending;
	}

//두번째 강교점
	if ((ret.moon_junction_2.moon_age<4)||((ret.moon_junction_2.moon_age>25)&&(ret.moon_new.eclipse!=0)))
	{

		if (ret.moon_junction_2.moon_age<4)
		{
			ret.moon_junction_3=calc_moon_position(new_moon_d+ret.moon_junction_2.moon_age+25, 180, +1,T_Moon_Junction);
		}
		else
		{
			ret.moon_junction_3=calc_moon_position(new_moon_d-5, 180, +1,T_Moon_Junction);
		}
		ret.moon_junction_3.moon_age=ret.moon_junction_3.calc_d-new_moon_d;

		if (ret.moon_junction_3.moon_age<ret.moon_next_new.moon_age)
			ret.moon_junction_3.remark=T_remark_date_interpol_moon_junction_descending;
		if (ret.moon_next_new.eclipse!=0)
			ret.moon_junction_3.remark=T_remark_date_interpol_moon_junction_descending;
	}

	ret.moon_datefix_array[5]=ret.moon_junction_1;
	ret.moon_datefix_array[6]=ret.moon_junction_2;


// 근점/원점 구하기
	ret.moon_perigee_apogee_1=calc_moon_position(new_moon_d, 0, +1,T_Moon_perigee);
	ret.moon_perigee_apogee_1.moon_age=ret.moon_perigee_apogee_1.calc_d-new_moon_d;
	ret.moon_perigee_apogee_1.remark=T_remark_date_interpol_moon_perigee;

	ret.moon_perigee_apogee_2=calc_moon_position(new_moon_d, 180, +1,T_Moon_apogee);
	ret.moon_perigee_apogee_2.moon_age=ret.moon_perigee_apogee_2.calc_d-new_moon_d;
	ret.moon_perigee_apogee_2.remark=T_remark_date_interpol_moon_apogee;


//두번째 근점
	ret.moon_perigee_apogee_3.remark=-1; // 없음
	if (ret.moon_perigee_apogee_1.moon_age<3)
	{
		ret.moon_perigee_apogee_3=calc_moon_position((new_moon_d+ret.moon_perigee_apogee_1.moon_age+10), 0, +1,T_Moon_perigee);
		ret.moon_perigee_apogee_3.moon_age=ret.moon_perigee_apogee_3.calc_d-new_moon_d;

		if (ret.moon_perigee_apogee_1.moon_age<0)
		{
			ret.moon_perigee_apogee_1=ret.moon_perigee_apogee_3;
			ret.moon_perigee_apogee_1.remark=T_remark_date_interpol_moon_perigee;
		}
		else
		if (ret.moon_perigee_apogee_3.moon_age<ret.moon_next_new.moon_age)
			{
				ret.moon_perigee_apogee_3.remark=T_remark_date_interpol_moon_perigee; // 근점
			}
	}
//두번째 원점
	if (ret.moon_perigee_apogee_2.moon_age<3)
	{
		ret.moon_perigee_apogee_3=calc_moon_position((new_moon_d+ret.moon_perigee_apogee_2.moon_age+10), 180, +1,T_Moon_apogee);
		ret.moon_perigee_apogee_3.moon_age=ret.moon_perigee_apogee_3.calc_d-new_moon_d;
		if (ret.moon_perigee_apogee_2.moon_age<0)
		{
			ret.moon_perigee_apogee_2=ret.moon_perigee_apogee_3;
			ret.moon_perigee_apogee_2.remark=T_remark_date_interpol_moon_apogee; // 근점
		}
		else
			if (ret.moon_perigee_apogee_3.moon_age<ret.moon_next_new.moon_age)
			{
				ret.moon_perigee_apogee_3.remark=T_remark_date_interpol_moon_apogee; // 원점
			}

	}
	ret.moon_datefix_array[7]=ret.moon_perigee_apogee_1;
	ret.moon_datefix_array[8]=ret.moon_perigee_apogee_2;

	ret.moon_datefix_array_count=9;

	if (ret.moon_perigee_apogee_3.remark!=-1)
	{
		ret.moon_datefix_array[ret.moon_datefix_array_count]=ret.moon_perigee_apogee_3;
		ret.moon_datefix_array_count++;
	}
	if (ret.moon_junction_3.remark!=-1)
	{
		ret.moon_datefix_array[ret.moon_datefix_array_count]=ret.moon_junction_3;
		ret.moon_datefix_array_count++;
	}

	int j,k;
	for(j=0;j<ret.moon_datefix_array_count;j++)
	{
		for(k=j;k<ret.moon_datefix_array_count;k++)
		{
			if (ret.moon_datefix_array[j].julian_day>ret.moon_datefix_array[k].julian_day)
			{
				date_interpol temp_moon_data;
				temp_moon_data=ret.moon_datefix_array[k];
				ret.moon_datefix_array[k]=ret.moon_datefix_array[j];
				ret.moon_datefix_array[j]=temp_moon_data;
			}
		}
	}


	/*
	for(j=0;j<ret.moon_datefix_array_count;j++)
	{
		printf(" j=%2d  %2d-%2d-%2d %2d:%2d  remark=%2d  julian_day=%10.3f\n",
				   j,
				   ret.moon_datefix_array[j].date.year, ret.moon_datefix_array[j].date.month, ret.moon_datefix_array[j].date.day, ret.moon_datefix_array[j].date.hour, ret.moon_datefix_array[j].date.min,
				   ret.moon_datefix_array[j].remark,
				   ret.moon_datefix_array[j].julian_day

				);
	}
	*/
	return ret;
}


so24_8letter sydtoso24yd(int soloryear, int solormonth, int solorday, int solorhour, int solormin)
{
	so24_8letter so24;
	long long displ2min;
	int displ2day,monthmin100,j,t;
	int i=0;

	displ2min=getminbytime(unityear,unitmonth,unitday,unithour,unitmin,
						   soloryear,solormonth,solorday,solorhour,solormin);

	displ2day=disp2days(unityear,unitmonth,unitday,soloryear,solormonth,solorday) ;

	so24.so24 = (int)(displ2min / 525949);
	if (displ2min >= 0) so24.so24 = so24.so24 + 1;
	so24.so24year=-1*(so24.so24 % 60);
	so24.so24year=so24.so24year+12; //{ 년주 구함 끝 }
	if (so24.so24year<0) so24.so24year=so24.so24year+60;
	if (so24.so24year>59) so24.so24year=so24.so24year-60;

	monthmin100= (int)(displ2min % 525949);
	monthmin100=525949-monthmin100;
	if (monthmin100<0)  monthmin100=monthmin100+525949;
	if (monthmin100>=525949)  monthmin100=monthmin100-525949;

	for(i=0;i<12;i++)
	{
		j=i*2;
		if ((montharray[j]<=monthmin100)&&(monthmin100< montharray[j+2]))
		{
			so24.so24month=i;
		}
	}

	i=so24.so24month;
	t = so24.so24year % 10 ;
	t = t % 5 ;
	t = t * 12 + 2 + i;
	so24.so24month = t ;  //{ 월주 구함 끝 }
	if (so24.so24month > 59) so24.so24month = so24.so24month - 60 ;

	so24.so24day= displ2day % 60 ;
	so24.so24day = -1 * so24.so24day  ;
	so24.so24day = so24.so24day + 7;
	if (so24.so24day < 0) so24.so24day = so24.so24day + 60 ;
	if (so24.so24day > 59) so24.so24day =so24.so24day - 60 ; //{ 일주 구함 끝}

	if (( solorhour==0 )||((solorhour==1) && (solormin < 30))) i= 0;
	if (((solorhour==1)&&(solormin >= 30))|| (solorhour==2) || ((solorhour==3) && (solormin<30))) i=1;
	if (((solorhour==3)&&(solormin >= 30))|| (solorhour==4) || ((solorhour==5) && (solormin<30))) i=2;
	if (((solorhour==5)&&(solormin >= 30))|| (solorhour==6) || ((solorhour==7) && (solormin<30))) i=3;
	if (((solorhour==7)&&(solormin >= 30))|| (solorhour==8) || ((solorhour==9) && (solormin<30))) i=4;
	if (((solorhour==9)&&(solormin >= 30))|| (solorhour==10) || ((solorhour==11) && (solormin<30))) i=5;
	if (((solorhour==11)&&(solormin >= 30))|| (solorhour==12) || ((solorhour==13) && (solormin<30))) i=6;
	if (((solorhour==13)&&(solormin >= 30))|| (solorhour==14) || ((solorhour==15) && (solormin<30))) i=7;
	if (((solorhour==15)&&(solormin >= 30))|| (solorhour==16) || ((solorhour==17) && (solormin<30))) i=8;
	if (((solorhour==17)&&(solormin >= 30))|| (solorhour==18) || ((solorhour==19) && (solormin<30))) i=9;
	if (((solorhour==19)&&(solormin >= 30))|| (solorhour==20) || ((solorhour==21) && (solormin<30))) i=10;
	if (((solorhour==21)&&(solormin >= 30))|| (solorhour==22) || ((solorhour==23) && (solormin<30))) i=11;


	if (( solorhour==23)&&(solormin>= 30))
	{
		so24.so24day = so24.so24day + 1;
		if (so24.so24day == 60) so24.so24day=0;
		i = 0;
	}

	t = so24.so24day % 10 ;
	t = t % 5 ;
	t = t * 12 + i;
	so24.so24hour= t;

	return so24;
}


double get_solar_lat(date_cal date)
{

	double day,hour;
	double day_hour;

	day=disp2days(date.year,date.month,date.day,1995,12,31);

	hour=date.hour*60+date.min;
	if (day<0)
	{
		hour=hour-1440;
		day=day+1;
	}

	day_hour=(double)(day+(hour/1440)) ;
	day_hour=day_hour-0.375;


	return moonsundegree(day_hour,T_Moon_Sol_lat);
}


so24term_month SolortoSo24(date_cal afix)
{
	so24term_month ret;
	so24_8letter so24;
	date_cal unit_ymdhm;

	int i,monthmin100,j;
	long long tmin,displ2min;

	so24=sydtoso24yd(afix.year,afix.month,afix.day,afix.hour,afix.min);

	displ2min=getminbytime(unityear,unitmonth,unitday,unithour,unitmin,
						   afix.year,afix.month,afix.day,afix.hour,afix.min
						   );

	monthmin100=(int)(displ2min % 525949);
	monthmin100=525949-monthmin100;
	if (monthmin100<0) monthmin100=monthmin100+525949;
	if (monthmin100>=525949) monthmin100=monthmin100-525949;
	i=so24.so24month % 12 - 2;
	if (i==-2) i=10;
	if (i==-1) i=11;

	ret.ingi.name=i*2;
	ret.mingi.name=i*2+1;
	ret.outgi.name=i*2+2;


	unit_ymdhm.year=unityear;
	unit_ymdhm.month=unitmonth;
	unit_ymdhm.day=unitday;
	unit_ymdhm.hour=unithour;
	unit_ymdhm.min=unitmin;


	j=i*2;
	tmin=displ2min +  ( monthmin100 - montharray[j]);
	ret.ingi.date=getdatebymin(tmin,unit_ymdhm);

	ret.ingi.solar_latitude=get_solar_lat(ret.ingi.date);


	tmin=displ2min +  ( monthmin100 - montharray[j+1]);
	ret.mingi.date=getdatebymin(tmin,unit_ymdhm);

	ret.mingi.solar_latitude=get_solar_lat(ret.mingi.date);

	tmin=displ2min +  ( monthmin100 - montharray[j+2]);
	ret.outgi.date=getdatebymin(tmin,unit_ymdhm);

	ret.outgi.solar_latitude=get_solar_lat(ret.outgi.date);

	if (ret.outgi.name==24) ret.outgi.name=0;

	return ret;
}


lunar_month solortolunar(int solyear, int solmon, int solday)
{
	lunar_month ret;
	moon_info get_moon;
	so24term_month so24_info;
	so24term_time midgi2;

	int i;
	long long s0;

	get_moon=getlunarfirst(solyear,solmon,solday);

	ret.moon=get_moon;

	ret.lday=disp2days(solyear,solmon,solday,get_moon.moon_new.date.year,get_moon.moon_new.date.month,get_moon.moon_new.date.day) + 1;

	i=fabs(disp2days(get_moon.moon_new.date.year,get_moon.moon_new.date.month,get_moon.moon_new.date.day,get_moon.moon_next_new.date.year,get_moon.moon_next_new.date.month,get_moon.moon_next_new.date.day));

	ret.largemonth=false;
	if (i==30) ret.largemonth=true;

	so24_info=SolortoSo24(get_moon.moon_new.date);

	midgi2.name=so24_info.mingi.name+2;
	if (midgi2.name>24) midgi2.name=1;
	s0 = montharray[midgi2.name]-montharray[so24_info.mingi.name];
	if (s0 < 0) s0=s0 + 525949 ;
	s0 = -1 * s0 ;

	midgi2.date=getdatebymin(s0, so24_info.mingi.date);

	ret.yun_leaf=false;
	if (((so24_info.mingi.date.month==get_moon.moon_new.date.month)&&(so24_info.mingi.date.day>=get_moon.moon_new.date.day))||
		((so24_info.mingi.date.month==get_moon.moon_next_new.date.month)&&(so24_info.mingi.date.day<get_moon.moon_next_new.date.day)))
	{
		ret.lmonth=(so24_info.mingi.name-1)/2 + 1;
		ret.yun_leaf=false;
	}
	else
	{
		if (((midgi2.date.month==get_moon.moon_next_new.date.month)&&(midgi2.date.day<get_moon.moon_next_new.date.day))||
			((midgi2.date.month==get_moon.moon_new.date.month)&&(midgi2.date.day>=get_moon.moon_new.date.day)))
		{
			ret.lmonth=(midgi2.name-1)/2 + 1;
			ret.yun_leaf=false;
		}
		else
		{
			if ( (get_moon.moon_new.date.month<midgi2.date.month)&&(midgi2.date.month<get_moon.moon_next_new.date.month) )
			{
				ret.lmonth=(midgi2.name-1)/2 + 1;
				ret.yun_leaf=false;
			}
			else
			{
				ret.lmonth=(so24_info.mingi.name-1)/2 + 1;
				ret.yun_leaf=true;
			}
		}
	}


	ret.lyear=get_moon.moon_new.date.year;

	if ((ret.lmonth==12) && (get_moon.moon_new.date.month==1)) ret.lyear=ret.lyear-1;

	if (((ret.lmonth==11)&&ret.yun_leaf)||(ret.lmonth==12)||(ret.lmonth<6))
	{

		so24_info.mingi.date=getdatebymin(2880, get_moon.moon_new.date);

		lunar_month temp=solortolunar(so24_info.mingi.date.year,so24_info.mingi.date.month,so24_info.mingi.date.day);

		temp.lday=ret.lmonth-1;
		if (temp.lday==0) temp.lday=12;
		if (temp.lday==temp.lmonth)
		{
			if (ret.yun_leaf) ret.yun_leaf=false;
		}
		else
		{
			if (ret.yun_leaf)
			{
				if (ret.lmonth!=temp.lmonth)
				{
					ret.lmonth=ret.lmonth-1;
					if (ret.lmonth==0)
					{
						ret.lyear=ret.lyear-1;
						ret.lmonth=12;
						ret.yun_leaf=false;
					}
				}
			}
			else
			{
				if (ret.lmonth==temp.lmonth)
				{
					ret.yun_leaf=true;
				}
				else
				{
					ret.lyear=ret.lyear-1;
					ret.lmonth=12;
				}
			}

		}
	}
	return ret;
}

int getweekday(int syear, int smonth, int sday)
{
	int d,i;
	d=disp2days(syear,smonth,sday,unityear,unitmonth,unitday);
	i=d/7;
	d=d-(i*7);
	while( (d>6)||(d<0) )
	{
		if (d>6) d=d-7;
		else d=d+7;
	}
	if (d<0) d=d+7;
	return d;
}

int get28sday(int syear, int smonth, int sday)
{
	int d,i;
	d = disp2days(syear,smonth,sday,unityear,unitmonth,unitday);
	i= d / 28;
	d=d-(i*28);
	while( (d>27)||(d<0) )
	{
		if (d>27) d=d-28;
		else d=d+28;
	}
	d=d-11;
	if (d<0) d=d+28;

	return d;
}

int getjulianday(int syear, int smonth, int sday)
{
	int d;
	d = disp2days(syear,smonth,sday,1899,12,31);

	d =d+2415020;
	return d;
}


date_cal getjuliandate_from_julianday(double day)
{
	date_cal ret;
	date_cal unit_ymdhm;

	unit_ymdhm.year=unityear;
	unit_ymdhm.month=unitmonth;
	unit_ymdhm.day=unitday;
	//unit_ymdhm.day=5;

	//unit_ymdhm.hour=unithour;
	//unit_ymdhm.min=unitmin;
	unit_ymdhm.hour=12;
	//unit_ymdhm.hour=21;
	unit_ymdhm.min=0;


	if (day>=2299161)
	{
		long long tmin=(long long)-1*(day-2450118)*1440;
		ret=getdatebymin(tmin,unit_ymdhm);
	}
	else
	{
		long long tmin=(long long)-1*(day-2450118)*1440;
		ret=getdatebymin(tmin,unit_ymdhm);

		int b=0;
		int c=day+32082;
		int d=(4*c+3)/1461;
		int e=c - (1461*d)/4;
		int m=(5*e+2)/153;

		ret.day=e - (153*m+2)/5 + 1;
		ret.month=m + 3 - 12*(m/10);
		ret.year=b*100 + d - 4800 + m/10;

		if (ret.hour<12)
		{
			date_cal t1=getdatebymin(-1440,ret);
			ret.year=t1.year;
			ret.month=t1.month;
			ret.day=t1.day;

		}
	}
	return ret;
}

date_cal GetEasterDate(int year)
{

	/*
	 (* :=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=
	 *  This algorithm is an arithmetic interpretation
	 *  of the 3 step Easter Dating Method developed
	 *  by Ron Mallen 1985, as a vast improvement on
	 *  the method described in the Common Prayer Book

	 *  Published Australian Almanac 1988
	 *  Refer to this publication, or the Canberra Library
	 *  for a clear understanding of the method used

	 *  Because this algorithm is a direct translation of the
	 *  official tables, it can be easily proved to be 100%
	 *  correct

	 *  It's free!  Please do not modify code or comments!

	 *  11.7.99 - Pascal converting by Thomas Koehler, www.thkoehler.de

	 :=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=:=*)

	 */

	date_cal ret;
	int FirstDig, Remain19, temp,              //{intermediate results}
	tA, tB, tC, tD, tE;					//  {table A to E results}

	FirstDig = year / 100;          //  {first 2 digits of year}
	Remain19 = year % 19;           //  {remainder of year / 19}

	//{calculate PFM date}
	temp = (FirstDig - 15) / 2 + 202 - 11 * Remain19;

	if (FirstDig > 26) temp = temp - 1;
	if (FirstDig > 38) temp = temp - 1;
	if ((FirstDig == 21) || (FirstDig == 24) || (FirstDig == 25) || (FirstDig == 33) || (FirstDig == 36) || (FirstDig == 37)) temp = temp - 1;

	temp = temp % 30;

	tA = temp + 21;
	if (temp == 29) tA = tA - 1;
	if ((temp == 28) && (Remain19 > 10)) tA = tA - 1;

	//{find the next Sunday}
	tB = (tA - 19) % 7;

	temp = (40 - FirstDig) % 4;
	// {//tC := temp - (temp > 1) - (temp := 3)}
	tC = temp;
	if (temp > 1)  tC = tC + 1;
	if (temp == 3)  tC = tC + 1;

	temp = year % 100;
	tD = (temp + temp / 4) % 7;

	tE = ((20 - tB - tC - tD) % 7) + 1;
	ret.day = tA + tE;

	//{return the date}
	ret.month = 3;
	if ( ret.day > 61)
	{
		ret.day = ret.day - 61; // {when the original calculation is converted to the}
		ret.month = 5;      // {Gregorian calendar, Easter Sunday can occur in May}
	}
	if (ret.day > 31)
	{
		ret.day = ret.day - 31;
		ret.month = 4;
	}
	return ret;

}

char get_string_time_local_ret[100];
char* get_string_time_local(date_cal time,int time_off)
{
	date_cal date_temp;
	date_temp=getdatebymin((time_off+540), time);

	sprintf(get_string_time_local_ret,"%d-%02d-%02d %02d:%02d",date_temp.year,date_temp.month,date_temp.day,date_temp.hour,date_temp.min);
	return get_string_time_local_ret;
}

char get_string_time_ret[100];
char* get_string_time(date_cal time)
{
	sprintf(get_string_time_ret,"%d-%02d-%02d %02d:%02d",time.year,time.month,time.day,time.hour,time.min);
	return get_string_time_ret;
}


char* get_Planet_event_string(int event)
{
	if (planet_event_result[event]==en_sun_julgi)
	{
		int jul_no=-1;
		int jul;
		for(jul=0;jul<24;jul++)
		{
			double reframda=jul*15.0;
			double reframda2=reframda+2.0;

			if ((planet_event_result_position[event][0].RAMDA>reframda)&&(planet_event_result_position[event][0].RAMDA<reframda2))
			{
				jul_no=jul+3;
				if (jul_no>23) jul_no=jul_no-24;
				break;
			}
		}

		if (lang_mode==T_English) return monthst_eng[jul_no];
		else return monthst[jul_no];
	}

	if (lang_mode==T_English) return Planet_event_string_eng[planet_event_result[event]];
	else return Planet_event_string_kr[planet_event_result[event]];
}

char get_Planet_event_string_2nd_ret[100];
char* get_Planet_event_string_2nd(int event)
{
	if ((planet_event_result[event]==en_planet_conjunction)||(planet_event_result[event]==en_inferior_conjuinction)||(planet_event_result[event]==en_superior_conjunction)||(planet_event_result[event]==en_conjunction))
	{
		sprintf(get_Planet_event_string_2nd_ret, "%s=%2.3f%s",
				lang_mode==T_Korean ? Planet_event_string_2nd_kr[0] : Planet_event_string_2nd_eng[0] ,//황위차이
				planet_event_result_position[event][planet_event_result_planet1[event]].BETA - planet_event_result_position[event][planet_event_result_planet2[event]].BETA
				,char_degree
				);

		return get_Planet_event_string_2nd_ret;

	}

	if (planet_event_result[event]==en_great_elong_east)
	{
		sprintf(get_Planet_event_string_2nd_ret,"%s=%2.3f%s",
				lang_mode==T_Korean ? Planet_event_string_2nd_kr[1] : Planet_event_string_2nd_eng[1],//최대이각(황경차이)
				degreelow(planet_event_result_position[event][planet_event_result_planet1[event]].RAMDA - planet_event_result_position[event][planet_event_result_planet2[event]].RAMDA)
				,char_degree );

		return get_Planet_event_string_2nd_ret;
	}

	if (planet_event_result[event]==en_great_elong_west)
	{
		sprintf(get_Planet_event_string_2nd_ret,"%s=%2.3f%s",
				lang_mode==T_Korean ? Planet_event_string_2nd_kr[1] : Planet_event_string_2nd_eng[1],//최대이각(황경차이)
				degreelow(planet_event_result_position[event][planet_event_result_planet2[event]].RAMDA - planet_event_result_position[event][planet_event_result_planet1[event]].RAMDA)
				,char_degree  );

		return get_Planet_event_string_2nd_ret;
	}

	if (planet_event_result[event]==en_sun_julgi)
	{
		sprintf(get_Planet_event_string_2nd_ret,"%s=%2.3f%s",
				lang_mode==T_Korean ? Planet_event_string_2nd_kr[2] : Planet_event_string_2nd_eng[2],//"황경=%2.3f˚",
				planet_event_result_position[event][planet_event_result_planet1[event]].RAMDA,char_degree);

		return get_Planet_event_string_2nd_ret;
	}



	if ((planet_event_result[event]==en_perigee)||(planet_event_result[event]==en_apogee))
	{
		double d=planet_event_result_position[event][planet_event_result_planet1[event]].moon_distance_to_earth;
		int dist_rate=moon_distance_rate((int)d);

		sprintf(get_Planet_event_string_2nd_ret,"%5.2fkm(%d)", d, dist_rate);

		return get_Planet_event_string_2nd_ret;

	}


	if ((planet_event_result[event]==en_ascending_node)||(planet_event_result[event]==en_descending_node))
	{
		double moon_ramda=planet_event_result_position[event][planet_event_result_planet1[event]].RAMDA;

		sprintf(get_Planet_event_string_2nd_ret,"%s=%2.3f%s",
				lang_mode==T_Korean ? Planet_event_string_2nd_kr[2] : Planet_event_string_2nd_eng[2],//"황경=%2.3f˚",
				moon_ramda,char_degree);

		return get_Planet_event_string_2nd_ret;

	}


	if ((planet_event_result[event]==en_new_moon)||(planet_event_result[event]==en_full_moon))
	{
		double moon_ramda=planet_event_result_position[event][planet_event_result_planet1[event]].RAMDA;
		double noderamda=planet_event_result_position[event][planet_event_result_planet1[event]].moon_node;
		double d=planet_event_result_position[event][planet_event_result_planet1[event]].moon_distance_to_earth;;

		int dist_rate=moon_distance_rate((int)d);

		if (noderamda>180) noderamda-=360;

		int st_no=0;

		if (planet_event_result[event]==en_new_moon)
		{
			if (fabs(noderamda)<15.3) st_no=1;// " 日蝕 ";  // 日식 < 15.3
			else if (fabs(noderamda)<18.5) st_no=2;  //日식가능 < 18.5
		}
		else
		{
			if (fabs(noderamda)<9.5) st_no=3;// " 月蝕 ";  // < 9.5
			else if (fabs(noderamda)<12.2) st_no=4;  //月蝕가능 < 12.2
		}

		sprintf(get_Planet_event_string_2nd_ret," %5.2fkm(%d) %s=%3.3f%s %s=%3.3f%s %s",
				d,dist_rate,
				lang_mode==T_Korean ? Planet_event_string_2nd_kr[2] : Planet_event_string_2nd_eng[2],
				moon_ramda,char_degree,
				lang_mode==T_Korean ? Planet_event_string_2nd_kr[3] : Planet_event_string_2nd_eng[3],
				noderamda,char_degree,St_moon_eclipse_type_kor[st_no]
				);

		return get_Planet_event_string_2nd_ret;
	}

	if ((planet_event_result[event]==en_perihelion)||(planet_event_result[event]==en_Aphelion)||(planet_event_result[event]==en_sun_perihelion)||(planet_event_result[event]==en_sun_aphelion))
	{
		sprintf(get_Planet_event_string_2nd_ret," %s=%3.6fAU ",
				lang_mode==T_Korean ? Planet_event_string_2nd_kr[4] : Planet_event_string_2nd_eng[4],//거리
				planet_event_result_position[event][planet_event_result_planet1[event]].planet_radius );

		return get_Planet_event_string_2nd_ret;
	}

	return "";
}


int get_equation_of_time(date_cal time)
{
    double sun_ramda,sun_beta,earth_radius,earth_x,earth_y,earth_z,planet_true_anomaly,earth_ecl;
    date_cal test_time=time;

    long long tmin=getminbytime(test_time.year,test_time.month,test_time.day,test_time.hour,test_time.min,1999,12,31,9,0);
    double day=(double)tmin/1440;

    double N = degreelow(Planet_Orbit[0]->longitude_ascending_node_initial_degree + Planet_Orbit[0]->longitude_ascending_node_period*day);
    double w = degreelow(Planet_Orbit[0]->longitude_perihelion_initial_degree+Planet_Orbit[0]->longitude_perihelion_period*day);

    sun_position(test_time,&sun_beta,&sun_ramda,&earth_radius,&earth_x,&earth_y,&earth_z,&planet_true_anomaly,&earth_ecl);

    double ed_n=10*sin(2*degreelow(sun_ramda-N)*M_PI/180);
    double ed_w=-8*sin(degreelow(sun_ramda-w)*M_PI/180);

    return (int)(ed_n+ed_w);
}


