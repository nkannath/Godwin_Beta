/*
    Code to test custom filenames
*/


// VARIABLES
unsigned long current_ms;

// ==== ask for subject ID and session number
const char ID_SESSION_QUESTION[] = "Please enter the subject's ID and session number (ex: NK94--01):  ";
const unsigned long QUESTION_INTERVAL = 5000;
unsigned long prev_response_ms = 0;
bool wating_for_response = false;

// == get user ID and session number
const byte BUFFER_SIZE = 31;
char subject_ID_answer[BUFFER_SIZE];
const char END_MARKER = '\r';
byte bytes_received = 0;
bool acknowledge_response = false;

bool ready_to_write = false;

char cust_filename[] = "00000000.csv";


void setup(){
    Serial.begin(9600);
    Serial.println("Custom Filename Test");
    Serial.println();

    delay(500);
}


void loop(){

    current_ms = millis();
    askForSubjectInfo();
    getUserResponse();
    acknowledgeResponse();
    setReadyToWrite();
}


void askForSubjectInfo(){
    // prompt the user for subject info


    // do nothing if waiting for user to respond
    if (wating_for_response == true){
        return;
    }

    // ask user if the elapsed time is within a certain interval
    if (current_ms - prev_response_ms >= QUESTION_INTERVAL){
        Serial.println(ID_SESSION_QUESTION);
        wating_for_response = true;
        bytes_received = 0;
    }
}


void getUserResponse(){
    // get the user's answers for subject information


    // do nothing if question has not been asked
    if (wating_for_response == true){
        return;
    }

    // do nothing if serial port is not open
    if (Serial.available() == 0){
        return;
    }

    // declare local incoming character variable
    char in_char = Serial.read();

    // receive characters one at a time while they are not the end marker
    if (in_char != END_MARKER){
        subject_ID_answer[bytes_received] = in_char;
        bytes_received++;
        // limit the amount of bytes_received
        if (bytes_received == BUFFER_SIZE){
            bytes_received = BUFFER_SIZE - 1;
        }
    }
    else{
        wating_for_response = false;
        subject_ID_answer[bytes_received] = 0;
        prev_response_ms = current_ms;
        acknowledge_response = true;
    }
}

void acknowledgeResponse(){
    // recognize when user sends a response
    
    
    if (acknowledge_response = false){
        return;
    }
    Serial.println();
    Serial.println("You entered the subject's info as: ");
    Serial.println(subject_ID_answer);
    Serial.println();
    getFilename(cust_filename);
    acknowledge_response = false;
}


void getFilename(char *cust_filename){
    // set cust_filename according to users answer


    cust_filename[0] = subject_ID_answer[0];
    cust_filename[1] = subject_ID_answer[1];
    cust_filename[2] = subject_ID_answer[2];
    cust_filename[3] = subject_ID_answer[3];
    cust_filename[4] = subject_ID_answer[4];
    cust_filename[5] = subject_ID_answer[5];
    cust_filename[6] = subject_ID_answer[6];
    cust_filename[7] = subject_ID_answer[7];
    cust_filename[8] = '.';
    cust_filename[9] = 'c';
    cust_filename[10] = 's';
    cust_filename[11] = 'v';
    return;
}

void setReadyToWrite(){
    // if cust_filename has been changed, get ready to write
    
    if (cust_filename != '01234567.csv'){
        Serial.println("File " + cust_filename + "is ready to write to");
    }
    ready_to_write = true;
}