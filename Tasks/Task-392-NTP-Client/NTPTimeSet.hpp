#include "uop_msb.h"
#include <functional>
#include <string>
using namespace uop_msb;

#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "NTPClient.h"
#include <iostream>
using namespace std;

class NTPTimeSet {
    private:
        NetworkInterface* netIF;

    function<void(const char*)> ui_callback; 

    public:
        NTPTimeSet (function<void(const char*)> cb = [](const char* str){} ) : ui_callback(cb) {};

        int setTime() {
            netIF = NetworkInterface::get_default_instance();
            if (netIF == nullptr) {
                ui_callback("No network interface found\n");
                return -1;
            }

            // Connect
            int ret = netIF->connect();
            if (ret != 0) {
                char strbuf[32];
                sprintf(strbuf, "Connection error: %d\n", ret);
                ui_callback(strbuf);
                return -1;
            }
            char strbuf[64];
            sprintf(strbuf, "Connection success, MAC: %s\n", netIF->get_mac_address());
            ui_callback(strbuf);

            ui_callback("Getting time from the NTP server\n");

            // Instantiate a NPTClient using the connected interface
            NTPClient ntp(netIF);
            // Configure for a public time server
            ntp.set_server("time.google.com", 123);

            // Get the time
            time_t timestamp = ntp.get_timestamp();
            if (timestamp < 0) {
                char strbuf1[64];
                sprintf(strbuf1, "Failed to get the current time, error: %i\n", timestamp);
                ui_callback(strbuf1);
                netIF->disconnect(); //This is easy to forget!
                return -1;
            }
            char strbuf1[32];
            sprintf(strbuf1, "Time: %s", ctime(&timestamp));
            ui_callback(strbuf1);
            // Set system time
            set_time(timestamp);
            
            //Close network connection (we are finished)
            ui_callback("Closing network connection\n");
            netIF->disconnect();
            return 0;
        }
};