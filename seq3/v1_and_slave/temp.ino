void temp_func()
{
  int count = 0;
  int times = 200;
  int flag = 0;
  //.....................................................loop start
  while (1)
  {
    if (mySerial_slave.available() )
    {
      for (int i = 0; i < 9; i++)
      {
        char c = mySerial_slave.read();

        if (c == 'X')
        {
          //            mySerial_slave.print("K");
          count = 0;
          break;
        }//if
        else if ( (c == '\n') || (c == '\r') || (c == '.'))
        {
          break;
        }//if
        else
        {
          redundancy_read_str += c;
          delay(2);
          flag = 1;
        }//else
      }//for
    }//if avai

    //.........................................................................
    if (flag == 1)
    {
      Serial.println(redundancy_read_str);
      //
      fill_arr(redundancy_read_str);
      //........................
      redundancy_read_str = "";
      //........................
      mySerial_slave.print("K");
      //........................
      count = 0;
      flag = 0;
    }//if flag

    if (count >= times )
    {
      Serial.println("slave active");
      slave_start = 1;
      mySerial_slave.end();
      break;
    }

    //..................................
    read_buttons();
    //................................
    count++;
    delay(10);
  }//while
  //.....................................................loop end

}//void
