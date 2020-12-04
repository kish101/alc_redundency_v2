//************************************************************ Sub IP
void read_sub_inputs()
{
  // Look for and handle WebSocket data
  webSocket.loop();
  webSocket_dashBoard.loop();
  //  SSDP.process();
  slave.run();
  if (eth_connected)
  {
    SSDP.process();
  }//if

  //........................................................ check inputs
  read_shift_regs_str();

  if (pinValueStr.equals(oldPinValueStr))
  {
    ;
  }//if
  else
  {
    //.................................................for
    for (int i = 0; i < pinValueStr.length(); i++)
    {
      //>>>>>>>>>>>>>>>>>>>>>>>>>>>
      if (pinValueStr[i] == '1' )
      {
        if (exor_state_str[i] == '0')
        {
          if (not_part_of_array(i))
          {
            //Serial.print("fault pin: "); //Serial.println(i);
            //........................
            if (redundancy_flag)
            {
              redundancy_str = 'a' + String(alarm_arr_int) + ',' + String(i) + '.';
              send_redundancy_str_func();
              //              delay(100);
            }//if
            //........................
            alarm_arr[alarm_arr_int] = i;
            alarm_arr_int++;
            issues++;
            //........................
            buz_state(1);
            //........................
            regBank.set(modbus_mapInputPin(i), 1);

          }//if not part of array
        }//if fault
      }//if 1
      //>>>>>>>>>>>>>>>>>>>>>>>>>>>

      //>>>>>>>>>>>>>>>>>>>>>>>>>>>
      if (pinValueStr[i] == '0' )
      {
        if (exor_state_str[i] == '1')
        {
          if (not_part_of_array(i))
          {
            //Serial.print("fault pin: "); println(i);
            //........................
            if (redundancy_flag)
            {
              redundancy_str = 'a' + String(alarm_arr_int) + ',' + String(i) + '.';
              send_redundancy_str_func();
              //              delay(100);
            }//if
            //........................
            alarm_arr[alarm_arr_int] = i;
            alarm_arr_int++;
            issues++;
            //........................
            buz_state(1);
            //........................
            regBank.set(modbus_mapInputPin(i), 1);

          }//if not part of array
        }//if fault
      }//if 0
      //>>>>>>>>>>>>>>>>>>>>>>>>>>
    }//for DATA_WIDTH
    //................................................./for

    print_arr(0);
    //....assign
    oldPinValueStr = pinValueStr;
  }//else
}//void

//************************************************************ lamp fast
void input_lamp_blink_fast()
{
  if (count == 50)
  {
    state = !state;

    for (int i = 0; i < alarm_arr_int; i++)
    {

      if ( alarm_arr[i] != 201)//33
        lamp_state(alarm_arr[i], state);

    }//for

    count = 0;
  }//if count 25

  count++;
}//void

//************************************************************ lamp slow
void input_lamp_blink_slow()
{
  if (sub_count == 100)
  {
    sub_state = !sub_state;

    for (int i = 0; i < slow_blink_arr_int; i++)
    {

      if ( slow_blink_arr[i] != 201)//33
        lamp_state(slow_blink_arr[i], sub_state);

    }//for

    sub_count = 0;
  }//if count 25

  sub_count++;
}//void

//************************************************************ not_part_of_array
int not_part_of_array(int which)
{
  for (int i = 0 ; i < alarm_arr_int; i++)//20
  {
    if (alarm_arr[i] == which)
      return 0;
  }//for
  for (int i = 0 ; i < after_ack_arr_int; i++)//20
  {
    if (after_ack_arr[i] == which)
      return 0;
  }//for
  for (int i = 0 ; i < slow_blink_arr_int; i++)//20
  {
    if (slow_blink_arr[i] == which)
      return 0;
  }//for

  return 1;
}//int
