#include "FS.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



extern "C"
{
#include <user_interface.h>
}

String val1 = "";

String val2 = "";

String SWITCH_1 = "";
String SWITCH_2 = "";
String SWITCH_3 = "";
String SWITCH_4 = "";

String wifi_C = "";
String ssid_C = "";
String pass_C = "";
String message = "";

String S_ssid = "";
String S_password = "";

const char *ssid = "....";
const char *password = "....";

// SSID-PASS
const char *ssid2 = "4PlugTest";
const char *password2 = "12345678";

String t1 = "L1";
String t2 = "L1";
String t3 = "L1";

String now_wifi_status = "";
String now_wifi_ssid = "";
String now_wifi_password = "";

int LIGH_DIM = 0;
/*
int GPIO_0 = 0; // 0 PIN
int GPIO_1 = 1; // TX PIN
int GPIO_2 = 2; // 2 PIN
int GPIO_3 = 3; // RX PIN
int GPIO_4 = 4; // 4 PIN ESP12E - SDA PIN
int GPIO_5 = 5; // 5 PIN ESP12E - SCL PIN
*/

int GPIO_SW1 = 12;
int GPIO_SW2 = 13;
int GPIO_SW3 = 14;
int GPIO_SW4 = 16;

String SW1_STATUS = "ON ";
String SW2_STATUS = "ON ";
String SW3_STATUS = "ON ";
String SW4_STATUS = "ON ";

String INTENRTET_STATUS = "DISCONNECT!";

LiquidCrystal_I2C lcd(0x27, 20, 4);

ESP8266WebServer server2(8082);
WiFiServer server(80);

IPAddress ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);

void handleRoot()
{
  val1 = server2.arg("SW");

  SWITCH_1 = server2.arg("sw1");
  SWITCH_2 = server2.arg("sw2");
  SWITCH_3 = server2.arg("sw3");
  SWITCH_4 = server2.arg("sw4");

  wifi_C = server2.arg("wifi");

  String s1 = server2.arg("s");
  String p1 = server2.arg("p");

  int check_wifi = 0;

  if (wifi_C == "NEW")
  {
    delay(500);
    now_wifi_status = "no";
    delay(500);
    now_wifi_ssid = s1;
    delay(500);
    now_wifi_password = p1;
    delay(500);

    int TIMER = 0;
    WiFiServer server(80);
    Serial.print("Connecting to ");
    Serial.println(s1);
    uint8_t mac[6]{0xb8, 0xd7, 0x64, 0x00, 0xfe, 0xef};
    wifi_set_macaddr(STATION_IF, mac);
    WiFi.hostname(ssid2);
    WiFi.begin(s1.c_str(), p1.c_str());

    while ((WiFi.status() != WL_CONNECTED) && TIMER < 6)
    {
      delay(500);
      TIMER++;
      Serial.print(".");
      if (TIMER == 4)
      {
        message = "<html><head><meta charset=\"UTF-8\"></head><body><script type=\"text/javascript\">alert('مهم :  لطفا جهت بررسی ارتباط پس از ۲۵ ثانیه دکمه تایید را فشار دهید. ');window.open('?', '_self');</script></body></html>";
        server2.send(200, "text/html", message);
      }
    }

    delay(3000);
    Serial.println("01010101010101011110101100011001");

    if (WiFi.status() == WL_CONNECTED)
    {

      /*
      Serial.println("WiFi connected");
      server.begin();
      Serial.println("Server started");
      Serial.print("Use this URL to connect: ");
      Serial.print("http://");
      Serial.print(WiFi.localIP());
      Serial.println("/");
   */

      Serial.print("------------------------------------------------");
      Serial.print("------------------------------------------------");

      check_wifi = 1;
      delay(500);
      now_wifi_status = "reconnect";
      delay(500);
      now_wifi_ssid = s1;
      delay(500);
      now_wifi_password = p1;
      delay(500);
      Serial.print("------------------------------------------------");
      Serial.print("------------------------------------------------");
    }
    Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

    ////////-////////  WRITE WLAN  DATA \\\\\\\-\\\\\\\


    delay(3000);
    File configFile = SPIFFS.open("/wifi_status.txt", "w");
    if (!configFile)
    {
      Serial.println("Failed to open STATUS.txt for writing");
      delay(1000);
    }
    else
    {
      Serial.println("Opened STATUS.txt for writing >>> " + now_wifi_status);
      delay(1000);
      configFile.println(now_wifi_status);
      configFile.close();
      Serial.println("Closed STATUS.txt.");
    }
    delay(3000);

    File configFile2 = SPIFFS.open("/wifi_ssid.txt", "w");
    if (!configFile2)
    {
      Serial.println("Failed to open wifi_ssid.txt for writing.");
      delay(1000);
    }
    else
    {
      Serial.println("Opened SSID.txt for writing >>> " + now_wifi_ssid);
      delay(1000);
      configFile2.println(s1);
      configFile2.close();
      Serial.println("Closed SSID.txt.");
    }

    delay(3000);

    File configFile3 = SPIFFS.open("/wifi_password.txt", "w");
    if (!configFile3)
    {
      Serial.println("Failed to open wifi_password.txt for writing");
      delay(1000);
    }
    else
    {
      Serial.println("Opened PASSWORD.txt for writing >>> " + now_wifi_password);
      delay(1000);
      configFile3.println(p1);
      configFile3.close();
      Serial.println("Closed PASSWORD.txt.");
    }

    delay(3000);
    ////////-//////// END - WRITE WLAN  DATA \\\\\\\-\\\\\\\
    //-----------------------------------------------------\\

  }

  if (val1 == "OK")
  {

    if (SWITCH_1 == "on")
    {
      digitalWrite(GPIO_SW1, HIGH);
      SW1_STATUS = "ON ";
      showDisplay(SW1_STATUS, SW2_STATUS, SW3_STATUS, SW4_STATUS);
    }
    if (SWITCH_1 == "off")
    {
      digitalWrite(GPIO_SW1, LOW);
      SW1_STATUS = "OFF";
      showDisplay(SW1_STATUS, SW2_STATUS, SW3_STATUS, SW4_STATUS);
    }

    if (SWITCH_2 == "on")
    {
      digitalWrite(GPIO_SW2, HIGH);
      SW2_STATUS = "ON ";
      showDisplay(SW1_STATUS, SW2_STATUS, SW3_STATUS, SW4_STATUS);
    }
    if (SWITCH_2 == "off")
    {
      digitalWrite(GPIO_SW2, LOW);
      SW2_STATUS = "OFF";
      showDisplay(SW1_STATUS, SW2_STATUS, SW3_STATUS, SW4_STATUS);
    }

    if (SWITCH_3 == "on")
    {
      digitalWrite(GPIO_SW3, HIGH);
      SW3_STATUS = "ON ";
      showDisplay(SW1_STATUS, SW2_STATUS, SW3_STATUS, SW4_STATUS);
    }
    if (SWITCH_3 == "off")
    {
      digitalWrite(GPIO_SW3, LOW);
      SW3_STATUS = "OFF";
      showDisplay(SW1_STATUS, SW2_STATUS, SW3_STATUS, SW4_STATUS);
    }

    if (SWITCH_4 == "on")
    {
      digitalWrite(GPIO_SW4, HIGH);
      SW4_STATUS = "ON ";
      showDisplay(SW1_STATUS, SW2_STATUS, SW3_STATUS, SW4_STATUS);
    }
    if (SWITCH_4 == "off")
    {
      digitalWrite(GPIO_SW4, LOW);
      SW4_STATUS = "OFF";
      showDisplay(SW1_STATUS, SW2_STATUS, SW3_STATUS, SW4_STATUS);
    }

    LIGH_DIM = val2.toInt();
    analogWrite(0, LIGH_DIM);

    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;
      String url = "http://koan.ir/KSHS/write.php";
      String postData = "state=";
      postData += String(LIGH_DIM);
      postData += "&t1=";
      postData += t1;
      postData += "&t2=";
      postData += t2;
      postData += "&t3=";
      postData += t3;

      const char *URL = url.c_str();
      const char *SHA1 = getSslFingerprint().c_str();
      http.begin(URL);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpCode = http.POST(postData);
      if (httpCode > 0)
      {
        String payload = http.getString();
        Serial.println(payload);
      }
      http.end(); //Close connection
    }
  }

  message = "<html ><head><meta charset=\"UTF-8\"><style>.rrttll {direction:rtl;text-align: right;}.btn {display: inline-block;font-weight: 400;text-align: center;white-space: nowrap;vertical-align: middle;-webkit-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;border: 1px solid transparent;padding: .375rem .75rem;font-size: 1rem;line-height: 1.5;border-radius: .25rem;transition: color .15s ease-in-out, background-color .15s ease-in-out, border-color .15s ease-in-out, box-shadow .15s ease-in-out;}.btn-info {background: #1976d2;border: 1px solid #1976d2;-webkit-box-shadow: 0 2px 2px 0 rgba(66, 165, 245, 0.14), 0 3px 1px -2px rgba(66, 165, 245, 0.2), 0 1px 5px 0 rgba(66, 165, 245, 0.12);box-shadow: 0 2px 2px 0 rgba(66, 165, 245, 0.14), 0 3px 1px -2px rgba(66, 165, 245, 0.2), 0 1px 5px 0 rgba(66, 165, 245, 0.12);-webkit-transition: 0.2s ease-in;-o-transition: 0.2s ease-in;transition: 0.2s ease-in;}.btn-warning {background: #ffb22b;-webkit-box-shadow: 0 2px 2px 0 rgba(248, 194, 0, 0.14), 0 3px 1px -2px rgba(248, 194, 0, 0.2), 0 1px 5px 0 rgba(248, 194, 0, 0.12);box-shadow: 0 2px 2px 0 rgba(248, 194, 0, 0.14), 0 3px 1px -2px rgba(248, 194, 0, 0.2), 0 1px 5px 0 rgba(248, 194, 0, 0.12);border: 1px solid #ffb22b;-webkit-transition: 0.2s ease-in;-o-transition: 0.2s ease-in;transition: 0.2s ease-in;color: #ffffff;}.btn-danger {background: #ef5350;border: 1px solid #ef5350;-webkit-box-shadow: 0 2px 2px 0 rgba(239, 83, 80, 0.14), 0 3px 1px -2px rgba(239, 83, 80, 0.2), 0 1px 5px 0 rgba(239, 83, 80, 0.12);box-shadow: 0 2px 2px 0 rgba(239, 83, 80, 0.14), 0 3px 1px -2px rgba(239, 83, 80, 0.2), 0 1px 5px 0 rgba(239, 83, 80, 0.12);-webkit-transition: 0.2s ease-in;-o-transition: 0.2s ease-in;transition: 0.2s ease-in;}.card {background: #ffffff none repeat scroll 0 0;margin: 15px 0;padding: 20px;border: 0 solid #e7e7e7;border-radius: 5px;box-shadow: 0 5px 20px 0 rgba(0,0,0,0.15);}.card1 {background-color: #fff;border: none;border-radius: .625rem;box-shadow: 0 0.46875rem 2.1875rem rgba(90,97,105,.1), 0 0.9375rem 1.40625rem rgba(90,97,105,.1), 0 0.25rem 0.53125rem rgba(90,97,105,.12), 0 0.125rem 0.1875rem rgba(90,97,105,.1);}.form-control {padding: .664rem 1rem;font-size: .95rem;line-height: 1.125rem;color: #495057;border: 1px solid #becad6;font-weight: 300;border-radius: .375rem;transition: border-color 250ms cubic-bezier(.27,.01,.38,1.06),box-shadow 250ms cubic-bezier(.27,.01,.38,1.06);}.btn-primary {background: #5c4ac7;border: 1px solid #5c4ac7;-webkit-box-shadow: 0 2px 2px 0 rgba(116, 96, 238, 0.14), 0 3px 1px -2px rgba(116, 96, 238, 0.2), 0 1px 5px 0 rgba(116 96, 238, 0.12);box-shadow: 0 2px 2px 0 rgba(116, 96, 238, 0.14), 0 3px 1px -2px rgba(116, 96, 238, 0.2), 0 1px 5px 0 rgba(116, 96, 238, 0.12);-webkit-transition: 0.2s ease-in;-o-transition: 0.2s ease-in;transition: 0.2s ease-in;}.slidecontainer{width: 100%;}.slider{-webkit-appearance: none;width: 100%;height: 3px;background: #eceeef;border-radius: 5px;box-shadow: inset 0 1px 2px rgba(90,97,105,.1);-webkit-transition: .2s;transition: opacity .2s;}.slider:hover {opacity: 1;}.slider::-webkit-slider-thumb {-webkit-appearance: none;appearance: none;width: 25px;height: 25px;border: 1px solid #e7e9ea;border-radius: 50%; width: 23px;height: 23px;box-shadow: 0 3px 15px rgba(90,97,105,.1), 0 2px 3px rgba(90,97,105,.2);background: #fff;transition: all 250ms cubic-bezier(.27,.01,.38,1.06);}.slider::-moz-range-thumb {width: 25px;height: 25px;background: #4CAF50;cursor: pointer;}</style></head><body style=\"direction:rtl\"><br><br><center> <div class=\" card \" style=\" width:60% \" >";
  message += "<h2 style=\"direction:rtl\"> وضعیت اینترنت : </h2>";
  if (WiFi.status() == WL_CONNECTED)
  {
    message += "<h2 style=\"direction:rtl\"> ارتباط برقرار است</h2>";
  }
  else
  {
    message += "<h2 style=\"direction:rtl\"> عدم ارتباط</h2>";
    message += "<h2 class=\"rrttll\"> دستگاه شما به شبکه اینترنت پیوسته نیست ! </h2>";
    message += "<h4 class=\"rrttll\"> این اتفاق به یکی از ۵ دلیل اصلی زیر ممکن است رخ داده باشد: </h4>";
    message += "<h5 class=\"rrttll\">۱. شما تمایل ندارید دستگاهتان به اینترنت پیوسته باشد</h5>";
    message += "<h5 class=\"rrttll\">۲. نام شبکه مودم اشتباه وارد شده است</h5>";
    message += "<h5 class=\"rrttll\">۳. گذرواژه شبکه مودم اشتباه وارد شده است</h5>";
    message += "<h5 class=\"rrttll\">۴. مودم خاموش است و یا از دسترس خارج شده است</h5>";
    message += "<h5 class=\"rrttll\">۵. مودم شما اجازه برقراری اتصال به گجت را نمی‌دهد که ممکن است ظرفیت تعداد اتصالات مودم شما پر شده باشد. این تعداد برای مودم‌های خانگی به طور معمول بین ۳۲ تا ۶۴ اتصال می‌باشد</h5>";
  }

  message += "<h4 style=\"direction:rtl\">  سوییچ اول </h4>";
  message += "<br><a class=\" btn btn-info \" style=\" color:#ffffff; text-decoration: none; \" onclick=\"sw1_on()\"> روشن کردن - Switch ON </a>   <a class=\" btn btn-danger \" style=\" color:#ffffff; text-decoration: none; \" onclick=\"sw1_off()\" > خاموش کردن - Switch OFF </a> <br>";
  message += " <script>function sw1_on()  {  window.open(\"?SW=OK&sw1=on\",\"_self\")}</script>";
  message += " <script>function sw1_off() {  window.open(\"?SW=OK&sw1=off\",\"_self\")}</script>";

  message += "<h4 style=\"direction:rtl\">  سوییچ دوم </h4>";
  message += "<br><a class=\" btn btn-info \" style=\" color:#ffffff; text-decoration: none; \" onclick=\"sw2_on()\"> روشن کردن - Switch ON </a>   <a class=\" btn btn-danger \" style=\" color:#ffffff; text-decoration: none; \"  onclick=\"sw2_off()\" > خاموش کردن - Switch OFF </a> <br>";
  message += " <script>function sw2_on()  {  window.open(\"?SW=OK&sw2=on\",\"_self\")}</script>";
  message += " <script>function sw2_off() {  window.open(\"?SW=OK&sw2=off\",\"_self\")}</script>";

  message += "<h4 style=\"direction:rtl\">  سوییچ سوم </h4>";
  message += "<br><a class=\" btn btn-info \" style=\" color:#ffffff; text-decoration: none; \" onclick=\"sw3_on()\"> روشن کردن - Switch ON </a>   <a class=\" btn btn-danger \" style=\" color:#ffffff; text-decoration: none; \"  onclick=\"sw3_off()\" > خاموش کردن - Switch OFF </a> <br>";
  message += " <script>function sw3_on()  {  window.open(\"?SW=OK&sw3=on\",\"_self\")}</script>";
  message += " <script>function sw3_off() {  window.open(\"?SW=OK&sw3=off\",\"_self\")}</script>";

  message += "<h4 style=\"direction:rtl\">  سوییچ چهارم </h4>";
  message += "<br><a class=\" btn btn-info \" style=\" color:#ffffff; text-decoration: none; \" onclick=\"sw4_on()\"> روشن کردن - Switch ON </a>   <a class=\" btn btn-danger \" style=\" color:#ffffff; text-decoration: none; \"  onclick=\"sw4_off()\" > خاموش کردن - Switch OFF </a> <br>";
  message += " <script>function sw4_on()  {  window.open(\"?SW=OK&sw4=on\",\"_self\")}</script>";
  message += " <script>function sw4_off() {  window.open(\"?SW=OK&sw4=off\",\"_self\")}</script>";

  message += "<br><br><br> <a style=\"direction:rtl\"> نام شبکه وای‌فای - WIFI SSID Name: </a><br> <input type=\"text\" id=\"myText1\" class=\" form-control \" value=\"\"  style=\"direction:ltr; text-align:left\"> <br><br>";
  message += "<a style=\"direction:rtl\"> گذرواژه - WIFI SSID Password: </a><br><input type=\"text\" id=\"myText2\" class=\" form-control \" value=\"\" style=\"direction:ltr; text-align:left\" ><br><br> <button class=\" btn btn-primary \" style=\"width:100%; color:#ffffff; text-decoration: none; \" onclick=\"myFunction()\">ذخیره - Save</button>";
  message += " <br> <br> <a class=\" btn btn-primary \" style=\"width:80%; color:#ffffff; text-decoration: none; \" href=\"?\" >بازنشانی صفحه - Reload Page</a> </div>";
  message += "<script>function myFunction() { var x = document.getElementById(\"myText1\").value; var y = document.getElementById(\"myText2\").value; window.open(\"?wifi=NEW&s=\"+x+\"&p=\"+y,\"_self\")}</script>";
  message += "</center> </body></html>\n";
  server2.send(200, "text/html", message);
}

//-------|-----------------------------|-------//
//-------|------- PORJECT SETUP -------|-------//
//-------|-----------------------------|-------//
void setup()
{
  Serial.begin(9600);
  delay(10);

  SPIFFS.begin();

  File wifi_status = SPIFFS.open("/wifi_status.txt", "r");
  File wifi_ssid = SPIFFS.open("/wifi_ssid.txt", "r");
  File wifi_password = SPIFFS.open("/wifi_password.txt", "r");

  delay(2000);

  if (!wifi_status)
  {
    File wifi_status = SPIFFS.open("/wifi_status.txt", "w");
    wifi_status.println("X");
    Serial.println("X");
  }
  else
  {
    while (wifi_status.available())
    {
      String line = wifi_status.readString();
      now_wifi_status = line;
      Serial.println("STATUS: " + now_wifi_status);
    }
  }
  wifi_status.close();

  delay(2000);

  if (!wifi_ssid)
  {
    File wifi_ssid = SPIFFS.open("/wifi_ssid.txt", "w");
    wifi_ssid.println("X");
  }
  else
  {
    while (wifi_ssid.available())
    {
      //Lets read line by line from the file
      String line = wifi_ssid.readStringUntil('\n');
      now_wifi_ssid = line;
      Serial.println("SSID: " + now_wifi_ssid);
    }
  }
  wifi_ssid.close();

  delay(2000);

  if (!wifi_password)
  {
    File wifi_password = SPIFFS.open("/wifi_password.txt", "w");
    wifi_password.println("X");
  }
  else
  {
    while (wifi_password.available())
    {
      String line = wifi_password.readString();
      now_wifi_password = line;
      Serial.println("PASS: " + now_wifi_password);
    }
  }
  wifi_password.close();

  delay(2000);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("+------------------+");
  lcd.setCursor(0, 1);
  lcd.print("|  KOAN Tech Group |");
  lcd.setCursor(0, 2);
  lcd.print("|  www.koanco.com  |");
  lcd.setCursor(0, 3);
  lcd.print("+------------------+");

  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");

  String line01 = "--------------------";
  String line02 = " P1: " + SW1_STATUS + " ||  P2: " + SW2_STATUS;
  String line03 = " P3: " + SW3_STATUS + " ||  P4: " + SW4_STATUS;
  String line04 = "--------------------";

  lcd.setCursor(0, 0);
  lcd.print(line01);
  lcd.setCursor(0, 1);
  lcd.print(line02);
  lcd.setCursor(0, 2);
  lcd.print(line03);
  lcd.setCursor(0, 3);
  lcd.print(line04);

  pinMode(GPIO_SW1, OUTPUT);
  digitalWrite(GPIO_SW1, HIGH);

  pinMode(GPIO_SW2, OUTPUT);
  digitalWrite(GPIO_SW2, HIGH);

  pinMode(GPIO_SW3, OUTPUT);
  digitalWrite(GPIO_SW3, HIGH);

  pinMode(GPIO_SW4, OUTPUT);
  digitalWrite(GPIO_SW4, HIGH);

  Serial.print("Configuring access point...");
  WiFi.disconnect(true);
  WiFi.setAutoReconnect(false);
  WiFi.setAutoConnect(false);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid2, password2);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server2.on("/", handleRoot);
  server2.begin();
}

int b = 0;
int wifi_counter = 0;

void loop()
{
  server2.handleClient();

  int pos;

  if (b >= 300)
  {
    if (WiFi.status() == WL_CONNECTED)
    {

      HTTPClient http;
      String url = "https://your-api?";

      String postData = "t1=";
      postData += t1;
      postData += "&t2=";
      postData += t2;
      postData += "&t3=";
      postData += t3;

      const char *URL = url.c_str();
      const char *SHA1 = getSslFingerprint().c_str();
      http.begin(URL);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpCode = http.POST(postData); //Send the request
      if (httpCode > 0)
      {                                    //Check the returning code
        String payload = http.getString(); //Get the request response payload
        Serial.println(payload);           //Print the response payload

        LIGH_DIM = payload.toInt();
        analogWrite(0, LIGH_DIM);
        //  showDisplay();
      }
      http.end(); //Close connection
    }

    b = 0;
  }

  if (wifi_counter >= 1500)
  {

    if (WiFi.status() == WL_CONNECTED)
    {
      /////
    }
    else
    {

      Serial.print("Scan start ... ");
      int n = WiFi.scanNetworks();
      Serial.print(n);
      Serial.println(" network(s) found");
      for (int i = 0; i < n; i++)
      {

        String outWIFI = WiFi.SSID(i);
        outWIFI.replace("\n", "");
        outWIFI.replace("\n \r", "");
        outWIFI.replace("\n\r", "");
        outWIFI.replace("\r", "");

        String myWIFI = now_wifi_ssid;
        myWIFI.replace("\n", "");
        myWIFI.replace("\n \r", "");
        myWIFI.replace("\n\r", "");
        myWIFI.replace("\r", "");

        String myPASS = now_wifi_password;
        myPASS.replace("\n", "");
        myPASS.replace("\n \r", "");
        myPASS.replace("\n\r", "");
        myPASS.replace("\r", "");

        if (outWIFI == myWIFI)
        {

          Serial.println("x x x x ");
          int TIMER = 0;
          WiFiServer server(80);
          Serial.print("Connecting to ");
          Serial.println(now_wifi_ssid);
          uint8_t mac[6]{0xb8, 0xd7, 0x64, 0x00, 0xfe, 0xef};
          wifi_set_macaddr(STATION_IF, mac);
          WiFi.hostname(ssid2);
          WiFi.begin(myWIFI.c_str(), myPASS.c_str());

          while ((WiFi.status() != WL_CONNECTED) && TIMER < 6)
          {
            delay(500);
            TIMER++;
            Serial.print(".");
          }
        }
        //  else
        //  {
        //   Serial.println("Out WIFI:" + WiFi.SSID(i));
        //   Serial.println("My  WIFI:" + now_wifi_ssid);
        //  }
      }
      Serial.println("");
      wifi_counter = 0;
    }
  }

  b++;
  wifi_counter++;

  delay(10);
  Serial.println("Client disonnected");
  Serial.println("");
}

String getSslFingerprint()
{
  HTTPClient httpSSL;
  String fingerprint = "";
  String urlSSL = "https://your-api?";
  const char *URLSSL = urlSSL.c_str();
  httpSSL.begin(URLSSL);
  int httpCodeSSL = httpSSL.GET();
  if (httpCodeSSL > 0)
  {
    fingerprint = httpSSL.getString();
  }
  httpSSL.end(); //Close connection
  return fingerprint;
}

String showDisplay(String s1, String s2, String s3, String s4)
{
  String line1 = "--------------------";
  String line2 = " P1: " + s1 + " ||  P2: " + s2;
  String line3 = " P3: " + s3 + " ||  P4: " + s4;
  String line4 = "--------------------";

  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  lcd.setCursor(0, 2);
  lcd.print(line3);
  lcd.setCursor(0, 3);
  lcd.print(line4);
}
