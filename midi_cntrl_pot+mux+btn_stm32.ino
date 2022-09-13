//astra@astra.org.ru

#include <Arduino.h>
#include <USBComposite.h>

USBMIDI midi;
// Pin Definitions //
/////////////////////////////////
const uint8 pot_pin = 5;
const uint8 pot_pin0 = 0;
const uint8 pot_pin1 = 1;
const uint8 pot_pin2 = 2;
const uint8 pot_pin3 = 3;
const uint8 pot_pin4 = 4;
const int selectPins[3] = {9, 6, 7}; // S0~2, S1~3, S2~4
const uint8 zInput = PB1; //A11; // Connect common (Z) to B1 (analog input)
const uint8 BUTTON1 = PB3; //1
const uint8 BUTTON2 = PB4; //2
const uint8 BUTTON3 = PB5; //3

unsigned int Val = 0;
unsigned int Val_7 = 0;
unsigned int Val_6 = 0;

int buttonState1 = LOW;
int buttonState2 = LOW;
int buttonState3 = LOW;

int buttonPrevState1 = LOW;
int buttonPrevState2 = LOW;
int buttonPrevState3 = LOW;

const uint8 threshold = 2;

const unsigned int midi_channel =6; // this might show up as channel 6 depending on start index
const unsigned int midi_channel0 = 1;   
const unsigned int midi_channel1 = 2;
const unsigned int midi_channel2 = 3;
const unsigned int midi_channel3 = 4;
const unsigned int midi_channel4 = 5;



const unsigned int cc_command = 7; // bank select command 0
const unsigned int cc_command6 = 71; // bank select command 0
const unsigned int cc_command7 = 74; // bank select command 0


unsigned int old_value = 0;
unsigned int new_value = 0;
unsigned int old_value0 = 0;
unsigned int new_value0 = 0;
unsigned int old_value1 = 0;
unsigned int new_value1 = 0;
unsigned int old_value2 = 0;
unsigned int new_value2 = 0;
unsigned int old_value3 = 0;
unsigned int new_value3 = 0;
unsigned int old_value4 = 0;
unsigned int new_value4 = 0;
unsigned int old_valMux6 = 0;
unsigned int new_valMux6 = 0;
unsigned int old_valMux7 = 0;
unsigned int new_valMux7 = 0;

void setup() {
 Serial.begin(9600); // Initialize the serial port
  // product id taken from library example
  USBComposite.setProductId(0x0031);
  
   pinMode(pot_pin, INPUT);
   pinMode(pot_pin0, INPUT);
   pinMode(pot_pin1, INPUT);
   pinMode(pot_pin2, INPUT);
   pinMode(pot_pin3, INPUT);
   pinMode(pot_pin4, INPUT);

   pinMode(BUTTON1, INPUT_PULLUP);
   pinMode(BUTTON2, INPUT_PULLUP);
   pinMode(BUTTON3, INPUT_PULLUP);

// Set up the select pins as outputs:

for (int i=0; i<3; i++)
{
pinMode(selectPins[i], OUTPUT);
digitalWrite(selectPins[i], HIGH);
}

pinMode(zInput, INPUT); // Set up Z as an input
   
  midi.begin();
  delay(1000);
}

void loop() {

  int temp = analogRead(pot_pin); // a value between 0-4095
  int temp0 = analogRead(pot_pin0);
  int temp1 = analogRead(pot_pin1); // a value between 0-4095
  int temp2 = analogRead(pot_pin2);
  int temp3 = analogRead(pot_pin3); // a value between 0-4095
  int temp4 = analogRead(pot_pin4);
  buttonState1 = digitalRead(BUTTON1);
  buttonState2 = digitalRead(BUTTON2);
  buttonState3 = digitalRead(BUTTON3);


  
  new_value = temp / 32;          // convert to a value between 0-127
  new_value0 = temp0 / 32;
  new_value1 = temp1 / 32;          // convert to a value between 0-127
  new_value2 = temp2 / 32;
  new_value3 = temp3 / 32;          // convert to a value between 0-127
  new_value4 = temp4 / 32;
////////////////////////////////////////////////////////////////////////////////
  if (buttonState1 != buttonPrevState1) {
    button1Handler();  ///////////// Detecting pressing or releasing the button
  }
  if (buttonState2 != buttonPrevState2){
    button2Handler();
  }
  if (buttonState3 != buttonPrevState3){
    button3Handler();
  }
////////////////////////////////////////////////////////////////////////////////////
/////////////                 reading values on multiplexed inputs     ------------
    selectMuxPin(7); // Select one at a time

    int inputValue = analogRead(zInput); // and read Z
       
    new_valMux7 = inputValue / 32;
   
   // Serial.print("7 = ");
    //Serial.println(Val_7);
    
    selectMuxPin(6); // Select one at a time

    int inputValue6 = analogRead(zInput); // and read Z
       
    new_valMux6 = inputValue6 / 32;
   
////////////////////////////////////////////////////////////////////////////////////////////////  

// If difference between new_value and old_value is grater than threshold mux 666666666666666
  if (new_valMux6 > old_valMux6 && new_valMux6 - old_valMux6 > threshold ||
      new_valMux6 < old_valMux6 && old_valMux6 - new_valMux6 > threshold) {
/////------------ sending a midi CC command ----------------
    midi.sendControlChange(midi_channel0, cc_command6, new_valMux6);

 //------------ Update old_value---------
    old_valMux6 = new_valMux6;
  }

// If difference between new_value and old_value is grater than threshold mux 77777777777
  if (new_valMux7 > old_valMux7 && new_valMux7 - old_valMux7 > threshold ||
      new_valMux7 < old_valMux7 && old_valMux7 - new_valMux7 > threshold) {
/////------------ sending a midi CC command ----------------
    midi.sendControlChange(midi_channel0, cc_command7, new_valMux7);

   
    //----------- Update old_value --------------
    old_valMux7 = new_valMux7;
  }


  // If difference between new_value and old_value is grater than threshold
  if (new_value > old_value && new_value - old_value > threshold ||
      new_value < old_value && old_value - new_value > threshold) {
/////------------ sending a midi CC command ----------------
    midi.sendControlChange(midi_channel, cc_command, new_value);

    // Update old_value
    old_value = new_value;
  }

// 00000 If difference between new_value6 and old_value6 is grater than threshold
  if (new_value0 > old_value0 && new_value0 - old_value0 > threshold ||
      new_value0 < old_value0 && old_value0 - new_value0 > threshold) {

    midi.sendControlChange(midi_channel0, cc_command, new_value0);

    // Update old_value
    old_value0 = new_value0;
  }
  // 1111 If difference between new_value1 and old_value1 is grater than threshold
  if (new_value1 > old_value1 && new_value1 - old_value1 > threshold ||
      new_value1 < old_value1 && old_value1 - new_value1 > threshold) {

    midi.sendControlChange(midi_channel1, cc_command, new_value1);

    // Update old_value
    old_value1 = new_value1;
  }
  // 2222 If difference between new_value2 and old_value2 is grater than threshold
  if (new_value2 > old_value2 && new_value2 - old_value2 > threshold ||
      new_value2 < old_value2 && old_value2 - new_value2 > threshold) {

    midi.sendControlChange(midi_channel2, cc_command, new_value2);

    // Update old_value
    old_value2 = new_value2;
  }
  // 3333 If difference between new_value3 and old_value3 is grater than threshold
  if (new_value3 > old_value3 && new_value3 - old_value3 > threshold ||
      new_value3 < old_value3 && old_value3 - new_value3 > threshold) {

    midi.sendControlChange(midi_channel3, cc_command, new_value3);

    // Update old_value
    old_value3 = new_value3;
  }
  // 4444 If difference between new_value4 and old_value4 is grater than threshold
  if (new_value4 > old_value4 && new_value4 - old_value4 > threshold ||
      new_value4 < old_value4 && old_value4 - new_value4 > threshold) {

    midi.sendControlChange(midi_channel4, cc_command, new_value4);

    // Update old_value
    old_value4 = new_value4;
  }


  
  // Wait 50ms before reading the pin again
  delay(50);
}

// The selectMuxPin function sets the S0, S1, and S2 pins

// accordingly, given a pin from 0-7.

void selectMuxPin(byte pin)

{

  for (int i=0; i<3; i++)

  {

    if (pin & (1<<i))

      digitalWrite(selectPins[i], HIGH);

    else

      digitalWrite(selectPins[i], LOW);

  }

}
/////------------ this function handles events that have occurred with buttons ----------------
void button1Handler() {
  buttonPrevState1 = buttonState1;
  //digitalWrite(LED1, buttonState1);
  if (buttonState1 == HIGH) {
    //Serial.println("Button 1 is pressed");
//-------- Sending Realtime category messages -----------    
    midi.sendStart();
  } 
  }

void button2Handler() {
  buttonPrevState2 = buttonState2;
  //digitalWrite(LED1, buttonState2);
  
  if (buttonState2 == HIGH) {
    //Serial.println("Button 2 is pressed");
    midi.sendStop();
  } 
}

void button3Handler() {
  buttonPrevState3 = buttonState3;
  //digitalWrite(LED1, buttonState3);
  
  if (buttonState3 == HIGH) {
    //Serial.println("Button 3 is pressed");
//-------- Sending Realtime category messages -----------        
    midi.sendContinue();
  } 
}
  
