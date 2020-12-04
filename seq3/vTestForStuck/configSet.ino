
//................................................ read
void read_config_data()
{
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  File file2 = SPIFFS.open("/config_data.txt");

  if (!file2) {
    //Serial.println("Failed to open file for reading");
    //    return;
  }
  else
  {
    //    //Serial.println("File ready to open");
  }

  //  //Serial.println("File Content:");

  while (file2.available())
  {
    //    //Serial.write(file2.read());
    char x = (char)file2.read();
    config_file_str += x;
  }//while

  file2.close();
  //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  deconstructSerJson(config_file_str);
  //Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>confi.txt credentials in 5 seconds");
  print_arr(6);
  delay(3000);
}//void


//...................................................................................... write
void write_config_data(String userData)
{
  File file = SPIFFS.open("/config_data.txt", FILE_WRITE);

  if (!file) {
    //Serial.println("There was an error opening the file for writing");
    return;
  }
  file.print(userData);

  file.close();
}//void
