# PlanetCalendar
Lunisolar calendar with planets events

#About this program

This is Korean lunisolar calendar program which is almost same to Chinese lunisolar.
And there are planets events in this program such as conjunctions, stationary, opposition, great elongation and more. The positions of Sun, moon and planets are calculated with minute accuracy and showed in this calendar.

This is a part of CAL20000 App in the Apple App store and Google Play Store.


#Compile & execute
 This is standard C source code. Use any C compiler under any os. put source code into a direcotry and compile like below
 
 gcc -lm -o month.cgi *.c
 
 or
 
 make
 
 if you use windows os, the output name should have '.exe' extenstion.

Once compiled, just type the output name in command prompt. You may redirect the output to html file like below.

month.cgi > a.html

Open the a.html in any web browser. Then you can see the result.

This program is made for Web CGI interface. If you put the execution file into your Web CGI directory and your web-server  execute this by a specific URL, You can see the result on your Web-browser.
You can assign Year,month,timezone on WEB-CGI inteface. Or you can assign these parameters like below on command line interface.

month.cgi 2016 7 -540 > 2016_7.html

Take source code for more informations.





