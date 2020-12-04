
//************************************************************************************************** master
int inital_uart_setup_master()
{
  int count = 50;
  int times = 50;
  int red_slave_present = 0;

  while (1)
  {
    if (count >= times)
    {
      mySerial_master.print("P");
      count = 0;
      red_slave_present++;
    }//if

    if (mySerial_master.available() )
    {
      char c = mySerial_master.read();

      if (c == 'P')
      {
        return 1;
        break;
      }//if
    }//if avai

    if (red_slave_present >= 6)
    {
      return 0;
      break;
    }

    count++;
    delay(10);
  }//while
}//void


//************************************************************************************************** slave
int inital_uart_setup_slave()
{
  int count = 0;
  int times = 300;

  while (1)
  {

    if (mySerial_slave.available() )
    {
      char c = mySerial_slave.read();

      if (c == 'P')
      {
        mySerial_slave.print("P");
        return 1;
      }//if
    }//if avai

    if (count >= times)
    {
      return 0;
    }

    count++;
    delay(10);
  }//while
}//void

//************************************************************************************************** core 0
void task1Code(void * pvParameters)
{
  // put your main code here, to run repeatedly:
  if (softUart_select)
  {
    //    mySerial_master.println("master_data");
    for (;;)
    {
      if (kish_lock)
      {
        kish_lock = 0;
        mySerial_master.print("X");
        kish_lock = 1;
      }
      //      mySerial_master.println(xPortGetCoreID());
      delay(250);
    }//for
  }//if
  else
  {
    int count = 0;
    int times = 110;

    for (;;)
    {
      if (mySerial_slave.available() )
      {
        char x = mySerial_slave.read();

        if ( x == 'X' )
        {
          Serial.println("slave inactive");
          slave_start = 0;
          count = 0;
        }//if


      }//if avai

      if (count >= times )
      {
        Serial.println("slave active");
        slave_start = 1;
      }

      count++;
      delay(10);
    }//for
  }//else

}//void




//************************************************************************************************** core 0
void send_redundancy_str_func()
{
  //........................
  while (!kish_lock)
    ;
  kish_lock = 0;
  mySerial_master.print(redundancy_str);
  //.....................
  int count = 0;
  int times = 100;
  while (1)
  {
    if (mySerial_master.available() )
    {
      char c = mySerial_master.read();

      if (c == 'K')
      {
        Serial.println("_");
        break;
      }//if
    }//if avai

    if (count >= times)
    {
      break;
    }

    count++;
    delay(1);
  }//while

  //....................................
  kish_lock = 1;

}//void
