/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-async-web-server-espasyncwebserver-library/
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import 
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi
const char* ssid = "FRITZ!Box 7490";
const char* password = "";


const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Augen
int weis_1 = 26;
int weis_2 = 27;

// LCD
LiquidCrystal_I2C lcd(0x27,16,2);

// AsyncWebServer objekt - port 80
AsyncWebServer server(80);

// Webseite erstellen
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADIAAAAyCAYAAAAeP4ixAAAABmJLR0QA/wD/AP+gvaeTAAAD/ElEQVRoge2ZTWhcVRSAv/Omjj800BYiVE1VKJTMdG+pupDMaDZWnMlEEKqIpSAGV1KiXRR1k42CgWz8AVs3kmSqRaw6k9JFE9uNG+2MInVh01Q3WhfRRNvc42LeK28ek5l38+7YFubbvPfuPff8zHnn/syDHj26gtgIZ0vTaXTLW6qyH9jeJY8uoxwT78qR2szov3GHbbKxYcy2NwU9ZO+dBco9wLjqFgVejzvMKhAR3Y+C8WTvjzO5s7Y+xmH3yKm9Rs0CyPNYBOJZWWn8WnQrCIDzs0PfhG3FxSojUbJPzw2rZ94HuS+JHmBRjBysfZr7aqMK7DISQT19z0EQAAMNXRsnUUaAAYB6OW81+0XJFKsa6NooiTJyM+E0kMFiZSFTqJ5xJWeD00BExSCoKzkbktZIE/Xj+UddytngNJAomWJlMTSrLdbL+R0OdL4N8rLAZK2cv77L6G6xS+og6CX8dSKxvtJ0CmQMuB0YC3d1NSP12aEvSTithsmy9V6FNIDCn+G+W2v6NTIc3Cp8H+5KmpFFYMBf0JKy2K5z1775PlgZv96gciLcnygj/nvf1oGYtK2hbOn05lR6ZVrhQb/p17uurnwUlkmUEX+Tl3gmWo9sqboDI8Nq1l4DHvCbFeGlbz9/8u+wbNtAdu2b70ulV59V1YIHWacrWAzU8AvN66YK8mptNnciKrtuINnC3IjKyiTKdoGWy7BtbUQ3l4PFyoKomDgLpMCSQcbq5dxnrfpbBjJYmDukohNYnult6bxV0UuInFe0LHLbJz/MPLa8rq5oQyMTOh3q+1lFJ1JX9WuzybsIybftnQgybWOnKSPZ0unNaq5N4Qchwhd3/LM6GhRWplh16K5bmqZfXbv2AnC3/3ghdad5Jjo7tCJTrGrcesmMVEqZkUrJ3tX2NK8jwlPBrapMfPfxE3+5NLazdLJfVKZEZWpn6WR/0O7ifBIt9mxws5aSjn8ERLPQ6d1Om/SUov3BPTAKbs4n0UD6/OvqT/z+WxLFraiXc6Mt2x2cT5oDUXkX0VdQmWRmdK2zY41ffiOzjGuaAqkfzx0GDnfb6GDh1P0i5kNgD3BWPF6szeQvJtHp5Dximwk/iCH/MYfqB8DjSXy4UeeRh8IPqrInqcJEGckUqmcQHml4w3xQtOu1hzgH5IIHET2XxA9wmZH1ps8W7areARGtAssKFUQOOPMjDjYr+P9txy4jwmVofMOwGmdBtlB5uGGKJZtxdjWiHAPGjZqFbm0ggzSoylGbcXZfrLwrR9RsBeE52w8xsW3AkqocldQfb1iOc0P4ne60rtjIxuXW+l+rDb1AbjZ6gbRgNXJ1JRsLd4GovAMs+1d3sj163Fj+A9nHf+Fwyh5/AAAAAElFTkSuQmCC
">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.5rem; background-color: #5CE1E6; padding: 5px 0 5px 0;}
    h4 {font-size: 1.5rem; margin-top: 10px; margin-bottom: 10px}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #5CE1E6}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>ESP32 Web Server</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Schalter erstellen
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Sag Hallo!</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"13\" " + outputState(13) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Zeig Liebe</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" " + outputState(5) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Zeig Schlechte Laune</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(12) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

void setup(){
  // Serial starten
  Serial.begin(115200);

  // LED-Modus definieren
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(14, OUTPUT);
  digitalWrite(14, LOW);
  pinMode(weis_1, OUTPUT);
  digitalWrite(weis_1, HIGH);
  pinMode(weis_2, OUTPUT);
  digitalWrite(weis_2, HIGH);

  // LCD Monitor starten
  lcd.init();   
  lcd.backlight();
  
  // Wi-Fi Verbindung
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // ESP Local IP Addresse ausdrucken
  Serial.println(WiFi.localIP());

  // Routing an die Webseite
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Senden GET-Request an <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    
  // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("PIN: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop() {
    int hallo = digitalRead(13);
    if(hallo) {
      lcd.setCursor(0,0);
      lcd.println("HALLO!             ");
      lcd.setCursor(0,1);
      lcd.println("                      ");
      delay(3000);
    } else {
      lcd.setCursor(0,0);
      lcd.println("                          ");
      lcd.setCursor(0,1);
      lcd.println("                               ");
      delay(1000);
    }

    int liebe = digitalRead(5);
    if(liebe) {
      lcd.setCursor(0,0);
      lcd.println("LIEBE                   ");
      lcd.setCursor(0,1);
      lcd.println("                            ");
      delay(3000);
    } else {
      lcd.setCursor(0,0);
      lcd.println("                          ");
      lcd.setCursor(0,1);
      lcd.println("                               ");
      delay(1000);
    }

    int laune = digitalRead(12);
    if(laune) {
      lcd.setCursor(0,0);
      lcd.println("WIR SEHEN UNS   ");
      lcd.setCursor(0,1);
      lcd.println("IN WALHALLA!    ");
      delay(3000);
    } else {
      lcd.setCursor(0,0);
      lcd.println("                          ");
      lcd.setCursor(0,1);
      lcd.println("                               ");
      delay(1000);
    }
}
