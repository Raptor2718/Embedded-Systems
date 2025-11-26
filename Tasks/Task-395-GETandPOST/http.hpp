#include "cmsis_os2.h"
#include "uop_msb.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include "EthernetInterface.h"
#include "TCPSocket.h"
#include "index_html.hpp"
#include <ostream>
#include <string>
template<typename T>

class Http {
    private:
        Thread t;
        EthernetInterface net;
        TCPSocket socket;
        T vals;
        Mail <T, 1> &txmailbox;
        Mail <T, 1> &rxmailbox;
        Thread *txthread;

        void connect_ethernet() {
            net.connect();
            SocketAddress a;
            net.get_ip_address(&a);
            printf("[http] IP address: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");

            
            socket.open(&net);
            socket.bind(80);
            if (socket.listen(5) != 0) {
                printf("[http] Listen error\n");
                socket.close();
                while(1);
            }
            printf("[http] Listening OK\n");

            while (true) {
                
                printf("[http] Waiting...      \n");
                TCPSocket* clt_sock = socket.accept(); // Blocking
                
                printf("[http] Connected...    \n");
                txthread->flags_set(1);

                //get the data to send from the mailbox
                T *payload;
                payload = txmailbox.try_get_for(60s); //non-blocking 
                if (payload) {
                    printf("pot recieved\n");
                    for (int i = 0; i < vals.tx_len; i++) {
                        vals.tx[i] = payload->tx[i];
                    }
                    txmailbox.free(payload);
                }
                std::cout << vals.tx[0].key << std::endl;
                std::cout << vals.tx[0].val << std::endl;
                string html(reinterpret_cast<char*>(www_index_html), www_index_html_len);

                for (int i = 0; i < vals.tx_len; i++) {
                    string placeholder = "{{" + vals.tx[i].key + "}}";
                    //std::cout << placeholder << std::endl;
                    size_t index = html.find(placeholder);
                    if (index != string::npos) {
                        html.replace(index, placeholder.length(), vals.tx[i].val);
                    }
                }
                
                char header[128];
                sprintf(header,
                        "HTTP/1.0 200 OK\r\n"
                        "Content-Type: text/html; charset=utf-8\r\n"
                        "Content-Length: %d\r\n"
                        "\r\n",
                        (int)html.size()
                );

                // Send header + HTML
                nsapi_size_or_error_t r1 = clt_sock->send(header, strlen(header));
                nsapi_size_or_error_t r2 = clt_sock->send(html.c_str(), html.size());
                if (r1 < 0 || r2 < 0) {
                    printf("[http] send error %d %d\n", r1, r2);
                    continue;
                } else printf("[http] Sent %d bytes\n", r1+r2);

                // Handle LED input from browser
                extractRxData(clt_sock);
                
                
                //post the mail
                T *rxmail = rxmailbox.try_alloc();
                if (rxmail == NULL) {
                    clt_sock->close();
                    printf("[http] No rx mail slot, skipping\n");
                    continue;
                }
                
                for (int i = 0; i <vals.rx_len; i++) {
                    rxmail->rx[i] = vals.rx[i];
                }
                osStatus stat = rxmailbox.put(rxmail);

                if (stat != osOK) {
                    rxmailbox.free(rxmail);
                    return;
                }
               
                clt_sock->close();
                printf("[http] Closed...       \n");

            }
        }
        

        void extractRxData(TCPSocket* rxSock)
        {
            rxSock->set_blocking(false);
            char rbuffer[1024];
            int rcount = rxSock->recv(rbuffer, sizeof rbuffer);

            if (rcount > 0) {
                rbuffer[rcount] = '\0';
                
                char* lineend = strchr(rbuffer, '\n');
                if (lineend == nullptr) {
                    printf("[http] malformed request\n");
                    return;
                }

                // Extract first line
                std::string firstLine(rbuffer, lineend - rbuffer);
                printf("[http] first line = %s\n", firstLine.c_str());

                // Extract remainder (for headers/body)
                std::string remainder(lineend + 1);
                //printf("[http] remainder = %s\n", remainder.c_str());


                for (int i = 0; i < vals.rx_len; i++) {
                    //printf("%d: %s, %s\n", i, vals.rx[i].key.c_str(), vals.rx[i].val.c_str());
                    char kstr[32];
                    sprintf(kstr, "%s=", vals.rx[i].key.c_str());
                    std::string v = extractBetween(firstLine, kstr, "&");
                    if (v == "") 
                        v = extractBetween(firstLine, kstr, " HTTP");
                    vals.rx[i].val = v;
                    //printf("%d: %s, %s\n", i, vals.rx[i].key.c_str(), vals.rx[i].val.c_str());
                }
            }
            rxSock->set_blocking(true);
        }

        std::string extractBetween(const std::string& s,
                           const std::string& start,
                           const std::string& end)
        {
            size_t startPos = s.find(start);
            if (startPos == std::string::npos) return "";

            startPos += start.length();

            size_t endPos = s.find(end, startPos);
            if (endPos == std::string::npos) return "";

            return s.substr(startPos, endPos - startPos);
        }

    public:
        Http(const T& v, Mail<T, 1>&tmb, Mail<T, 1>&rmb, Thread& txthreadref) : vals(v), txmailbox(tmb),  rxmailbox(rmb){
            this->txthread = &txthreadref;
            t.start(callback(this, &Http::connect_ethernet));
        };

};