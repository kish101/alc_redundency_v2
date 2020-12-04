
//************************************************************ Sub IP
void read_sub_inputs_7()
{
  pinValues = read_shift_regs();

  if (pinValues != oldPinValues)
  {

    for (int i = 0; i < DATA_WIDTH; i++)
    {

      if ((pinValues >> i) & 1)
      {
        if (not_part_of_array_subq(i))
        {
          //..............................&& (after_ack_alarm_pin == 33)
          if ((alarm_blink_pin == 33) )
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
        }//if not part of alarm array

      }//if pin is HIGH

    }//for DATA_WIDTH

  }//if
  oldPinValues = pinValues;

}//void

//************************************************************first out seq blink
void alarm_pin_fast_blink_7()
{
  if (count >= 25)
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

    if (alarm_blink_pin != 33)
      if (imt_flag)
        lamp_state(alarm_blink_pin, sub_state);

    for (int i = 0; i < subq_arr_int ; i++)
    {

      if (subq_arr[i] != 33)
      {
        lamp_state(subq_arr[i], sub_state);
      }//if != 33
    }//for


    if (alarm_blink_pin != 33)
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
