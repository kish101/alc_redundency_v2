
//************************************************************ Sub IP
void read_subq_inputs()
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
          if (not_part_of_array_subq(i))
          {
            //..............................
            //seq 10,12,13, dont add subq IPs to alarm pin
            if ((alarm_blink_pin == 201) && (sequence_pattern_number <= 9))
            {
              alarm_blink_pin = i;
              issues++;

              buz_state(1);

              count = 25;
              state = 0;
            }//if alarm_blink_pin
            else
            {
              //            ////Serial.println("new subQ IP");
              subq_arr[subq_arr_int] = i;
              subq_arr_int++;
              issues++;

              //seq 5 has no Buz for Subq IPs
              if (sequence_pattern_number != 5)
                buz_state(1);

              //seq 10,12,13 require no Lamp ON, require count/state reset
              if ((sequence_pattern_number <= 9) || (sequence_pattern_number == 11))
              {
                lamp_state(i, 1);
              }//if
              else
              {
                subq_count = 25;
                subq_state = 0;
              }//else

            }//else solid state array
            //..............................
            //            regBank.set(modbus_mapInputPin(i), 1);//moved to shiftReg read
          }//if not part of array
        }//if fault
      }//if 1
      //>>>>>>>>>>>>>>>>>>>>>>>>>>>

      //>>>>>>>>>>>>>>>>>>>>>>>>>>>
      if (pinValueStr[i] == '0' )
      {
        if (exor_state_str[i] == '1')
        {
          if (not_part_of_array_subq(i))
          {
            //..............................
            //seq 10,12,13, dont add subq IPs to alarm pin
            if ((alarm_blink_pin == 201) && (sequence_pattern_number <= 9))
            {
              alarm_blink_pin = i;
              issues++;

              buz_state(1);

              count = 25;
              state = 0;
            }//if alarm_blink_pin
            else
            {
              //            ////Serial.println("new subQ IP");
              subq_arr[subq_arr_int] = i;
              subq_arr_int++;
              issues++;

              //seq 5 has no Buz for Subq IPs
              if (sequence_pattern_number != 5)
                buz_state(1);

              //seq 10,12,13 require no Lamp ON, require count/state reset
              if ((sequence_pattern_number <= 9) || (sequence_pattern_number == 11))
              {
                lamp_state(i, 1);
              }//if
              else
              {
                subq_count = 25;
                subq_state = 0;
              }//else

            }//else solid state array
            //..............................
            //            regBank.set(modbus_mapInputPin(i), 1);//moved to shiftReg read
          }//if not part of array
        }//if fault
      }//if 0
      //>>>>>>>>>>>>>>>>>>>>>>>>>>
    }//for

    oldPinValueStr = pinValueStr;
  }//else
}//void

//************************************************************ First out alarm pin
void alarm_pin_blink_fast()
{
  if (count >= 50)//25
  {
    state = !state;
    lamp_state(alarm_blink_pin, state);
    count = 0;
  }//if count 25
  count++;

}//void

void alarm_pin_blink_faster()
{
  if (count >= 10)
  {
    state = !state;
    lamp_state(alarm_blink_pin, state);
    count = 0;
  }//if count 10
  count++;

}//void

//************************************************************ SubQ IP Blink
void subq_lamp_blink()
{
  if (subq_count >= 50)
  {
    subq_state = !subq_state;

    if (after_ack_alarm_pin != 201)//33
      lamp_state(after_ack_alarm_pin, subq_state);

    for (int i = 0; i < subq_arr_int ; i++)
    {
      if (subq_arr[i] != 201)//33
      {
        lamp_state(subq_arr[i], subq_state);
      }//if != 201 //33
    }//for

    subq_count = 0;
  }//if count 10
  subq_count++;

}//void

//************************************************************ not_part_of_array
int not_part_of_array_subq(int which)
{
  if (which == alarm_blink_pin)
    return 0;

  if (which == after_ack_alarm_pin)
    return 0;

  for (int i = 0 ; i < subq_arr_int; i++)//20
  {
    if (subq_arr[i] == which)
      return 0;
  }//for

  for (int i = 0 ; i < after_ack_arr_int; i++)//20
  {
    if (after_ack_arr[i] == which)
      return 0;
  }//for

  return 1;
}//int
