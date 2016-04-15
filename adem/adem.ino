#include <TickerSchedlr.h>
#include <Adafruit_NeoPixel.h>
#include <particulate_PPD42.h>
#include <barometer_BMP085.h>
#include <humidity_HTU21D.h>
//#include <gps_SoftwareSerial.h>

#define NEOPIXEL_PIN 5
#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 14
#define SERIAL_RX_PIN 8
#define SERIAL_TX_PIN 7

#define SERIAL_BAUD 74880

// Different states of the program
enum state_t {START, SLEEP, CONFIG, GPS_TEST, COLLECT, WIFI_TEST, UPLOAD, COLLECT_START, COLLECT_STOP, GPS_START, GPS_STOP, WIFI_START, WIFI_STOP, AP_START, AP_STOP};
state_t state = START;

// Objects for all the sensor libraries
//MPU6050Sensor accelerometer;
BMP085Sensor barometer;
//PassiveBuzzer buzzer;
//GPSSensor gps;
HTU21DFSensor humidity;
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
//LEDSensor led;
PPD42Sensor particulate;
//WIFIap wifiap;
//WIFIcl wificl;

// Scheduler
TickerSchedlr *schedule = 0; //TickerSchedlr::Instance(200); HERE OR IN SETUP?

// Tasks
TickerTask *accelerometer_task = NULL;
TickerTask *barometer_task = NULL;
TickerTask *buzzer_task = NULL;
TickerTask *gps_task = NULL;
TickerTask *humidity_task = NULL;
TickerTask *led_task = NULL;
TickerTask *particulate_task = NULL;
//TickerTask *wifiap_task = NULL;
//TickerTask *wificl_task = NULL;

// Task code blocks

void accelerometer_block(void *) {
  Serial.println(" :accelerometer_block: ");
}

void barometer_block(void *) {
  Serial.println(" :barometer_block: ");
  Serial.println(" -> Dump barometer record");
  Serial.print(barometer.report());
}

void buzzer_block(void *) {
//  Serial.println(" :buzzer_block: ");
}

void gps_block(void *) {
  Serial.println(" :gps_block: ");
  // gps.report();
  Serial.println(" -> Dump time record");
  Serial.println(" -> Dump location record");
}

void humidity_block(void *) {
  Serial.println(" :humidity_block: ");
  Serial.println(" -> Dump humidity record");
  Serial.print(humidity.report());
}

void led_block(void *) {
  Serial.println(" :led_block: ");
}

void particulate_block(void *) {
  Serial.println(" :particulate_block: ");
  Serial.println(" -> Dump particulate record");
  //Serial.print(particulate.report());
}

//void wifiap_block(void *) {
//  Serial.println(" :wifiap_block: ");
//}

//void wificlient_block(void *) {
//  Serial.println(" :wificlient_block: ");
//}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("Setup started.");
  Serial.println("Serial communication initialized...");

  neopixel.begin();
  neopixel.setBrightness(63);
  neopixel.setPixelColor(0, 63, 0, 0); neopixel.show(); // Red
  
  Serial.println("NeoPixel initialized...");

//  gps_Serial.begin(GPS_BAUD);  
//  Serial.print("GPS initialized...");

  Serial.print("Initializing PPD42...");
  particulate.begin();
  Serial.print(" done");

  humidity.begin();
  Serial.println("Humidity sensor initialized...");

  barometer.begin();
  Serial.println("Barometer sensor initialized...");

  schedule = TickerSchedlr::Instance(200); // HERE OR IN SETUP?
  accelerometer_task = TickerTask::createPeriodic(&accelerometer_block, 500);
  accelerometer_task->name = "accelerometer";
  barometer_task = TickerTask::createPeriodic(&barometer_block, 60000);
  barometer_task->name = "barometer";
  buzzer_task = TickerTask::createPeriodic(&buzzer_block, 100);
  buzzer_task->name = "buzzer";
  gps_task = TickerTask::createPeriodic(&gps_block, 1000);
  gps_task->name = "gps";
  humidity_task = TickerTask::createPeriodic(&humidity_block, 60000);
  humidity_task->name = "humidity";
  led_task = TickerTask::createPeriodic(&led_block, 1000);
  led_task->name = "led";
  particulate_task = TickerTask::createPeriodic(&particulate_block, 30000);
  particulate_task->name = "particulate";
  //wifiap_task = TickerTask::createPeriodic(&wifiap_block, 5000);
  //wifiap_task->name = "wifiap";
  //wificlient_task = TickerTask::createPeriodic(&wificlient_block, 5000);
  //wificlient_task->name = "wificl";

  schedule = TickerSchedlr::Instance();  // HERE OR IN SETUP? OR IS THIS DOUBLE
  
  Serial.print("Task accelerometer next time is "); Serial.println(accelerometer_task->getNextTime());
  Serial.print("Task accelerometer period is "); Serial.println(accelerometer_task->interval);
  Serial.print("Task accelerometer type is "); Serial.println(accelerometer_task->tasktype);
  Serial.print("Schedule time "); Serial.println(schedule->getTickTime());

  Serial.println("schedule added");
  Serial.println("Tasks created...");

  Serial.println("Setup finished.");

  state = SLEEP;
  neopixel.setPixelColor(0, 0, 0, 0); neopixel.show(); // Black
}

// the loop function runs over and over again until power down or reset
void loop() {
  
  if (state == SLEEP) {

    // if (accelerometer.moving) {
    if (true) {
      
        state = GPS_START;
        // gps.begin();
        // Activate gps task

        state = GPS_TEST;
        neopixel.setPixelColor(0, 63, 31, 0); neopixel.show(); // Orange
        
    } else {

      // if (! buffer.empty) {
      if (false) {
        
        state = WIFI_START;
        // wificlient.begin();
        // Activate wificlient task

        state = WIFI_TEST;
        neopixel.setPixelColor(0, 63, 0, 63); neopixel.show(); // Purple



      //} else if (accelerometer.shaking) {
      } else if (false) {
        
        state = AP_START;
        // wifiap.begin();
        // Activate wifiap task

        state = CONFIG;
        neopixel.setPixelColor(0, 0, 63, 63); neopixel.show(); // Yellow
        
      }
      
    }
  
  } else if (state == CONFIG) {

    //if (finished || timeout || canceled) {
    if (true) {

      state = AP_STOP;
      // Suspend wifiap task
      // wifiap.end();

      state = SLEEP;
      neopixel.setPixelColor(0, 0, 0, 0); neopixel.show(); // Black
      
    }
    
  } else if (state = GPS_TEST) {

    //if (accelerometer.moving) {
    if (true) {

      //if (gps.ready) {
      if (true) {
        
        state = COLLECT_START;
        barometer.begin();
        humidity.begin();
        particulate.begin();
        // Activate barometer task
        // Activate humidity task
        // Activate particulate task

        state = COLLECT;
        neopixel.setPixelColor(0, 0, 63, 0); neopixel.show(); // Green
        
      }
      
    } else {

      state = GPS_STOP;
      // Suspend barometer task
      // Suspend humidity task
      // Suspend particulate task
      // Suspend gps task
      // barometer.end();
      // humidity.end();
      // particulate.end();
      // gps.end();

      state = SLEEP;
      neopixel.setPixelColor(0, 0, 0, 0); neopixel.show(); // Black

    }
    
  } else if (state == COLLECT) {

    // sensor tasks should be reporting on their own
//    Serial.println("Collecting...");

    //if (! accelerometer.moving || buffer.empty || wifi.timeout )) {
    if (false) {
      
      state = GPS_TEST;
      neopixel.setPixelColor(0, 63, 31, 0); neopixel.show(); // Orange
      
    }
    
  } else if (state == WIFI_TEST) {

    //if (wificlient.fix) {
    if (false) {
      
      state = UPLOAD;
      neopixel.setPixelColor(0, 0, 0, 63); neopixel.show(); // Blue
      
    }

    //if (accelerometer.moving || buffer.empty || wificlient.timeout) {
    if (false) {
      state = WIFI_STOP;
      // Suspend wificlient task
      // wificlient.end();

      state = SLEEP;
      neopixel.setPixelColor(0, 0, 0, 0); neopixel.show(); // Black
      
    }
    
  } else if (state = UPLOAD) {

    // Upload action finishes successfully or times out
    // Create JSON of X records
    // Send to server
    // Empty datastore

    state = WIFI_TEST;
    neopixel.setPixelColor(0, 63, 0, 63); neopixel.show(); // Purple
 
  }
   
  schedule->tick();
}
