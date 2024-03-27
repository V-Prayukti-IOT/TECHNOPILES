#include "ACS712.h"
ACS712 sensor(ACS712_05B, A0);

#include "AdafruitIO_WiFi.h"
#define IO_USERNAME  "lipin"
#define IO_KEY  "aio_JgLQ2486cm9C7cwPlEQRKkpXZ3gE"
#define WIFI_SSID "Lipin's galaxy a12"
#define WIFI_PASS "lipinisgood"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
int power = 0;
int p_threshold = 20;
float tot_power = 0;
float units=0;
// set up the 'analog' feed my feeds name gas

AdafruitIO_Feed *Power = io.feed("power_consumption");
AdafruitIO_Feed *email = io.feed("email");


void setup() {
  Serial.begin(115200);
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  sensor.callibrate();
}

void loop() 
{
  float i=sensor.getCurrentAC();
  if(i<0.05)
    i=0;
  Serial.println("Current : ");
  Serial.println(i);
  io.run();
  Serial.print("sending -> ");
  Serial.println(power);
  Power->save(power);//send data
  tot_power=tot_power+(power/3600);
  power=power+2;
  units = units + ((power/3600))/1000;
  if(power==p_threshold){
    email->save(power);
  }
  if(tot_power>5000 && tot_power<5050)
     email->save(tot_power);
  Serial.println(tot_power,5);
  delay(3000);
}
