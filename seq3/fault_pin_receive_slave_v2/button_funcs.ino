//********************************************************************* read sensors/SWs
int read_test()
{
  int x = digitalRead(test);
  return x;
}//int

int read_ack()
{
  int x = digitalRead(ack);
  return x;
}//int

int read_mute()
{
  int x = digitalRead(mute);
  return x;
}//int

int read_rst()
{
  int x = digitalRead(rst);
  return x;
}//int

//********************************************************************* button press actions
void mute_press_func()
{
  //...........................
  buz_state_flag = 0;
}//void

//.........................................................................
void ack_press_func()
{
  //...........................
  arr_pos_int = atoi(&redundancy_read_str[1]);
  pin = atoi(&redundancy_read_str[redundancy_read_str.indexOf(',') + 1]);
  //.................................
  alarm_arr_int = arr_pos_int;
  alarm_arr[alarm_arr_int] = pin;
  alarm_arr_int++;
  issues++;
  //.....................................
  print_arr(0);
  print_arr(1);
  //...........................
  buz_state_flag = 1;
}//void

//********************************************************************* read buttons
void read_buttons()
{
  //................................................. buz
  if (!read_mute())
  {
    mute_press_func();
  }//if

  //.....................................................ACK
  if (!read_ack() )
  {
    ack_press_func();
  }//if

  //..................................................... RST
  if (!read_rst() )
  {
    ;
  }//if
}//void
//*********************************************************************
