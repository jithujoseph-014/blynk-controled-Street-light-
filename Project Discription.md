The streetlight can be controlled via blynk
cloud web-dashboard or blynk Android application.
It has a LDR and a DHT sensor,The LDR detects the 
Current state of the LED(ON/OFF) and uploads it to a
Thingspeak channel.It also has a DHT sensor which reads
The temperature and humidity values and these values
are uploaded to both a thingspeak channel and a an OM2M
Server. These values can be used to assess the local
Weather conditions. In most areas if the street lights
Are damaged or faulty it is not noticed by the authorities
Unless someone files a complaint.
Using the state reading given by The LDR we can know 
if an LED is turned on/off so we can use this data
to figure out if the led is working.
If the LDR is not showing that the led is on even after
Turning on through the blynk application or if the graph
Inthe thingspeak channel shows uneven values instead
Ofa smooth straight line we can understand That the 
Led is faulty and needs replacing . Thus this 
Project provides a smart solution to the problem
