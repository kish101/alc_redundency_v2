//A1,1.
//A201,200.
//A33,33.
#include <SoftwareSerial.h>

SoftwareSerial mySerial_slave(5, 15); // RX, TX

//********************************************************************** variables
int alarm_arr[200];
int alarm_arr_int = 0;
int after_ack_arr[200];
int after_ack_arr_int = 0;
int slow_blink_arr[200];
int slow_blink_arr_int = 0;
//int solid_state_arr[20];
//int solid_state_arr_int = 0;
int subq_arr[200];
int subq_arr_int = 0;
//int dashBoard_arr[16];
String dashBoard_str = "0000000000000000";
uint8_t dashBoard_wb_num = 0;
int ws_db_connect_flag = 0;

int alarm_blink_pin = 201;//33
int after_ack_alarm_pin = 201;//33

int issues = 0;
int solved_issues = 0;
String read_str = "";
int flag = 0;

int pin = 201;
int arr_pos_int = 0;

int print_flag = 0;

//**********************************************************************
void setup()
{
  Serial.begin(115200);
  delay(100);
  mySerial_slave.begin(9600);
  delay(100);

  Serial.println("Hello");

  reset_all();
}

//**********************************************************************
void loop()
{
  // put your main code here, to run repeatedly:
  if (mySerial_slave.available())
  {
    for (int i = 0; i < 9; i++)
    {
      char c = mySerial_slave.read();
//      Serial.println(c);

      if ( (c == 'P') )
      {
        mySerial_slave.print("P");
        break;
      }//if
       else if ( (c == 'X') || (c == '\n') || (c == '\r') || (c == '.'))
      {
        break;
      }//if
      else
      {
        read_str += c;
        delay(2);
        flag = 1;
      }
    }//for

  }//avai

  if (flag == 1)
  {
    Serial.println(read_str);
    //........................
    fill_arr();
    //........................
    mySerial_slave.print("K");
    read_str = "";
    flag = 0;
  }//if flag
}//loop
