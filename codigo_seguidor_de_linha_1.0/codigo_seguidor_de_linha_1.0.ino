/*
 * OTAWebUpdater.ino Example from ArduinoOTA Library
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 */

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "esp32";
const char* ssid = "IFAL - Rio Largo";
const char* password = "ifalriolargo";

int motorA1 = 25;
int motorA2 = 26;

int motorB1 = 12;
int motorB2 = 14;
int sensor1 = 32;
int sensor2 = 35;

int valorSensor1;
int valorSensor2;

int luminosidade = 30000;


int freq = 30000;
int canalPWM = 0;
int resolution = 8;

int freq2 = 30000;
int canalPWM2 = 1;
int resolution2 = 8;


WebServer server(80);

/*
 * Login page
 */
const char* loginIndex =
  "<form name='loginForm'>"
  "<table width='20%' bgcolor='A09F9F' align='center'>"
  "<tr>"
  "<td colspan=2>"
  "<center><font size=4><b>ESP32 Login Page</b></font></center>"
  "<br>"
  "</td>"
  "<br>"
  "<br>"
  "</tr>"
  "<td>Username:</td>"
  "<td><input type='text' size=25 name='userid'><br></td>"
  "</tr>"
  "<br>"
  "<br>"
  "<tr>"
  "<td>Password:</td>"
  "<td><input type='Password' size=25 name='pwd'><br></td>"
  "<br>"
  "<br>"
  "</tr>"
  "<tr>"
  "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
  "</tr>"
  "</table>"
  "</form>"
  "<script>"
  "function check(form)"
  "{"
  "if(form.userid.value=='admin' && form.pwd.value=='admin')"
  "{"
  "window.open('/serverIndex')"
  "}"
  "else"
  "{"
  " alert('Error Password or Username')/*displays error message*/"
  "}"
  "}"
  "</script>";

/*
 * Server Index Page
 */

const char* serverIndex =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>"
  "<div id='prg'>progress: 0%</div>"
  "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>";

/*
 * setup function
 */
void setup(void) {

  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);

  ledcSetup(canalPWM, freq, resolution);
  ledcSetup(canalPWM2, freq2, resolution2);
  ledcAttachPin(motorA1, canalPWM);
  ledcAttachPin(motorB1, canalPWM2);

  ledcWrite(canalPWM, 0);
  ledcWrite(canalPWM2, 0);
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) {  //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on(
    "/update", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        /* flashing firmware to ESP*/
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {  //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
      }
    });
  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(1);





  //sensores

  valorSensor1 = !digitalRead(sensor1);
  valorSensor2 = digitalRead(sensor2);




  Serial.println(valorSensor1);
  Serial.println(valorSensor2);
  delay(1000);

  if (valorSensor1 == 1 && valorSensor2 == 1) {
    ledcAttachPin(motorA2, canalPWM);
    ledcWrite(canalPWM, 100);
    digitalWrite(motorA1, LOW);

    ledcAttachPin(motorB1, canalPWM2);
    ledcWrite(canalPWM2, 100);
    digitalWrite(motorB2, LOW);
  }

  if (valorSensor1 == 0 && valorSensor2 == 1 ) {
    ledcAttachPin(motorB1, canalPWM2);
    ledcWrite(canalPWM2, 100);
    digitalWrite(motorB2, LOW);

    ledcDetachPin(motorA1);
    ledcDetachPin(motorA2);
  }
  if(valorSensor1 == 1 && valorSensor2 == 0){
    ledcAttachPin(motorA2, canalPWM);
    ledcWrite(canalPWM, 100);
    digitalWrite(motorA1, LOW);

    ledcDetachPin(motorB1);
    ledcDetachPin(motorB2);
  }
  // ledcDetachPin(motorA1);
  //ledcDetachPin(motorA2);
  //ledcDetachPin(motorB1);
  //ledcDetachPin(motorB2);
}
