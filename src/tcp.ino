/*******************************
  Listen for tcp commands
 *******************************/

void tcp_listen(){

    if (!wifi_client.connected()) {
        // try to connect to a new client
        wifi_client = server.available();
    } else {
        // read data from the connected client
        if (wifi_client.available() > 0) {
            char req = wifi_client.read();
            if(req == 'L'){
                wifi_client.write(toggle_output(wifi_client.read()-48)+48);
                return;
            }
            if(req == 'H'){
                char buf [30];
                sprintf (buf, "%2.1f", humidity);
                wifi_client.write((const char*)&buf[0],(size_t)(2));
                return;

            }
            if(req == 'T'){
                char buf [30];
                sprintf (buf, "%2.1f", temp);
                wifi_client.write((const char*)&buf[0],(size_t)(2));
                return;
            }
            if(req == 'A'){
                char buf [30] = {0};
                sprintf (buf, "A%2.1f_%2.1f_%d_%d_%d_%d",temp,humidity,
                digitalRead(OUT_PINS[0]) ,digitalRead(OUT_PINS[1]),
                digitalRead(OUT_PINS[2]) ,digitalRead(OUT_PINS[3]));
                wifi_client.write((const char*)&buf[0],strlen(buf));
                return;
            }
            if(req == 'J'){
                wifi_client.write(write_all(wifi_client.read()-48));
                return;
            }

            if(req == 'P'){
                pirTimeout = wifi_client.read()-48 *10000;
                // wifi_client.write();
                return;
            }


            while(wifi_client.read()!=-1);

        }
    }
}

void tcp_write_string(const char* string){

  if (!wifi_client.connected()) {
        // try to connect to a new client
        wifi_client = server.available();
    } else {
    wifi_client.write(string, strlen(string));
    }

}
