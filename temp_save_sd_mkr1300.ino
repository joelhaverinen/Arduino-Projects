
#include <Arduino_MKRENV.h>
// Libraries for SD card
#include <SPI.h>
#include <SD.h>
File myFile;

// Counter for measurement number
int measurementCount = 1;

// Time measurement variables
unsigned long previousMillis = 0;
const unsigned long interval = 5000; // Time interval in milliseconds (5 seconds)

// File name variables
int fileNumber = 1;
String fileName;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize SD card
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    
  }
  Serial.println("initialization done.");

  // Find an available file name for writing
  fileName = getAvailableFileName();

  // Open the file for writing in .csv format
  myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    // Write the header line
    myFile.println("Measurement Number, Temperature (C), Humidity (%), Pressure (kPa), Counted time (ms)");  // lisattu v3  --- voltage 2 ja voltage 3
    myFile.close();
  } else {
    Serial.println("error opening file: " + fileName);
    
  }

}

void loop() {
  // read all the sensor values
  float temperature = ENV.readTemperature();
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
 

  // print each of the sensor values
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure    = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  
  
  Serial.println(); // print an empty line

  // Get current time
  unsigned long currentMillis = millis();

  // Append sensor data to the .csv file
  myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    // Write the data in .csv format               ---------------------
    myFile.print(measurementCount);
    myFile.print(",");
    myFile.print(temperature);
    myFile.print(",");
    myFile.print(humidity);  // lisattu v2
    myFile.print(",");   // lisattu v2
    myFile.print(pressure);  // lisattu v2
    myFile.print(",");   // lisattu v2
  
    myFile.println(currentMillis);  // Device own time
    myFile.close();
    measurementCount++;
  } else {
    Serial.println("error opening file: " + fileName);
  }

  // Check if the time interval has elapsed
  if (currentMillis - previousMillis >= interval) {
    // Update the previous time
    previousMillis = currentMillis;

    // Perform additional actions here, if desired
  }
  
  //delay(5000); // wait 5 second to print again
  
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(2500);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(2500); 

}


// Function to get an available file name by incrementing the file number
String getAvailableFileName() {
  String baseName = "data";
  String fileExt = ".csv";
  String newFileName;
  int tempFileNumber = fileNumber;

  while (true) {
    newFileName = baseName + "_" + String(tempFileNumber) + fileExt;
    if (!SD.exists(newFileName)) {
      break;
    }
    tempFileNumber++;
  }

  fileNumber = tempFileNumber;
  return newFileName;
}
