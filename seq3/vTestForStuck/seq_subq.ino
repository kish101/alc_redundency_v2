//********************************************************************* main
void subq_seq_pattern_func(int seq_type)
{
  switch (seq_type)
  {
    //...................................................................................... seq 5
    case 5:
      {
        buz_state(1);

        while (1)
        {
          //................................................ LAMP
          if (alarm_blink_pin != 33)
            alarm_pin_blink_fast();

          //................................................. subq Inputs
          read_subq_inputs();

          //................................................... buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
          }//if

          //.....................................................ACK
          if (!read_ack() )
          {
            buz_state(0);

            if (alarm_blink_pin != 33)
            {
              lamp_state(alarm_blink_pin, 1);

              subq_arr[subq_arr_int] = alarm_blink_pin;
              subq_arr_int++;
              alarm_blink_pin = 33;
            }//if
            //........................................

            while (!read_ack())
            {
              ;
            }//while
          }//if

          //..................................................... Automatic RST
          for (int i = 0; i < subq_arr_int; i++)
          {
            if (subq_arr[i] != 33)
            {
              pin = subq_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1); //Lamp ON
              }//if
              else
              {
                lamp_state(pin, 0);
                subq_arr[i] = 33;
                solved_issues++;
              }//else read_input == LO
            }//if ! 33
          }//for

          //......................................................... break
          if (solved_issues == issues)
          {
            ////Serial.println("...... BREAKING OUT ..... ");
            break;
          }//if

          //........................................................ lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

            //to keep lamp on after Lamp test
            //            for (int i = 0; i < subq_arr_int; i++)
            //            {
            //              if (subq_arr[i] != 33)
            //              {
            //                pin = subq_arr[i];
            //                lamp_state(pin, 1);
            //
            //              }//if != 33
            //            }//

          }//if

          //................................................. delay
          //          delay(3000);
          delay(10);
        }//while

      }//case 5
      break;

    //...................................................................................... seq 6
    case 6:
      {
        buz_state(1);

        while (1)
        {
          //................................................ LAMP
          if (alarm_blink_pin != 33)
            alarm_pin_blink_fast();

          //................................................. subq Inputs
          read_subq_inputs();

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

            if (alarm_blink_pin != 33)
            {
              //              lamp_state(alarm_blink_pin, 1);

              after_ack_arr[after_ack_arr_int] = alarm_blink_pin;
              after_ack_arr_int++;
              alarm_blink_pin = 33;
            }//if
            //........................................
            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33 )
              {
                after_ack_arr[after_ack_arr_int] = subq_arr[i];
                after_ack_arr_int++;
                subq_arr[i] = 33;
              }//if ! 33
            }//for

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //..................................................... free logic
          //after ACK
          for (int i = 0; i < after_ack_arr_int; i++)
          {
            if (after_ack_arr[i] != 33)
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1); //Lamp on
              }//if
              else
              {
                lamp_state(pin, 0);
                after_ack_arr[i] = 33;
                solved_issues++;
              }//else read_input == LO
            }//if ! 33
          }//for

          //......................................................... break
          if (solved_issues == issues)
          {
            ////Serial.println(".... BREAKING OUT ..... ");
            break;
          }//if

          //........................................................ lamp_test
          if (!read_test())
          {
            lamp_test_on();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_off();

            //to keep lamp on after Lamp test
            //before ACK, case 1
            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33)
              {
                pin = subq_arr[i];
                lamp_state(pin, 1);

              }//if != 33
            }//

          }//if
          //................................................. delay
          //          delay(3000);
          delay(10);
        }//while

      }//case 6
      break;

    //.............................................................................................. seq 7
    case 7:
      {
        buz_state(1);

        state = 0;
        count = 25;
        sub_state = 0;
        sub_count = 10;
        imt_count = 6;
        imt_flag = 0;

        after_ack_alarm_pin = 33;
        //        alarm_blink_pin = 33;

        while (1)
        {
          //................................................ LAMP
          if (after_ack_alarm_pin != 33)
            alarm_pin_fast_blink_7();

          subq_lamp_blink_7();

          //................................................. subq Inputs
          read_sub_inputs_7();

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

            if (alarm_blink_pin != 33)
            {
              after_ack_alarm_pin = alarm_blink_pin;
              alarm_blink_pin = 33;
            }//if
            //........................................
            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33 )
              {
                pin = subq_arr[i];
                lamp_state(pin, 1);

                after_ack_arr[after_ack_arr_int] = subq_arr[i];
                after_ack_arr_int++;
                subq_arr[i] = 33;
              }//if ! 33
            }//for

            ////Serial.println(alarm_blink_pin);
            ////Serial.println(after_ack_alarm_pin);
            
            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //..................................................... free logic
          //after ACK
          for (int i = 0; i < after_ack_arr_int; i++)
          {
            if (after_ack_arr[i] != 33)
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                //lamp_state(pin, 1);
              }//if
              else
              {
                lamp_state(pin, 0);
                after_ack_arr[i] = 33;
                solved_issues++;
              }//else read_input == LO
            }//if ! 33
          }//for

          //.....................................................  RST
          if (!read_rst())
          {
            if (after_ack_alarm_pin != 33)
            {
              //              lamp_state(after_ack_alarm_pin, 1);
              after_ack_arr[after_ack_arr_int] = after_ack_alarm_pin;
              after_ack_arr_int++;
              after_ack_alarm_pin = 33;
            }//if

            while (!read_rst())
            {
              ;
            }//while
          }//if rst

          //......................................................... break
          if (solved_issues == issues)
          {
            ////Serial.println("...... BREAKING OUT ........ ");
            break;
          }//if

          //.................................................. lamp_test
          if (!read_test())
          {
            lamp_test_off();
            while (!read_test())
            {
              delay(10);
            }//while
            lamp_test_on();

            //to keep lamp on after Lamp test
            for (int i = 0; i < after_ack_arr_int; i++)
            {
              if (after_ack_arr[i] != 33)
              {
                pin = after_ack_arr[i];
                lamp_state(pin, 0);

              }//if != 33
            }//

          }//if

          //................................................. delay
          //          delay(3000);
          delay(10);
        }//while

      }//case 7
      break;

    //........................................................................................................ seq 9
    case 9:
      {
        buz_state(1);

        while (1)
        {
          //................................................ LAMP
          if (alarm_blink_pin != 33)
            alarm_pin_blink_fast();

          //................................................. subq Inputs
          read_subq_inputs();

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

            if (alarm_blink_pin != 33)
            {
              lamp_state(alarm_blink_pin, 1);

              subq_arr[subq_arr_int] = alarm_blink_pin;
              subq_arr_int++;
              alarm_blink_pin = 33;
            }//if
            //........................................

            while (!read_ack())
            {
              ;
            }//while
          }//if

          //..................................................... RST
          if (!read_rst())
          {

            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33)
              {
                pin = subq_arr[i];

                if (read_alarm_input(pin))
                {
                  ;
                }//if
                else
                {
                  lamp_state(pin, 0);
                  subq_arr[i] = 33;
                  solved_issues++;
                }//else read_input == LO
              }//if ! 33
            }//for

            while (!read_rst())
            {
              ;
            }//while

          }//if rst

          //......................................................... break
          if (solved_issues == issues)
          {
            ////Serial.println("....... BREAKING OUT ...... ");
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

          //to keep lamp on after Lamp test
          for (int i = 0; i < subq_arr_int; i++)
          {
            if (subq_arr[i] != 33)
            {
              pin = subq_arr[i];
              lamp_state(pin, 1);

            }//if != 33
          }//
          //................................................. delay
          //          delay(3000);
          delay(10);
        }//while

      }//case 9
      break;

    //.............................................................................................. seq 10
    case 10:
      {
        buz_state(1);

        while (1)
        {
          //................................................ LAMP
          if (alarm_blink_pin != 33)
            alarm_pin_blink_faster();

          subq_lamp_blink();

          //................................................. subq Inputs
          read_subq_inputs();

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

            if (alarm_blink_pin != 33)
            {
              after_ack_alarm_pin = alarm_blink_pin;
              alarm_blink_pin = 33;
            }//if
            //........................................
            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33 )
              {
                //                pin = subq_arr[i];
                //                lamp_state(pin, 1);

                after_ack_arr[after_ack_arr_int] = subq_arr[i];
                after_ack_arr_int++;
                subq_arr[i] = 33;
              }//if ! 33
            }//for

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //..................................................... free logic
          //after ACK for Subq IPs //case 1
          for (int i = 0; i < after_ack_arr_int; i++)
          {
            if (after_ack_arr[i] != 33)
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1);
              }//if
              else
              {
                lamp_state(pin, 0);
                after_ack_arr[i] = 33;
                solved_issues++;
              }//else read_input == LO
            }//if ! 33
          }//for

          //.....................................................  RST
          if (!read_rst())
          {
            if (after_ack_alarm_pin != 33)
            {
              //              lamp_state(after_ack_alarm_pin, 1);
              after_ack_arr[after_ack_arr_int] = after_ack_alarm_pin;
              after_ack_arr_int++;
              after_ack_alarm_pin = 33;
            }//if

            while (!read_rst())
            {
              ;
            }//while
          }//if rst

          //......................................................... break
          if (solved_issues == issues)
          {
            ////Serial.println("....... BREAKING OUT ........ ");
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

          //................................................. delay
          //          delay(3000);
          delay(10);
        }//while

      }//case 10
      break;

    //.............................................................................................. seq 11
    case 11:
      {
        buz_state(1);

        while (1)
        {
          //................................................ LAMP
          if (alarm_blink_pin != 33)
            alarm_pin_blink_fast();

          //subq_lamp_blink();

          //................................................. subq Inputs
          read_subq_inputs();

          //.................................................. buz
          if (!read_mute())
          {
            //send_func('M');
            buz_state(0);
            //......................
            if (alarm_blink_pin != 33)
              after_ack_alarm_pin = alarm_blink_pin;

            //........................................
            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33 )
              {
                after_ack_arr[after_ack_arr_int] = subq_arr[i];
                after_ack_arr_int++;
                subq_arr[i] = 33;
              }//if ! 33
            }//for

            while (!read_mute())
            {
              ;
            }//while
          }//if mute

          //..................................................... free logic
          //after ACK for Subq IPs
          for (int i = 0; i < after_ack_arr_int; i++)
          {
            if (after_ack_arr[i] != 33)
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                lamp_state(pin, 1);
              }//if
              else
              {
                lamp_state(pin, 0);
                after_ack_arr[i] = 33;
                solved_issues++;
              }//else read_input == LO
            }//if ! 33
          }//for

          //.....................................................ACK
          if (!read_ack() )
          {
            if (after_ack_alarm_pin != 33)
            {
              pin = after_ack_alarm_pin;

              if (read_alarm_input(pin))
              {
                ;
              }//if
              else
              {
                lamp_state(pin, 0);
                after_ack_alarm_pin = 33;
                alarm_blink_pin = 33;
                solved_issues++;
              }//else read_input == LO

            }//if != 33

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while

          }//if ACK




          //......................................................... break
          if (solved_issues == issues)
          {
            ////Serial.println("....... BREAKING OUT ........ ");
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
            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33)
              {
                pin = subq_arr[i];
                lamp_state(pin, 1);
              }//if != 33
            }//

          }//if

          //................................................. delay
          //          delay(3000);
          delay(10);
        }//while

      }//case 11
      break;

    //.................................................................................... seq 12
    case 12:
      {
        buz_state(1);

        while (1)
        {
          //................................................ LAMP
          if (alarm_blink_pin != 33)
            alarm_pin_blink_faster();

          subq_lamp_blink();

          //................................................. subq Inputs
          read_subq_inputs();

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

            if (alarm_blink_pin != 33)
            {
              lamp_state(alarm_blink_pin, 1);
              //              after_ack_alarm_pin = alarm_blink_pin;

              after_ack_arr[after_ack_arr_int] = alarm_blink_pin;
              after_ack_arr_int++;
              alarm_blink_pin = 33;
            }//if
            //........................................
            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33 )
              {
                int pin = subq_arr[i];

                lamp_state(pin, 1);
                after_ack_arr[after_ack_arr_int] = pin;
                after_ack_arr_int++;
                subq_arr[i] = 33;
              }//if ! 33
            }//for

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //.....................................................  RST
          if (!read_rst())
          {

            for (int i = 0; i < after_ack_arr_int; i++)
            {
              if (after_ack_arr[i] != 33)
              {
                pin = after_ack_arr[i];

                if (read_alarm_input(pin))
                {
                  ;
                }//if
                else
                {
                  lamp_state(pin, 0);
                  after_ack_arr[i] = 33;
                  solved_issues++;
                }//else read_input == LO
              }//if ! 33

            }//for

            while (!read_rst())
            {
              ;
            }//while

          }//if rst

          //......................................................... break
          if (solved_issues == issues)
          {
            ////Serial.println("..... BREAKING OUT ..... ");
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
              if (after_ack_arr[i] != 33)
              {
                pin = after_ack_arr[i];
                lamp_state(pin, 1);

              }//if != 33
            }//

          }//if




          //................................................. delay
          //          delay(3000);
          delay(10);
        }//while

      }//case 12
      break;

    //.................................................................................... seq 13
    case 13:
      {
        buz_state(1);

        while (1)
        {
          //................................................ LAMP
          if (alarm_blink_pin != 33)
            alarm_pin_blink_faster();

          subq_lamp_blink();

          //................................................. subq Inputs
          read_subq_inputs();

          //................................................ free logic
          //before ACK for subQ
          for (int i = 0; i < subq_arr_int; i++)
          {

            if (subq_arr[i] != 33)
            {
              pin = subq_arr[i];
              if (read_alarm_input(pin))
              {
                ;
              }//if
              else
              {
                lamp_state(pin, 0);
                subq_arr[i] = 33;
                solved_issues++;
              }//else read_input == LOW
            }//if != 33

          }//for

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

            if (alarm_blink_pin != 33)
            {
              after_ack_alarm_pin = alarm_blink_pin;
            }//if
            //........................................
            for (int i = 0; i < subq_arr_int; i++)
            {
              if (subq_arr[i] != 33 )
              {
                int pin = subq_arr[i];

                lamp_state(pin, 1);
                after_ack_arr[after_ack_arr_int] = pin;
                after_ack_arr_int++;
                subq_arr[i] = 33;
              }//if ! 33
            }//for

            //TODO remove this wait
            while (!read_ack())
            {
              ;
            }//while
          }//if

          //................................................ free logic
          //after ACK for First OUt alarm pin
          if (after_ack_alarm_pin != 33)
          {
            if (read_alarm_input(after_ack_alarm_pin))
            {
              ;
            }
            else
            {
              lamp_state(after_ack_alarm_pin, 1);
              alarm_blink_pin = 33;
            }
          }//if

          //after ACK for subQ
          for (int i = 0; i < after_ack_arr_int; i++)
          {
            if (after_ack_arr[i] != 33)
            {
              pin = after_ack_arr[i];

              if (read_alarm_input(pin))
              {
                ;
              }//if
              else
              {
                lamp_state(pin, 0);
                after_ack_arr[i] = 33;
                solved_issues++;
              }//else read_input == LO
            }//if ! 33

          }//for


          //.....................................................  RST
          if (!read_rst())
          {
            lamp_state(after_ack_alarm_pin, 1);
            after_ack_arr[after_ack_arr_int] = after_ack_alarm_pin;
            after_ack_arr_int++;
            alarm_blink_pin = 33;
            after_ack_alarm_pin = 33;

            while (!read_rst())
            {
              ;
            }//while

          }//if rst

          //......................................................... break
          if (solved_issues == issues)
          {
            ////Serial.println(".... BREAKING OUT ....... ");
            break;
          }//if

          //................................................. delay
          //          delay(3000);
          delay(10);
        }//while

      }//case 13
      break;

    default:
      {
        //Nothing... YET
      }//default
      break;
  }//switch

}//void
