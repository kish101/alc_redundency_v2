
//......................................................................... CSS
void onCSSRequest_src1(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //            "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/src.21df5e84.css", "text/css");
}

void onCSSRequest_materialize_min(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //          "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/materialize.min.dad11602.css", "text/css");
}

void onCSSRequest_material_icon(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //        "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/material-icons.79cbed4e", "text/css");
}


//......................................................................... JS
void onJSRequest_src1(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //      "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/src.c291981e.js", "text/js");
}

void onJSRequest_materialize_min(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //    "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/materialize.min.0fe193ae.js", "text/js");
}


//......................................................................... images
void onImageReq_1(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //  "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/1.png", "image/png");
}
void onImageReq_2(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //"] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/2.png", "image/png");
}
void onImageReq_3(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //               "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/3.png", "image/png");
}
void onImageReq_4(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //             "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/4.png", "image/png");
}
void onImageReq_5(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //           "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/5.png", "image/png");
}
void onImageReq_6(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //         "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/6.png", "image/png");
}
void onImageReq_7(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //"] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/7.png", "image/png");
}
void onImageReq_8(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //"] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/8.png", "image/png");
}
void onImageReq_9(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //"] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/9.png", "image/png");
}
void onImageReq_10(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //"] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/10.png", "image/png");
}
void onImageReq_11(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //                 "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/11.png", "image/png");
}
void onImageReq_12(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //              "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/12.png", "image/png");
}
void onImageReq_13(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //              "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/13.png", "image/png");
}
void onImageReq_14(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //             "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/14.png", "image/png");
}
void onImageReq_15(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //             "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/15.png", "image/png");
}
void onImageReq_16(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  //Serial.println("[" + remote_ip.toString() +
  //              "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/assets/png/16.png", "image/png");
}
