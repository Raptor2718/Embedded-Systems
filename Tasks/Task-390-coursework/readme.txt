

Flow chart of the system




+--------------------+                                      +---------------+  
|sensor st           |                                      |data_log()     |  
|                    |              +----+                  |               |  
|ldr.read_u16()      +---mailbox--->|buft+---mailbuf------->|sd.write_file()|  
|dev.getTemperature()|              +-+--+   (Mutexed       |               |  
|dev.getPreassure()  |                |    circular buffer) |               |  
+--------------------+                |                     +---------------+  
                                      |                                        
                                   dispbox ---------+ 
                                                    v            
       +-----------------+                  +---------------------+           
       |disp txThread    |                  |disp dt              |
       |                 |<-----txmail------+                     |   
+----->|flags_wait_any(1)|   (Mutexed       |get_data() (blocking)|        
|      |                 |      variable)   |alarm()              |       
|      +-------+---------+                  |serial_print()       |        
|              |                            |led_bar()            |<-----+      
|              |                            +---------------------+      |      
|           txmailbox                                                    |
|              |                                                         |      
|              v                                     setField(Mutexed variable)
|      +--------------------------+                                      |
|      |http                      |                +----------------------+     
|      |                          |                |disp rxThread         |     
|      |socket.accept() (blocking)|                |                      |     
+------+tx_thread->flags_set(1)   +----rxmailbox-->|sets setField         |     
       |                          |                |  using from rxmailbox|     
       |POST formatted HTML       |                |                      |     
       |         from txmailbox   |                +----------------------+     
       |GET parameters            |                                             
       |        for rxmailbox     |                                             
       +--------------------------+                                                         

Note: index_html.hpp is generatied using the following Linux command run from within this directory. It generates a cppp array using the file index.html

xxd -i www/index.html > index_html.hpp