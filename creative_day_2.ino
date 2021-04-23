int cabinLight = 12; // white LED for cabin light
int cabinLightSwitch = 8; // dip switch to activate cabin light
int backupLight = 13; // red LED represents backup lighting
int sensorPin = A0; // photoresistor

int solarValue; // variable to represent solar quality
int solarMin = 550; // minimum acceptable solar value for charge

unsigned int batteryCap = 10000; // total capacity of batteries
int batteryStatus = 2500; // current battery charge
int lifeSupport = 250; // power requirement for life support functions
int lightCost = 150; // power requirement for cabin light

int batteryR1 = 7;
int batteryR2 = 6;
int batteryY1 = 5;
int batteryY2 = 4;
int batteryG1 = 3;
int batteryG2 = 2;

int rgb_Red = 11;
int rgb_Green = 10;
int rgb_Blue = 9;

// Charge battery using solar, unless battery has reached zero
// don't allow battery to exceed full charge
void batteryCharge(){
  if(batteryStatus <= 0){
    batteryStatus = 0;
  } else {
      batteryStatus += solarValue;
  }
  if(batteryStatus >= batteryCap){
    batteryStatus = batteryCap;
  }
}

// Consume power for life support and cabin light functions
// but don't drop below zero
void batteryConsume(){
  lifeSupport = random(250, 301);
  if(batteryStatus <= lifeSupport){
    batteryStatus = 0;
  } else {
  batteryStatus -= lifeSupport;
  }
  if(digitalRead(cabinLight) == HIGH){
    batteryStatus -= lightCost;
  }
}

double getBatteryPercentage(){
  return(((double)batteryStatus / (double)batteryCap) * 100);
}

void printBatteryPercentage(){
  if(batteryStatus == 0){
    Serial.println("You have died of asphyxia.");
  } else {
  Serial.print(((double)batteryStatus / (double)batteryCap) * 100);
  Serial.print("%");
  Serial.print("\t\t");
  Serial.print((double)batteryStatus);
  Serial.print("\t\t");
  Serial.print(solarValue);
  Serial.print("\t\t");
  Serial.println(lifeSupport);
  }
}

void showBatteryCharge(){
  double percentage = getBatteryPercentage();
  if(percentage >= 10){
    digitalWrite(batteryR1, HIGH);
  } else {
    digitalWrite(batteryR1, LOW);
  }
  if(percentage >= 25){
    digitalWrite(batteryR2, HIGH);
  } else {
    digitalWrite(batteryR2, LOW);
  }
  if(percentage >= 40){
    digitalWrite(batteryY1, HIGH);
  } else {
    digitalWrite(batteryY1, LOW);
  }
  if(percentage >= 60){
    digitalWrite(batteryY2, HIGH);
  } else {
    digitalWrite(batteryY2, LOW);
  }
  if(percentage >= 80){
    digitalWrite(batteryG1, HIGH);
  } else {
    digitalWrite(batteryG1, LOW);
  }
  if(percentage >= 95){
    digitalWrite(batteryG2, HIGH);
  } else {
    digitalWrite(batteryG2, LOW);
  }
}

void showSolarValue(int red_Value, int green_Value, int blue_Value){
  analogWrite(rgb_Red, red_Value);
  analogWrite(rgb_Green, green_Value);
  analogWrite(rgb_Blue, blue_Value);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(cabinLight, OUTPUT);
  pinMode(backupLight, OUTPUT);
  pinMode(cabinLightSwitch, INPUT_PULLUP);
  
  pinMode(batteryR1, OUTPUT);
  pinMode(batteryR2, OUTPUT);
  pinMode(batteryY1, OUTPUT);
  pinMode(batteryY2, OUTPUT);
  pinMode(batteryG1, OUTPUT);
  pinMode(batteryG2, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // behavior of Cabin Lights and Backup Lights
  double percentage = getBatteryPercentage();
  if(digitalRead(cabinLightSwitch) == 1){
    digitalWrite(cabinLight, LOW);
  } else if(digitalRead(cabinLightSwitch) == 0 && digitalRead(batteryR1) == HIGH){
    digitalWrite(cabinLight, HIGH);
  }
  
  if(percentage < 10){
    digitalWrite(cabinLight, LOW);
  }
  
  if(digitalRead(cabinLight) == HIGH){
    digitalWrite(backupLight, LOW);
  } else if(percentage > 0){
    digitalWrite(backupLight, HIGH);
  }

  if(percentage <= 0){
    digitalWrite(backupLight, LOW);
  }

  // score of solarValue for loop cycle
  if(analogRead(sensorPin) > solarMin){
    solarValue = analogRead(sensorPin);  
  } else {
    solarValue = 0;
  }

  // Show solarValue score on RGB LED for the loop cycle
  if(solarValue >= 750){
    showSolarValue(0, 155, 0);
  } else if(solarValue < 750 && solarValue > solarMin){
    showSolarValue(155, 155, 0);
  } else {
    showSolarValue(155, 0, 0);
  }

  batteryCharge();
  showBatteryCharge();
  printBatteryPercentage();
  delay(1000);

  batteryConsume();
}
