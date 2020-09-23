#include <M5Stack.h>
#include "TinyGPS++.h"
#include "LabeledControl.h"

#define BG_COLOR    NAVY
#define FG_COLOR    WHITE

// static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

/*
   This sample code demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

static const uint32_t GPSBaud = 9600;
TinyGPSPlus           gps;
HardwareSerial        ss(2);
LabeledControl        *c1, *c2, *c3, *c4, *c5, *c6, *c7, *c8, *c9, *c10, *c11, *c12;


// This custom version of delay() ensures that the gps object
// is being "fed".
//
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available()) gps.encode(ss.read());
  } while (millis() - start < ms);
}


// Given a value, return it as a string in the format X:MM:SS
//
String frac_to_min_sec(double value) {
  char    buffer[16];
  double  frac    = abs(value - trunc(value));
  int     minutes = floor(frac * 60.0);
  int     seconds = floor(((frac * 60.0) - minutes) * 60.0);
  sprintf(buffer, "%d:%02d:%02d", int(value), int(minutes), int(seconds));
  return String(buffer);
}


void show_date(TinyGPSDate& value) {
  char sz[32];
  sprintf(sz, "%02d/%02d/%02d ", value.month(), value.day(), value.year());
  c5->set_value(sz);
}


void show_time(TinyGPSTime& value) {
  char sz[32];
  sprintf(sz, "%02d:%02d:%02d ", value.hour(), value.minute(), value.second());
  c6->set_value(sz);
}


void create_controls() {
  LabelDefinition     lab;
  DisplayDefinition   def;

  lab.width         = 156;
  lab.align         = AlignRight;

  def.position      = { 4, 2 };
  def.width         = 120;
  def.margin_top    = 0;
  def.font          = &FreeSans9pt7b;
  def.fg_color      = FG_COLOR;
  def.bg_color      = BG_COLOR;
  lab.label_text    = "# Satellites";
  c1                = new LabeledControl(lab, def);
  def.position.y    += c1->get_height();
  lab.label_text    = "Latitude";
  c2                = new LabeledControl(lab, def);
  def.position.y    += c2->get_height();
  lab.label_text    = "Longitude";
  c3                = new LabeledControl(lab, def);
  def.position.y    += c3->get_height();
  lab.label_text    = "HDOP";
  c4                = new LabeledControl(lab, def);
  def.position.y    += c4->get_height();
  lab.label_text    = "Date";
  c5                = new LabeledControl(lab, def);
  def.position.y    += c5->get_height();
  lab.label_text    = "Time";
  c6                = new LabeledControl(lab, def);
  def.position.y    += c6->get_height();
  lab.label_text    = "Altitude";
  c7                = new LabeledControl(lab, def);
  def.position.y    += c7->get_height();
  lab.label_text    = "Heading";
  c8                = new LabeledControl(lab, def);
  def.position.y    += c8->get_height();
  lab.label_text    = "Speed";
  c9                = new LabeledControl(lab, def);
  def.position.y    += c9->get_height();
  lab.label_text    = "Chars Processed";
  c10               = new LabeledControl(lab, def);
  def.position.y    += c10->get_height();
  lab.label_text    = "Fixed Sentences";
  c11               = new LabeledControl(lab, def);
  def.position.y    += c11->get_height();
  lab.label_text    = "Failed Checksums";
  c12               = new LabeledControl(lab, def);
}


void setup() {
  M5.begin();
  M5.Power.begin();
  ss.begin(GPSBaud);
  M5.Lcd.fillScreen(BG_COLOR);
  create_controls();
}


void loop()
{
  if(gps.satellites.isValid()) c1->set_value(gps.satellites.value());               else c1->set_value("?"); smartDelay(0);  c1->draw();
  if(gps.location.isValid())   c2->set_value(frac_to_min_sec(gps.location.lat()));  else c2->set_value("?"); smartDelay(0);  c2->draw();
  if(gps.location.isValid())   c3->set_value(frac_to_min_sec(gps.location.lng()));  else c3->set_value("?"); smartDelay(0);  c3->draw();
  if(gps.hdop.isValid())       c4->set_value(gps.hdop.value() / 100.0);             else c4->set_value("?"); smartDelay(0);  c4->draw();
  if(gps.date.isValid())       show_date(gps.date);                                 else c5->set_value("?"); smartDelay(0);  c5->draw();
  if(gps.time.isValid())       show_time(gps.time);                                 else c6->set_value("?"); smartDelay(0);  c6->draw();
  if(gps.altitude.isValid())   c7->set_value(gps.altitude.feet());                  else c7->set_value("?"); smartDelay(0);  c7->draw();
  if(gps.course.isValid())     c8->set_value(gps.course.deg());                     else c8->set_value("?"); smartDelay(0);  c8->draw();
  if(gps.speed.isValid())      c9->set_value(gps.speed.mph());                      else c9->set_value("?"); smartDelay(0);  c9->draw();
  c10->set_value(gps.charsProcessed());    smartDelay(0);  c10->draw();
  c11->set_value(gps.sentencesWithFix());  smartDelay(0);  c11->draw();
  c12->set_value(gps.failedChecksum());    smartDelay(0);  c12->draw();

  // unsigned long distanceKmToLondon =
  //   (unsigned long)TinyGPSPlus::distanceBetween(
  //     gps.location.lat(),
  //     gps.location.lng(),
  //     LONDON_LAT,
  //     LONDON_LON) / 1000;
  // printInt(distanceKmToLondon, gps.location.isValid(), 9);

  // double courseToLondon =
  //   TinyGPSPlus::courseTo(
  //     gps.location.lat(),
  //     gps.location.lng(),
  //     LONDON_LAT,
  //     LONDON_LON);

  // printFloat(courseToLondon, gps.location.isValid(), 7, 2);

  // const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);

  // printStr(gps.location.isValid() ? cardinalToLondon : "*** ", 6);

  // printInt(gps.charsProcessed(), true, 6);
  // printInt(gps.sentencesWithFix(), true, 10);
  // printInt(gps.failedChecksum(), true, 9);
  // M5.Lcd.println();

  smartDelay(1000);

  // if (millis() > 5000 && gps.charsProcessed() < 10)
  //   M5.Lcd.println(F("No GPS data received: check wiring"));
}
