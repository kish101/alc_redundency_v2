
//................................................ read
void read_config_data()
{
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  File file2 = SPIFFS.open("/config_data.txt");

  if (!file2) {
    Serial.println("Failed to open file for reading");
    //    return;
  }
  else
  {
    //    Serial.println("File ready to open");
  }

  //  Serial.println("File Content:");

  while (file2.available())
  {
    //    Serial.write(file2.read());
    char x = (char)file2.read();
    config_file_str += x;
  }//while

  file2.close();
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  deconstructSerJson(config_file_str);
  Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>confi.txt credentials in 5 seconds");
  print_arr(6);
  delay(3000);
}//void


//...................................................................................... write
void write_config_data(String userData)
{
  File file = SPIFFS.open("/config_data.txt", FILE_WRITE);

  if (!file) {
    Serial.println("There was an error opening the file for writing");
    return;
  }
  file.print(userData);

  file.close();
}//void


//......................................................................................file transfer
void configFile_transfer()
{
  mySerial_master.println(config_file_str);
  //.....................
  int count = 0;
  int times = 300;
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
    delay(10);
  }//while
}//void


//......................................................................................file read
int configFile_read()
{
  int count = 0;
  int times = 300;
  int flag = 0;
  char c = 'x';

  config_file_str = "";
  //.....................................................loop start
  while (1)
  {
    if (mySerial_slave.available())
    {
      config_file_str = "";

      for (int i = 0; (c != '}'); i++)
      {
        c = mySerial_slave.read();
        //        Serial.print(c);
        config_file_str += c;
        delayMicroseconds(1100);

      }//while avai
      flag = 1;
    }//if

    //.........................................................................
    if (flag == 1)
    {
      write_config_data(config_file_str);
      mySerial_slave.print("K");
      flag = 0;
      break;
      //      return 1;
    }//if flag

    if (count >= times )
    {
      //      return 0;
      Serial.println("timeout");
      break;
    }//if

    //................................
    count++;
    delay(10);
  }//while
  //.....................................................loop end

  deconstructSerJson(config_file_str);
  return 1;
}//int
