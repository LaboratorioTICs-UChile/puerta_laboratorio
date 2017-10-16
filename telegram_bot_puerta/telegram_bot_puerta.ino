//https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/master/src/UniversalTelegramBot.cpp

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Initialize Telegram BOT
#include "config.h"    // -> BOTtoken , ID_ADMIN , WIFI_SSID , WIFI_PASS
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;

#define ledPin LED_BUILTIN
#define relePin 14 // D5 en arduino

void handleNewMessages(int numNewMessages) {
  //Serial.println("handleNewMessages");
  //Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    Serial.print(i);
    Serial.print(". chat_id:");
    Serial.print(chat_id);
    Serial.print(", text:");
    Serial.print(text);
    Serial.print(", from_name:");
    Serial.println(from_name);

    if (text == "/start") {
      // String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      // welcome += "This is Flash Led Bot example.\n\n";
      // welcome += "/ledon : to switch the Led ON\n";
      // welcome += "/ledoff : to switch the Led OFF\n";
      // welcome += "/status : Returns current status of LED\n";
      // bot.sendMessage(chat_id, welcome, "Markdown");
      
      String keyboard = "[['Abrir']]";
      String welcome = "Buena loco!\nBienvenido al Lab de TIC's\nSi estás autorizado presiona para abrir la puerta.";
      bot.sendMessageWithReplyKeyboard(chat_id, welcome, "", keyboard, true, false, false);
      
      String admin_msg1 = "Nuevo usuario -> chat_id: " + chat_id + ", name: " + from_name;
      bot.sendMessage(ID_ADMIN, admin_msg1, "");
    }

    if (text == "Abrir") {
      digitalWrite(ledPin, LOW); digitalWrite(relePin, HIGH);
      delay(500);
      digitalWrite(ledPin, HIGH); digitalWrite(relePin, LOW);
      bot.sendMessage(chat_id, "Abriendoo", "");
      
      String admin_msg2 = "Ingreso: " + chat_id + " " + from_name;
      bot.sendMessage(ID_ADMIN, admin_msg2, "");
    }
  }
}


void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  pinMode(relePin, OUTPUT);
  delay(10);
  digitalWrite(ledPin, LOW);  // digitalWrite(relePin, LOW);
  delay(200);
  digitalWrite(ledPin, HIGH); // digitalWrite(relePin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);  // digitalWrite(relePin, LOW);
  delay(200);
  digitalWrite(ledPin, HIGH); // digitalWrite(relePin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);  // digitalWrite(relePin, LOW);
  delay(200);
  digitalWrite(ledPin, HIGH); // digitalWrite(relePin, HIGH); 
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      //Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
