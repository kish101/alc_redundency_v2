
//************************************************************************************************** read input
void read_single_input_func()
{

  // Look for and handle WebSocket data
  webSocket.loop();


  //........................................................ check inputs
  read_shift_regs_str();

  if (pinValueStr.equals(oldPinValueStr))
  {
    ;
  }//if
  else
  {
    print_arr(7);
    //.................................................for
    for (int i = 0; i < pinValueStr.length(); i++)
    {
      //>>>>>>>>>>>>>>>>>>>>>>>>>>>
      if (pinValueStr[i] == '1' )
      {
        if (exor_state_str[i] == '0')
        {
          //Serial.print("fault pin: "); //Serial.println(i);
          //........................
          if (redundancy_flag)
          {
            redundancy_str = 'a' + String(alarm_arr_int) + ',' + String(i) + '.';
            send_redundancy_str_func();
            //            delay(100);
          }//if
          //........................
          alarm_arr[alarm_arr_int] = i;
          alarm_arr_int++;
          issues++;
          //........................
          //regBank.set(modbus_mapInputPin(i), 1);
        }//if fault
      }//if 1
      //>>>>>>>>>>>>>>>>>>>>>>>>>>>

      //>>>>>>>>>>>>>>>>>>>>>>>>>>>
      if (pinValueStr[i] == '0' )
      {
        if (exor_state_str[i] == '1')
        {
          //Serial.print("fault pin: "); //Serial.println(i);
          //........................
          if (redundancy_flag)
          {
            redundancy_str = 'a' + String(alarm_arr_int) + ',' + String(i) + '.';
            send_redundancy_str_func();
            //            delay(100);
          }//if
          //......................
          alarm_arr[alarm_arr_int] = i;
          alarm_arr_int++;
          issues++;
          //......................
          //regBank.set(modbus_mapInputPin(i), 1);
        }//if fault
      }//if 0
      //>>>>>>>>>>>>>>>>>>>>>>>>>>
    }//for
    //................................................. /for

    print_arr(0);

    if (sequence_pattern_number == 4)
      lamp_test_on();

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    //....off running_flasher
    mcp.digitalWrite(running_flasher, LOW);
    //....on error_led
    mcp.digitalWrite(error_led, LOW);
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    //....call sequence
    single_seq_pattern_func(sequence_pattern_number);
    //....assign
    oldPinValueStr = pinValueStr;
    //.... reset all
    reset();
  }//else
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

  delay(9);
}//void

//************************************************************************************************** read input
void read_subq_input_func()
{

  pinValues = read_shift_regs();

  if (pinValues != oldPinValues)
  {
    send_func('N');

    for (int i = 0; i < DATA_WIDTH; i++)
    {
      if ((pinValues >> i) & 1)
      {
        alarm_blink_pin = i;
        issues++;
      }//if
    }//for

    //....off running_flasher
    mcp.digitalWrite(running_flasher, LOW);
    //....on error_led
    mcp.digitalWrite(error_led, HIGH);
    //....call sequence
    subq_seq_pattern_func(sequence_pattern_number);
    //....assign
    oldPinValues = pinValues;
    //....indcate ESP 2
    send_func('X');
    //.... reset all
    reset();
    //    delay(10);
  }//if
  delay(9);
}//void
