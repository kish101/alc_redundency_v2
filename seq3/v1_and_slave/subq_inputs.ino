
//************************************************************ Sub IP
void read_subq_inputs()
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
          //..............................
          //seq 10,12,13, dont add subq IPs to alarm pin
          if ((alarm_blink_pin == 33) && (sequence_pattern_number <= 9))
          {
            alarm_blink_pin = i;
            issues++;

            buz_state(1);

            count = 25;
            state = 0;
          }//if alarm_blink_pin
          else
          {
            //            //Serial.println("new subQ IP");
            subq_arr[subq_arr_int] = i;
            subq_arr_int++;
            issues++;

            //seq 5 has no Buz for Subq IPs
            if (sequence_pattern_number != 5)
              buz_state(1);

            //seq 10,12,13 require no Lamp ON, require count/state reset
            if ((sequence_pattern_number <= 9) || (sequence_pattern_number == 11))
              lamp_state(i, 1);
            else
            {
              subq_count = 25;
              subq_state = 0;
            }//else

          }//else solid state array
          //..............................

        }//if not part of alarm array

      }//if pin is HIGH

    }//for DATA_WIDTH

  }//if
  oldPinValues = pinValues;

}//void

//************************************************************ First out alarm pin
void alarm_pin_blink_fast()
{
  if (count == 25)
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
  }//if count 25
  count++;

}//void

//************************************************************ SubQ IP Blink
void subq_lamp_blink()
{
  if (subq_count == 25)
  {
    subq_state = !subq_state;

    if (after_ack_alarm_pin != 33)
      lamp_state(after_ack_alarm_pin, subq_state);

    for (int i = 0; i < subq_arr_int ; i++)
    {
      if (subq_arr[i] != 33)
      {
        lamp_state(subq_arr[i], subq_state);
      }//if != 33
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

  for (int i = 0 ; i < 20; i++)
  {
    if (subq_arr[i] == which)
      return 0;
  }//for

  for (int i = 0 ; i < 20; i++)
  {
    if (after_ack_arr[i] == which)
      return 0;
  }//for

  return 1;
}//int
