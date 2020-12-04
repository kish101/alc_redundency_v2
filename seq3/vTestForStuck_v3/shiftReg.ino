//******************************************************************************************* Read Shift Reg
void read_shift_regs_str()
{
  //Serial.println(">>>>>>>>> read_shift_regs_str() enrty  >>>>>>>>>");
  long bitVal;
  pinValueStr = "";

  /* Trigger a parallel Load to latch the state of the data lines,
  */
  digitalWrite(ploadPin, LOW);
  delayMicroseconds(PULSE_WIDTH_USEC);
  digitalWrite(ploadPin, HIGH);

  /* Loop to read each bit value from the serial out line
     of the SN74HC165N.
     for (int i = 0; i < DATA_WIDTH; i++)
  */
  for (int i = DATA_WIDTH; i > 0 ; i--)
  {
    bitVal = digitalRead(dataPin);
    if (bitVal == 1)
    {
      pinValueStr += '1';
      regBank.set(modbus_mapInputPin(DATA_WIDTH - i), 1);
    }
    else
    {
      pinValueStr += '0';
      regBank.set(modbus_mapInputPin(DATA_WIDTH - i), 0);
    }

    /* Pulse the Clock (rising edge shifts the next bit).
    */
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(clockPin, LOW);
  }//for

  //  Serial.println("pinValueStr : ");
  //  Serial.println(pinValueStr);
  //  delay(1000);

  //Serial.println(">>>>>>>>> read_shift_regs_str() EXIT  >>>>>>>>>");
}//void

//******************************************************************************************* Display Reg Values
void display_pin_values_str()
{
  //Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  //Serial.print("String value as pin state: ");
  //Serial.println(pinValueStr);
  //Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
}//void



BYTES_VAL_T read_shift_regs()
{
  return 1;
}
