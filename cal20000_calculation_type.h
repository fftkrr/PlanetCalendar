//
//  cal20000_calculation_value.h
//  CAL20000
//
//  Created by YOUNGCHANG KOH(fftkrr@gmail.com)
//
//



#ifndef CAL20000_cal20000_calculation_type_h
#define CAL20000_cal20000_calculation_type_h

typedef int bool;
typedef int BOOL;
#define false 0
#define FALSE 0
#define true 1
#define TRUE 1
#define prev_1 (2)

 typedef struct _datecal
 {
 	int year;
 	int month;
 	int day;
 	int hour;
 	int min;
 } date_cal;

 typedef struct _so24_8letter
 {
 	int so24;
 	int so24year;
 	int so24month;
 	int so24day;
 	int so24hour;
 } so24_8letter;

 typedef struct _date_interpol //date(합삭,망,교점,근원)시점의 교점과의 각거리(ip)와 지구와 달의 거리 및 거리 비율
 {
 	date_cal date;
 	double calc_d; //New_moon Julian_day
 	double moon_age;
 	double julian_day;
 	double ip;
 	//int distance; //km
     double distance; //km
 	int distance_rate; //0~100  최저 0(=356000km), 평균 50(=385000km), 최고 100(=406800km)
 	int remark;
 	//int index;  // fullmoon=0, 그다음은 시간순
 	int eclipse;  //0=없음, 1=일식/월식, 2 일식/월식 가능
 	double solor_lat;
 	double lunar_lat;
 	/*
 	public date_interpol()
 	{
 		remark=-1;
 		date=new date_cal();
 	}
 	*/

 } date_interpol;

 //class moon_info  //음력 한달의 정보
 typedef struct _moon_info
 {
 	int syear,smonth,sday;
 	date_interpol moon_new;  //합삭시점

 	date_interpol moon_1st_quarter;  //상현시점
 	date_interpol moon_full; //망시점
 	date_interpol moon_last_quarter;  //하현시점

 	date_interpol moon_junction_1; //교점
 	date_interpol moon_junction_2; //교점
 	date_interpol moon_junction_3; //교점

 	date_interpol moon_perigee_apogee_1; //근지점_원지점
 	date_interpol moon_perigee_apogee_2; //근지점_원지점
 	date_interpol moon_perigee_apogee_3; //근지점_원지점

 	date_interpol moon_jaki_1; //다음 합삭시점
 	date_interpol moon_jaki_2; //다음 합삭시점

 	date_interpol moon_next_new; //다음 합삭시점

 	date_interpol moon_datefix_array[13];
 	int moon_datefix_array_count;

 	int islamic_year,islamic_month;
     int jewish_year,jewish_month,jewish_day;
     int moon_month_length_min;
 } moon_info;

 typedef struct _lunar_month
 {
 	int lyear;
 	int lmonth;
 	int lday;
 	int yun_leaf;
 	int largemonth;
 	moon_info moon;
 } lunar_month;

 typedef struct _so24term_time
 {
 	int name;
 	date_cal date;
 	double solar_latitude;
 	/*
 	public so24term_time()
 	{
 		date=new date_cal();
 	}
 	*/
 } so24term_time;

 typedef struct _so24term_month
 {
 	so24term_time ingi;
 	so24term_time mingi;
 	so24term_time outgi;

 	int ku_year;
 	int ku_year_3won;
 	int ku_month;

 } so24term_month;

 typedef struct Planet_Position_instance_record
 {
     //int year,month,day,hour,min;// : integer;
     date_cal time;
     double planet_lon,
     planet_lat,
     planet_radius,
     planet_x,
     planet_y,
     planet_z,
     true_anomaly,
     planet_distance,
     //planet_alpha,planet_delta
     RA,DEC,
     RAMDA,BETA,//: extended;
     elongation,elongation_moon,//;// : extended;
     moon_node,moon_perigee,moon_apogee,moon_distance_to_earth;
     //lunar_month lmonth;
     int s60day;
 } Planet_Position_instance_record;

 typedef struct stconstell_name_record
 {
	 int number;
     char *NAME, *NAME_ENG, *NAME_KOR;
     double RA,DEC;  //center RA,DEC
 } stconstell_name_record;

 typedef struct stboundary_rec
 {
	 int number;
	 int name_number;
	 char *CON_NAME;
     double RA,DEC;  //center RA,DEC
     double RAp,DECp;  //center RA,DEC
 } stboundary_rec;

 typedef struct stSTAR_DATA
 {
     int number;


	    double RA;
	    double DEC;
	    double visual_mag;
	    double pmRA;
	    double pmDE;


     char *Name_Kor,*Name_China,*Name_Eng,*Name_Chine_Degree,*Name_Star_location,*Name_old;

     //double Magnitude_apparent,Magnitude_Absolute,Distance_light_year;
     //int     RA_Hour,RA_Min;
     //double RA_Sec;
     //int DE_Deg,DE_min,DE_sec,
     int Line_toNumber,Line2_toNumber;
     char *Name_Group;

     char *Constellation;
     char *Constellation_name;
     char *Symbol;
     int LineA,LineB;

     int su28_number,su28_order,su28_star_no,su28_star_total;


 } stSTAR_DATA;







//13.09.21 add
typedef struct Planet_info
{
    /*
	so24term_time ingi;
	so24term_time mingi;
	so24term_time outgi;

	int ku_year;
	int ku_year_3won;
	int ku_month;
    */
    int Number;// : integer;
    char *Name_Kor,// : string;
    *Name_Han,// : string;
    *Name_Eng;// : string;
    double longitude_ascending_node_initial_degree,// : extended;        //승교점에서 2000.Jan 0.0 UT(1999DEC31,0:00UT)까지의 거리(각도degree)
     longitude_ascending_node_period,// : extended;  //승교점으로부터 1일간 변화하는 변화량(각도_degree)
     inclination_initial_degree,// : extended;       //황도경사각 초기값 - 2000.Jan 0.0 UT(1999DEC31,0:00UT) 시점
     inclination_period,// : extended;               //황도경사각 1일간 변화하는 변화량
    longitude_perihelion_initial_degree,// : extended;  //근일점(태양과 행성의 최소거리위치)에서 2000.Jan 0.0 UT(1999DEC31,0:00UT)까지의 거리(각도degree)
    longitude_perihelion_period,// : extended;  //근일점에서 1일간 변화하는 변화량(각도_degree)
    mean_distance,// : extended;                //태양과 행성의 평균거리(단위:태양-지구 1 A.U 기준)
    mean_distance_variation,// : extended ;
    eccentricity_initial_degree,// : extended;  //이심율 초기값 -  2000.Jan 0.0 UT(1999DEC31,0:00UT) 시점(각도_degree), 0=circle, 0-1=ellipse, 1=parabola
    eccentricity_period,// : extended;          //이심율 1일간 변화량(각도_degree)
    mean_anomaly_initial_degree,// : extended;  //평균근점이각 초기값 - 2000.Jan 0.0 UT(1999DEC31,0:00UT) 시점의 평균 근점이각 (각도_degree), 근일점에서 0도
    mean_anomaly_period;// : extended;  // 1일간 평균근점이각 변화량(각도_degree)


    int color;//UIColor *Color;// : TColor ;
    //UIColor *color;
} Planet_info;


enum T_Moon
{
	T_Moon_Real=0,
	T_Moon_Junction=1,
	T_Moon_perigee=2,
	T_Moon_apogee=10,
	T_Moon_Junction_distance=3,
	T_Moon_perigee_correct=4,
	T_Moon_apogee_correct=5,
	T_Moon_distance=6,
	T_Moon_Jaki=7,
	T_Moon_Sol_lat=8,
	T_Moon_lunar_lat=9
};


enum
{
	T_Korean=0,
	T_English=1
};

enum T_Day_value
{
 TYPE_Solor_Year = 1,
 TYPE_Solor_Month = 2,

 TYPE_Lunar_Year = 3,
 TYPE_Lunar_Month = 4,
 TYPE_Lunar_Day = 5,

 TYPE_S60_Year = 6,
 TYPE_S60_Month = 7,
 TYPE_S60_Day = 8,

 TYPE_S_Month_Value = 9,
 TYPE_L_Month_Value = 10,

 TYPE_Weekday=11,

 TYPE_gabja_day_gu=12,
 TYPE_gusung=13,

 TYPE_kimoon_leap=14,
 TYPE_kimoon_julgi=15,
 TYPE_kimoon_3orig=16,  //0-상원, 1-중원, 2-하원
 TYPE_kimoon_plus_minus=17,  //0-음, 1-양
 TYPE_kimoon_kuk=18,

 TYPE_kimoon_julgi_pre_need=19,
 TYPE_kimoon_julgi_pre=20,
 TYPE_kimoon_plus_minus_pre=21,

 TYPE_28su_day=22,
 TYPE_Lunar_largemonth=23, //0-작은달, 1-큰달
 TYPE_Lunar_yun_leaf=24, //0-평달, 1-윤달
 TYPE_Julian_Date=25,
};

enum T_Moon_remark
{
	T_remark_date_interpol_moon_new=0,
	T_remark_date_interpol_moon_1st_quarter=1,
	T_remark_date_interpol_moon_full=2,
	T_remark_date_interpol_moon_last_quarter=3,
	T_remark_date_interpol_moon_next_new=4,
	T_remark_date_interpol_moon_junction_ascending=5,
	T_remark_date_interpol_moon_junction_descending=6,
	T_remark_date_interpol_moon_perigee=7,
	T_remark_date_interpol_moon_apogee=8,
	T_remark_date_interpol_moon_jaki=9
};

enum e_planet_event
{
    en_nothing=0,
    en_new_moon = 1,
    en_full_moon,
    en_ascending_node,
    en_descending_node,
    en_perigee,
    en_apogee,
    en_first_quarter,
    en_last_quarter,
    en_stationary_west_to_east,
    en_stationary_east_to_west,
    en_inferior_conjuinction,
    en_superior_conjunction,
    en_conjunction,
    en_opposition,
    en_great_elong_east,
    en_great_elong_west,
    en_right_east,
    en_right_west,
    en_planet_conjunction,
    en_moon_conjunction=20,
    en_moon_opposition,
    en_sun_julgi,
    en_perihelion,
    en_Aphelion,
    en_ascecliptic,
    en_desecliptic,
    en_sun_perihelion,
    en_sun_aphelion
};



enum e_planet_event_checkbox
{
    en_ck_conjunction_superior_new_moon = 0,
    en_ck_opposition_inferior_full_moon,
    en_ck_right_east_first_quarter,
    en_ck_right_west_last_quarter,
    en_ck_great_elong_east,
    en_ck_great_elong_west,
    en_ck_stationary_east_to_west,
    en_ck_stationary_west_to_east,
    en_ck_ascending_node,
    en_ck_descending_node,
    en_ck_perigee_Perihelion,
    en_ck_apogee_Aphelion,
    en_ck_planet_conjunction,
    en_ck_sun_julgi,
};

enum en_chk_event
{
  en_chk_event_perigee_apogee=1,
  en_chk_event_First_Q_LAST_Q_planet_no_1=2,
  en_chk_event_conjunction_opposition_right_east_west=4,
  en_chk_event_planet_conjunction=8,
  en_chk_event_stationary_or_great_elong=16,
  en_chk_event_Node=32,
  en_chk_event_sun_julgi=64,
  en_chk_event_sun_helion=128
};



#endif


