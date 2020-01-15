  /*
    C source code example: Convert UTC to local time zone, considering daylight
    savings. Uses mktime(), gmtime() and localtime(). Works for dates between
    years 1902 and 2037. Should compile and run with any recent GNU C if your
    tzdata is healthy. Written by Robert Larsson  https://rl.se

    Code put in public domain; do what you want with it.
*/

#include    <stdio.h>
#include    <stdlib.h>
#include    <time.h>

#define     DESTZONE    "TZ=America/New_York"       // Our destination time zone

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){
    delay(1); //wait for serial to begin
  }
  Serial.print("Begining UTC conversion\n");
  struct tm   i;
    time_t      stamp;                              // Can be negative, so works before 1970
    putenv("TZ=America/New_York");

    i.tm_year = 2020-1900;                          // Populate struct members with
    i.tm_mon  = 1-1;                                // the UTC time details, we use
    i.tm_mday = 14;                                 // 29th August, 2009 12:34:56
    i.tm_hour = 16;                                 // in this example
    i.tm_min  = 03;
    i.tm_sec  = 56;

    stamp = mktime(&i);                             // Convert the struct to a Unix timestamp

    Serial.printf("UTC  : %s", asctime(gmtime(&stamp)));
    Serial.printf("Local: %s", asctime(localtime(&stamp)));
}

void loop() {
  // put your main code here, to run repeatedly:

}
