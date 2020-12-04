
//************************************************************ Sub IP
void read_sub_inputs_7()
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
            //..............................&& (after_ack_alarm_pin == 33)
            if ((alarm_blink_pin == 201) )//33
            {
              alarm_blink_pin = i;
              issues++;
              //Serial.print("issues: "); Serial.println(issues);

              imt_count = 6;
              imt_flag = 1;

              sub_count = 10;
              sub_state = 0;
            }//if alarm_blink_pin
            else
            {
              //lamp_state(i, 1);
              subq_arr[subq_arr_int] = i;
              subq_arr_int++;
              issues++;
              //Serial.print("issues: "); Serial.println(issues);

              sub_count = 10;
              sub_state = 0;
            }//else solid state array
            //..............................

            buz_state(1);
            //..............................
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
          if (not_part_of_array_subq(i))
          {
            //..............................&& (after_ack_alarm_pin == 33)
            if ((alarm_blink_pin == 201) )//33
            {
              alarm_blink_pin = i;
              issues++;

              imt_count = 6;
              imt_flag = 1;

              sub_count = 10;
              sub_state = 0;
            }//if alarm_blink_pin
            else
            {
              //lamp_state(i, 1);
              subq_arr[subq_arr_int] = i;
              subq_arr_int++;
              issues++;

              sub_count = 10;
              sub_state = 0;
            }//else solid state array
            //..............................

            buz_state(1);
            //..............................
            regBank.set(modbus_mapInputPin(i), 1);
          }//if not part of array
        }//if fault
      }//if 0
      //>>>>>>>>>>>>>>>>>>>>>>>>>>
    }//for

    oldPinValueStr = pinValueStr;
  }//else
}//void

//************************************************************first out seq blink
void alarm_pin_fast_blink_7()
{
  if (count >= 50)
  {
    state = !state;
    lamp_state(after_ack_alarm_pin, state);
    count = 0;
  }//if count 25
  count++;

}//void

//************************************************************ SubQ IP Blink
void subq_lamp_blink_7()
{
  if (sub_count >= 10)
  {
    sub_state = !sub_state;

    if (alarm_blink_pin != 201)//33
      if (imt_flag)
        lamp_state(alarm_blink_pin, sub_state);

    for (int i = 0; i < subq_arr_int ; i++)
    {

      if (subq_arr[i] != 201)//33
      {
        lamp_state(subq_arr[i], sub_state);
      }//if != 33
    }//for


    if (alarm_blink_pin != 201)//33
    {
      if (imt_count >= 6)
      {
        imt_flag = !imt_flag;
        imt_count = 0;
        lamp_state(alarm_blink_pin, 0);
      }
      imt_count++;
    }//if

    sub_count = 0;
  }//if count 10
  sub_count++;

}//void
