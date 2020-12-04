#include <WiFi.h>
#include <SPIFFS.h>

#include <SoftwareSerial.h>
#include <ETH.h>

#include <ESPAsyncWebServer.h>

#include <WebSocketsServer.h>
#include <uSSDP.h>

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ShiftRegister74HC595Edit.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Wire.h>
#include "Adafruit_MCP23008.h"

//#include <ShiftRegister74HC595.h>

//#include <uSSDP.h>

#include <modbus.h>
#include <modbusDevice.h>
#include <modbusRegBank.h>
#include <modbusSlave.h>

//................................................ Constants for Wifi

const char* r_ssid = "GraspIO_AP";
const char* r_password = "s49591513038**";

char ssid[20];
char *password;
const char *msg_toggle_led = "toggleLED";
const char *msg_get_led = "getLEDState";
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;

//............................................... webserver
AsyncWebServer server(80);

WebSocketsServer webSocket_dashBoard = WebSocketsServer(4000);
WebSocketsServer webSocket = WebSocketsServer(1337);

uDevice device;
uSSDP SSDP;

char msg_buf[500];

//******************************************************************************************* I2c IO expander
Adafruit_MCP23008 mcp;

//******************************************************************************************* Output Board Settings

// create a global shift register object
// parameters: (number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 sr (2, 33, 12, 14);
//ShiftRegister74HC595 sr (4, 25, 27, 26);


//******************************************************************************************* ModBus
modbusDevice regBank;
modbusSlave slave;


//******************************************************************************************* Soft Serial
int softUart_select = 1;

SoftwareSerial mySerial_master(15, 5); // RX, TX
SoftwareSerial mySerial_slave(5, 15); // RX, TX

TaskHandle_t Task1;

int slave_start = 0;

//******************************************************************************************* Dalas
#define ONE_WIRE_BUS 4
#define DS18B20MODEL 40
#define DS1822MODEL 34
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
// arrays to hold device addresses
DeviceAddress hope;

int bit_8 = 0;
int bit_16 = 0;
int total_IO = 0;
int expansion_board_count = 0;
int numberOfShiftRegisters = 0;

//******************************************************************************************* Ethernet
//. working for 33 -> 201. and 20 -> 200
#ifdef ETH_CLK_MODE
#undef ETH_CLK_MODE
#endif

#define ETH_CLK_MODE    ETH_CLOCK_GPIO17_OUT

// Pin# of the enable signal for the external crystal oscillator (-1 to disable for internal APLL source)
#define ETH_POWER_PIN   -1

// Type of the Ethernet PHY (LAN8720 or TLK110)
#define ETH_TYPE        ETH_PHY_LAN8720

// I²C-address of Ethernet PHY (0 or 1 for LAN8720, 31 for TLK110)
#define ETH_ADDR        0

// Pin# of the I²C clock signal for the Ethernet PHY
#define ETH_MDC_PIN     23

// Pin# of the I²C IO signal for the Ethernet PHY
#define ETH_MDIO_PIN    18


//******************************************************************************************* Input Board Settings

int NUMBER_OF_SHIFT_CHIPS = 0;
int DATA_WIDTH = 0;
#define PULSE_WIDTH_USEC   5
#define POLL_DELAY_MSEC   1
#define BYTES_VAL_T unsigned long

int ploadPin        = 16;  // Connects to Parallel load pin the 165
//int clockEnablePin  = 9;  // Connects to Clock Enable pin the 165
int dataPin         = 32; // Connects to the Q7 pin the 165
int clockPin        = 13; // Connects to the Clock pin the 165
//int ploadPin        = 2;  // Connects to Parallel load pin the 165
//int clockEnablePin  = 9;  // Connects to Clock Enable pin the 165
//int dataPin         = 5; // Connects to the Q7 pin the 165
//int clockPin        = 4; // Connects to the Clock pin the 165

BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;

String pinValueStr;
String oldPinValueStr;

//............................................................... Config mode
String master_name = "" , master_ip = "", is_master = "";
String slave_name = "", slave_ip = "";
int sequence_number = 0, mod_bus_addr = 0;
String shift_register_sequence = "", exor_state_str = "";

String config_file_str = "";

//************************************************************************ PushButtons
//int input = 15;  :(
int ack = 39;
int mute = 34;
int rst = 36;
int test = 35;

int buz = 0;
int buz_rb = 1;
int running_flasher = 2;
int error_led = 3;
int buffer_enable_pin = 4;
//int lamp = 33;  :(

//*********************************************************************** initials
int sequence_pattern_number = 0;

int single_seq_arr[8] = {1, 2, 3, 4, 8, 14, 15, 16};
int single_seq_flag = 0;

String seq_number = "";

//********************************************************************** variables
int alarm_arr[200];
int alarm_arr_int = 0;
int after_ack_arr[200];
int after_ack_arr_int = 0;
int slow_blink_arr[200];
int slow_blink_arr_int = 0;
//int solid_state_arr[20];
//int solid_state_arr_int = 0;
int subq_arr[200];
int subq_arr_int = 0;
//int dashBoard_arr[16];
String dashBoard_str = "0000000000000000";
uint8_t dashBoard_wb_num = 0;
int ws_db_connect_flag = 0;

int alarm_blink_pin = 201;//33
int after_ack_alarm_pin = 201;//33

int issues = 0;
int solved_issues = 0;

int count = 50;
int state = 0;
int sub_count = 100;
int sub_state = 0;

int subq_count = 25;
int subq_state = 0;

int imt_count = 3;
int imt_flag = 0;

int wb_loop_count = 500;

int pin = 33;

int rf_count = 60;
int rf_state = 0;

String temp_str = "";

//*********************************************************************** redundancy
String redundancy_str = "";
String redundancy_read_str = "";
// 0 -> do not process redundancy_str, 1 -> process redundancy_str
int redundancy_str_read_flag = 0;
//1 -> redundance present, 0-> redundancy absent
//if 1-> transfer redundancy_str, 0-> do not transfer redundancy_str
int redundancy_flag = 0;
//if 0-> send redundancy_str, if 1-> send "X"
int kish_lock = 1;
//
String redundancy_reset_srt = "R0.";
//0 -> mute pressed, 1 -> not pressed
//1 -> normal, i.e. for master and for slave red absent, for red slave present but mute not pressed
//0 -> red slave present and mute pressed
int buz_state_flag = 1;
//
//int pin = 201;
int arr_pos_int = 0;

//*********************************************************************** local
int local_int = 0;
int local_int_2 = 0;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ Ethernet
static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      ////Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      ////Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      ////Serial.print("ETH MAC: ");
      ////Serial.print(ETH.macAddress());
      ////Serial.print(", IPv4: ");
      ////Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        ////Serial.print(", FULL_DUPLEX");
      }
      ////Serial.print(", ");
      ////Serial.print(ETH.linkSpeed());
      ////Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      ////Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      ////Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}//void


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ webSocket / dashBoard

void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
  const uint8_t* src = (const uint8_t*) mem;
  ////Serial.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
  for (uint32_t i = 0; i < len; i++) {
    if (i % cols == 0) {
      ////Serial.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
    }
    ////Serial.printf("%02X ", *src);
    src++;
  }
  ////Serial.printf("\n");
}

void webSocketEvent_dashBoard(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      ////Serial.printf("[%u] Disconnected!\n", num);
      ws_db_connect_flag = 0;
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket_dashBoard.remoteIP(num);
        ////Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // send message to client
        //webSocket.sendTXT(num, "Connected");
        ws_db_connect_flag = 1;
        dashBoard_wb_num = num;
        webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);
      }
      break;
    case WStype_TEXT:
      ////Serial.printf("[%u] get Text: %s\n", num, payload);
      // send message to client
      // webSocket.sendTXT(num, "message here");
      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      ////Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}//void

void setSSDP() {
  byte mac[6];
  char base[UUIDBASE_SIZE];
  WiFi.macAddress(mac);
  sprintf(base, "esp8266x-%02x%02x-%02x%02x-%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  ////Serial.printf("Starting uSSDP-ESP32: BASE: %s, MAC: %02x:%02x:%02x:%02x:%02x:%02x\n", base, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  device.begin((const char*)base, mac);
  device.serialNumber((char*)"1241");
  device.manufacturer((char*)"GraspIO");
  device.manufacturerURL((char*)"http://grasp.io");
  device.modelName((char*)"Werner ALC");
  device.modelNumber(1, 0);
  device.friendlyName(ssid);
  device.presentationURL((char*)"/"); SSDP.begin(&device);
  ////Serial.println("SSDP Started");
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ webSocket / config screen
// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length)
{
  // Figure out the type of WebSocket event
  switch (type) {

    // Client has disconnected
    case WStype_DISCONNECTED:
      ////Serial.printf("[%u] Disconnected!\n", client_num);
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        ////Serial.printf("[%u] Connection from ", client_num);
        ////Serial.println(ip.toString());
        delay(1000);
        ////Serial.println("sending the data");

        //        webSocket.sendTXT(client_num, "{\"master_name\":\"Alc\",\"master_ip\":\"192.168.2.1\",\"slave_name\":\"Alc_slave\",\"slave_ip\":\"192.168.2.2\",\"sequence_number\":2,\"is_master\":true,\"exp_seq\":\"16,8,8\",\"exp_state\":\"00000000111111110000111100001100\" }");
        //        webSocket.sendTXT(client_num, "{\"master_name\":\"Yes\",\"master_ip\":\"192.168.2.3\",\"slave_name\":\"yes_slave\",\"slave_ip\":\"128.338.5.6\",\"sequence_number\":" + sequence_pattern_number + ",\"is_master\":true,\"exp_seq\":\"16,8,8\",\"exp_state\":\"00000000111111110000111100001100\"}");
        //        temp_str = "{\"master_name\":\"" + master_name + "\",\"master_ip\":\"192.168.2.1\",\" " + slave_name + "\":\"Alc_slave\",\"slave_ip\":\"192.168.2.2\",\"sequence_number\":" + sequence_pattern_number + ",\"is_master\":true,\"exp_seq\":\"16,8,8\",\"exp_state\":\"00000000111111110000111100001100\"}";
        create_json();
        webSocket.sendTXT(client_num, temp_str);
        temp_str = "";
      }
      break;

    // Handle text messages from client
    case WStype_TEXT:
      {
        // Print out raw message
        ////Serial.printf("[%u] Received text: %s\n", client_num, payload);
        String userData = String((char *) &payload[0]);
        //>>>> JSON deconstruct
        deconstructSerJson(userData);
        ////Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>new COnfig data from user is as follows: 5 sec count down");
        print_arr(6);
        delay(3000);
        //>>>>>write the New Config to "config.txt"
        ////Serial.println("writing to SPIFFS");
        write_config_data(userData);
        //>>>>> restart ESP
        ////Serial.println("RESTARTING in 3 seconds");
        delay(3000);
        ESP.restart();
      }
      break;

    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  ////Serial.println("[" + remote_ip.toString() +
   //              "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/index.html", "text/html");
}

// Callback: send style sheet
// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  ////Serial.println("[" + remote_ip.toString() +
   //              "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

//*****************************************************************setup
void setup()
{
  // put your setup code here, to run once:
  //Serial.begin(115200);
  delay(1000);
  //Serial.println("welcome..");

  //........................................Make sure we can read the file system
  if ( !SPIFFS.begin())
  {
    //Serial.println("Error mounting SPIFFS");
    while (1);
  }
  delay(100);

  //..................................................................... read config file
  read_config_data();

  //..................................................................... master or slave flag set
  softUart_select = softSerial_config();

  //..................................................................... gpio setup
  //.......................................
  Wire.begin(0, 2);
  delay(100);
  //.......................................
  mcp.begin();
  delay(500);
  //.......................................
  gpioSetup();
  //.......................................
  if (softUart_select)
  {
    intial_gpio_config();
  }
  else
  {
    //    if (sequence_pattern_number == 0)
    intial_gpio_config();
  }//else

  //..................................................................... set software serial
  {
    if (softUart_select)
    {
      mySerial_master.begin(9600);
      delay(1000);
      //Serial.print("configured as master and ");

      if (inital_uart_setup_master())
      {
        //Serial.println("redundency slave present");
        redundancy_flag = 1;
        //......................................... Core 0
        xTaskCreatePinnedToCore(task1Code, "Task1", 10000, NULL, 1, &Task1, 0);
        delay(500);
      }//if
      else
      {
        //Serial.println("redundency slave absent");
        //        mySerial_master.end();
        redundancy_flag = 0;
      }//else

    }//if
    else
    {
      mySerial_slave.begin(9600);
      delay(1000);
      //Serial.print("configured as slave and ");

      if (inital_uart_setup_slave())
      {
        //Serial.println("redundency master present");
        redundancy_flag = 1;
        //......................................... Core 0
        //        xTaskCreatePinnedToCore(task1Code, "Task1", 10000, NULL, 1, &Task1, 0);
        //        delay(500);
      }//if
      else
      {
        //Serial.println("redundency master absent");
        //        mySerial_slave.end();
        redundancy_flag = 0;
      }//else
    }//else
  }//scope

  //.....................................................................
  //initial config for slave and red absent
  if ((softUart_select == 0) && (redundancy_flag == 0))
  {
    intial_gpio_config();
  }//if

  //..................................................................... set ssid name
  if (softUart_select)
  {
    master_name.toCharArray(ssid, 20);
  }
  else
  {
    slave_name.toCharArray(ssid, 20);
  }//else

  //.....................................................................Setup Ethernet
  WiFi.onEvent(WiFiEvent);
  //  mySerial_master.println(xPortGetCoreID());
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE, ETH_CLK_MODE);
  delay(500);

  //..................................................................... AP mode
  //Serial.print("setting AP name as: ");
  //Serial.println(ssid);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid);
  delay(1000);

  //..................................................................... Config screen setup
  {
    //.............On HTTP request for root, provide index.html file
    server.on("/", HTTP_GET, onIndexRequest);

    //.............On HTTP request for style sheet, provide style.css
    server.on("/src.21df5e84.css", HTTP_GET, onCSSRequest_src1);
    server.on("/materialize.min.dad11602.css", HTTP_GET, onCSSRequest_materialize_min);
    server.on("/material-icons.79cbed4e.css", HTTP_GET, onCSSRequest_material_icon);

    //.............On HTTP request for style sheet, JS files
    server.on("/src.c291981e.js", HTTP_GET, onJSRequest_src1);
    server.on("/materialize.min.0fe193ae.js", HTTP_GET, onJSRequest_materialize_min);


    //.............On HTTP request for style sheet, IMages
    server.on("/assets/png/1.png", HTTP_GET, onImageReq_1);
    server.on("/assets/png/2.png", HTTP_GET, onImageReq_2);
    server.on("/assets/png/3.png", HTTP_GET, onImageReq_3);
    server.on("/assets/png/4.png", HTTP_GET, onImageReq_4);
    server.on("/assets/png/5.png", HTTP_GET, onImageReq_5);
    server.on("/assets/png/6.png", HTTP_GET, onImageReq_6);
    server.on("/assets/png/7.png", HTTP_GET, onImageReq_7);
    server.on("/assets/png/8.png", HTTP_GET, onImageReq_8);
    server.on("/assets/png/9.png", HTTP_GET, onImageReq_9);
    server.on("/assets/png/10.png", HTTP_GET, onImageReq_10);
    server.on("/assets/png/11.png", HTTP_GET, onImageReq_11);
    server.on("/assets/png/12.png", HTTP_GET, onImageReq_12);
    server.on("/assets/png/13.png", HTTP_GET, onImageReq_13);
    server.on("/assets/png/14.png", HTTP_GET, onImageReq_14);
    server.on("/assets/png/15.png", HTTP_GET, onImageReq_15);
    server.on("/assets/png/16.png", HTTP_GET, onImageReq_16);

    //.............Handle requests for pages that do not exist
    server.onNotFound(onPageNotFound);

    //.............Start web server
    server.begin();
    delay(100);
    //mySerial_master.println(xPortGetCoreID());
    //.............Start WebSocket server and assign callback
    webSocket.begin();
    webSocket.onEvent(onWebSocketEvent);
    delay(100);
  }//scope

  //...................................................................... dallas 1 wire
  sensors.begin();
  delay(500);

  //..................................................................... actions
  // for master
  // for slave -> if redunancy is absent (redundancy_flag=0)
  if (softUart_select)
  {
    utils_config();
  }
  else
  {
    //if redundancy absent or factory slave setting
    if ( (redundancy_flag == 0) || (sequence_pattern_number == 0) )
    {
      utils_config();
    }//if
  }//else

  //...................................................................... for seq 0 / factory boot
  waitForSeqNum();


  //......................................................................
  setSSDP();
  //mySerial_master.println(xPortGetCoreID());

  //mySerial_master.println(xPortGetCoreID());
  //................
  webSocket_dashBoard.begin();
  webSocket_dashBoard.onEvent(webSocketEvent_dashBoard);
  //mySerial_master.println(xPortGetCoreID());
  //................




  //..................................................................... read slave start
  if ((softUart_select == 0) && (redundancy_flag == 1))
  {
    //.............................................. set OP/IP
    numberOfShiftRegisters = 2;
    set_shift_regs_data_width();
    //..........................
    lamp_test_off();

    //Serial.println(">>>>>> red_slave_wait >>>>>");
    //...................................
    temp_func();

    //...................................
    mcp.digitalWrite(buffer_enable_pin, HIGH);
    //..........................
    //    lamp_test_off();
    //...................................
    check_for_single_seq();
    //.............................................. set OP/IP
    //    numberOfShiftRegisters = 2;
    //    set_shift_regs_data_width();
    //.................................
    read_shift_regs_str();
    //.................................
    //Serial.println("starting slave with");
    print_arr(0);
    print_arr(1);
    print_arr(8);
    delay(5000);
    single_seq_pattern_func(sequence_pattern_number);
    reset();
  }//if

  //Serial.println("exit setup");
  delay(1000);

}//setup


//*****************************************************************loop
void loop()
{
  //Serial.println(">>>>>>>>> Loop entry  >>>>>>>>>");
  // Look for and handle WebSocket data
  webSocket.loop();
  webSocket_dashBoard.loop();
  //  SSDP.process();
  slave.run();
  if (eth_connected)
  {
    SSDP.process();
  }//if


  //........................................................ if single input seq execution
  if (single_seq_flag == 1)
    read_single_input_func();
  else
    read_subq_input_func();

  //  //Serial.println(">>>>>>>>> temp while  >>>>>>>>>");
  //  while (1)
  //  {
  //    delay(1000);
  //  }

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //......................... lamp_test
  if (!read_test())
  {
    lamp_test_on();
    while (!read_test())
    {
      delay(10);
    }//while
    lamp_test_off();

  }//if

  //........................ dashBoard send data
  //  if ( (ws_db_connect_flag == 1) && (wb_loop_count >= 100) )
  if ( (wb_loop_count >= 100) )
  {
    //    //Serial.println("hello");
    //
    //    if (softUart_select)
    //    {
    //      //Serial.println("hello");
    //      mySerial_master.println("master_data");
    //    }

    if ((ws_db_connect_flag == 1))
      webSocket_dashBoard.sendTXT(dashBoard_wb_num, dashBoard_str);

    wb_loop_count = 0;
  }//if
  wb_loop_count++;

  //.............................
  running_flasher_blink_slow();
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  //................................................................................. delay
  //          delay(3000);
  delay(POLL_DELAY_MSEC);

  yield();
}//loop

/*
   if ( (softUart_select == 1) || (slave_start == 1) )
  {
*/
