
//*****************************************************************
void fill_arr(String temp_redundancy_read_str)
{
  switch (temp_redundancy_read_str[0])
  {
    case 'a':
      {
        //...........................
        arr_pos_int = atoi(&temp_redundancy_read_str[1]);
        pin = atoi(&temp_redundancy_read_str[temp_redundancy_read_str.indexOf(',') + 1]);
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
      }//scope
      break;

    case 'C':
      {
        //...........................
        pin = atoi(&temp_redundancy_read_str[temp_redundancy_read_str.indexOf(',') + 1]);
        //.................................
        after_ack_arr[pin] = 201;
        solved_issues++;
        //.....................................
        print_arr(0);
        print_arr(1);
      }//scope
      break;

    case 'R':
      {
        reset();
        //.....................................
        print_arr(0);
        print_arr(1);
        print_arr(2);
      }//scope
      break;
  }//swicth

}//void
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
  for (int i = 0; i < alarm_arr_int; i++)
  {

    if (alarm_arr[i] != 201)//33
    {
      pin = alarm_arr[i];
      //                lamp_state(pin, 1);
      after_ack_arr[after_ack_arr_int] = pin;
      after_ack_arr_int++;
      alarm_arr[i] = 201;//33
    }// if !33

  }//for
  //...........................
  buz_state_flag = 0;
  //.....................................
  //        print_arr(0);
  //        print_arr(1);
  print_arr(2);
}//void

//********************************************************************* read buttons
void read_buttons()
{
  //................................................. buz
  if (!read_mute())
  {
    mute_press_func();
    while (!read_mute())
    {
      delay(10);
    }//while
  }//if

  //.....................................................ACK
  if (!read_ack() )
  {
    ack_press_func();
    while (!read_mute())
    {
      delay(10);
    }//while
  }//if

  //..................................................... RST
  if (!read_rst() )
  {
    ;
  }//if
}//void
//*********************************************************************
