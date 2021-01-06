#ifdef MQQT
// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void MQQT_connect() {
  // Loop until we're reconnected
  while (!mqqt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect

    if (mqqt_client.connect(HOSTNAME "_client")) {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      mqqt_client.subscribe(HOSTNAME "/control/pagrindine_lempa");
      mqqt_client.subscribe(HOSTNAME "/control/ledai");
      mqqt_client.subscribe(HOSTNAME "/control/ventiliatorius");
      mqqt_client.subscribe(HOSTNAME "/control/veidrodis");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqqt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void MQQT_publish(){
  char tempS[8];
  char humiS[8];
  sprintf (tempS, "%2.1f", temp);
  sprintf (humiS, "%2.1f", humidity);

  // Publishes Temperature and Humidity values
  mqqt_client.publish(HOSTNAME "/temperatura", tempS);
  mqqt_client.publish(HOSTNAME "/dregme", humiS);

#ifdef VONIA
  mqqt_client.publish(HOSTNAME "/pagrindine_lempa", read_output(0) == LIGHTS_ON ? "on":"off");
  mqqt_client.publish(HOSTNAME "/ledai", read_output(1) == LIGHTS_ON ? "on":"off");
  mqqt_client.publish(HOSTNAME "/ventiliatorius", read_output(2) == LIGHTS_ON ? "on":"off");
  mqqt_client.publish(HOSTNAME "/veidrodis", read_output(3) == LIGHTS_ON ? "on":"off");
#endif

}

void MQQT_callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

#ifdef VONIA
  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(String(topic) == HOSTNAME"/control/pagrindine_lempa"){
        write_output(0, messageTemp == "on" ? LIGHTS_ON : LIGHTS_OFF);
  }
    if(String(topic) == HOSTNAME"/control/ledai"){
        write_output(1, messageTemp == "on" ? LIGHTS_ON : LIGHTS_OFF);
  }
    if(String(topic) == HOSTNAME"/control/ventiliatorius"){
        write_output(2, messageTemp == "on" ? LIGHTS_ON : LIGHTS_OFF);
  }
    if(String(topic) == HOSTNAME"/control/veidrodis"){
        write_output(3, messageTemp == "on" ? LIGHTS_ON : LIGHTS_OFF);
  }
  #endif
}

#endif //MQQT
