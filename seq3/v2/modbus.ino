//....................................................................... setup
void modbus_setup()
{
  //Assign the modbus device ID.
  regBank.setId(1);

  //Add Digital Output registers 00001-00016 to the register bank
  regBank.add(1);
  regBank.add(2);
  regBank.add(3);
  regBank.add(4);
  regBank.add(5);
  regBank.add(6);
  regBank.add(7);
  regBank.add(8);
  regBank.add(9);
  regBank.add(10);
  regBank.add(11);
  regBank.add(12);
  regBank.add(13);
  regBank.add(14);
  regBank.add(15);
  regBank.add(16);

  //Add Digital Input registers 10001-10008 to the register bank
  regBank.add(10001);
  regBank.add(10002);
  regBank.add(10003);
  regBank.add(10004);
  regBank.add(10005);
  regBank.add(10006);
  regBank.add(10007);
  regBank.add(10008);
  regBank.add(10009);
  regBank.add(10010);
  regBank.add(10011);
  regBank.add(10012);
  regBank.add(10013);
  regBank.add(10014);
  regBank.add(10015);
  regBank.add(10016);

  slave._device = &regBank;

  // Initialize the serial port for coms at 9600 baud
  slave.setBaud(9600);
}//void

//....................................................................... default
void modbus_setDefaultValues()
{

  regBank.set(1, 0);
  regBank.set(2, 0);
  regBank.set(3, 0);
  regBank.set(4, 0);
  regBank.set(5, 0);
  regBank.set(6, 0);
  regBank.set(7, 0);
  regBank.set(8, 0);
  regBank.set(9, 0);
  regBank.set(10, 0);
  regBank.set(11, 0);
  regBank.set(12, 0);
  regBank.set(13, 0);
  regBank.set(14, 0);
  regBank.set(15, 0);
  regBank.set(16, 0);
  
  regBank.set(10001, 0);
  regBank.set(10002, 0);
  regBank.set(10003, 0);
  regBank.set(10004, 0);
  regBank.set(10005, 0);
  regBank.set(10006, 0);
  regBank.set(10007, 0);
  regBank.set(10008, 0);
  regBank.set(10009, 0);
  regBank.set(10010, 0);
  regBank.set(10011, 0);
  regBank.set(10012, 0);
  regBank.set(10013, 0);
  regBank.set(10014, 0);
  regBank.set(10015, 0);
  regBank.set(10016, 0);
}//void

//....................................................................... inout pin mapping
int modbus_mapInputPin(int pin)
{
  int y = map(pin, 0, 15, 10001, 10016);
  return y;
}//void
