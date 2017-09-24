/* Call server API for multiple sensor information [P002] : http://rdiot.tistory.com/328 [RDIoT Demo] */

#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <DHT11.h>

/*
- Arduino Uno R3
- Ethernet Shield
- LCD 1602
- DHT11
- Button
*/

/* --------------------------------------------------------------------
*  PIN
* -------------------------------------------------------------------*/
#define SENSOR_DHT11 2 // DHT11온도센서 D2
#define BTN 3 // Button D1
#define SENSOR_KY001 A0

/* --------------------------------------------------------------------
 *  Ethernet Config
 * -------------------------------------------------------------------*/
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; //MAC Address
char server[] = "54.65.30.222"; //API Server
String s_data1 = ""; // Request Data

/* --------------------------------------------------------------------
 *  Ethernet Library 초기화
 * -------------------------------------------------------------------*/
EthernetClient client; /* D10, D11, D12, D13 */

/* --------------------------------------------------------------------
 *  LCD 초기화
 * -------------------------------------------------------------------*/
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

/*
[LCD 핀번호][브레드보드][아두이노]
1.  Vss------------------ - GND(청색)------------------ - GND  ---> GND
2.  Vdd------------------ - VCC(적색)------------------ - 5V   ---> 5V (+)
3.  V0------------------ - GND(청색)                           ---> 저항 4.7K
4.  RS----------------------------------------------------D13  ------------> RS D9
5.  RW------------------ - GND(청색)                           ------------> GND
6.  E----------------------------------------------------D12   ------------> RS D8
7.  DB0(사용안함)
8.  DB1(사용안함)
9.  DB2(사용안함)
10. DB3(사용안함)
11  DB4----------------------------------------------------D11 ----------> 7
12  DB5----------------------------------------------------D10 ----------> 6
13  DB6----------------------------------------------------D9  ----------> 5
14  DB7----------------------------------------------------D8  ----------> 4
15  LEDA------------------ - VCC(적색)                         -----> 저항220옴 ---- VCC
16  LEDK------------------ - GND(청색)                         -----> GND
*/

/* --------------------------------------------------------------------
*  DHT11 초기화
* -------------------------------------------------------------------*/
DHT11 dht11(SENSOR_DHT11);

/* --------------------------------------------------------------------
*  전역변수
* -------------------------------------------------------------------*/
int counter = 0;

/* --------------------------------------------------------------------
*  SETUP
* -------------------------------------------------------------------*/
void setup() {
 /* PINMODE */
 pinMode(SENSOR_DHT11, INPUT); // DHT11 핀모드 설정
 pinMode(BTN, INPUT); // 버튼 핀모드 설정
 pinMode(SENSOR_KY001, INPUT);
 /* Serial Open */
 Serial.begin(9600);
 Serial.println("### setup start ###");
 /* LCD 초기화 */
 lcd.begin(16, 2);
 lcd.print("LCD-START");
 delay(100);
 lcd.clear();
 // Open serial communications and wait for port to open:
 // this check is only needed on the Leonardo:
 //while (!Serial) {
 // ; // wait for serial port to connect. Needed for Leonardo only
 //}
 
 // start the Ethernet connection:
 if (Ethernet.begin(mac) == 0) {
 Serial.println("Failed to configure Ethernet using DHCP");
 // no point in carrying on, so do nothing forevermore:
 for (;;)
 ;
 }
 // print your local IP address:
 Serial.print("My IP address: ");
 for (byte thisByte = 0; thisByte < 4; thisByte++) {
 // print the value of each byte of the IP address:
 Serial.print(Ethernet.localIP()[thisByte], DEC);
 Serial.print(".");
 }
 Serial.println();
 
}

/* --------------------------------------------------------------------
*  LOOP
* -------------------------------------------------------------------*/
void loop() {
 /*
 
 if (digitalRead(BTN) == HIGH) {
  delay(50); //누를 때의 채터링 방지용 딜레이
  while (digitalRead(BTN) == HIGH); // 스위치에서 손을 뗄 때까지 대기
  delay(50); //뗄 때의 채터링 방지용 딜레이
  if (counter == 5)
   counter = 0;
  else
   counter++;
  Serial.print("counter-" + (String)counter);
 }
 //delay(100);
 //카운터가 0일때는 화면을 클리어 한다.
 if (counter == 0)
 {
  delay(500);
  lcd.print("Button Push..");
  delay(500);
  lcd.clear();
 }
 else if (counter == 1)
 {
  getDht11();
  lcd.clear();
 }
 else if (counter == 2)
 {
  lcd.print("2");
  lcd.clear();
 }
 else if (counter == 3)
 {
  lcd.print("3");
  delay(500);
  lcd.clear();
 }
 else if (counter == 4)
 {
  lcd.print("4");
  delay(500);
  lcd.clear();
 }
 else if (counter == 5)
 {
  lcd.print("5");
  delay(500);
  lcd.clear();
 }
 */
 getDht11();
 //lcd.clear();

 String val1 = getDht11_C();
 String val2 = (String)val1.substring(0).toInt();
 //센서1의 값 가져오기 - DHT11 온도
 s_data1 = "sensor_id=S007&";
 s_data1 += "sensor_board=ArduinoUno&";
 s_data1 += "sensor_shield=W5100&";
 s_data1 += "sensor_comm=Ethernet&";
 s_data1 += "sensor_name=KY015&";
 s_data1 += "sensor_func=F01&";
 s_data1 += "sensor_value=" + val2 + "&";
 s_data1 += "sensor_value_desc="+ val1 +"C";
 //Serial.println(s_data1);
 //Serial.println("S007-"+ (String)analogRead(A0));

 //서버 연결
 client.stop();
 connect_server();
 if (client.connected())
 {
 Serial.println("c1");
 client.println("POST /iotHome-api/v1/sensor/post-form2xml-mysqldb HTTP/1.0");
 client.println("Host: 54.65.30.222");
 client.println("User-Agent: Arduino");
 client.println("Connection: close");
 client.println("Content-Type: application/x-www-form-urlencoded");
 client.print("Content-Length: ");
 client.println(s_data1.length());
 client.println();
 client.println(s_data1);
 //lcd.print("sent S007-F01");
 //lcd.clear();
 }
 delay(1000);

 val1 = getDht11_F();
 val2 = (String)val1.substring(0).toInt();
 //센서2의 값 가져오기 - DHT11 습도
 s_data1 = "sensor_id=S007&";
 s_data1 += "sensor_board=ArduinoUno&";
 s_data1 += "sensor_shield=W5100&";
 s_data1 += "sensor_comm=Ethernet&"; 
 s_data1 += "sensor_name=KY015&";
 s_data1 += "sensor_func=F02&";
 s_data1 += "sensor_value=" + val2 + "&";
 s_data1 += "sensor_value_desc="+ val1 +"H";
 //Serial.println(s_data1);
 //Serial.println("S007-" + (String)analogRead(A1));
 //서버 연결
 client.stop();
 connect_server();
 if (client.connected())
 {
 Serial.println("c2");
 client.println("POST /iotHome-api/v1/sensor/post-form2xml-mysqldb HTTP/1.0");
 client.println("Host: 54.65.30.222");
 client.println("User-Agent: Arduino");
 client.println("Connection: close");
 client.println("Content-Type: application/x-www-form-urlencoded");
 client.print("Content-Length: ");
 client.println(s_data1.length());
 client.println();
 client.println(s_data1);
 //lcd.print("sent S007-F02");
 //lcd.clear();
 }
 delay(60000);
 
}
 

void connect_server() {
 //서버 연결
 if (client.connect(server, 80)) {
  Serial.print("# server connected-");
 }
 //서버 연결 실패시 
 else {
  Serial.println("server connection failed");
  setup();
  /*
  digitalWrite(led_red, HIGH);
  delay(100);
  digitalWrite(led_red, LOW);
  delay(100);
  digitalWrite(led_red, HIGH);
  delay(100);
  digitalWrite(led_red, LOW);
  delay(100);
  digitalWrite(led_red, HIGH);
  delay(100);
  digitalWrite(led_red, LOW);
  */
 }
}
 
int GetTemper(int v)
{
 // 29.0 도 일때 adc 값 172
 // 34.1 도 일때 adc 값 160
 return (v - 172)* ((341 - 290) / (160 - 172)) + 290;
}

/* DHT11 */
void getDht11()
{
 int err;
 float humi;
 float tem;
 // dht11.read() 함수는 DHT11 센서로부터 측정된 값을 읽으며,
 // 성공시에는 0을 반환하고, 실패시 에러코드를 반환합니다.
 if ((err = dht11.read(humi, tem)) == 0) {

  // float 데이터 타입의 온도 값을 문자 배열로 변환하고 소숫점 1자리만 유지하도록 합니다.
  char temCharValue[10];
  dtostrf(tem, 4, 1, temCharValue);
  // 온도 정보를 표시하기 위한 문자열을 생성합니다.
  String temDisplayStr = "1) Temper:";
  temDisplayStr += (String)temCharValue;
  temDisplayStr += "'C";
  // 1번째 줄 0번째 셀부터 입력하게 합니다.
  lcd.setCursor(0, 0);
  // 현재 온도를 LCD에 출력합니다.
  lcd.print(temDisplayStr);

  // float 데이터 타입의 습도 값을 문자 배열로 변환하고 소숫점 1자리만 유지하도록 합니다.
   char humiCharValue[10];
  dtostrf(humi, 4, 1, humiCharValue);
  // 습도 정보를 표시하기 위한 문자열을 생성합니다.
  String humiDisplayStr = " Humidity:";
  humiDisplayStr += (String)humiCharValue;
  humiDisplayStr += " %";
  //Serial.println(humiDisplayStr);
  // 0번째 줄 0번째 셀부터 입력하게 합니다.
  lcd.setCursor(0, 1);
  // 현재 습도를 LCD에 출력합니다.
  lcd.print(humiDisplayStr);

 }
 // 에러 발생시 에러번호와 에러를 시리얼 모니터에 출력합니다.
 else {
  Serial.println();
  Serial.print("Error No :");
  Serial.print(err);
  Serial.println();
 }
 // DHT11_RETRY_DELAY는 DHT11.h에 정의되어있으며, 1초(1000)입니다.
 // 따라서 1초동안 대기합니다.
 delay(DHT11_RETRY_DELAY);
}
String getDht11_C()
{
 int err;
 float humi;
 float tem;
 String str;
 // dht11.read() 함수는 DHT11 센서로부터 측정된 값을 읽으며,
 if ((err = dht11.read(humi, tem)) == 0) {

  // float 데이터 타입의 온도 값을 문자 배열로 변환하고 소숫점 1자리만 유지하도록 합니다.
  char temCharValue[10];
  dtostrf(tem, 4, 1, temCharValue);
   str = (String)temCharValue;
 }
 else
 {
  str = "0";
 }
 return str;
}

String getDht11_F()
{
 int err;
 float humi;
 float tem;
 String str;
 // dht11.read() 함수는 DHT11 센서로부터 측정된 값을 읽으며,
 if ((err = dht11.read(humi, tem)) == 0) {
  // float 데이터 타입의 온도 값을 문자 배열로 변환하고 소숫점 1자리만 유지하도록 합니다.
  char humiCharValue[10];
  dtostrf(humi, 4, 1, humiCharValue);
  str = (String)humiCharValue;
 }
 else
 {
  str = "0";
 }
 return str;
}
 
