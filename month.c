//
//  cal20000_calculation_value.h
//  CAL20000
//
//  Created by YOUNGCHANG KOH(fftkrr@gmail.com)
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "cal20000_calculation_type.h"
#include "cal_calculation.h"

int year,month,timez_off,lang;
int day_value[31][26];
so24term_month smonth_info[2];
moon_info lmonth_info[3];

date_cal cur_time, cur_time_tz;
int cur_time_sec;


void check_planet_event_month(int wbyear, int wbmonth, int time_offset_chkevent)
{
	int i,j;

	event_result_count_over=false;
	event_result_count=0;

	for(i=0;i<14;i++)
	{
		Planet_event_process_event[i]=true;;
	}

	for(i=1;i<9;i++)
	{
		Planet_event_process[i]=true;
	}

	date_cal start_date,end_date,date_temp,current_time;

	date_temp.year=wbyear;
	date_temp.month=wbmonth;
	date_temp.day=1;
	date_temp.hour=0;
	date_temp.min=0;


	start_date=getdatebymin( -540-time_offset_chkevent, date_temp);

	date_temp.year=wbyear;
	date_temp.month=wbmonth+1;
	if (date_temp.month==13)
	{
		date_temp.year=wbyear+1;
		date_temp.month=1;
	}
	date_temp.day=1;
	date_temp.hour=0;
	date_temp.min=0;

	end_date=getdatebymin( -540-time_offset_chkevent, date_temp);

	long long local_duration=getminbytime(end_date.year,end_date.month,end_date.day,end_date.hour,end_date.min,
			                start_date.year,start_date.month,start_date.day,start_date.hour,start_date.min
			             )-1;

	int tmin_add;
	int remain_duration=(int)local_duration+1;



	int tmin;
	for(tmin=0;tmin<(local_duration+1);)
	{

		current_time=getdatebymin(-1*tmin, start_date);

		calc_current_planet_position( current_time, &moon_node[1],&moon_perigee[1],&moon_apogee[1],&moon_distance[1]);
		for(j=0;j<9;j++)
			planet_position[j][1]=calc_planet_position_calc[j];


		if (tmin==0)
		{
			for(j=0;j<9;j++)
			{
				planet_position[j][0]=calc_planet_position_calc[j];
				planet_position[j][prev_1]=calc_planet_position_calc[j];
				if (j==1)
				{
					moon_node[0]=moon_node[1];
					moon_perigee[0]=moon_perigee[1];
					moon_apogee[0]=moon_apogee[1];
					moon_distance[0]=moon_distance[1];
				}
			}

		}
		else
		{
			depth_count=0;
			chk_stationary_and_great_elong=true;
			chk_planet_events_not_stat_greatelong=true;
			check_planet_event(0);
		}


		for(j=0;j<9;j++)
		{
			planet_position[j][prev_1]=planet_position[j][0];
			planet_position[j][0]=planet_position[j][1];

			if (j==1)
			{
				moon_node[0]=moon_node[1];
				moon_perigee[0]=moon_perigee[1];
				moon_apogee[0]=moon_apogee[1];
				moon_distance[0]=moon_distance[1];
			}
		}


		if (remain_duration>1024) tmin_add=1024;
		else
		if (remain_duration>512) tmin_add=512;
		else if (remain_duration>256) tmin_add=256;
		else if (remain_duration>128) tmin_add=128;
		else if (remain_duration>64) tmin_add=64;
		else if (remain_duration>32) tmin_add=32;
		else if (remain_duration>16) tmin_add=16;
		else if (remain_duration>8) tmin_add=8;
		else if (remain_duration>4) tmin_add=4;
		else tmin_add=1;


		tmin+=tmin_add;
		remain_duration-=tmin_add;


	} // for(int tmin=0;tmin<(local_duration+1);)


}

void print_input_form()
{
	int i;
	printf("<p>\n");
	printf("<form action=month.cgi method=GET>\n");
	if (lang==T_English)
	{
		printf("Year:<input type=text Name=year size=6 value=%d>\n",year);
		printf(" Month:<input type=text Name=month size=3 value=%d>\n",month);
		printf(" TimeZone:<select name=\"timez_off\">");
	}
	else
	{
		printf("<input type=text Name=year size=6 value=%d>년\n",year);
		printf("<input type=text Name=month size=3 value=%d>월\n",month);
		printf(" 표시시간대:<select name=\"timez_off\">");
	}


	for(i=-26;i<26;i=i+1)
	{

		printf("<option value=%d",-i*30);
		if (-i*30==timez_off) printf(" selected ");
		printf(">GMT%+2.1fH</option>",(float)i/2.0);
	}
	printf("</select>\n");

	printf(" Language:<select name=\"lang\">");
	if (lang==T_English)
	{
		printf("<option value='kor'>Korean</option>");
		printf("<option value='eng' selected>English</option>");
		printf("</select>\n");
	}
	else
	{
		printf("<option value='kor' selected>Korean</option>");
		printf("<option value='eng' >English</option>");
		printf("</select>\n");
	}

	if (lang==T_English)
	{
		printf("<input type=SUBMIT value=GO>\n");
		printf("<br>Effective Year -9999~9999년\n");
	}
	else
	{
		printf("<input type=SUBMIT value=보기>\n");
		printf("<br>유효기간  -9999~9999년\n");
	}
	printf("</form>\n");


}


void print_head()
{
	printf("<HTML>\n");
	printf("<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=UTF-8\">\n");
	printf("<HEAD><TITLE>CAL20000 V2.6 %d-%d </TITLE>\n",year,month);
	printf("</HEAD>\n");
}

void print_body_first()
{
	int pyear,pmonth,nmonth,nyear;
	printf("<BODY>\n");

	pmonth=month-1;
	pyear=year;
	if (pmonth==0)
	{
		pmonth=12;
		pyear=pyear-1;
		if (pyear==-10000)
		{
			pyear=-9999;
			pmonth=1;
		}
	}
	nmonth=month+1;
	nyear=year;
	if (nmonth==13)
	{
		nmonth=1;
		nyear=nyear+1;
		if (nyear==10001)
		{
			nyear=10000;
			nmonth=12;
		}
	}

	if (lang==T_English)
		printf("<font size=7>CAL20000</font>");
	else
		printf("<font size=7>진짜만세력</font>");
  
	printf("<table border=0>\n");

	int less_year=year-1;
	if (less_year==-10000) less_year=-9999;
	int more_year=year+1;
	if (more_year==10000) less_year=9999;

	if (lang==T_English)
	{
		printf("<tr><td><a href=month.cgi?year=%d&month=%d&timez_off=%d&lang=%d>%d-%d</a>\n",less_year,month,timez_off,lang,less_year,month);
		printf("<br><a href=month.cgi?year=%d&month=%d&timez_off=%d&lang=%d>%d-%d</a></td>",pyear,pmonth,timez_off,lang,pyear,pmonth);
		printf("<td><font size=7>%d-%d</font></td>",year,month);
		printf("<td><a href=month.cgi?year=%d&month=%d&timez_off=%d&lang=%d>%d-%d</a>",nyear,nmonth,timez_off,lang,nyear,nmonth);
		printf("<br><a href=month.cgi?year=%d&month=%d&timez_off=%d&lang=%d>%d-%d</a></td></tr>\n",more_year,month,timez_off,lang,more_year,month);
	}
	else
	{
		printf("<tr><td><a href=month.cgi?year=%d&month=%d&timez_off=%d&lang=%d>%d년%d월</a>",less_year,month,timez_off,lang,less_year,month);
		printf("<br><a href=month.cgi?year=%d&month=%d&timez_off=%d&lang=%d>%d년%d월</a></td>",pyear,pmonth,timez_off,lang,pyear,pmonth);
		printf("<td><font size=7>%d년 %d월</font></td>",year,month);
		printf("<td><a href=month.cgi?year=%d&month=%d&timez_off=%d&lang=%d>%d년%d월</a>",nyear,nmonth,timez_off,lang,nyear,nmonth);
		printf("<br><a href=month.cgi?year=%d&month=%d&timez_off=%d&lang=%d>%d년%d월</a></td></tr>\n",more_year,month,timez_off,lang,more_year,month);
	}



	printf("</table><p>\n");

	print_input_form();
}



void print_body_last()
{
	if (lang==T_English)		
	{
			printf("<P> Current Time : %04d-%02d-%02d %02d:%02d:%02d GMT%+2.1fH<P>",
              cur_time_tz.year,cur_time_tz.month,cur_time_tz.day,cur_time_tz.hour,cur_time_tz.min,cur_time_sec,(double)-timez_off/60.0);			
	}
	else				
	{
		printf("<P> 현재시간 : %04d-%02d-%02d %02d:%02d:%02d GMT%+2.1fH<P>",
              cur_time_tz.year,cur_time_tz.month,cur_time_tz.day,cur_time_tz.hour,cur_time_tz.min,cur_time_sec,(double)-timez_off/60.0);
	}

	printf("<p>\n");
	printf("</BODY>\n");

}


void print_month_table()
{

	date_cal test_date1,test_date2;

	lunar_month lunar_date;

	int month_lastday;
	int easter_month,easter_day;

	lunar_date=solortolunar(2014,6,27);

    int next_year=year;
    int next_month=month+1;
    if (next_month==13)
    {
    	next_month=1;
    	next_year=next_year+1;
    }

    test_date1.year=next_year;
    test_date1.month=next_month;
    test_date1.day=1;
    test_date1.hour=0;
    test_date1.min=0;

    test_date2=getdatebymin(200,test_date1);

    month_lastday=test_date2.day;

    lunar_date=solortolunar(year,month,1);

    int lmv=0;
    lmonth_info[lmv]=lunar_date.moon;

    test_date1.year=year;
    test_date1.month=month-1;
	if ((month-1)==0)
	{
		test_date1.year=year-1;
		test_date1.month=12;
	}
	test_date1.day=15;
	test_date1.hour=0;
	test_date1.min=0;

	smonth_info[0]=SolortoSo24(test_date1);  //양력1일날의 절기

    test_date1.year=year;
    test_date1.month=month;
    test_date1.day=15;
    test_date1.hour=0;
    test_date1.min=0;
	smonth_info[1]=SolortoSo24(test_date1);  //양력15일날의 절기

    //부활절 계산
	easter_day=0;
	easter_month=0;
	if (year>0)
	{
		test_date1=GetEasterDate(year);

		easter_day=test_date1.day;
		easter_month=test_date1.month;
	}

	int lday=lmonth_info[lmv].moon_next_new.date.day;
	int ingi_day=smonth_info[0].outgi.date.day;

	int dd;
	for(dd=0;dd<month_lastday;dd++)
	{
		int day=dd+1;

		if (day==1)
		{

			day_value[dd][TYPE_Solor_Year]=year; //lunar_info
			day_value[dd][TYPE_Solor_Month]=month; //lunar_info

			day_value[dd][TYPE_Lunar_Year]=lunar_date.lyear; //lunar_info
			day_value[dd][TYPE_Lunar_Month]=lunar_date.lmonth; //lunar_info
			day_value[dd][TYPE_Lunar_Day]=lunar_date.lday; //so24term_month
			if (lunar_date.largemonth) day_value[dd][TYPE_Lunar_largemonth]=1;
			else day_value[dd][TYPE_Lunar_largemonth]=0;
			if (lunar_date.yun_leaf) day_value[dd][TYPE_Lunar_yun_leaf]=1;
			else day_value[dd][TYPE_Lunar_yun_leaf]=0;

			so24_8letter so24_8letter;
			so24_8letter=sydtoso24yd(year,month,1,1,0);

			day_value[dd][TYPE_S60_Year]=so24_8letter.so24year;
			day_value[dd][TYPE_S60_Month]=so24_8letter.so24month;
			day_value[dd][TYPE_S60_Day]=so24_8letter.so24day;

			day_value[dd][TYPE_S_Month_Value]=0;
			day_value[dd][TYPE_L_Month_Value]=lmv;

			day_value[dd][TYPE_Weekday]=getweekday(year,month,1);

			day_value[dd][TYPE_28su_day]=get28sday(year,month,1) ;

			day_value[dd][TYPE_Julian_Date]=getjulianday(year,month,1);


			lday=lmonth_info[lmv].moon_next_new.date.day;
			ingi_day=smonth_info[0].outgi.date.day;
		}
		else
		{
			day_value[dd][TYPE_Weekday]=day_value[dd-1][TYPE_Weekday]+1;
			if (day_value[dd][TYPE_Weekday]==7) day_value[dd][TYPE_Weekday]=0;

			day_value[dd][TYPE_28su_day]=day_value[dd-1][TYPE_28su_day]+1;
			if (day_value[dd][TYPE_28su_day]==28) day_value[dd][TYPE_28su_day]=0;

			day_value[dd][TYPE_Julian_Date]=day_value[dd-1][TYPE_Julian_Date]+1;

			if (day==lday)
			{

				lunar_date=solortolunar(year,month,day);
				lmv=lmv+1;

				lmonth_info[lmv]=lunar_date.moon;


				day_value[dd][TYPE_Lunar_Year]=lunar_date.lyear;
				day_value[dd][TYPE_Lunar_Month]=lunar_date.lmonth; //lunar_info
				day_value[dd][TYPE_Lunar_Day]=lunar_date.lday; //so24term_month

				if (lunar_date.largemonth) day_value[dd][TYPE_Lunar_largemonth]=1;
				else day_value[dd][TYPE_Lunar_largemonth]=0;
				if (lunar_date.yun_leaf) day_value[dd][TYPE_Lunar_yun_leaf]=1;
				else day_value[dd][TYPE_Lunar_yun_leaf]=0;

				day_value[dd][TYPE_L_Month_Value]=lmv;
				lday=lmonth_info[lmv].moon_next_new.date.day;

			}
			else
			{
				day_value[dd][TYPE_Lunar_Year]=day_value[dd-1][TYPE_Lunar_Year];
				day_value[dd][TYPE_Lunar_Month]=day_value[dd-1][TYPE_Lunar_Month];
				day_value[dd][TYPE_Lunar_Day]=day_value[dd-1][TYPE_Lunar_Day]+1;

				day_value[dd][TYPE_Lunar_largemonth]=day_value[dd-1][TYPE_Lunar_largemonth];
				day_value[dd][TYPE_Lunar_yun_leaf]=day_value[dd-1][TYPE_Lunar_yun_leaf];


				day_value[dd][TYPE_L_Month_Value]=day_value[dd-1][TYPE_L_Month_Value];
			}

			if (day==ingi_day)
			{

				day_value[dd][TYPE_S_Month_Value]=1;

				if (smonth_info[1].ingi.name==0)
				{
					day_value[dd][TYPE_S60_Year]=day_value[dd-1][TYPE_S60_Year]+1;
					if (day_value[dd][TYPE_S60_Year]==60) day_value[dd][TYPE_S60_Year]=0;
				}
				else
				{
					day_value[dd][TYPE_S60_Year]=day_value[dd-1][TYPE_S60_Year];
				}

				day_value[dd][TYPE_S60_Month]=day_value[dd-1][TYPE_S60_Month]+1;
				if (day_value[dd][TYPE_S60_Month]==60) day_value[dd][TYPE_S60_Month]=0;

				day_value[dd][TYPE_S60_Day]=day_value[dd-1][TYPE_S60_Day]+1;
				if (day_value[dd][TYPE_S60_Day]==60) day_value[dd][TYPE_S60_Day]=0;

			}
			else
			{
				day_value[dd][TYPE_S60_Year]=day_value[dd-1][TYPE_S60_Year];
				if (day_value[dd][TYPE_S60_Year]==60) day_value[dd][TYPE_S60_Year]=0;

				day_value[dd][TYPE_S60_Month]=day_value[dd-1][TYPE_S60_Month];
				if (day_value[dd][TYPE_S60_Month]==60) day_value[dd][TYPE_S60_Month]=0;

				day_value[dd][TYPE_S60_Day]=day_value[dd-1][TYPE_S60_Day]+1;
				if (day_value[dd][TYPE_S60_Day]==60) day_value[dd][TYPE_S60_Day]=0;

				day_value[dd][TYPE_S_Month_Value]=day_value[dd-1][TYPE_S_Month_Value];
			}


		}
	}


	check_planet_event_month(year,month,timez_off);


	so24_8letter mid_gi_8=sydtoso24yd(smonth_info[1].mingi.date.year,smonth_info[1].mingi.date.month,smonth_info[1].mingi.date.day,smonth_info[1].mingi.date.hour,smonth_info[1].mingi.date.min);


	printf("<table border=1>\n");

	printf("<tr>\n");
	bool date_set=false;

	int day=0;
	int cell;
	for(cell=0;cell<7;cell++)
	{
		printf("<td>");

		if (cell==0)
			printf("<font color=red>");
		else if (cell==6)
			printf("<font color=blue>");
		else
			printf("<font color=black>");

		if (lang==T_English)
			printf("%s",weekday_eng[cell]);
		else
			printf("%s",weekday_kor[cell]);
		printf("</font>");
		printf("</td>");
	}
	printf("</tr>\n");

	int line;
	for(line=1;line<7;line++)
	{
		if ((line==6)&&(date_set==false)) break;
		printf("<tr>\n");
		for (cell=0;cell<7;cell++)
		{

			printf("<td  style=text-align:left;vertical-align:top;padding:0 >\n");

			if ((day==0)&&(day_value[0][TYPE_Weekday]==cell))
			{
				date_set=true;
			}

			if (((line==1)&&(cell==0)&&(date_set==false))||((date_set==false)&&((line==5)||(line==6))&&(cell==6)))
			{

				printf("<font size=5 color=blue>%d",year);
				if (lang==T_English)
				{
					printf("<br>%s<br>%s년<br>%s월",English_month_name[month-1],ganji[mid_gi_8.so24year],ganji[mid_gi_8.so24month]);
				}
				else
				{
					printf("년<br>%d월",month);
					printf("<br>%s년<br>%s월",ganji[mid_gi_8.so24year],ganji[mid_gi_8.so24month]);
				}

				printf("</font>");
			}


			if (date_set)
			{
				day++;

				printf("<font size=\"2\">");
				printf("%s", s28day[day_value[day-1][TYPE_28su_day]]);
				printf("</font>");

				int lunar_hol=-1;

				if ((day_value[day-1][TYPE_Lunar_yun_leaf]==0)
					&&(day_value[day-1][TYPE_Lunar_Month]==1)
					&&(day_value[day-1][TYPE_Lunar_Day]==1)
				   )
				{
					lunar_hol=0;
				}

				if ((day_value[day-1][TYPE_Lunar_yun_leaf]==0)
						&&(day_value[day-1][TYPE_Lunar_Month]==4)
						&&(day_value[day-1][TYPE_Lunar_Day]==8)
					   )
					{
						if (year>-544)
							lunar_hol=1;
					}

				if ((day_value[day-1][TYPE_Lunar_yun_leaf]==0)
						&&(day_value[day-1][TYPE_Lunar_Month]==8)
						&&(day_value[day-1][TYPE_Lunar_Day]==15)
					   )
					{
						lunar_hol=2;
					}

				if ((day==easter_day)&&(month==easter_month))
				{
					if (year>35)
						lunar_hol=3;
				}


				if ((cell==0)||lunar_hol!=-1)
					printf("<font size=6 color=red>");
				else if (cell==6)
					printf("<font size=6 color=blue>");
				else
					printf("<font size=6 color=black>");

				printf("%d", day);
				printf("</font><br>");


				if ((lunar_hol!=-1)&&!((day==easter_day)&&(month==easter_month)))
					printf("<font color=red>");
				else
					printf("<font color=blue>");

				if (lunar_hol==1)
				{
					printf("4.8");
				}
				else
				{
					printf("%d.%d", day_value[day-1][TYPE_Lunar_Month],day_value[day-1][TYPE_Lunar_Day]);
				}
				printf("</font>");


				printf("<font color=black>");

				printf("%s", ganji[day_value[day-1][TYPE_S60_Day]] );
				printf("</font>");

				printf("<br>");

				if (lunar_hol!=-1)
				{
					if (lang==T_English)
						printf("<font color=red>%s</font><br>",st_lunar_freeday_eng[lunar_hol]);
					else
						printf("<font color=red>%s</font><br>",st_lunar_freeday_kr[lunar_hol]);
				}


				int i;

				for (i=0;i<event_result_count;i++)
				{
					date_cal date_temp;

					date_temp=getdatebymin((timez_off+540),planet_event_result_position[i][0].time);

					if ((day==date_temp.day)&&(month==date_temp.month))
					{

						printf("<p><font color=blue size=\"2\">%02d:%02d %s<br>",
									 date_temp.hour,date_temp.min,get_Planet_event_string(i));


						if (lang==T_English)
						{
							printf("%s ",Planet_Orbit[planet_event_result_planet1[i]]->Name_Eng);
						}
						else
						{
							printf("%s ",Planet_Orbit[planet_event_result_planet1[i]]->Name_Kor);
						}

						if (planet_event_result[i]==en_sun_julgi)
						{
				        	if (lang==T_English)
				        		printf(" λ=%2.3f˚", planet_event_result_position[i][planet_event_result_planet1[i]].RAMDA);
				        	else
				        		printf("<br>황경=%2.3f˚", planet_event_result_position[i][planet_event_result_planet1[i]].RAMDA);

						}



				        if (planet_event_result[i]==en_great_elong_east)
				        {
				        	printf("<br>%2.3f˚",
				        			degreelow(planet_event_result_position[i][planet_event_result_planet1[i]].RAMDA - planet_event_result_position[i][planet_event_result_planet2[i]].RAMDA)
				        			);
				        }

				        if (planet_event_result[i]==en_great_elong_west)
				        {
				        	printf("<br>%2.3f˚",
				        			degreelow(planet_event_result_position[i][planet_event_result_planet2[i]].RAMDA - planet_event_result_position[i][planet_event_result_planet1[i]].RAMDA)
				        			);
				        }

						if (planet_event_result[i]==en_planet_conjunction)
						{
							if (lang==T_English)
							{
								printf("%s",Planet_Orbit[planet_event_result_planet2[i]]->Name_Eng);
							}
							else
							{
								printf("%s",Planet_Orbit[planet_event_result_planet2[i]]->Name_Kor);
							}
						}

				        if (
				        		(planet_event_result[i]==en_planet_conjunction)
				        		||(planet_event_result[i]==en_inferior_conjuinction)
				        		||(planet_event_result[i]==en_superior_conjunction)
				        		||(planet_event_result[i]==en_conjunction)
				        	)
				        {
							if (lang==T_English)
							{
								printf(" dβ=");
							}
							else
							{
								printf("<br>황위차이=");
							}

			        		printf("%2.3f˚",
			        				planet_event_result_position[i][planet_event_result_planet1[i]].BETA - planet_event_result_position[i][planet_event_result_planet2[i]].BETA
			            			);
				        }

						printf("</font>");


					}
				}
				double jd=getjuliandate_point_pre(year,month,day,21,0);

				if (jd<2299161)
				{
					date_cal jddate=getjuliandate_from_julianday(jd);
					printf("<br><font size=\"2\">%d-%d<br></font>",jddate.month,jddate.day);
				}


				if (day==month_lastday) date_set=false;
			}



			printf("</td>\n");
		}
		printf("</tr>\n");
	}

	printf("</table><P>\n");

}


void print_month_info()
{
	int i;
	if (lang==T_English)
	{
		printf("This month enter term=%s %s<br>\n",monthst_eng[smonth_info[1].ingi.name] ,get_string_time_local(smonth_info[1].ingi.date,timez_off));
		printf("This month mid term=%s %s<P>\n",monthst_eng[smonth_info[1].mingi.name] ,get_string_time_local(smonth_info[1].mingi.date,timez_off));
	}
	else
	{
		printf("이번달 절입=%s %s<br>\n",monthst[smonth_info[1].ingi.name] ,get_string_time_local(smonth_info[1].ingi.date,timez_off));
		printf("이번달 중기=%s %s<P>\n",monthst[smonth_info[1].mingi.name] ,get_string_time_local(smonth_info[1].mingi.date,timez_off));
	}
	printf("<P>");


	printf("<P>");
	if (lang==T_English)
	{
		printf("Moon Orbit Information\n<table border=1>\n");
	}
	else
	{
		printf("달 궤도 정보\n<table border=1>\n");
	}

	for (i=0;i<event_result_count;i++)
	{
		if (planet_event_result_planet1[i]==1) {


			printf("<tr>\n");

			printf("<td>%s</td><td> %s</td><td> JD=%7.3f</td><td> %s</td><td> %s",
					 get_Planet_event_string(i),get_string_time_local(planet_event_result_position[i][0].time,timez_off ),
					 getjuliandate_point(planet_event_result_position[i][0].time),
					 lang==T_Korean ? Planet_Orbit[planet_event_result_planet1[i]]->Name_Kor:Planet_Orbit[planet_event_result_planet1[i]]->Name_Eng,
					 get_Planet_event_string_2nd(i)
			);

			if ((planet_event_result[i]==en_first_quarter)||(planet_event_result[i]==en_last_quarter))
			{
				double moon_lambda=planet_event_result_position[i][planet_event_result_planet1[i]].RAMDA;
				double sun_lambda=planet_event_result_position[i][0].RAMDA;
				if (lang==T_English)
					printf("Moon λ=%3.2f%s, Sun λ=%3.2f%s, dλ=%3.2f%s",moon_lambda,char_degree,sun_lambda,char_degree,degreelow(moon_lambda-sun_lambda),char_degree);
				else
					printf("달황경=%3.2f%s, 해황경=%3.2f%s, 황경차=%3.2f%s",moon_lambda,char_degree,sun_lambda,char_degree,degreelow(moon_lambda-sun_lambda),char_degree);
			}

			printf("</td></tr>\n");

		}

	}
	printf("</table>\n");


	printf("<P>");
	if (lang==T_English)
	{
		printf("Planets Events Information\n<table border=1>\n");
	}
	else
	{
		printf("행성 천문현상<table border=1>\n");
	}

	for (i=0;i<event_result_count;i++)
	{


			if (planet_event_result_planet1[i]>1)
			{
				printf("<tr>\n");
				printf("<td>%s</td><td> %s</td><td> JD=%7.3f</td><td> %s</td><td> %s</td><td> %s</td>\n",
					 get_Planet_event_string(i),get_string_time_local(planet_event_result_position[i][0].time,timez_off ),
					 getjuliandate_point(planet_event_result_position[i][0].time),
					 lang==T_Korean ? Planet_Orbit[planet_event_result_planet1[i]]->Name_Kor:Planet_Orbit[planet_event_result_planet1[i]]->Name_Eng,
					 lang==T_Korean ? Planet_Orbit[planet_event_result_planet2[i]]->Name_Kor:Planet_Orbit[planet_event_result_planet2[i]]->Name_Eng,
					 get_Planet_event_string_2nd(i)
				);
				printf("</tr>\n");
			}


	}
	printf("</table>\n");
	if (lang==T_English)
	{
		printf("λ-celestial longitude, β-celestial latitude, dλ/dβ-celestial difference degree between two object.\n");
	}

}

void print_month_table_test()
{

	int year_s,month_s;
	int count=0;
	date_cal test_date1, test_date2,test_date3;
	int i;
	int dmin;

	for(year_s=1900;year_s<2100;year_s++)
	{
		for(month_s=1;month_s<13;month_s++)
		{
			test_date1.year=year_s;
			test_date1.month=month_s;
			test_date1.day=15;
			test_date1.hour=0;
			test_date1.min=0;
			smonth_info[1]=SolortoSo24(test_date1);  //양력15일날의 절기
			test_date2=smonth_info[1].mingi.date;

			check_planet_event_month(year_s,month_s,timez_off);

			printf("%d 기존 중기 시각 %d-%02d-%02d %02d:%02d ",++count,test_date2.year,test_date2.month,test_date2.day,test_date2.hour,test_date2.min);

			for (i=0;i<event_result_count;i++)
			{
					int eq_time=get_equation_of_time(test_date2);
					test_date3= getdatebymin(eq_time,planet_event_result_position[i][0].time);

					if ((planet_event_result[i]==en_sun_julgi)&&(planet_event_result_position[i][0].time.day>10))
					{
						dmin=getminbytime(test_date2.year,test_date2.month,test_date2.day,test_date2.hour,test_date2.min,
								test_date3.year,test_date3.month,test_date3.day,test_date3.hour,test_date3.min
										);


							printf("새중기 %s %s %s, %s %s,dmin=,%d, eqtime=,%d",
							 get_Planet_event_string(i),get_string_time_local(test_date3,timez_off ),
							 Planet_Orbit[planet_event_result_planet1[i]]->Name_Kor,
							 Planet_Orbit[planet_event_result_planet2[i]]->Name_Kor,
							 get_Planet_event_string_2nd(i),dmin,eq_time
								);

					}

			}
			printf("<br>\n");

		}
	}
}


int main(int argc, char **argv, char **env)
{
	printf("Content-type: text/html\n\n");

	//default value
	year=2016;
	month=5;
	timez_off=-540;
	//lang=T_Korean;
	lang=T_English;

	
	//system time
	time_t ltime;
	struct tm *today,*gmt_time;
	int tzoffset;

	time(&ltime);
	today=localtime(&ltime);

	year=today->tm_year+1900;
	month=today->tm_mon+1;

	cur_time.year=today->tm_year + 1900;
	cur_time.month=today->tm_mon + 1;
	cur_time.day=today->tm_mday;
	cur_time.hour=today->tm_hour;
	cur_time.min=today->tm_min;
	cur_time_sec=today->tm_sec;
	  

	gmt_time=gmtime(&ltime);

	timez_off=getminbytime(gmt_time->tm_year+1900,gmt_time->tm_mon+1,gmt_time->tm_mday ,gmt_time->tm_hour,gmt_time->tm_min,
			cur_time.year,cur_time.month,cur_time.day ,cur_time.hour,cur_time.min);

    int i,in_st_no ;

    int cnt;
    char *buf;

    char *tmp;

    char *d[20];
	int time_year_system=1;

	//commandline
	if (argc!=1)
	{
		for(i=1;i<argc;i++)
		{
			printf("argc[%d]=%s  ",i,argv[i]);
			in_st_no=atoi(argv[i]);
			if (i==1)
			{
				if ((in_st_no<10000)&&(in_st_no>-10000))
				{
					year=in_st_no;
					time_year_system=0;
				}
			}
			if (i==2)
			{
    			if ((in_st_no<13)&&(in_st_no>0))
    			{
    				month=in_st_no;
    				time_year_system=0;
    			}
			}
			if (i==3)
			{
				printf("  timaezone in_st_no=%d",in_st_no);
    			if ((in_st_no<781)&&(in_st_no>-751))
    			{
    				timez_off=in_st_no;
    			}
			}
			if (i==4)
			{
    			if (in_st_no==0)
    			{
    				lang=T_Korean;
    			}
    			else
    				lang=T_English;
			}

		}
	}


	//cgi
    cnt=0;
	buf=getenv("QUERY_STRING");

    tmp = buf!=NULL ? strtok(buf,"&=") : NULL;
    if(tmp != NULL) {
		d[cnt]=tmp;

		while(tmp){
			tmp = strtok(NULL,"&=");
			cnt++;
			d[cnt]= tmp;
		}
    }

    for(i=0 ; i<cnt ; i=i+2)
    {
    	in_st_no=atoi(d[i+1]);
    	if (strcmp( d[i],"year")==0)
    	{

    		if (in_st_no!=0)
    		{
    			if ((in_st_no<10000)&&(in_st_no>-10000))
    			{
    				year=in_st_no;
    				time_year_system=0;
    			}
    		}
    		else
    		{
    			if (*d[i+1]==0x30) {
    				year=0;
    				time_year_system=0;
    			}
    		}
    	}

    	if (strcmp( d[i],"month")==0)
    	{

    		if (in_st_no!=0)
    		{
    			if ((in_st_no<13)&&(in_st_no>0))
    			{
    				month=in_st_no;
    				time_year_system=0;
    			}
    		}
    	}
    	if (strcmp( d[i],"timez_off")==0)
    	{

    		if (in_st_no!=0)
    		{
    			if ((in_st_no<781)&&(in_st_no>-751))
    				timez_off=in_st_no;
    		}
    		else
    		{
    			if (*d[i+1]==0x30) timez_off=0;
    		}
    	}
    	if (strcmp( d[i],"lang")==0)
    	{
    		if (*d[i+1]==0x31) lang=T_English;
    		else lang=T_Korean;

    		if (strcmp( d[i+1],"eng")==0) lang=T_English;
    		if (strcmp( d[i+1],"kor")==0) lang=T_English;
    	}

    }

	  cur_time_tz=getdatebymin(timez_off+540,cur_time);
	  if (time_year_system==1)
	  {
		  	  year=cur_time_tz.year;
			month=cur_time_tz.month;
		}

    lang_mode=lang;

    print_head();
    print_body_first();

    print_month_table();
    print_month_info();

             


    print_body_last();

    return 0;


}
