/*
    Simple Test Script for Single Sensor
*/




//===== LIBRARIES =====
#include <FileIO.h>
#include <Process.h>



//===== VARIABLES =====
Process date;

const int FSR_PIN = 0;
int pin_reading;
long cur_time_ms;

char filename[];
boolean ready_to_write = false;

// timer
unsigned long current_time_ms;

// user_prompt
const char user_question[] = "Please enter the subjects ID and Session Number (ex. NK94-1):  ";
const unsigned long question_interval = 5000;
unsigned long prev_response_ms = 0;
boolean waiting_on_response = false;

// get_user_response
const byte buffer_size = 31;
char user_answer[buffer_size];
const char end_marker = '\r';
byte received_bytes = 0;
boolean recognize_response = false;

//======================


//===== Main ===========
void setup(){
    Serial.begin(9600);
    Serial.println(" Welcome to Wiggle-o-Meter ");

    delay(5000);
}


void loop(){
    current_time_ms = millis();

    user_prompt();
    get_user_answer();
    acknowledge_response();
    write_watch();
}

//======================


//===== Methods ========
void user_prompt() {
    if (waiting_on_response == false){
        return;
    }
    if (current_time_ms - prev_response_ms >= question_interval){
        Serial.println(user_question);
        waiting_on_response = true;
        received_bytes = 0;
    }
}


void get_user_answer() {
    if (waiting_on_response == false){
        return;
    }
    if(Serial.available() == 0){
        return;
    }
    char incoming_char = Serial.read();
    if (incoming_char != end_marker){
        user_answer[received_bytes] = incoming_char;
        received_bytes++;
        if (received_bytes == buffer_size){
            received_bytes = buffer_size-1;
        }
    }
    // if incoming_char is end_marker
    else {
        waiting_on_response = false;
        user_answer[received_bytes] = 0;
        prev_response_ms = current_time_ms;
        recognize_response = true;
    }
}


void acknowledge_response(){
    if (recognize_response == false){
        return;
    }
    filename[] = String(user_answer);
    Serial.println();
    Serial.println("Answer Received...");
    Serial.println("The filename for this session will be called ");
    Serial.println(filename);
    Serial.println(".csv");
    Serial.println("Starting to record sensors...")
    ready_to_write = true;
    recognize_response = false;
}


void write_watch(){
    while (ready_to_write == true){
        data_save();
    }
}

void data_save(){
    String readings;
    readings += millis();

    int sensor_reading = analogRead(FSR_PIN);
    readings += sensor_reading;
    
    File data_file = FileSystem.open(filename, FILE_APPEND);

    if (data_file) {
        // write readings to data_file
        data_file.println(readings);
        data_file.close();
        // also print to serial 
        print_readings_to_serial(readings);
    }
}

void print_readings_to_serial(readings){
    Serial.println(readings);
}

