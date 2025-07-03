#include "BluetoothSerial.h"
#include <LiquidCrystal_I2C.h>
const int on_off_button = 14;
const int player1_score_increase = 35;
const int player1_score_decrease = 34;
const int first_button = 27;
const int second_button = 26;
const int led = 33;
unsigned long button_start = 0;
//adding player1_start
unsigned long player_start = 0;
//toggle player1 button
int count = 0; //add flag for counting how many times the button is press, once for player 1, and twice for second player
//toggle player1 button
bool p1_toggle = false; //increase button
bool m1_toggle = false; //decrease button
int player1_score = 0; //adding score for p1
int player2_score = 0; //adding score for p2
bool p1_state = false;
bool p2_state = false;
bool led_state = false;
bool toggle = false;
bool p_toggle = false;
bool player_state = false;//adding the flag for the player1
char buffer[17];
char incoming_char;
BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
  // put your setup code here, to run once:
  
  pinMode(on_off_button, INPUT_PULLUP);//the button press is high at its inital state
  //pinMode(increase_button, INPUT_PULLUP);
  //pinMode(decrease_button, INPUT_PULLUP);
  pinMode(player1_score_increase,INPUT_PULLUP);
  pinMode(player1_score_decrease,INPUT_PULLUP);
  pinMode(led,OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("ESP32");
  //setting up for the lcd I2C display 
  lcd.init();                      // Initialize the LCD
  lcd.noBacklight();                 // Turn on backlight

  //end for lcd set up

}

void loop() {
 
  if(SerialBT.available()){
    incoming_char = SerialBT.read();
  }

  if(digitalRead(on_off_button) == LOW) //if the button is pressed
  
  {
    
    if(button_start == 0)
    {
      button_start = millis();
    }
    


   

  //if the button is pressed over 3 seconds
    if(!toggle && millis() - button_start > 4000)
    {
      led_state = !led_state;
      digitalWrite(led, led_state?HIGH:LOW);
      toggle = true;
      SerialBT.write('A');
      if(led_state)
      {
        p1_state = true;
        p2_state = false;
        lcd.backlight();
        sprintf(buffer, "%02d - %02d",player1_score,player2_score);
        lcd.print(buffer);
      }
      else if(!led_state)
      {
        lcd.clear();
        lcd.noBacklight();
        player1_score =0;
        player2_score =0;
      }
    }
    
  }
  else
  {
    button_start = 0;
    toggle = false;
   
  }

  //FOR choosing the player
  if(led_state)
  
  {
    if(digitalRead(on_off_button) == LOW) //if the button is pressed
  
    {
    
      if(button_start == 0)
      {
        button_start = millis();
      }
    
      if(!p_toggle && millis() - button_start > 200)
      {
      
        p_toggle = true;
        count++;
        if(count == 1)
        {
          SerialBT.write('D');
          //changing the score for the player
          p1_state = true;
          p2_state = false;
          lcd.setCursor(10, 0);
          lcd.print("P1");
         
        }
        //make another if for if pressing the button is less than 200
        else if(count ==2 )
        {
          SerialBT.write('E');
          p2_state = true;
          p1_state = false;
          lcd.setCursor(10, 0);
          lcd.print("P2");
        }
        else if(count > 2)
        {
          count =1;
          SerialBT.write('D');
          p1_state = true;
          p2_state = false;
          lcd.setCursor(10, 0);
          lcd.print("P1");
        }
      }
    
    }
    else
    {
      button_start = 0;
      p_toggle = false;
    }
    //when the player1 is active
    if(p1_state)
    {
      if (digitalRead(player1_score_decrease) == LOW)
      {
        if (player_start == 0) {
          player_start = millis();  

        }
        if (!m1_toggle && millis() - player_start > 100) 
        {
          SerialBT.write('C');
        //Serial.println("Score increases");
          m1_toggle = true; // prevent multiple triggers
          player1_score--;
          lcd.setCursor(0, 0);
          sprintf(buffer, "%02d - %02d",player1_score,player2_score);
          lcd.print(buffer);
          
        }
        
        
      }
      else
      {
        m1_toggle = false;
    
        player_start = 0;
      }

      if (digitalRead(player1_score_increase) == LOW) 
      {
        if (player_start == 0) 
        {
          player_start = millis();
        }
        if (!p1_toggle && millis() - player_start > 100) 
        {
          SerialBT.write('B');
          player1_score++;
          p1_toggle = true;
          lcd.setCursor(0, 0);
         
          sprintf(buffer, "%02d - %02d",player1_score,player2_score);
          lcd.print(buffer);
          
        }
        
      }
      else
      {
        p1_toggle = false;
        player_start = 0;
      } 
    }

    if(p2_state)
    {
      if (digitalRead(player1_score_decrease) == LOW)
      {
        if (player_start == 0) {
          player_start = millis();
        }
        if (!m1_toggle && millis() - player_start > 100) 
        {
          SerialBT.write('C');
        //Serial.println("Score increases");
          player2_score--;
          m1_toggle = true; // prevent multiple triggers
          lcd.setCursor(0, 0);
          sprintf(buffer, "%02d - %02d",player1_score,player2_score);
          lcd.print(buffer);
        }
        
      }
      else
      {
        m1_toggle = false;
    
        player_start = 0;
      }

      if (digitalRead(player1_score_increase) == LOW) 
      {
        if (player_start == 0) 
        {
          player_start = millis();
        }
        if (!p1_toggle && millis() - player_start > 100) 
        {
          SerialBT.write('B');
          player2_score++;
          p1_toggle = true;
          lcd.setCursor(0, 0);
          sprintf(buffer, "%02d - %02d",player1_score,player2_score);
          lcd.print(buffer);
        }
        
      }
      else
      {
        p1_toggle = false;
        player_start = 0;
      } 
    }




  }

}

