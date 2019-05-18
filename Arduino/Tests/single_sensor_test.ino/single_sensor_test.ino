/*
    Simple Test Script for Single Sensor
*/




//===== LIBRARIES =====
#include <FileIO.h>
#include <Process.h>



//===== VARIABLES =====
Process date;
const byte BUFFER_SIZE = 31;
const int FSR_PIN = 0;
int pin_reading;
long cur_time_ms;
boolean ready_to_write = false;

// timer
unsigned long current_time_ms;

// user_prompt
const char USER_QUESTION[] = "Please enter the subjects ID and Session Number (ex. NK94-1):  ";
const unsigned long QUESTION_INTERVAL = 5000;
unsigned long prev_response_ms = 0;
boolean waiting_on_response = false;

// get_user_response
char filename[BUFFER_SIZE];
const char END_MARKER = '\r';
byte received_bytes = 0;
boolean recognize_response = false;

// set_filename
char cust_filename[] = "01234567.csv";
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
    if (current_time_ms - prev_response_ms >= QUESTION_INTERVAL){
        Serial.println(USER_QUESTION);
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
    if (incoming_char != END_MARKER){
        filename[received_bytes] = incoming_char;
        received_bytes++;
        if (received_bytes == BUFFER_SIZE){
            received_bytes = BUFFER_SIZE-1;
        }
    }
    // if incoming_char is END_MARKER
    else {
        waiting_on_response = false;
        filename[received_bytes] = 0;
        prev_response_ms = current_time_ms;
        recognize_response = true;
    }
}


void acknowledge_response(){
    if (recognize_response == false){
        return;
    }
    Serial.println();
    Serial.println("Answer Received...");
    Serial.println("The filename for this session will be called ");
    Serial.println(filename);
    Serial.println(".csv");
    Serial.println("Starting to record sensors...");
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
    


    // NEEDS FIXING
    char filename_c = char(filename);
    char name_of_file[BUFFER_SIZE] = filename_c + '.csv';
    
    File data_file = FileSystem.open(name_of_file, FILE_APPEND);

    if (data_file) {
        // write readings to data_file
        data_file.println(readings);
        data_file.close();
        // also print to serial 
        print_readings_to_serial(readings);
    }
}


void print_readings_to_serial(String readings){
    Serial.println(readings);
}


void set_filename(char *cust_filename){
    // sets filename for sd card
    filename[0] = '2';
    filename[1] = '0';
    filename[2] = year%10 + '0';
    filename[3] = year%10 + '0';
    filename[4] = month/10 + '0';
    filename[5] = month%10 + '0';
    filename[6] = day/10 + '0';
    filename[7] = day%10 + '0';
    filename[8] = '.';
    filename[9] = 'c';
    filename[10] = 's';
    filename[11] = 'v';
    return; 
}

