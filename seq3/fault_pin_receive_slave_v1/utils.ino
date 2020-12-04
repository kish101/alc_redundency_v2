//.............
void reset_all()
{
  alarm_arr_int = 0;
  for (int i = 0; i < 20; i++)
  {
    alarm_arr[i] = 201;//33
    delay(10);
  }

  //.............
  after_ack_arr_int = 0;
  for (int i = 0; i < 20; i++)
  {
    after_ack_arr[i] = 201;//33
    delay(10);
  }

  issues = 0;
  solved_issues = 0;
}//reset

//**********************************************************************
void print_these()
{
  print_arr(0);
  print_arr(1);
}

//**********************************************************************
void print_arr(int which)
{
  switch (which)
  {
    case 0:
      {
        Serial.println("............. alarm arr ............");
        for (int p = 0; p < (alarm_arr_int + 1); p++)
        {
          Serial.print(alarm_arr[p]);
          Serial.print(",");
        }
        Serial.println();
      }//case0
      break;

    case 1:
      {
        Serial.println("............. after ack arr ............");
        for (int p = 0; p < (after_ack_arr_int + 1); p++)
        {
          Serial.print(after_ack_arr[p]);
          Serial.print(",");
        }
        Serial.println();
      }//case1
      break;

    case 2:
      {
        Serial.println("............. ack pressed ............");
        Serial.println(alarm_arr_int);
        Serial.println(after_ack_arr_int);
        Serial.println();
      }//case1
      break;
  }//switch
}//void
