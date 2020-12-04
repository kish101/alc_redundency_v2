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

int read_alarm_input(int alarm_pin)
{
  //  pinValues = read_shift_regs();
  read_shift_regs_str();

  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  if (pinValueStr[alarm_pin] == '1' )
  {
    if (exor_state_str[alarm_pin] == '0')
    {
      //fault state
      //regBank.set(modbus_mapInputPin(alarm_pin), 1);
      return 1;
    }//if fault
    else
    {
      regBank.set(modbus_mapInputPin(alarm_pin), 0);
      return 0;
    }//else
  }//if 1
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>
  else if (pinValueStr[alarm_pin] == '0' )
  {
    if (exor_state_str[alarm_pin] == '1')
    {
      //fault state
      return 1;
    }//if fault
    else
    {
      return 0;
    }//else
  }//if 0
  //>>>>>>>>>>>>>>>>>>>>>>>>>>

}//int

//********************************************************************* buzzer
void buz_state(int state)
{
  //................................if master
  if (softUart_select)
  {
    if (state == 1)
      mcp.digitalWrite(buz, LOW);
    else
      mcp.digitalWrite(buz, HIGH);
  }//if
  else
  {
    //.................................... start slave control
    if (slave_start == 1)
    {
      if (state == 1)
        mcp.digitalWrite(buz, LOW);
      else
        mcp.digitalWrite(buz, HIGH);
    }//if slave active
  }//esle

}//void


//********************************************************************* buzzer RB
void buz_rb_state(int state)
{
  if (state == 1)
    mcp.digitalWrite(buz_rb, LOW);
  else
    mcp.digitalWrite(buz_rb, HIGH);
}//void


//********************************************************************* Lamp
void lamp_state(int pin, int state)
{
  //................................if master
  if (softUart_select)
  {
    if (state == 1)
    {
      sr.set(pin, LOW);
      dashBoard_str.setCharAt(pin, '1');
      regBank.set((pin + 1), 1);
    }
    else
    {
      sr.set(pin, HIGH);
      dashBoard_str.setCharAt(pin, '0');
      regBank.set((pin + 1), 0);
    }//else
  }//if

  //................................if slave
  else
  {
    //.................................... start slave control
    if (slave_start == 1)
    {
      if (state == 1)
      {
        sr.set(pin, LOW);
        dashBoard_str.setCharAt(pin, '1');
        //    regBank.set((pin + 1), 1);
      }
      else
      {
        sr.set(pin, HIGH);
        dashBoard_str.setCharAt(pin, '0');
        //    regBank.set((pin + 1), 0);
      }//else
    }//if start slave
    else
    {
      if (state == 1)
      {
        dashBoard_str.setCharAt(pin, '1');
      }
      else
      {
        dashBoard_str.setCharAt(pin, '0');
      }//else

    }//else slave is inactive
  }//else
}//void

//********************************************************************* main
void single_seq_pattern_func(int seq_type)
{
  switch (seq_type)
  {
    //......................................................................................... seq 1
    case 1:
      {
        buz_state(1);
        int ws_send_count = 50;

        while (1)
        {
          ////Serial.println("..");
          //................................................ LAMP
          input_lamp_blink_fast();

          //................................................. subq Inputs
          read_sub_inputs();

          //................................................. buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //............................................... free logic
          // before ACK
          for (int i = 0; i < alarm_arr_int; i++)
          {

            if (alarm_arr[i] != 201)//33
            {
              pin = alarm_arr[i];

              if (read_alarm_input(pin))
              {
                ;
              }//if
              else
              {
                lamp_state(pin, 0);
                alarm_arr[i] = 201;//33
                solved_issues++;
              }//else read_input == LOW
            }//if !33

          }//for

          //.....................................................ACK
          if (!read_ack() )
          {
            buz_state(0);

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

            //TODO remove this wait
            print_arr(0);
            print_arr(1);
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //..................................................... free logic
          // after ACK
          for (int i = 0; i < after_ack_arr_int; i++)
          {

            if (after_ack_arr[i] != 201)//33
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1);
              }//if
              else
              {
                lamp_state(pin, 0);
                after_ack_arr[i] = 201;//33
                solved_issues++;
              }//else read_input == LOW
            }//if !33

          }//for

          //................................................. break
          if (solved_issues == issues)
          {
            //Serial.println("......... BREAKING OUT ......... ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

          }//if

          //................................................. dashBoard send data
          if ( (ws_db_connect_flag == 1) && (ws_send_count >= 50) )
          {
            Serial.println(dashBoard_str);
            webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
            ws_send_count = 0;
          }//if

          //................................................. delay
          //          delay(3000);
          ws_send_count++;
          delay(10);
        }//while

      }//case 1
      break;

    //....................................................................................... seq 2
    case 2:
      {
        buz_state(1);
        int ws_send_count = 50;

        while (1)
        {
          //................................................ LAMP
          input_lamp_blink_fast();

          //................................................. subq Inputs
          read_sub_inputs();

          //................................................... buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //.....................................................ACK
          if (!read_ack() )
          {
            //........................
            if (redundancy_flag)
            {
              redundancy_str = "A0.";
              send_redundancy_str_func();
              //                  send_redundancy_str_func('A', after_ack_arr_int, pin);
              //                  delay(100);
            }//if
            //...........................
            buz_state(0);
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
            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //..................................................... free logic
          for (int i = 0; i < after_ack_arr_int; i++)
          {

            if (after_ack_arr[i] != 201)//33
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1);
              }//if
              else
              {
                //........................
                if (redundancy_flag)
                {
                  redundancy_str = 'C' + String(0) + ',' + String(i) + '.';
                  send_redundancy_str_func();
                  //                  send_redundancy_str_func('C', 0, i);
                  //                  delay(100);
                }//if
                //..........................
                lamp_state(pin, 0);
                after_ack_arr[i] = 201;//33
                solved_issues++;
              }//else read_input == LOW
            }//if !33

          }//for

          //................................................. break
          if (solved_issues == issues)
          {
            //Serial.println("....... BREAKING OUT ......... ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

          }//if

          //................................................. dashBoard send data
          if ( (ws_db_connect_flag == 1) && (ws_send_count >= 50) )
          {
            Serial.println(dashBoard_str);
            webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
            ws_send_count = 0;
          }//if

          //................................................. delay
          //          delay(3000);
          ws_send_count++;
          delay(10);
        }//while

      }//case 2
      break;

    //.............................................................................................. seq 3
    case 3:
      {
        buz_state(1);
        int ws_send_count = 50;

        while (1)
        {
          //................................................ LAMP
          input_lamp_blink_fast();

          //................................................. subq Inputs
          read_sub_inputs();

          //.................................................. buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //.....................................................ACK
          if (!read_ack() )
          {
            //........................
            if (redundancy_flag)
            {
              redundancy_str = "A0.";
              send_redundancy_str_func();
              //                  delay(100);
            }//if
            buz_state(0);
            //................................
            for (int i = 0; i < issues; i++)
            {

              if (alarm_arr[i] != 201)//33
              {
                pin = alarm_arr[i];
                lamp_state(pin, 1);

                after_ack_arr[after_ack_arr_int] = pin;
                after_ack_arr_int++;
                alarm_arr[i] = 201;//33
              }// if !33

            }//for
            //................................
            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //.....................................................after ACK,
          //TODO ?

          //..................................................... RST
          if (!read_rst() )
          {
            for (int i = 0; i < after_ack_arr_int; i++)
            {

              if (after_ack_arr[i] != 201)//33
              {
                pin = after_ack_arr[i];

                if (read_alarm_input(pin))
                {
                  ;
                }//if
                else
                {
                  //........................
                  if (redundancy_flag)
                  {
                    redundancy_str = 'C' + String(0) + ',' + String(i) + '.';
                    send_redundancy_str_func();
                    //                  delay(100);
                  }//if
                  lamp_state(pin, 0);
                  after_ack_arr[i] = 201;//33
                  solved_issues++;
                }//else read_input == LOW
              }//if !33

            }//for

            //TODO remove this wait
            while (!read_rst())
            {
              ;
            }//while
          }//if RST

          //................................................. break
          if (solved_issues == issues)
          {
            //Serial.println("......... BREAKING OUT .......... ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

            //to keep lamp on after Lamp test
            for (int i = 0; i < after_ack_arr_int; i++)
            {
              if (after_ack_arr[i] != 201)//33
              {
                pin = after_ack_arr[i];
                lamp_state(pin, 1);

              }//if != 33
            }//for

          }//if

          //................................................. dashBoard send data
          if ( (ws_db_connect_flag == 1) && (ws_send_count >= 50) )
          {
            Serial.println(dashBoard_str);
            webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
            ws_send_count = 0;
          }//if

          //................................................. delay
          //          delay(3000);
          ws_send_count++;
          delay(10);
        }//while

      }//case 3
      break;

    //......................................................................................... seq 4
    case 4:
      {
        buz_state(1);
        int ws_send_count = 50;

        while (1)
        {
          ////Serial.println("..");
          //................................................ LAMP
          input_lamp_blink_fast();

          //................................................. subq Inputs
          read_sub_inputs();

          //................................................. buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //............................................... free logic
          // before ACK
          for (int i = 0; i < alarm_arr_int; i++)
          {

            if (alarm_arr[i] != 201)//33
            {
              pin = alarm_arr[i];

              if (read_alarm_input(pin))
              {
                ;
              }//if
              else
              {
                lamp_state(pin, 1);
                alarm_arr[i] = 201;//33
                solved_issues++;
              }//else read_input == LOW
            }//if !33

          }//for

          //.....................................................ACK
          if (!read_ack() )
          {
            buz_state(0);

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

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //..................................................... free logic
          // after ACK
          for (int i = 0; i < after_ack_arr_int; i++)
          {

            if (after_ack_arr[i] != 201)//33
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 0);
              }//if
              else
              {
                lamp_state(pin, 1);
                after_ack_arr[i] = 201;//33
                solved_issues++;
              }//else read_input == LOW
            }//if !33

          }//for

          //................................................. break
          if (solved_issues == issues)
          {
            //Serial.println("......... BREAKING OUT ......... ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

          }//if

          //................................................. dashBoard send data
          if ( (ws_db_connect_flag == 1) && (ws_send_count >= 50) )
          {
            Serial.println(dashBoard_str);
            webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
            ws_send_count = 0;
          }//if

          //................................................. delay
          //          delay(3000);

          ws_send_count++;
          delay(10);
        }//while

      }//case 4
      break;

    //.......................................................................................................... seq 8
    case 8:
      {
        buz_state(1);
        int ws_send_count = 50;

        while (1)
        {
          //................................................ LAMP
          input_lamp_blink_fast();
          input_lamp_blink_slow();

          //................................................. subq Inputs
          read_sub_inputs();

          //.................................................. buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //.....................................................ACK
          if (!read_ack() )
          {
            buz_state(0);

            for (int i = 0; i < alarm_arr_int; i++)
            {

              if (alarm_arr[i] != 201)//33
              {
                after_ack_arr[after_ack_arr_int] = alarm_arr[i];
                after_ack_arr_int++;
                alarm_arr[i] = 201;//33
              }// if !33

            }//for

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //.....................................................free logic
          //after ACK
          for (int i = 0; i < after_ack_arr_int; i++)
          {
            if (after_ack_arr[i] != 201)//33
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1);
              }//if
              else
              {
                slow_blink_arr[slow_blink_arr_int] = pin;
                slow_blink_arr_int++;
                after_ack_arr[i] = 201;//33

                buz_rb_state(1);
              }//else read_input == LOW
            }//if !=33

          }//for

          //.....................................................free logic
          //Slow BLink
          for (int i = 0; i < slow_blink_arr_int; i++)
          {

            if (slow_blink_arr[i] != 201)//33
            {
              pin = slow_blink_arr[i];

              if (read_alarm_input(pin))
              {

                alarm_arr[alarm_arr_int] = pin;
                alarm_arr_int++;
                slow_blink_arr[i] = 201;//33

                buz_state(1);
                buz_rb_state(0);
              }//if
              else
              {
                ;
              }//else read_input == LOW
            }//if !33

          }//for
          //..................................................... RST
          if (!read_rst() )
          {

            for (int i = 0; i < slow_blink_arr_int; i++)
            {

              if (slow_blink_arr[i] != 201)//33
              {
                pin = slow_blink_arr[i];

                if (read_alarm_input(pin))
                {
                  ;
                }//if
                else
                {
                  lamp_state(pin, 0);
                  slow_blink_arr[i] = 201;//33
                  solved_issues++;

                  buz_rb_state(0);
                }//else read_input == LOW
              }//if !33

            }//for

            //TODO remove this wait
            while (!read_rst())
            {
              ;
            }//while
          }//if RST

          //................................................. break
          if (solved_issues == issues)
          {
            //Serial.println("...... BREAKING OUT ..... ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

          }//if

          //................................................. dashBoard send data
          if ( (ws_db_connect_flag == 1) && (ws_send_count >= 50) )
          {
            Serial.println(dashBoard_str);
            webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
            ws_send_count = 0;
          }//if

          //................................................. delay
          //          delay(3000);
          ws_send_count++;
          delay(10);
        }//while

      }//case 8
      break;

    //.................................................................................... seq 14
    case 14:
      {
        buz_state(1);
        int ws_send_count = 50;

        while (1)
        {
          //................................................ LAMP
          input_lamp_blink_fast();

          //................................................. subq Inputs
          read_sub_inputs();

          //.................................................. buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //.....................................................ACK
          if (!read_ack() )
          {
            buz_state(0);

            for (int i = 0; i < alarm_arr_int; i++)
            {

              if (alarm_arr[i] != 201)//33
              {
                pin = alarm_arr[i];
                lamp_state(pin, 1);

                after_ack_arr[after_ack_arr_int] = pin;
                after_ack_arr_int++;
                alarm_arr[i] = 201;//33
              }// if !33

            }//for

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //.....................................................second ACK
          //TODO ?

          //..................................................... RST
          if (!read_rst() )
          {
            for (int i = 0; i < after_ack_arr_int; i++)
            {

              if (after_ack_arr[i] != 201)//33
              {
                pin = after_ack_arr[i];

                if (read_alarm_input(pin))
                {
                  buz_state(1);
                  alarm_arr[alarm_arr_int] = pin;
                  alarm_arr_int++;

                  after_ack_arr[i] = 201;//33
                }//if
                else
                {
                  lamp_state(pin, 0);
                  after_ack_arr[i] = 201;//33
                  solved_issues++;
                }//else read_input == LOW
              }//if !33

            }//for

            //TODO remove this wait
            while (!read_rst())
            {
              ;
            }//while
          }//if RST

          //................................................. break
          if (solved_issues == issues)
          {
            //Serial.println("...]..... BREAKING OUT .......... ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

            //to keep lamp on after Lamp test
            for (int i = 0; i < after_ack_arr_int; i++)
            {
              if (after_ack_arr[i] != 201)//33
              {
                pin = after_ack_arr[i];
                lamp_state(pin, 1);

              }//if != 33
            }//for

          }//if test

          //................................................. dashBoard send data
          if ( (ws_db_connect_flag == 1) && (ws_send_count >= 50) )
          {
            Serial.println(dashBoard_str);
            webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
            ws_send_count = 0;
          }//if

          //................................................. delay
          //          delay(3000);
          ws_send_count++;
          delay(10);
        }//while

      }//case 14
      break;

    //...................................................................................... seq 15
    case 15:
      {
        buz_state(1);
        int ws_send_count = 50;

        while (1)
        {
          //................................................ LAMP
          input_lamp_blink_fast();
          input_lamp_blink_slow();

          //................................................. subq Inputs
          read_sub_inputs();

          //.................................................. buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //.....................................................free logic
          //before ACK
          for (int i = 0; i < alarm_arr_int; i++)
          {

            if (alarm_arr[i] != 201)//33
            {
              pin = alarm_arr[i];

              if (read_alarm_input(pin))
              {
                ;
              }//if
              else
              {
                slow_blink_arr[slow_blink_arr_int] = pin;
                slow_blink_arr_int++;
                alarm_arr[i] = 201;//33

                buz_rb_state(1);
              }//else

            }// if !33
          }//for

          //.....................................................ACK
          if (!read_ack() )
          {
            buz_state(0);

            for (int i = 0; i < alarm_arr_int; i++)
            {

              if (alarm_arr[i] != 201)//33
              {
                after_ack_arr[after_ack_arr_int] = alarm_arr[i];
                after_ack_arr_int++;
                alarm_arr[i] = 201;//33
              }// if !33

            }//for

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //.....................................................free logic
          //after ACK
          for (int i = 0; i < after_ack_arr_int; i++)
          {
            if (after_ack_arr[i] != 201)//33
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1);
              }//if
              else
              {
                slow_blink_arr[slow_blink_arr_int] = pin;
                slow_blink_arr_int++;
                after_ack_arr[i] = 201;//33

                buz_rb_state(1);
              }//else read_input == LOW
            }//if !=33

          }//for

          //..................................................... RST
          if (!read_rst() )
          {
            for (int i = 0; i < slow_blink_arr_int; i++)
            {

              if (slow_blink_arr[i] != 201)//33
              {
                pin = slow_blink_arr[i];

                if (read_alarm_input(pin))
                {
                  ;
                }//if
                else
                {
                  lamp_state(pin, 0);
                  slow_blink_arr[i] = 201;//33
                  solved_issues++;

                  buz_rb_state(0);
                }//else read_input == LOW
              }//if !33

            }//for

            //TODO remove this wait
            while (!read_rst())
            {
              ;
            }//while
          }//if RST

          //................................................. break
          if (solved_issues == issues)
          {
            //Serial.println("...... BREAKING OUT .......... ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

          }//if

          //................................................. dashBoard send data
          if ( (ws_db_connect_flag == 1) && (ws_send_count >= 50) )
          {
            Serial.println(dashBoard_str);
            webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
            ws_send_count = 0;
          }//if

          //................................................. delay
          //          delay(3000);
          ws_send_count++;
          delay(10);
        }//while

      }//case 15
      break;

    //........................................................................................ seq 16
    case 16:
      {
        buz_state(1);
        int ws_send_count = 50;

        while (1)
        {
          //................................................ LAMP
          input_lamp_blink_fast();
          input_lamp_blink_slow();

          //................................................. subq Inputs
          read_sub_inputs();

          //.................................................. buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //.....................................................ACK
          if (!read_ack() )
          {
            buz_state(0);

            for (int i = 0; i < alarm_arr_int; i++)
            {

              if (alarm_arr[i] != 201)//33
              {
                after_ack_arr[after_ack_arr_int] = alarm_arr[i];
                after_ack_arr_int++;
                alarm_arr[i] = 201;//33
              }// if !33

            }//for

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //.....................................................free logic
          //after ACK
          for (int i = 0; i < after_ack_arr_int; i++)
          {
            if (after_ack_arr[i] != 201)//33
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1);
              }//if
              else
              {
                slow_blink_arr[slow_blink_arr_int] = pin;
                slow_blink_arr_int++;
                after_ack_arr[i] = 201;//33

                buz_rb_state(1);
              }//else read_input == LOW
            }//if !=33

          }//for

          //..................................................... RST
          if (!read_rst() )
          {
            //.......................................
            //case 1
            for (int i = 0; i < alarm_arr_int; i++)
            {

              if (alarm_arr[i] != 201)//33
              {
                pin = alarm_arr[i];

                if (read_alarm_input(pin))
                {
                  ;
                }//if
                else
                {
                  lamp_state(pin, 0);
                  alarm_arr[i] = 201;//33
                  solved_issues++;
                }//else ALARM LOW

              }//if != 33
            }//for

            //........................................
            for (int i = 0; i < slow_blink_arr_int; i++)
            {

              if (slow_blink_arr[i] != 201)//33
              {
                pin = slow_blink_arr[i];

                if (read_alarm_input(pin))
                {
                  ;
                }//if
                else
                {
                  lamp_state(pin, 0);
                  slow_blink_arr[i] = 201;//33
                  solved_issues++;

                  buz_rb_state(0);
                }//else read_input == LOW
              }//if !33

            }//for

            //TODO remove this wait
            while (!read_rst())
            {
              ;
            }//while
          }//if RST

          //................................................. break
          if (solved_issues == issues)
          {
            //Serial.println("...... BREAKING OUT ......... ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

          }//if

          //................................................. dashBoard send data
          if ( (ws_db_connect_flag == 1) && (ws_send_count >= 50) )
          {
            Serial.println(dashBoard_str);
            webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
            ws_send_count = 0;
          }//if

          //................................................. delay
          //          delay(3000);
          ws_send_count++;
          delay(10);
        }//while

      }//case 16
      break;

    default:
      {
        //Nothing... YET
      }//default
      break;

  }//switch

}//void

//................................................. Print
//          print_arr(0);
//          print_arr(1);
//          //Serial.print(".............. issues = ");
//          //Serial.println(issues);
//
//          //Serial.print(".............. solved issues = ");
//          //Serial.println(solved_issues);
