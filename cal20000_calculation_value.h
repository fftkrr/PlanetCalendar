//
//  cal20000_calculation_value.h
//  CAL20000
//
//  Created by YOUNGCHANG KOH(fftkrr@gmail.com)
//
//



#ifndef CAL20000_cal20000_calculation_value_h
#define CAL20000_cal20000_calculation_value_h

//{병자년 경인월 신미일 기해시 - 입춘 }
int unityear = 1996;
unsigned short unitmonth  = 2;
unsigned short unitday    = 4;
unsigned short unithour   = 22;
unsigned short unitmin    = 8;
unsigned short unitsec    = 0;


//{1996년 음력 1월 1일 합삭 일시 }

int unitmyear  =1996;
unsigned short unitmmonth =2;
unsigned short unitmday =19;
unsigned short unitmhour =8;
unsigned short unitmmin =30;
unsigned short unitmsec =0;
int  moon_month_length= 42524 ; //{ =42524분 2.9초 }



/*
 { 입춘시점에서 24절기의 시점까지의 길이 - 분 }
 {  yearmin := 525948.75 ; - 실제 1년길이 (분)
 yearmini := 525949 ; - 근사치 (오차 3456년에 1일)}
 */

 int montharray[] =
{0,21355,42843,64498,86335,108366,130578,152958,
	175471,198077,220728,243370,265955,288432,310767,332928,
	354903,376685,398290,419736,441060,462295,483493,504693,525949};


/*
  char *monthst =
 {"입춘","우수","경칩","춘분","청명","곡우",
 "입하","소만","망종","하지","소서","대서",
 "입추","처서","백로","추분","한로","상강",
 "입동","소설","대설","동지","소한","대한","입춘"};
 */

 char *monthst[] =
{   "立春","雨水","驚蟄","春分","淸明","穀雨",
	"立夏","小萬","芒種","夏至","小暑","大暑",
	"立秋","處暑","白露","秋分","寒露","霜降",
	"立冬","小雪","大雪","冬至","小寒","大寒","立春"};


/*
 char *monthst_eng[] =
{"New Spring","Rain","Awake from hibernate","Spring equinox","clear and balmy","Good rain",
	"New Summer","Contentment","Sow seed","Summer solstice","Hot","Hotest",
	"New Fall","Hot end","White dew","Autumnal equinox","Cool dew","Frost",
	"New Winter","Snow","Snowstorm","Winter solstice","Cold","Coldest","New Spring"};
*/
char *monthst_eng[] =
{"24-New Spring","24-Rain","24-Awake from hibernate","24-Spring equinox","24-clear and balmy","24-Good rain",
	"24-New Summer","24-Good Season","24-Sow seed","24-Summer solstice","24-Hot","24-Hotest",
	"24-New Fall","24-Hot end","24-White dew","24-Autumnal equinox","24-Cool dew","24-Frost",
	"24-New Winter","24-Light Snow","24-Heavy Snow","24-Winter solstice","24-Moderate Cold","24-Severe Cold","24-New Spring"};


 char *month_color[]=
{
	"red","green","red","green","red","green",
	"white","red","white","red","white","red",
	"blue","white","blue","white","blue","white",
	"green","blue","green","blue","green","blue"
};
 char *month_bgcolor[]=
{
	"green","red","green","red","green","red",
	"red","white","red","white","red","white",
	"white","blue","white","blue","white","blue",
	"blue","green","blue","green","blue","green"
};

 char *gan[] =
{"甲","乙","丙","丁","戊","己","庚","辛","壬","癸"};

 char *ji[]  =
{"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥"};

 char *gan_color[] =
{
	"blackColor", "blackColor", "blackColor", "blackColor", "blackColor",
	"blackColor", "blackColor", "blackColor", "blackColor", "blackColor"
	
};

 char *gan_bg_color_white[] =
{
    /*
	"greenColor","whiteColor","whiteColor","whiteColor","whiteColor",
	"whiteColor","whiteColor","whiteColor","whiteColor","whiteColor"
     */
    "greenColor","clearColor","clearColor","clearColor","clearColor",
	"clearColor","clearColor","clearColor","clearColor","clearColor"

};
char *gan_bg_color_black[] =
{
     "greenColor","whiteColor","whiteColor","whiteColor","whiteColor",
     "whiteColor","whiteColor","whiteColor","whiteColor","whiteColor"
};

 char *ji_color[] =
{
	"whiteColor", "blackColor", "blackColor", "blackColor", "blackColor", "blackColor",
	"whiteColor", "blackColor", "blackColor", "blackColor", "blackColor", "blackColor"
	
};

 char *ji_bg_color_white[] =
{
	"blueColor","clearColor","clearColor","clearColor","clearColor","clearColor",
	"redColor","clearColor","clearColor","clearColor","clearColor","clearColor",
};
char *ji_bg_color_black[] =
{
	"blueColor","whiteColor","whiteColor","whiteColor","whiteColor","whiteColor",
	"redColor","whiteColor","whiteColor","whiteColor","whiteColor","whiteColor",
};

char *st_lunar_freeday_kr[]={"설날","부처님오신날","추석","부활절"};
char *st_lunar_freeday_eng[]={"NewYear","Buddha Birth","Thanksgiving","EasterDay"};






 char *jijang[][3]  =
{
	{"己","丙","甲"}, //인월
	{"甲","乙",""},
	{"乙","癸","戊"},
	
	{"戊","庚","丙"},
	{"丙","丁",""},
	{"丁","乙","己"},
	
	{"己","壬","庚"},
	{"庚","辛",""},
	{"辛","丁","戊"},
	
	{"戊","甲","壬"},
	{"壬","癸",""},
	{"癸","辛","己"}
};

 char *ganji[]  =
{"甲子","乙丑","丙寅","丁卯","戊辰",
	"己巳","庚午","辛未","壬申","癸酉",
	"甲戌","乙亥","丙子","丁丑","戊寅",
	
	"己卯","庚辰","辛巳","壬午","癸未",
	"甲申","乙酉","丙戌","丁亥","戊子",
	"己丑","庚寅","辛卯","壬辰","癸巳",
	
	"甲午","乙未","丙申","丁酉","戊戌",
	"己亥","庚子","辛丑","壬寅","癸卯",
	"甲辰","乙巳","丙午","丁未","戊申",
	
	"己酉","庚戌","辛亥","壬子","癸丑",
	"甲寅","乙卯","丙辰","丁巳","戊午",
	"己未","庚申","辛酉","壬戌","癸亥"};

 char *weekday[] = //{ "일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일",nil };
{"Sun","Mon","Tues","Wedne","Thurs","Fri","Satur"};

 char *weekday_Full[]  =
{"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};



 char *s28day[]  =
{"角","亢","氐","房","心","尾","箕",
	"斗","牛","女","虛","危","室","壁",
	"奎","婁","胃","昴","畢","觜","參",
	"井","鬼","柳","星","張","翼","軫"};

 char *gabja_day[] =
{
	"陽遁上元","陽遁中元","陽遁下元","陰遁上元","陰遁中元","陰遁下元","閏陽遁上元","閏陰遁上元","Error"
};

 int gabja_day_v[] = {
	0,6,3,8,2,5,0,8, -1
};

/*
  char *kusung_gabja[] =
 { "양둔상원","양둔중원","양둔하원","음둔상원","음둔중원","음둔하원","윤양둔","윤음둔","Error" };
 */
 char *kusung_gabja[] =
{
	"陽遁上元","陽遁中元","陽遁下元","陰遁上元","陰遁中元","陰遁下元","閏陽遁","閏陰遁"
};

 char *kusung_year_3won[] =
{
	"上元","中元","下元"
};

//kimoon_yunkuk : array[0..57,0..1] of integer =  //(년도, 동지=1/하지=0)
 int kimoon_yunkuk[][2] =
{
	{1890,1},{1893,0},{1896,0},{1898,1},
	{1901,1},{1904,1},{1907,1},{1910,1},{1913,0},{1916,0},{1918,1},{1921,1},{1924,1},{1927,1},
	{1930,1},{1933,0},{1936,0},{1939,0},{1941,1},{1944,1},{1947,1},{1950,1},{1953,0},{1956,0},
	{1959,0},{1961,1},{1964,1},{1967,1},{1970,1},{1973,1},{1976,0},{1979,0},{1981,1},{1984,1},
	{1987,1},{1990,1},{1993,1},{1996,0},{1999,0},{2001,1},{2004,1},{2007,1},{2010,1},{2013,1},
	{2016,0},{2019,0},{2021,1},{2024,1},{2027,1},{2030,1},{2033,1},{2036,0},{2039,0},{2042,0},
	{2044,1},{2047,1},{2050,1},{2053,1}
};

 char *kusung[][2]  =
{ {"一","白"},  {"二","黑"},  {"三","碧"},  {"四","綠"},  {"五","黃"},
	{"六","白"},  {"七","赤"},  {"八","白"},  {"九","紫"} };

 char *kimoon_3one[]  =
{"上元","中元","下元"};

 char *kimoon_PN[]  =
{"陰","陽"};

 char *kusing_html_table[] =
{
	"\n<font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font><font color=black style=\"BACKGROUND-COLOR: yellow\">五</font><font color=white style=\"BACKGROUND-COLOR: red\">七</font><br>\n<font color=black style=\"BACKGROUND-COLOR: white\">八</font><font color=black style=\"BACKGROUND-COLOR: white\">一</font><font color=white style=\"BACKGROUND-COLOR: blue\">三</font><br><font color=white style=\"BACKGROUND-COLOR: green\">四</font><font color=black style=\"BACKGROUND-COLOR: white\">六</font><font color=white style=\"BACKGROUND-COLOR: black\">二</font>\n",
	
	"\n<font color=black style=\"BACKGROUND-COLOR: white\">一</font><font color=black style=\"BACKGROUND-COLOR: white\">六</font><font color=black style=\"BACKGROUND-COLOR: white\">八</font><br>\n<font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font><font color=white style=\"BACKGROUND-COLOR: black\">二</font><font color=white style=\"BACKGROUND-COLOR: green\">四</font><br><font color=black style=\"BACKGROUND-COLOR: yellow\">五</font><font color=white style=\"BACKGROUND-COLOR: red\">七</font><font color=white style=\"BACKGROUND-COLOR: blue\">三</font>\n",
	
	"\n<font color=white style=\"BACKGROUND-COLOR: black\">二</font><font color=white style=\"BACKGROUND-COLOR: red\">七</font><font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font><br>\n<font color=black style=\"BACKGROUND-COLOR: white\">一</font><font color=white style=\"BACKGROUND-COLOR: blue\">三</font><font color=black style=\"BACKGROUND-COLOR: yellow\">五</font><br><font color=black style=\"BACKGROUND-COLOR: white\">六</font><font color=black style=\"BACKGROUND-COLOR: white\">八</font><font color=white style=\"BACKGROUND-COLOR: green\">四</font>\n",
	
	"\n<font color=white style=\"BACKGROUND-COLOR: blue\">三</font><font color=black style=\"BACKGROUND-COLOR: white\">八</font><font color=black style=\"BACKGROUND-COLOR: white\">一</font><br>\n<font color=white style=\"BACKGROUND-COLOR: black\">二</font><font color=white style=\"BACKGROUND-COLOR: green\">四</font><font color=black style=\"BACKGROUND-COLOR: white\">六</font><br><font color=white style=\"BACKGROUND-COLOR: red\">七</font><font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font><font color=black style=\"BACKGROUND-COLOR: yellow\">五</font>\n",
	
	"\n<font color=white style=\"BACKGROUND-COLOR: green\">四</font><font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font><font color=white style=\"BACKGROUND-COLOR: black\">二</font><br>\n<font color=white style=\"BACKGROUND-COLOR: blue\">三</font><font color=black style=\"BACKGROUND-COLOR: yellow\">五</font><font color=white style=\"BACKGROUND-COLOR: red\">七</font><br><font color=black style=\"BACKGROUND-COLOR: white\">八</font><font color=black style=\"BACKGROUND-COLOR: white\">一</font><font color=black style=\"BACKGROUND-COLOR: white\">六</font>\n",
	
	"\n<font color=black style=\"BACKGROUND-COLOR: yellow\">五</font><font color=black style=\"BACKGROUND-COLOR: white\">一</font><font color=white style=\"BACKGROUND-COLOR: blue\">三</font><br>\n<font color=white style=\"BACKGROUND-COLOR: green\">四</font><font color=black style=\"BACKGROUND-COLOR: white\">六</font><font color=black style=\"BACKGROUND-COLOR: white\">八</font><br><font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font><font color=white style=\"BACKGROUND-COLOR: black\">二</font><font color=white style=\"BACKGROUND-COLOR: red\">七</font>\n",
	
	"\n<font color=black style=\"BACKGROUND-COLOR: white\">六</font><font color=white style=\"BACKGROUND-COLOR: black\">二</font><font color=white style=\"BACKGROUND-COLOR: green\">四</font><br>\n	<font color=black style=\"BACKGROUND-COLOR: yellow\">五</font><font color=white style=\"BACKGROUND-COLOR: red\">七</font><font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font><br><font color=black style=\"BACKGROUND-COLOR: white\">一</font><font color=white style=\"BACKGROUND-COLOR: blue\">三</font><font color=black style=\"BACKGROUND-COLOR: white\">八</font>\n",
	
	"\n<font color=white style=\"BACKGROUND-COLOR: red\">七</font><font color=white style=\"BACKGROUND-COLOR: blue\">三</font><font color=black style=\"BACKGROUND-COLOR: yellow\">五</font><br>\n<font color=black style=\"BACKGROUND-COLOR: white\">六</font><font color=black style=\"BACKGROUND-COLOR: white\">八</font><font color=black style=\"BACKGROUND-COLOR: white\">一</font><br><font color=white style=\"BACKGROUND-COLOR: black\">二</font><font color=white style=\"BACKGROUND-COLOR: green\">四</font><font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font>\n",
	
	"\n<font color=black style=\"BACKGROUND-COLOR: white\">八</font><font color=white style=\"BACKGROUND-COLOR: green\">四</font><font color=black style=\"BACKGROUND-COLOR: white\">六</font><br>\n<font color=white style=\"BACKGROUND-COLOR: red\">七</font><font color=white style=\"BACKGROUND-COLOR: BlueViolet \">九</font><font color=white style=\"BACKGROUND-COLOR: black\">二</font><br><font color=white style=\"BACKGROUND-COLOR: blue\">三</font><font color=black style=\"BACKGROUND-COLOR: yellow\">五</font><font color=black style=\"BACKGROUND-COLOR: white\">一</font>\n"
	
};

 int kimoon_kuk[][3]  =
{
	{8,5,2},{9,6,3},{1,7,4},  //입춘,우수,춘분
	{3,9,6},{4,1,7},{5,2,8},
	{4,1,7},{5,2,8},{6,3,9},
	{9,3,6},{8,2,5},{7,1,4},
	{2,5,8},{1,4,7},{9,3,6},
	{7,1,4},{6,9,3},{5,8,2},
	{6,9,3},{5,8,2},{4,7,1},
	{1,7,4},{2,8,5},{3,9,6} //동지,소한,대한
};


 int month_kusung[][12] =  //month_kusung : array[0..2,0..11] of integer =
{
	{2,1,9,8,7,6,5,4,3,2,1,9},
	{8,7,6,5,4,3,2,1,9,8,7,6},
	{5,4,3,2,1,9,8,7,6,5,4,3}
};

 char *kusung_color[] = //: array[0..8] of integer =
{
	"blackColor", "whiteColor", "whiteColor", "blackColor", "blackColor", "blackColor", "whiteColor", "blackColor", "whiteColor"
	
};

 char *kusung_bg_color_white[] = //: array[0..8] of integer =
{
	//"whiteColor","blackColor","blueColor","greenColor","yellowColor","whiteColor","redColor","whiteColor","magentaColor",
    "clearColor","blackColor","blueColor","greenColor","yellowColor","clearColor","redColor","clearColor","magentaColor",
};

char *kusung_bg_color_black[] = //: array[0..8] of integer =
{
	"whiteColor","blackColor","blueColor","greenColor","yellowColor","whiteColor","redColor","whiteColor","magentaColor",
    //"clearColor","blackColor","blueColor","greenColor","yellowColor","clearColor","redColor","clearColor","magentaColor",
};


 char *islamic_month_name[]=
{
	"المحرّم",
	"صفر",
	"ربيع الأوّل",
	"ربيع الآخر",
	"جمادى الأولى",
	"جمادى الآخرة",
	"رجب",
	"شعبان",
	"رمضان",
	"شوّال",
	"ذو القعدة",
	"ذو الحجّة",
	"المحرّم"
	
};
/*
 
 1.Muḥarram — المحرّم — means "forbidden" in Arabic, so called because it was unlawful to fight during this month. Muharram is the second most sacred Muslim month, and includes the Day of Ashura.
 2.Ṣafar — صفر — means "void" in Arabic, supposedly named because pagan Arabs looted during this month and left the houses empty.
 3.Rabīʿ I (Rabīʿ al-Awwal) — ربيع الأوّل — means "the first Spring" in Arabic.
 4.Rabīʿ II (Rabīʿ ath-Thānī or Rabīʿ al-Ākhir) — ربيع الآخر , ربيع الثاني — means "the second (or last) Spring" in Arabic.
 5.Jumādā I (Jumādā al-Ūlā) — جمادى الأولى — means "the first month of parched land" in Arabic.
 6.Jumādā II (Jumādā ath-Thāniya or Jumādā al-Ākhira) — جمادى الآخرة , جمادى الثانية — means "the second (or last) month of parched land" in Arabic.
 7.Rajab — رجب — means "respect" or "honor" in Arabic. Rajab is another of the sacred months in which fighting was traditionally forbidden.
 8.Shaʿbān — شعبان — means "scattered" in Arabic, marking the time of year when Arab tribes dispersed to find water.
 9.Ramaḍān — رمضان — means "scorched" in Arabic. Ramadan is the most venerated month of the Hijri calendar, during which Muslims fast between dawn and sunset.
 10.Shawwāl — شوّال — means "raised" in Arabic, as she-camels begin to raise their tails during this time of year, after giving birth.
 11.Dhū al-Qaʿda — ذو القعدة — means "the one of truce" in Arabic. Dhu al-Qa'da was another month during which war was banned.
 12.Dhū al-Ḥijja — ذو الحجّة — means "the one of pilgrimage" in Arabic, referring to the annual Muslim pilgrimage to Mecca, the Hajj.
 
 
 
 */

 char *Hebrew_month_name[]=
{
    "Tishrei",
    "Heshvan",
    "Kislev",
    "Tevet",
    "Shvat",
    "Adar",
    "Nissan",
    "Iyar",
    "Sivan",
    "Tammuz",
    "Av",
    "Elul",
    "Veadar" //윤달
};


 /*
 char *St_moon_remark_type[] =
{
	"合朔<br>(New_Moon)",
	"上弦<br>(First_quarter)",
	"望<br>(Full_Moon)",
	"下弦<br>(Last_quarter)",
	"合朔<br>(Next_New_Moon)",
	"昇交點<br>(羅睺 Ascending_node)",
	"降交點<br>(計都 Descending_node)",
	"近地點<br>(Perigee)",
	"遠地點<br>(月孛 Apogee)",
	"紫氣"
};

*/

 char *St_moon_remark_type[] =
{
		"합삭/초하루",
 			"상현",
 			"망",
 			"하현",
 			"다음 합삭/초하루",
 			"승교점",
 			"강교점",
 			"근지점",
 			"원지점"
};

 char *St_moon_eclipse_type[] =
{
	"",
	"日蝕(Solar_eclipse)",
	"日蝕可能(Possible_Solar_eclipse)",
	"月蝕(Lunar_eclipse)",
	"月蝕可能(Possible_Lunar_eclipse)"
};

char *St_moon_eclipse_type_han[] =
{
	"",
	"日蝕",
	"日蝕可能",
	"月蝕",
	"月蝕可能"
};

char *St_moon_eclipse_type_eng[] =
{
	"",
	"Solar_eclipse",
	"Possible_Solar_eclipse",
	"Lunar_eclipse",
	"Possible_Lunar_eclipse"
};

char *St_moon_eclipse_type_kor[] =
{   "",
	"일식",
	"일식가능",
	"월식",
	"월식가능",
};

char *Planet_event_string_kr[] = //for planet_event_result
{
    	    "","삭","망","승교점","강교점","근지점","원지점","상현","하현",/*@"유(서->동)"*/"유(역행종료)",/*@"유(동->서)"*/"유(역행시작)","내합","외합","합","충","최대이각(동)","최대이각(서)","서구","동구","행성합","달합","달충","절기","근일","원일","승황교점","강황교점"
};


char *Planet_event_string_eng[] =
{
    	    "","New Moon","Full Moon","ascending node","descending node","perigee","apogee","first quarter","last quarter",/*@"유(서->동)"*/"stationary(finish retrograde)",/*@"유(동->서)"*/"stationary(start retrograde)","inferior conjunction","superior conjunction","conjunction","opposition","Greatest elongation(East)","Greatest elongation(West)","western quadrature","eastern quadrature","planet conjuction","Moon conjuction","Moon opposition","24 solar term","Perihelion","Aphelion","Ascending ecliptic","Descending ecliptic"
};


char *Planet_event_string_2nd_kr[] = //for planet_event_result
{
    	    "황위차이","최대이각(황경차이)","황경","교점거리","거리"
};
char *Planet_event_string_2nd_eng[] = //for planet_event_result
{
    	    "dβ","dλ","λ","dλ to node","distance"
};


char *char_degree="˚";

char *weekday_eng[]  =
		{"Sun","Mon","Tues","Wedne","Thurs","Fri","Satur"};
char *weekday_kor[]  =
	    {"일요일","월요일","화요일","수요일","목요일","금요일","토요일"};
char *English_month_name[]=
	{
		"January",
		"Feburary",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};


//13.09.21 add





//2000.Jan 0.0 UT(1999DEC31,0:00 UT) 기준
struct Planet_info SUN=
{
    /*
        Orbital elements of the Sun:
        N = 0.0
        i = 0.0
        w = 282.9404 + 4.70935E-5 * d
        a = 1.000000  (AU)
        e = 0.016709 - 1.151E-9 * d
        M = 356.0470 + 0.9856002585 * d
    */

    .Number=0,
    .Name_Kor="해",
    .Name_Han="日",
    .Name_Eng="SUN",
    .longitude_ascending_node_initial_degree=0.0,
    .longitude_ascending_node_period=0.0,
    .inclination_initial_degree =0.0 ,
    .inclination_period = 0.0 ,
    .longitude_perihelion_initial_degree = 282.9404,
    .longitude_perihelion_period =  4.70935E-5,
    .mean_distance = 1.0,
    .mean_distance_variation = 0,
    .eccentricity_initial_degree = 0.016709,
    .eccentricity_period = -1.151E-9,
    .mean_anomaly_initial_degree = 356.0470,
    .mean_anomaly_period = 0.9856002585,
    //.color = 0x60ff
    .color = 0xff0101
    
};

struct Planet_info MOON=
{
    /*
     Orbital elements of the Moon:
     N = 125.1228 - 0.0529538083 * d
     i = 5.1454
     w = 318.0634 + 0.1643573223 * d
     a = 60.2666  (Earth radii)
     e = 0.054900
     M = 115.3654 + 13.0649929509 * d
     */
    .Number=1,
    .Name_Kor="달",
    .Name_Han="月",
    .Name_Eng="MOON",
    .longitude_ascending_node_initial_degree=125.1228,
    .longitude_ascending_node_period= -0.0529538083,
    .inclination_initial_degree =5.1454,
    .inclination_period = 0.0 ,
    .longitude_perihelion_initial_degree = 318.0634  ,
    .longitude_perihelion_period = 0.1643573223 ,
    .mean_distance = 60.2666, //지구반경의 60.2666배
    .mean_distance_variation = 0,
    .eccentricity_initial_degree = 0.0549 ,
    .eccentricity_period = 0,
    .mean_anomaly_initial_degree = 115.3654 ,
    .mean_anomaly_period = 13.0649929509 ,
    //mean_anomaly_period : 13.17639648 ;
    //.color = 0x00FFFF   //clYellow
    .color = 0xffff00
    
};

struct Planet_info MERCURY=
{
    /*
     Orbital elements of Mercury:
     N =  48.3313 + 3.24587E-5 * d
     i = 7.0047 + 5.00E-8 * d
     w =  29.1241 + 1.01444E-5 * d
     a = 0.387098  (AU)
     e = 0.205635 + 5.59E-10 * d
     M = 168.6562 + 4.0923344368 * d
     */
    .Number=2,
    .Name_Kor="수성",
    .Name_Han="水星",
    .Name_Eng="MERCURY",
    .longitude_ascending_node_initial_degree=48.3313,
    .longitude_ascending_node_period= 3.24587E-5,
    .inclination_initial_degree =7.0047,
    .inclination_period = 5.00E-8,
    .longitude_perihelion_initial_degree = 29.1241  ,
    .longitude_perihelion_period = 1.01444E-5 ,
    .mean_distance = 0.387098, // AU
    .mean_distance_variation = 0,
    .eccentricity_initial_degree = 0.205635 ,
    .eccentricity_period = 5.59E-10,
    .mean_anomaly_initial_degree = 168.6562 ,
    .mean_anomaly_period = 4.0923344368 ,
    //.color = 0x800080   //clPurple
    .color=0x3399ff
    
};

struct Planet_info VENUS=
{
/*
        Orbital elements of Venus:
        N =  76.6799 + 2.46590E-5 * d
        i = 3.3946 + 2.75E-8 * d
        w =  54.8910 + 1.38374E-5 * d
        a = 0.723330  (AU)
        e = 0.006773 - 1.302E-9 * d
        M =  48.0052 + 1.6021302244 * d
*/

    .Number=3,
    .Name_Kor="금성",
    .Name_Han="金星",
    .Name_Eng="VENUS",
    .longitude_ascending_node_initial_degree=76.6799,
    .longitude_ascending_node_period= 2.46590E-5,
    .inclination_initial_degree =3.3946,
    .inclination_period = 2.75E-8,
    .longitude_perihelion_initial_degree = 54.8910  ,
    .longitude_perihelion_period = 1.38374E-5 ,
    .mean_distance = 0.723330, // AU
    .mean_distance_variation = 0,
    .eccentricity_initial_degree = 0.006773 ,
    .eccentricity_period = -1.302E-9,
    .mean_anomaly_initial_degree = 48.0052 ,
    .mean_anomaly_period = 1.6021302244 ,
    //.color = 0xFF0000   //clBlue
    .color=0xb266ff
    
};


struct Planet_info MARS=
{
    /*
     Orbital elements of Mars:
     N =  49.5574 + 2.11081E-5 * d
     i = 1.8497 - 1.78E-8 * d
     w = 286.5016 + 2.92961E-5 * d
     a = 1.523688  (AU)
     e = 0.093405 + 2.516E-9 * d
     M =  18.6021 + 0.5240207766 * d
     */
    
    .Number=4,
    .Name_Kor="화성",
    .Name_Han="火星",
    .Name_Eng="MARS",
    .longitude_ascending_node_initial_degree=49.5574,
    .longitude_ascending_node_period= 2.11081E-5,
    .inclination_initial_degree =1.8497,
    .inclination_period = -1.78E-8,
    .longitude_perihelion_initial_degree = 286.5016  ,
    .longitude_perihelion_period = 2.92961E-5 ,
    .mean_distance = 1.523688, // AU
    .mean_distance_variation = 0,
    .eccentricity_initial_degree = 0.093405 ,
    .eccentricity_period = 2.516E-9,
    .mean_anomaly_initial_degree = 18.6021 ,
    .mean_anomaly_period = 0.5240207766 ,
    //.color = 0x0000FF   //clRed
    .color=0x9933ff
};

struct Planet_info JUPITER=
{
    /*
     Orbital elements of Jupiter:
     N = 100.4542 + 2.76854E-5 * d
     i = 1.3030 - 1.557E-7 * d
     w = 273.8777 + 1.64505E-5 * d
     a = 5.20256  (AU)
     e = 0.048498 + 4.469E-9 * d
     M =  19.8950 + 0.0830853001 * d
     */
    
    .Number=5,
    .Name_Kor="목성",
    .Name_Han="木星",
    .Name_Eng="JUPITER",
    .longitude_ascending_node_initial_degree=100.4542 ,
    .longitude_ascending_node_period= 2.76854E-5,
    .inclination_initial_degree =1.3030,
    .inclination_period = -1.557E-7 ,
    .longitude_perihelion_initial_degree = 273.8777  ,
    .longitude_perihelion_period = 1.64505E-5 ,
    .mean_distance = 5.20256 ,
    .mean_distance_variation = 0,
    .eccentricity_initial_degree = 0.048498 ,
    .eccentricity_period = 4.469E-9 ,
    .mean_anomaly_initial_degree = 19.8950 ,
    .mean_anomaly_period = 0.0830853001 ,
    //.color = 0x008000   //clGreen
    .color=0x99ff99
    
    
};

struct Planet_info SATURN=
{
    /*
        Orbital elements of Saturn:
        N = 113.6634 + 2.38980E-5 * d
        i = 2.4886 - 1.081E-7 * d
        w = 339.3939 + 2.97661E-5 * d
        a = 9.55475  (AU)
        e = 0.055546 - 9.499E-9 * d
        M = 316.9670 + 0.0334442282 * d
    */
    
    .Number=6,
     .Name_Kor="토성",
     .Name_Han="土星",
     .Name_Eng="SATURN",
     .longitude_ascending_node_initial_degree=113.6634,
     .longitude_ascending_node_period= 2.38980E-5,
     .inclination_initial_degree =2.4886,
     .inclination_period = - 1.081E-7 ,
     .longitude_perihelion_initial_degree = 339.3939  ,
     .longitude_perihelion_period = 2.97661E-5 ,
     .mean_distance = 9.55475 ,
     .mean_distance_variation = 0,
     .eccentricity_initial_degree = 0.055546 ,
     .eccentricity_period = - 9.499E-9 ,
     .mean_anomaly_initial_degree = 316.9670 ,
     .mean_anomaly_period = 0.0334442282 ,
     //.color = 0x00FFFF   //clYellow
    .color=0xffc089
};


struct Planet_info URANUS=
{
    /*
     Orbital elements of Uranus:
     N =  74.0005 + 1.3978E-5 * d
     i = 0.7733 + 1.9E-8 * d
     w =  96.6612 + 3.0565E-5 * d
     a = 19.18171 - 1.55E-8 * d  (AU)
     e = 0.047318 + 7.45E-9 * d
     M = 142.5905 + 0.011725806 * d
     */
    
    .Number=7,
    .Name_Kor="천왕성",
    .Name_Han="天王星",
    .Name_Eng="URANUS",
    .longitude_ascending_node_initial_degree=74.0005,
    .longitude_ascending_node_period= 1.3978E-5,
    .inclination_initial_degree =0.7733,
    .inclination_period = 1.9E-8 ,
    .longitude_perihelion_initial_degree = 96.6612  ,
    .longitude_perihelion_period = 3.0565E-5 ,
    .mean_distance = 19.18171 ,
    .mean_distance_variation = - 1.55E-8,
    .eccentricity_initial_degree = 0.047318 ,
    .eccentricity_period = 7.45E-9 ,
    .mean_anomaly_initial_degree = 142.5905 ,
    .mean_anomaly_period = 0.011725806 ,
    //.color = 0xA4A0A0   //clMedGray
    .color=0xff99ff
};

struct Planet_info NEPTUNE=
{
    /*
     Orbital elements of Neptune:
     N = 131.7806 + 3.0173E-5 * d
     i = 1.7700 - 2.55E-7 * d
     w = 272.8461 - 6.027E-6 * d
     a = 30.05826 + 3.313E-8 * d  (AU)
     e = 0.008606 + 2.15E-9 * d
     M = 260.2471 + 0.005995147 * d
     */
    
    .Number=8,
    .Name_Kor="해왕성",
    .Name_Han="海王星",
    .Name_Eng="NEPTUNE",
    .longitude_ascending_node_initial_degree=131.7806,
    .longitude_ascending_node_period= 3.0173E-5,
    .inclination_initial_degree =1.7700,
    .inclination_period = -2.55E-7 ,
    .longitude_perihelion_initial_degree = 272.8461  ,
    .longitude_perihelion_period = -6.027E-6 ,
    .mean_distance = 30.05826 ,
    .mean_distance_variation = 3.313E-8,
    .eccentricity_initial_degree = 0.008606 ,
    .eccentricity_period = 2.15E-9  ,
    .mean_anomaly_initial_degree = 260.2471 ,
    .mean_anomaly_period = 0.005995147 ,
    //Color =  TColor($DB70DB);
    //.color = 0xDB70DB   //clMedGray
    .color=0x99ccff
};

struct Planet_info *Planet_Orbit[] =
{
    &SUN,
    &MOON,
    &MERCURY,
    &VENUS,
    &MARS,
    &JUPITER,
    &SATURN,
    &URANUS,
    &NEPTUNE
};


#endif


