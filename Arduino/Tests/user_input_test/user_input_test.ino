
unsigned long currentMillis;

//======for user question========
const char question[] = "Please type some text and press ENTER";
const unsigned long questionInterval = 5000;
unsigned long prevResponseMillis = 0;
boolean waitingForResponse = false;

//=====for user response=========
const byte buffSize = 31;
char userResponse[buffSize];
const char endMarker = '\r';
byte bytesRecvd = 0;
boolean ackResponse = false;


void setup() {
  Serial.begin(9600);
  Serial.println("Starting LessonG.ino");
  
  delay(5000);
}

void loop() {

    currentMillis = millis();
    askForUserInput();
    getUserResponse();
    acknowledgeResponse();

}

void askForUserInput() {
  if (waitingForResponse == true) {
    return;
  }
  
  if (currentMillis - prevResponseMillis >= questionInterval) {
    Serial.println(question);
    waitingForResponse = true;
    bytesRecvd = 0;
  }
}

void getUserResponse() {
  if (waitingForResponse == false) {
    return;
  }

  if(Serial.available() == 0) {
    return;
  }

  char inChar = Serial.read();
   
  if (inChar != endMarker) {
    userResponse[bytesRecvd] = inChar;
    bytesRecvd ++;
    if (bytesRecvd == buffSize) {
      bytesRecvd = buffSize - 1;
    }
  }
  else { // inChar is the endMarker
    waitingForResponse = false;
    userResponse[bytesRecvd] = 0;
    prevResponseMillis = currentMillis;
    ackResponse = true;
    
  }
}

void acknowledgeResponse() {
  if (ackResponse == false) {
    return;
  }
  Serial.println();
  Serial.println("Thank you");
  Serial.print("You entered the following ");
  Serial.print(bytesRecvd);
  Serial.println(" bytes ");
  Serial.print(" --- ");
  Serial.println(userResponse);
  Serial.println();
  
  ackResponse = false;
}