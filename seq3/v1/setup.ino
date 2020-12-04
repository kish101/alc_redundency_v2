//*****************************************************************GPIO setup
void gpioSetup()
{
  pinMode(mute, INPUT);
  pinMode(ack, INPUT);
  pinMode(rst, INPUT);
  pinMode(test, INPUT);
  //........................................ I2C IO exp
  //  pinMode(buz, OUTPUT);
  //  pinMode(buz_rb, OUTPUT);
  mcp.pinMode(buz, OUTPUT);
  //  mcp.pullUp(buz, HIGH);
  mcp.pinMode(buz_rb, OUTPUT);
  mcp.pinMode(running_flasher, OUTPUT);
  mcp.pinMode(error_led, OUTPUT);
  mcp.pinMode(buffer_enable_pin, OUTPUT);
  delay(100);
  //  mcp.digitalWrite(buz, HIGH);
  //  mcp.digitalWrite(buz_rb, HIGH);
  //  mcp.digitalWrite(running_flasher, LOW);
  //  mcp.digitalWrite(error_led, LOW);
  //  delay(100);

  //  mcp.digitalWrite(buz, LOW);
  //  delay(1000);
  //  mcp.digitalWrite(buz_rb, LOW);
  //  delay(1000);
  //  mcp.digitalWrite(running_flasher, HIGH);
  //  delay(1000);
  //  mcp.digitalWrite(error_led, HIGH);
  //  delay(1000);
  //  mcp.digitalWrite(buffer_enable_pin, HIGH);
  //  delay(100);
  //  mcp.pinMode(2, OUTPUT);
  //  mcp.pinMode(3, OUTPUT);

  mcp.digitalWrite(buz, HIGH);
  mcp.digitalWrite(buz_rb, HIGH);
  mcp.digitalWrite(running_flasher, LOW);
  mcp.digitalWrite(error_led, LOW);
  mcp.digitalWrite(buffer_enable_pin, HIGH);
  delay(100);
  //........................................ Input Shift Reg
  pinMode(ploadPin, OUTPUT);
  //  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);

  digitalWrite(clockPin, LOW);
  digitalWrite(ploadPin, HIGH);

  //........................................
  modbus_setup();
  waitForSeqNum();
  
}//void


//************************************************************************************************** Expansion Board
void expansion_board()
{
  bit_8 = 0;
  bit_16 = 0;
  uint8_t x;
  numberOfShiftRegisters = 0;
  shift_register_sequence = "16";

  //  dashBoard_str = "";

  expansion_board_count = sensors.getDeviceCount();

  //................................
  for (int i = 0; i < expansion_board_count; i++)
  {
    if (!sensors.getAddress(hope, i))
    {
      Serial.print("Unable to find address for Device ");
      Serial.println(i);
    }

    x = hope[0];

    //.............................
    if (x == DS18B20MODEL)
    {
      //      Serial.println("8 bit");
      bit_8++;
      shift_register_sequence += ",8";
    }
    else if (x == DS1822MODEL)
    {
      //      Serial.println("16 bit");
      bit_16++;
      shift_register_sequence += ",16";
    }
  }//for

  //  shift_register_sequence += '\0';
  numberOfShiftRegisters = 2 + bit_8 + (2 * bit_16);

  Serial.print("8_bit boards count: ");
  Serial.println(bit_8);
  Serial.print("16_bit boards count: ");
  Serial.println(bit_16);
  Serial.print("number of Shift registers: ");
  Serial.println(numberOfShiftRegisters);

  //................................
  total_IO = (numberOfShiftRegisters * 8);
  Serial.print("total IOs : ");
  Serial.println(total_IO);

  //.......................................................... set OP/IP
  sr.setNoOfShiftRegisters(numberOfShiftRegisters);
  DATA_WIDTH = (numberOfShiftRegisters * 8);


  //.......................................................... check Exor string
  exor_state_arr_check();

  //..................
  for (int i = 0; i < (DATA_WIDTH - 16); i++)
    dashBoard_str += '0';

  Serial.println("dashBoard str: ");
  Serial.println(dashBoard_str);
  delay(4000);
  //  dashBoard_str += '\0';
}//void

//*****************************************************************get seq num
void waitForSeqNum()
{
  if (sequence_pattern_number == 0)
  {
    Serial.println("Sequence in 0" );
    while (sequence_pattern_number == 0)
    {
      webSocket.loop();
    }//while

    Serial.println("Got the sequence number");
    delay(100);
  }//if
}//void

//*****************************************************************part of sequence
int part_of_single_seq_arr(int which)
{
  for (int i = 0 ; i < 8; i++)
  {
    if (single_seq_arr[i] == which)
      return 1;
  }//for

  return 0;
}

//***************************************************************** which seq_arr
void check_for_single_seq()
{
  if (part_of_single_seq_arr(sequence_pattern_number))
    single_seq_flag = 1;
  else
    single_seq_flag = 0;

  if (single_seq_flag == 1)
    ;//Serial.println("single seq arr");
  else
    ;//Serial.println("SubQ seq arr");
}//void


//***************************************************************** Exor
void exor_state_arr_check()
{
  int exor_len = exor_state_str.length();

  if (exor_len <= DATA_WIDTH)
  {
    for (int i = exor_len ; i <= (DATA_WIDTH - 1); i++)
    {
      exor_state_str += '0';
    }//for
  }//if

}//void
/*
  void waitForSeqNum()
  {
  ////Serial.println("waiting for Seq Number");
  while (!Serial.available())
  {

  }//while
  if (Serial.available())
  {
    for (int i = 0; i < 3; i++)
    {
      char x = Serial.read();
      seq_number += x;
      delay(10);
    }//for
  }//if

  ////Serial.print("Seq number is: ");
  sequence_pattern_number = atoi(&seq_number[0]);
  ////Serial.println(sequence_pattern_number);
  }//void
*/
