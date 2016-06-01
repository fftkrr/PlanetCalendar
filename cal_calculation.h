//
//  cal_calculation_value.h
//  CAL20000
//
//  Created by YOUNGCHANG KOH(fftkrr@gmail.com)
//
//

#ifndef cal_calculation_h
#define cal_calculation_h


double degreelow(double d);
double arctan2(double y, double x);
double get_mean_sun_lon(double day);


double get_sun_nearest_lon( double day);


double moonsundegree(double day, int mode);

double interpointdisp(double day);

double moon_distance_double(double day);


short disptimeday( int year, int month, int day);

int disp2days( int y1, int m1, int d1, int y2, int m2, int d2);




long long getminbytime_pre( int  uy, int  umm, int  ud, int  uh, int  umin,  int  y1,   int  mo1, int  d1, int  h1, int  mm1);



long long getminbytime( int uy, int umm, int ud, int uh, int umin,int y1, int mo1, int d1, int h1, int mm1);

void getdatebymin_pre( long long tmin,
                   int uyear,  int umonth, int uday, int uhour, int umin,
                   int *y1,  int *mo1, int *d1, int *h1, int *mi1);

date_cal getdatebymin(long long dmin,date_cal indate);

double get_mean_moon_lon( double day);


double get_moon_nearest_lon( double day);


double get_moon_node_lon( double day);


void moon_perigee_calc( double day,double *degree, double *correct_day);

double perigee_point( double day);



void moon_apogee_calc( double day, double *degree, double *correct_day);


double apogee_point( double day);
void moon_position(date_cal utime, double *moon_lat, double *moon_lon, double *ip, double *perigee, double *apogee, double *moon_distance);

void pr_planet_position(
		int Planet_number, date_cal utime,
        double *planet_lat, double *planet_lon, double *planet_radius,
          double *planet_x, double *planet_y, double *planet_z, double *planet_true_anomaly, double *earth_ecl,
          char **Name_Kor, char **Name_Han, char **Name_Eng,
           int *P_Color, int *loop_count
		);


int compare_time_equal(date_cal time1, date_cal time2);


void get_LamdaBeta_from_RADec(double RA, double Dec, double ecl, double *Ramda, double *Beta);

void get_RADec_from_RamdaBeta( double Ramda, double Beta, double ecl, double *RA, double *Dec);




void sort_event_result();
void put_event_to_result( int Type, int planet1, int planet2, Planet_Position_instance_record *position);






void sun_position(date_cal utime,
        double *earth_lat,double *earth_lon, double *earth_radius,
        double *earth_x, double *earth_y, double *earth_z,
        double *planet_true_anomaly,double *earth_ecl);


void calc_current_planet_position( date_cal current_time, double *moon_node, double *perigee, double *apogee, double *moon_distance);


void init_check_planet_event();

int check_planet_event( int planet_start_no);

extern date_cal calc_1, calc_0, calc_prev;
extern double moon_node[3],moon_perigee[3],moon_apogee[3],moon_distance[3];
extern Planet_Position_instance_record planet_position[9][3];
extern Planet_Position_instance_record calc_planet_position_calc[9];
extern int depth_count;
extern bool chk_stationary_and_great_elong;
extern bool chk_planet_events_not_stat_greatelong;
extern int event_sum;
extern double g_earth_ecl;
extern int event_result_count;
extern bool event_result_count_over;

#define devent_result_cap 3000
extern int planet_event_result[devent_result_cap];
extern int planet_event_result_planet1[devent_result_cap];
extern int planet_event_result_planet2[devent_result_cap];
extern Planet_Position_instance_record planet_event_result_position[devent_result_cap][9];
extern BOOL Planet_event_process[9];
extern BOOL Planet_event_process_event[14];
extern int internal_calc_planet_current;


extern struct Planet_info *Planet_Orbit[];

extern lunar_month solortolunar(int solyear, int solmon, int solday);
extern so24_8letter sydtoso24yd(int soloryear, int solormonth, int solorday, int solorhour, int solormin);

extern so24term_month SolortoSo24(date_cal afix);
extern date_cal GetEasterDate(int year);
extern int getweekday(int syear, int smonth, int sday);
extern int getjulianday(int syear, int smonth, int sday);
extern int get28sday(int syear, int smonth, int sday);

extern char get_string_time_ret[100];
extern char* get_string_time(date_cal time);
extern char* get_string_time_local(date_cal time,int time_off);
extern char* get_Planet_event_string(int event);
extern char* get_Planet_event_string_2nd(int event);

extern char *St_moon_eclipse_type_eng[];
extern char *St_moon_eclipse_type_kor[];

extern char *st_lunar_freeday_kr[];
extern char *st_lunar_freeday_eng[];

extern char *weekday_eng[];
extern char *weekday_kor[];
extern char *English_month_name[];

extern int event_result_count;

extern char *s28day[];
extern char *ganji[];

extern char *monthst[];
extern char *St_moon_remark_type[];
extern int moon_distance_rate(int distance);
extern int get_equation_of_time(date_cal time);
extern char *char_degree;
extern double getjuliandate_point(date_cal date);
extern date_cal getjuliandate_from_julianday(double day);
extern double getjuliandate_point_pre(int syear, int smonth, int sday, int shour, int smin);
extern int lang_mode;
extern char *monthst_eng[];

#endif
