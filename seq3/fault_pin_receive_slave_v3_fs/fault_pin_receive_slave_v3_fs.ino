//A1,1.
//A201,200.
//A33,33.
#include <SoftwareSerial.h>

SoftwareSerial mySerial_slave(5, 15); // RX, TX

//************************************************************************ PushButtons
//int input = 15;  :(
int ack = 39;
int mute = 34;
int rst = 36;
int test = 35;

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
//String read_str = "";
int flag = 0;

int pin = 201;
int arr_pos_int = 0;

int print_flag = 0;

//***********************************************************************
TaskHandle_t Task1;

int softUart_select = 0;
int slave_start = 0;

//*********************************************************************** redundancy
String redundancy_str = "";
String redundancy_read_str = "";
int redundancy_str_read_flag = 0;
//1 -> redundance present, 0-> redundancy absent
//if 1-> transfer redundancy_str, 0-> do not transfer redundancy_str
int redundancy_flag = 0;
//if 0-> send redundancy_str, if 1-> send "X"
int kish_lock = 1;
//
String redundancy_reset_srt = "R0.";
//0 -> mute pressed, 1 -> not pressed
//1 -> normal, i.e. for master and for slave red absent, for red slave present but mute not pressed
//0 -> red slave present and mute pressed
int buz_state_flag = 1;

//**********************************************************************
void setup()
{
  Serial.begin(115200);
  delay(100);
  mySerial_slave.begin(9600);
  delay(100);

  Serial.println("Hello");
  delay(100);

  if (inital_uart_setup_slave())
  {
    Serial.println("starting slave");
    redundancy_flag = 1;
    delay(100);
    xTaskCreatePinnedToCore(task1Code, "Task1", 10000, NULL, 1, &Task1, 0);
  }//if

  reset_all();

  //.....................................................................
  if ((softUart_select == 0) && (redundancy_flag == 1))
  {
    while (!slave_start)
    {
      delay(1000);
//      if (redundancy_str_read_flag)
//      {
//        //........................
//        fill_arr();
//        //........................
//        redundancy_str_read_flag = 0;
//      }//if
    }//while
  }//if


}//setup

//**********************************************************************
void loop()
{
  Serial.println("in loop");
  delay(2000);
}//loop
