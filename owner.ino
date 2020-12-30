/*
 * This is the code of owner side
 * Please change the ssid and the password.Enter your wifi sssid(name)and the password.
 * If you want then you can also change the server. To do that you can either call to me or call to Amardeep.
 * You can also change the server, to do so go to https://thingspeak.com/ -> sign up -> create two seprate channel -> go to api section-> copy the url
 * and replace with current url in ther code.
 * There is a delay of 15 second in  server between two data. 
 */

/////////////////////////////////////// Library /////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <LedControl.h>
//////////////////////////////////////
const char* ssid     = "ssid";     // Change here Network credentials Name
const char* password = "password"; //// chnage here Network credentials Password
//////////////////////////////////////////////
WiFiServer server(80);
////////////////////////////   Input and Output Devices /////////////////////
int DIN = 16; // D0
int CS =  5;  // D1
int CLK = 4;  // D2
byte devices = 2;
int Buzzer = 14; // D5
int counter = 0;
int buzzer_count = 0;

LedControl lc = LedControl(DIN, CLK, CS, devices);

byte pixels[95][8] = {
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0020 (space)
  { 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00 },   // U+0021 (!)
  { 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0022 (")
  { 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00 },   // U+0023 (#)
  { 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00 },   // U+0024 ($)
  { 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00 },   // U+0025 (%)
  { 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00 },   // U+0026 (&)
  { 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0027 (')
  { 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00 },   // U+0028 (()
  { 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00 },   // U+0029 ())
  { 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00 },   // U+002A (*)
  { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00 },   // U+002B (+)
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06 },   // U+002C (,)
  { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00 },   // U+002D (-)
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00 },   // U+002E (.)
  { 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00 },   // U+002F (/)
  { 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00 },   // U+0030 (0)
  { 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00 },   // U+0031 (1)-17
  { 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00 },   // U+0032 (2)-18
  { 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00 },   // U+0033 (3)-19
  { 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00 },   // U+0034 (4)-20
  { 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00 },   // U+0035 (5)-21
  { 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00 },   // U+0036 (6)-22
  { 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00 },   // U+0037 (7)-23
  { 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00 },   // U+0038 (8)-24
  { 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00 },   // U+0039 (9)-25
  { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00 },   // U+003A (:)-26
  { 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06 },   // U+003B (//)-27
  { 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00 },   // U+003C (<)-28
  { 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00 },   // U+003D (=)-29
  { 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00 },   // U+003E (>)-30
  { 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00 },   // U+003F (?)-31
  { 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00 },   // U+0040 (@)-32
  { 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00 },   // U+0041 (A)-33
  { 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00 },   // U+0042 (B)-34
  { 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00 },   // U+0043 (C)-35
  { 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00 },   // U+0044 (D)-36
  { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00 },   // U+0045 (E)-37
  { 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00 },   // U+0046 (F)-38
  { 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00 },   // U+0047 (G)-39
  { 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00 },   // U+0048 (H)-40
  { 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   // U+0049 (I)-41
  { 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00 },   // U+004A (J)-42
  { 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00 },   // U+004B (K)-43
  { 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00 },   // U+004C (L)-44
  { 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00 },   // U+004D (M)-45
  { 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00 },   // U+004E (N)-46
  { 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00 },   // U+004F (O)-47
  { 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00 },   // U+0050 (P)-48
  { 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00 },   // U+0051 (Q)-49
  { 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00 },   // U+0052 (R)-50
  { 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00 },   // U+0053 (S)-51
  { 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   // U+0054 (T)-52
  { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00 },   // U+0055 (U)-53
  { 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00 },   // U+0056 (V)-54
  { 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00 },   // U+0057 (W)-55
  { 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00 },   // U+0058 (X)-56
  { 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00 },   // U+0059 (Y)-57
  { 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00 },   // U+005A (Z)-58
  { 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00 },   // U+005B ([)
  { 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00 },   // U+005C (\)
  { 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00 },   // U+005D (])
  { 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00 },   // U+005E (^)
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF },   // U+005F (_)
  { 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+0060 (`)
  { 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00 },   // U+0061 (a)
  { 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00 },   // U+0062 (b)
  { 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00 },   // U+0063 (c)
  { 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00 },   // U+0064 (d)
  { 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00 },   // U+0065 (e)
  { 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00 },   // U+0066 (f)
  { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F },   // U+0067 (g)
  { 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00 },   // U+0068 (h)
  { 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   // U+0069 (i)
  { 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E },   // U+006A (j)
  { 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00 },   // U+006B (k)
  { 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00 },   // U+006C (l)
  { 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00 },   // U+006D (m)
  { 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00 },   // U+006E (n)
  { 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00 },   // U+006F (o)
  { 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F },   // U+0070 (p)
  { 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78 },   // U+0071 (q)
  { 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00 },   // U+0072 (r)
  { 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00 },   // U+0073 (s)
  { 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00 },   // U+0074 (t)
  { 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00 },   // U+0075 (u)
  { 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00 },   // U+0076 (v)
  { 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00 },   // U+0077 (w)
  { 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00 },   // U+0078 (x)
  { 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F },   // U+0079 (y)
  { 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00 },   // U+007A (z)
  { 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00 },   // U+007B ({)
  { 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00 },   // U+007C (|)
  { 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00 },   // U+007D (})
  { 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // U+007E (~)
};
void setup ()
{
  Serial.begin(115200);
  pinMode(Buzzer, OUTPUT);
  for (byte address = 0; address < devices; address++) {
    lc.shutdown(address, false);  //powersaving
    lc.setIntensity(address, 8);  //set light intensity 0 - min, 15 - max
    lc.clearDisplay(address);   //clear display
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("Connecting..");
  }
  Serial.println("Connected to");
  Serial.println(ssid);
  server.begin();
  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  ///////////////////////////////////////////////////////////////////////////////////////////////////

  HTTPClient http;  //Declare an object of class HTTPClient
  http.begin("http://api.thingspeak.com/channels/302157/fields/1.json?results=1");
  int httpCode = http.GET();
  if (httpCode > 0)  //Check the returning code
  {
    String payload = http.getString();   //Get the request response payload
    int data = payload.indexOf("feeds");
    String feeds = payload.substring(data + 18);
    int field2_index = feeds.indexOf("field1");  //getting index of the Field1 after feeds
    int field2_index_xtra_data = feeds.indexOf("person"); // getting index of cm after feeds
    field2_index = field2_index + 9;
    String field2 =  feeds.substring(field2_index, field2_index_xtra_data);
    counter = field2.toInt();
    Serial.print("Old Counter  ");
    Serial.println(counter);
    buzzer_count = counter;
    Serial.print("buzzer_count  ");
    Serial.println(buzzer_count);
    http.end();   //Close connection
  }
}

void DrawSymbol(byte adr, byte symbol, byte offset)
{
  for (int i = 0; i <= 7; i++) {          //     adr start with 0 and it is the number of 8x8 led matrix. In case of two devices 0 and 1
    byte dataRow = pixels[symbol][i];         //    Symbol is the char that you want to print
    dataRow = ByteRevers(dataRow) >> offset;    // offset is the gape from right side of the 8x8 led matrix
    lc.setRow(adr, i, dataRow);
  }
}
byte ByteRevers(byte in) {
  //font turning
  byte out;
  out = 0;
  if (in & 0x01) out |= 0x80;
  if (in & 0x02) out |= 0x40;
  if (in & 0x04) out |= 0x20;
  if (in & 0x08) out |= 0x10;
  if (in & 0x10) out |= 0x08;
  if (in & 0x20) out |= 0x04;
  if (in & 0x40) out |= 0x02;
  if (in & 0x80) out |= 0x01;
  return (out);
}

void loop()
{
  HTTPClient http;  //Declare an object of class HTTPClient
  http.begin("http://api.thingspeak.com/channels/302157/fields/1.json?results=1");
  int httpCode = http.GET();
  if (httpCode > 0)  //Check the returning code
  {
    String payload = http.getString();   //Get the request response payload
    int data = payload.indexOf("feeds");
    String feeds = payload.substring(data + 18);
    int field2_index = feeds.indexOf("field1");  //getting index of the Field1 after feeds
    int field2_index_xtra_data = feeds.indexOf("person"); // getting index of cm after feeds
    field2_index = field2_index + 9;
    String field2 =  feeds.substring(field2_index, field2_index_xtra_data);
    counter = field2.toInt();
    Serial.print("data");
    Serial.println(payload);
    
    Serial.print(" New counter ");
    Serial.println(counter);
    Serial.print(" buzzer counter ");
    Serial.println(buzzer_count);

    if (counter > buzzer_count)
    {
      digitalWrite(Buzzer, HIGH);
      buzzer_count++;
      delay(1000);
      digitalWrite(Buzzer, LOW);
    }
   switch (counter) {
    case 0:
      break;
    case 1:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 2:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 3:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 4:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 5:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 6:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 7:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 8:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 9:
      DrawSymbol(0, 16, 1);
      DrawSymbol(1, 25, 1);
      break;
    case 10:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 11:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 12:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 13:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 14:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 15:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 16:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 17:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 18:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 19:
      DrawSymbol(0, 17, 1);
      DrawSymbol(1, 25, 1);
      break;
    case 20:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 21:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 22:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 23:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 24:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 25:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 26:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 27:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 28:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 29:
      DrawSymbol(0, 18, 1);
      DrawSymbol(1, 25, 1);
      break;     
    case 30:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 31:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 32:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 33:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 34:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 35:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 36:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 37:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 38:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 39:
      DrawSymbol(0, 19, 1);
      DrawSymbol(1, 25, 1);
      break;
    case 40:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 41:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 42:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 43:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 44:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 45:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 46:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 47:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 48:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 49:
      DrawSymbol(0, 20, 1);
      DrawSymbol(1, 25, 1);
      break;
    case 50:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 51:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 52:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 53:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 54:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 55:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 56:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 57:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 58:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 59:
      DrawSymbol(0, 21, 1);
      DrawSymbol(1, 25, 1);
      break;
    case 60:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 61:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 62:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 63:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 64:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 65:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 66:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 67:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 68:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 69:
      DrawSymbol(0, 22, 1);
      DrawSymbol(1, 25, 1);
      break;
    case 70:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 71:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 72:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 73:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 74:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 75:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 76:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 77:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 78:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 79:
      DrawSymbol(0, 23, 1);
      DrawSymbol(1, 25, 1);
      break;
    case 80:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 81:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 82:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 83:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 84:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 85:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 86:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 87:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 88:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 89:
      DrawSymbol(0, 24, 1);
      DrawSymbol(1, 25, 1);
      break;
    case 90:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 16, 1);
      break;
    case 91:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 17, 1);
      break;
    case 92:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 18, 1);
      break;
    case 93:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 19, 1);
      break;
    case 94:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 20, 1);
      break;
    case 95:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 21, 1);
      break;
    case 96:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 22, 1);
      break;
    case 97:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 23, 1);
      break;
    case 98:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 24, 1);
      break;
    case 99:
      DrawSymbol(0, 25, 1);
      DrawSymbol(1, 25, 1);
      break;
  }
}
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }


  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>Your Token number</title> <style>";
  s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> Your Token number</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <tr> <th></th>";
  s += "<td ALIGN=CENTER >";
  s += counter;
  s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""https://thingspeak.com/channels/895185/private_show";
  s += """ target=""_top"">Click here!</a> To got o thingspeak server</p>";
  s += "</body> </html> \n";

  client.print(s);
  http.end();   //Close connection
  delay(2000);    //Send a request every 30 seconds
}
