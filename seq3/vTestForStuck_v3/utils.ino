
//.............................................................................................................. Reset
void reset()
{
  //Serial.println(">>>>>>>>> reset  >>>>>>>>>");
  delay(500);
  //    memset(alarm_arr, 0, sizeof(alarm_arr));
  //memset(after_ack_arr, 0, sizeof(after_ack_arr));
  //  memset(slow_blink_arr, 0, sizeof(slow_blink_arr));
  //  memset(subq_arr, 0, sizeof(subq_arr));
  //  memset(dashBoard_arr, 0, sizeof(dashBoard_arr));

  wb_loop_count = 0;

  //.............
  //..................
  for (int i = 0; i < DATA_WIDTH; i++)
    dashBoard_str[i] = '0';
  //  dashBoard_str = "0000000000000000000000000000000000000000";
  //................
  if ( (ws_db_connect_flag == 1) )
  {
    //Serial.println(dashBoard_str);
    webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
  }//if

  //.............
  alarm_arr_int = 0;
  for (int i = 0; i < 40; i++)
  {
    alarm_arr[i] = 201;//33
    delay(10);
  }

  //.............
  after_ack_arr_int = 0;
  for (int i = 0; i < 40; i++)
  {
    after_ack_arr[i] = 201;//33
    delay(10);
  }

  //.............
  slow_blink_arr_int = 0;
  for (int i = 0; i < 40; i++)
  {
    slow_blink_arr[i] = 201;//33
    delay(10);
  }

  //.............
  subq_arr_int = 0;
  for (int i = 0; i < 40; i++)
  {
    subq_arr[i] = 201;//33
    delay(10);
  }
  //  memset(solid_state_arr, 0, sizeof(solid_state_arr));
  //  solid_state_arr_int = 0;

  modbus_setDefaultValues();

  alarm_blink_pin = 201;//33
  after_ack_alarm_pin = 201;//33

  issues = 0;
  solved_issues = 0;

  state = 0;
  count = 50;
  sub_state = 0;
  sub_count = 100;
  subq_state = 0;
  subq_count = 25;

  imt_count = 6;
  imt_flag = 0;

  rf_count = 60;
  rf_state = 0;
  //  buz_state(0);
  //  buz_rb_state(0);

  mcp.digitalWrite(buz, HIGH);
  mcp.digitalWrite(buz_rb, HIGH);
  mcp.digitalWrite(error_led, HIGH);

  pin = 33;

  redundancy_str = redundancy_reset_srt;
  send_redundancy_str_func();

  redundancy_str = "";

  if (sequence_pattern_number == 4)
    lamp_test_on();
  else
    lamp_test_off();

}//void

//************************************************************************************************** choose soft serial port
int softSerial_config()
{
  //...................................................FS read
  //  File file2 = SPIFFS.open("/is_master.txt");
  //
  //  while (file2.available()) {
  //
  //    is_master += (char)file2.read();
  //  }
  //  file2.close();


  //...................................................

  if (is_master == "true")
  {
    //Serial.println("this ALC device is MASTER");
    return 1;
  }
  else
  {
        //Serial.println("this ALC device is SLAVE");
    return 0;
  }//else

}//int

//........................................................................................................ Lamp test
void lamp_test_on()
{
  //////Serial.println("lamp test on");
  sr.setAllLow();
  //  for (int i = 0; i < DATA_WIDTH; i++)
  //  {
  //    sr.set(i, LOW);
  //    delay(10);
  //  }//for

}
void lamp_test_off()
{
  //////Serial.println("lamp test off");
  sr.setAllHigh();
  //  for (int i = 0; i < DATA_WIDTH; i++)
  //  {
  //    sr.set(i, HIGH);
  //    delay(10);
  //  }//for

}

//****************************************************************************************************** running_flasher blink
void running_flasher_blink_slow()
{
  if (rf_count == 60)
  {
    rf_state = !rf_state;

    if (rf_state == 1)
      mcp.digitalWrite(running_flasher, HIGH);
    else
      mcp.digitalWrite(running_flasher, LOW);

    rf_count = 0;
  }//if count 25

  rf_count++;
}//void


//****************************************************************************************************** temp_stuck
void temp_stuck()
{
  while (1)
  {
    //Serial.println("temp stuck");
    delay(1000);
  }//while
}//void


//****************************************************************************************************** print
void print_arr(int which)
{
  switch (which)
  {
    case 0:
      {
        //Serial.println("............. alarm arr ............");
        for (int p = 0; p < (alarm_arr_int + 1); p++)
        {
          //Serial.print(alarm_arr[p]);
          //Serial.print(",");
        }
        //Serial.println();
      }//case0
      break;

    case 1:
      {
        //Serial.println("............. after ack arr ............");
        for (int p = 0; p < (after_ack_arr_int + 1); p++)
        {
          //Serial.print(after_ack_arr[p]);
          //Serial.print(",");
        }
        //Serial.println();
      }//case1
      break;

    case 2:
      {
        //Serial.println("............. slow blink arr ............");
        for (int p = 0; p < 20; p++)
        {
          //Serial.print(slow_blink_arr[p]);
          //Serial.print(",");
        }
        //Serial.println();
      }//case1
      break;

    case 3:
      {
        //Serial.println("............. subQ arr ............");
        for (int p = 0; p < 20; p++)
        {
          //Serial.print(subq_arr[p]);
          //Serial.print(",");
        }
        //Serial.println();
      }//case1
      break;

    case 4:
      {
        //Serial.println("............. alarm_blink_pin ............");
        //Serial.print(alarm_blink_pin);
        //Serial.println();
      }//case1
      break;

    case 5:
      {
        //Serial.println("............. after_ack_alarm_pin ............");
        //Serial.print(after_ack_alarm_pin);
        //Serial.println();
      }//case1
      break;

    case 6:
      {
        //..............
//        Serial.println("............. master..............");
//        Serial.println(master_name);
//        Serial.println(master_ip);
//        Serial.println(sequence_pattern_number);
//        Serial.println(mod_bus_addr);
//        Serial.println("............. slave..............");
//        Serial.println(slave_name);
//        Serial.println(slave_ip);
//        Serial.println("............. Exp..............");
//        Serial.println(shift_register_sequence);
//        Serial.println(exor_state_str);
      }//
      break;

    case 7:
      {
        //..............
        //Serial.print("pinValueStr: "); //Serial.println(pinValueStr);
        //Serial.print("oldPinValueStr: "); //Serial.println(oldPinValueStr);
        //Serial.print("exor_state_str: "); //Serial.println(exor_state_str);
      }//
      break;

    case 8:
      {
        //Serial.println("............. ack pressed ............");
        //Serial.println(alarm_arr_int);
        //Serial.println(after_ack_arr_int);
        //Serial.println();
      }//case1
      break;

    default:
      //Serial.println("guruuu");
      break;
  }//switch
}//void

//************************************************************ ack K
void send_func(char cur_data)
{
  //  if (cur_data != prev_data)
  //  {
  //    Serial2.print(cur_data);
  //    responce_func();
  //  }//if
  //  prev_data = cur_data;
}//void

//************************************************************ ack K
void responce_func()
{
  //  char r_data;

  //  while (r_data != 'K')
  //  {
  //    if (Serial2.available())
  //    {
  //      r_data = char(Serial2.read());
  //    }//if
  //
  //  }//while
}//void

/*
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
            for (int i = 0; i < after_ack_arr_int; i++)
            {
              if (after_ack_arr[i] != 33)
              {
                pin = after_ack_arr[i];
                lamp_state(pin, 1);

              }//if != 33
            }//
*/
