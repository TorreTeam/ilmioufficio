
#include <dht.h>

#define INTERVAL_DHT 10000
#define DELAY_TIME 5

// umidità & temperatura DHT11
int dht_int_pin = A1;
int dht_ext_pin = A0;

dht DHT;
double dht_ext_temperature = 0;
double dht_ext_humidity = 0;
double dht_time = 0;

//dht DHT;
double dht_int_temperature = 0;
double dht_int_humidity = 0;
double dht_int_time = 0;


// photoresistor      - funziona al contrario 0 high light 1024 low light
int photoresistor_pin = A2;
int photoresistor_value = 0;


// passaggio
int photo_1_pin = A3;
int photo_1_value = 0;
int photo_2_pin = A4;
int photo_2_value = 0;

int soglia1 = 0;
int soglia2 = 0;


// luce
int red_2_pin = 11;
int green_2_pin = 9;
int blue_2_pin = 10;

int red_1_pin = 6;
int green_1_pin = 3;
int blue_1_pin = 5;

int count = 0;


// funzioni
void dht_get();
void dht_print();



int cosa_sara = 0;

int count1 = 10;
int count2 = 5;

int night_day = 7;


void setup() {

  // impostazioni generali
  Serial.begin(115200);

  pinMode(red_1_pin, INPUT);
  pinMode(green_1_pin, INPUT);
  pinMode(blue_1_pin, INPUT);
  
    pinMode(red_2_pin, INPUT);
  pinMode(green_2_pin, INPUT);
  pinMode(blue_2_pin, INPUT);

  pinMode(night_day, INPUT_PULLUP);

  soglia1 = analogRead(photo_1_pin) * 2;
  soglia2 = analogRead(photo_2_pin) * 2;
  Serial.println(soglia1);
  Serial.println(soglia2);

}

void loop() {

  photo_1_value = analogRead(photo_1_pin);
  photo_2_value = analogRead(photo_2_pin);

  if (cosa_sara == 0) {

    if (photo_1_value > soglia1) {
      while (photo_1_value > soglia1) {
        photo_1_value = analogRead(photo_1_pin);
      }
      cosa_sara = 1;
    } else if (photo_2_value > soglia2) {
      while (photo_2_value > soglia2) {
        photo_2_value = analogRead(photo_2_pin);
      }
      cosa_sara = 2;
    }

  }
  if (cosa_sara == 1 && photo_2_value > soglia2) {
    count1 --;
    count2 ++;
    cosa_sara = 0;
    while (photo_2_value > soglia2) {
      photo_2_value = analogRead(photo_2_pin);
    }
  }
  if (cosa_sara == 2 && photo_1_value > soglia1) {
    count1 ++;
    count2 --;
    cosa_sara = 0;
    while (photo_1_value > soglia1) {
      photo_1_value = analogRead(photo_1_pin);
    }
  }


  photoresistor_value = analogRead(photoresistor_pin);

  if (count >= (100 * DELAY_TIME)) {
    dht_get();

    Serial.println("{");

    Serial.print("\"lightext\": ");
    Serial.print(photoresistor_value);
    Serial.println(",");

    Serial.print("\"peops1\": ");
    Serial.print(count1);
    Serial.println(",");

    Serial.print("\"peops2\": ");
    Serial.print(count2);
    Serial.println(",");

    dht_print();

    Serial.println("}");

    count = 0;
  }

  luce(photoresistor_value / 4, photoresistor_value / 4, photoresistor_value / 4);

  if (digitalRead(night_day) == 0) {
    luce(photoresistor_value / 4, photoresistor_value / 4, 0);
  }

  count += 1;
  delay(DELAY_TIME);
}

void dht_get() {
  double time_now = millis();

  if (time_now - dht_time > INTERVAL_DHT) {
    //Serial.println("AGGIORNO DHT");
    DHT.read11(dht_ext_pin);

    dht_ext_temperature = DHT.temperature;
    dht_ext_humidity = DHT.humidity;
    dht_time = millis();

    DHT.read11(dht_int_pin);
    dht_int_temperature = DHT.temperature;
    dht_int_humidity = DHT.humidity;

  }
}

void dht_print() {
  Serial.print("\"humsext\": ");

  Serial.print(dht_ext_humidity);

  Serial.println(",");

  Serial.print("\"tempsext\": ");

  Serial.print(dht_ext_temperature);

  Serial.println(",");

  Serial.print("\"hums1\": ");

  Serial.print(dht_int_humidity);

  Serial.println(",");

  Serial.print("\"temps1\": ");

  Serial.print(dht_int_temperature);

  Serial.println("");

  //Serial.print("\t time: ");

  //Serial.println(millis() - dht_time);

}

void luce(int red, int green, int blue) {

  if (count1 > 0) {
    analogWrite(red_1_pin, red);
    analogWrite(green_1_pin, green);
    analogWrite(blue_1_pin, blue);
  } else {
    analogWrite(red_1_pin, 0);
    analogWrite(green_1_pin, 0);
    analogWrite(blue_1_pin, 0);
  }

  if (count2 > 0) {
    analogWrite(red_2_pin, red);
    analogWrite(green_2_pin, green);
    analogWrite(blue_2_pin, blue);
  } else {
    analogWrite(red_2_pin, 0);
    analogWrite(green_2_pin, 0);
    analogWrite(blue_2_pin, 0);
  }

}

