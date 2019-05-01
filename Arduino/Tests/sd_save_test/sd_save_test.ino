// simple SD saver for testing purposes



#include <FileIO.h>

void setup() {
  Bridge.begin();
  Serial.begin(9600);
  FileSystem.begin();

  while (!SerialUSB); // wait for Serial port to connect.
  SerialUSB.println("Filesystem datalogger\n");
}


void loop() {
  int dataString = 100;
  

  File dataFile = FileSystem.open("/mnt/sd/datalog.txt", FILE_APPEND);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    SerialUSB.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    SerialUSB.println("error opening datalog.txt");
  }

  delay(15000);

}
