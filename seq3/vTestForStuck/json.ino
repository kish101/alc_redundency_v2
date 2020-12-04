

//....................................................................... deserialize Json
void deconstructSerJson(String input)
{
  DynamicJsonDocument doc(1023);
  String JsonObjectType;

  DeserializationError err = deserializeJson(doc, input);
  if (err)
  {
    //Serial.print(F("deserializeJson() failed with code ")); //Serial.println(err.c_str());
  }
  else
  {
    JsonObject obj = doc.as<JsonObject>();
    is_master = obj["is_master"].as<String>();
    //Serial.println(is_master);

    //................................................ master_name
    //................................................ master_name
    mod_bus_addr = obj["id"].as<signed int>();
    master_name = obj["master_name"].as<String>();
    master_ip = obj["master_ip"].as<String>();
    slave_name = obj["slave_name"].as<String>();
    slave_ip = obj["slave_ip"].as<String>();
    sequence_pattern_number = obj["sequence_number"].as<signed int>();
    shift_register_sequence = obj["exp_seq"].as<String>();
    exor_state_str = obj["exp_state"].as<String>();

    //..............
    //    //Serial.println("............. master..............");
    //    //Serial.println(master_name);
    //    //Serial.println(master_ip);
    //    //Serial.println(sequence_number);
    //    //Serial.println("............. slave..............");
    //    //Serial.println(slave_name);
    //    //Serial.println(slave_ip);

  }//else

  doc.clear();

}//void

//......................................................................... Construct json
void create_json()
{
  //  String m_n = master_name;
  //  String s_n = slave_name;
  DynamicJsonDocument  doc(400);

  doc["id"] = mod_bus_addr;
  doc["master_name"] = master_name;
  doc["master_ip"] = ETH.localIP().toString();
  doc["slave_name"] = slave_name;
  doc["slave_ip"] = "192.178.44.5";
  doc["sequence_number"] = sequence_pattern_number;
  doc["is_master"] = is_master;
  //  doc["exp_seq"] = "16,8";
  doc["exp_seq"] = shift_register_sequence;
  doc["exp_state"] = exor_state_str;

  serializeJson(doc, temp_str);
  serializeJson(doc, Serial);
  doc.clear();
}//void
