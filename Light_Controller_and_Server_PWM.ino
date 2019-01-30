//Działa dla oświetlenia LED 3VDC, w planach sterowanie oświetleniem LED 230VAC i/lub 12VDC

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>

/// PARAMETERS TO MODIFY ///
const char* ssid     = "SSID";
const char* password = "PASSWORD";
IPAddress ip(192,168,43,15);  //ustalenie adresu IP sterownika oświetlenia
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
WiFiServer server(80);
/////////////////////////////////////////////////////////////

String header;
int connectingTime = 0; //odliczanie czasu próby łączenia z WiFi
int i;
String operatingMode;

unsigned long int tab[1024] =
{
0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 
5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 
6, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 
8, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 
10, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 
12, 12, 13, 13, 13, 13, 13, 13, 14, 14, 14, 
14, 14, 15, 15, 15, 15, 15, 16, 16, 16, 16, 
16, 17, 17, 17, 17, 18, 18, 18, 18, 18, 19, 
19, 19, 19, 20, 20, 20, 20, 20, 21, 21, 21, 
21, 22, 22, 22, 22, 23, 23, 23, 23, 24, 24, 
24, 25, 25, 25, 25, 26, 26, 26, 26, 27, 27, 
27, 28, 28, 28, 28, 29, 29, 29, 30, 30, 30, 
31, 31, 31, 31, 32, 32, 32, 33, 33, 33, 34, 
34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 
38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 
42, 42, 42, 43, 43, 43, 44, 44, 45, 45, 45, 
46, 46, 46, 47, 47, 48, 48, 48, 49, 49, 50, 
50, 50, 51, 51, 52, 52, 53, 53, 53, 54, 54, 
55, 55, 56, 56, 56, 57, 57, 58, 58, 59, 59, 
60, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 
65, 65, 66, 66, 67, 67, 68, 68, 69, 69, 70, 
70, 71, 71, 72, 72, 73, 73, 74, 74, 75, 75, 
76, 76, 77, 77, 78, 78, 79, 79, 80, 80, 81, 
82, 82, 83, 83, 84, 84, 85, 85, 86, 87, 87, 
88, 88, 89, 89, 90, 91, 91, 92, 92, 93, 94, 
94, 95, 95, 96, 97, 97, 98, 98, 99, 100, 100, 
101, 102, 102, 103, 103, 104, 105, 105, 106, 
107, 107, 108, 109, 109, 110, 110, 111, 112, 
112, 113, 114, 114, 115, 116, 117, 117, 118, 
119, 119, 120, 121, 121, 122, 123, 123, 124, 
125, 126, 126, 127, 128, 128, 129, 130, 131, 
131, 132, 133, 133, 134, 135, 136, 136, 137, 
138, 139, 139, 140, 141, 142, 143, 143, 144, 
145, 146, 146, 147, 148, 149, 149, 150, 151, 
152, 153, 153, 154, 155, 156, 157, 158, 158, 
159, 160, 161, 162, 162, 163, 164, 165, 166, 
167, 167, 168, 169, 170, 171, 172, 173, 173, 
174, 175, 176, 177, 178, 179, 180, 180, 181, 
182, 183, 184, 185, 186, 187, 188, 188, 189, 
190, 191, 192, 193, 194, 195, 196, 197, 198, 
199, 200, 200, 201, 202, 203, 204, 205, 206, 
207, 208, 209, 210, 211, 212, 213, 214, 215, 
216, 217, 218, 219, 220, 221, 222, 223, 224, 
225, 226, 227, 228, 229, 230, 231, 232, 233, 
234, 235, 236, 237, 238, 239, 240, 241, 242, 
243, 244, 245, 247, 248, 249, 250, 251, 252, 
253, 254, 255, 256, 257, 258, 260, 261, 262, 
263, 264, 265, 266, 267, 268, 270, 271, 272, 
273, 274, 275, 276, 277, 279, 280, 281, 282, 
283, 284, 286, 287, 288, 289, 290, 291, 293, 
294, 295, 296, 297, 298, 300, 301, 302, 303, 
304, 306, 307, 308, 309, 311, 312, 313, 314, 
315, 317, 318, 319, 320, 322, 323, 324, 325, 
327, 328, 329, 330, 332, 333, 334, 336, 337, 
338, 339, 341, 342, 343, 345, 346, 347, 349, 
350, 351, 352, 354, 355, 356, 358, 359, 360, 
362, 363, 364, 366, 367, 369, 370, 371, 373, 
374, 375, 377, 378, 379, 381, 382, 384, 385, 
386, 388, 389, 391, 392, 393, 395, 396, 398, 
399, 400, 402, 403, 405, 406, 408, 409, 411, 
412, 413, 415, 416, 418, 419, 421, 422, 424, 
425, 427, 428, 430, 431, 433, 434, 436, 437, 
439, 440, 442, 443, 445, 446, 448, 449, 451, 
452, 454, 455, 457, 458, 460, 461, 463, 465, 
466, 468, 469, 471, 472, 474, 476, 477, 479, 
480, 482, 483, 485, 487, 488, 490, 491, 493, 
495, 496, 498, 500, 501, 503, 504, 506, 508, 
509, 511, 513, 514, 516, 518, 519, 521, 523, 
524, 526, 528, 529, 531, 533, 534, 536, 538, 
540, 541, 543, 545, 546, 548, 550, 552, 553, 
555, 557, 558, 560, 562, 564, 565, 567, 569, 
571, 572, 574, 576, 578, 580, 581, 583, 585, 
587, 588, 590, 592, 594, 596, 597, 599, 601, 
603, 605, 607, 608, 610, 612, 614, 616, 618, 
619, 621, 623, 625, 627, 629, 631, 632, 634, 
636, 638, 640, 642, 644, 646, 648, 649, 651, 
653, 655, 657, 659, 661, 663, 665, 667, 669, 
671, 673, 674, 676, 678, 680, 682, 684, 686, 
688, 690, 692, 694, 696, 698, 700, 702, 704, 
706, 708, 710, 712, 714, 716, 718, 720, 722, 
724, 726, 728, 730, 732, 734, 736, 739, 741, 
743, 745, 747, 749, 751, 753, 755, 757, 759, 
761, 763, 766, 768, 770, 772, 774, 776, 778, 
780, 782, 785, 787, 789, 791, 793, 795, 797, 
800, 802, 804, 806, 808, 810, 813, 815, 817, 
819, 821, 824, 826, 828, 830, 832, 835, 837, 
839, 841, 843, 846, 848, 850, 852, 855, 857, 
859, 861, 864, 866, 868, 870, 873, 875, 877, 
880, 882, 884, 886, 889, 891, 893, 896, 898, 
900, 903, 905, 907, 910, 912, 914, 917, 919, 
921, 924, 926, 928, 931, 933, 935, 938, 940, 
942, 945, 947, 950, 952, 954, 957, 959, 962, 
964, 966, 969, 971, 974, 976, 979, 981, 983, 
986, 988, 991, 993, 996, 998, 1001, 1003, 1006, 
1008, 1011, 1013, 1016, 1018, 1021, 1023};

void setup() {
  Serial.begin(115200);
  pinMode(D3, OUTPUT); //podłączone oświetlenie
  pinMode(D4, OUTPUT); //sygnalizacja połączenia Wi-Fi
  WiFi.config(ip,gateway,subnet);
  
 /*  for(int i = 0, j = 1; i < 1024; i++, j++) { //funkcja wyliczająca elementy tablicy, 1024 - ilość elementów tablicy (0-1023 10 bit)
        int v = (int)(pow((double)i / 1023.0, 2.5) * 1023.0 + 0.5); // 2.5 - współczynnik gamma
        if(v == 0 && i > 0) // Have 0 only for 0, at least 1 for anything above
            v = 1;
        Serial.print(v); Serial.print(", ");
    }*/
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    connectingTime++;
    if(connectingTime>60){break;} //warunek przerwania próby łączenia z WiFi (30s)
  }
  if(connectingTime<60){ //jeśli się połączyło
  digitalWrite(D4, HIGH); //dioda sygnalizująca poł. Wi-Fi
  Serial.println(" connected");
  }
  if(connectingTime>60){Serial.printf("No connection with router");} 
  server.begin();
}

void loop() {

  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /on") >= 0) { //włączanie światła
              operatingMode = "wl";
             
            } else if (header.indexOf("GET /off") >= 0) { //wyłączanie światła
              operatingMode = "wy";
              
            } else if (header.indexOf("GET /10") >= 0) { //10%
              operatingMode = "10";
              
            } else if (header.indexOf("GET /20") >= 0) { //20%
              operatingMode = "20";
              
            } else if (header.indexOf("GET /30") >= 0) { //30%
              operatingMode = "30";
              
            } else if (header.indexOf("GET /40") >= 0) { //40%
              operatingMode = "40";
              
            } else if (header.indexOf("GET /50") >= 0) { //50%
              operatingMode = "50";
              
            } else if (header.indexOf("GET /60") >= 0) { //60%
              operatingMode = "60";
              
            } else if (header.indexOf("GET /70") >= 0) { //70%
              operatingMode = "70";
              
            } else if (header.indexOf("GET /80") >= 0) { //80%
              operatingMode = "80";
              
            } else if (header.indexOf("GET /90") >= 0) { //90%
              operatingMode = "90";
           
            } else if (header.indexOf("GET /pulse") >= 0) { //pulsujące
              operatingMode = "p";
            }

            // Początek HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // Style CSS
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".dropbtn { background-color: gray; color: white; padding: 16px;font-size: 16px; border: none;}");
            client.println(".dropdown { position: relative; display: inline-block;}");
            client.println(".dropdown-content {display: none; position: absolute; background-color: #f1f1f1; min-width: 160px; box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2); z-index: 1;}");
            client.println(".dropdown-content a { color: black; padding: 16px 40px; text-decoration: none; display: block;}");
            client.println(".dropdown-content a:hover {background-color: #ddd;}");
            client.println(".dropdown:hover .dropdown-content {display: block;}");
            client.println(".dropdown:hover .dropbtn {background-color: gray;}");
            client.println(".button { background-color: gray; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println("select { width: 20%;padding: 16px 40px; border: none; border-radius: 4px; background-color: gray;");
            client.println(".button2 {background-color: #1ca3df;}</style></head>");

            // Tytuł strony
            client.println("<body><h1>Sterowanie o&#347wietleniem</h1>");

            client.println("<p><a href=\"/on\"><button class=\"button\">W&#322&#261cz o&#347wietlenie</button></a></p><br>");
            client.println("<p><a href=\"/off\"><button class=\"button\">Wy&#322&#261cz o&#347wietlenie</button></a></p><br>");
            client.println("<p><a href=\"/pulse\"><button class=\"button\">W&#322&#261cz o&#347wietlenie pulsuj&#261ce</button></a></p><br>");

            //Rozwijany przycisk do zmiany jasności 0% - 100% co 10%
            client.println("<div class=\"dropdown\">");
            client.println("<button class=\"dropbtn\">Zmie&#324 jasno&#347&#263</button>");
            client.println("<div class=\"dropdown-content\">");
            client.println("<a href=\"/off\">0%</a>");
            client.println("<a href=\"/10\">10%</a>");
            client.println("<a href=\"/20\">20%</a>");
            client.println("<a href=\"/30\">30%</a>");
            client.println("<a href=\"/40\">40%</a>");
            client.println("<a href=\"/50\">50%</a>");
            client.println("<a href=\"/60\">60%</a>");
            client.println("<a href=\"/70\">70%</a>");
            client.println("<a href=\"/80\">80%</a>");
            client.println("<a href=\"/90\">90%</a>");
            client.println("<a href=\"/on\">100%</a>");
            client.println("</div>");
            client.println("</div>");

            client.println("</body>");
            client.println();
                      
            break;
            } else { 
              currentLine = "";
              }
            } else if (c != '\r') {  
              currentLine += c;     
              }
            }
         }
         header = "";
         client.stop();
       }
  
  if(operatingMode=="p"){
    pulse();}
  if(operatingMode=="wl"){
    digitalWrite(D3, HIGH);}
  if(operatingMode=="wy"){
    digitalWrite(D3, LOW);}
  if(operatingMode=="10"){
    analogWrite(D3, tab[102]);}
  if(operatingMode=="20"){
    analogWrite(D3, tab[204]);}
  if(operatingMode=="30"){
    analogWrite(D3, tab[306]);}
  if(operatingMode=="40"){
    analogWrite(D3, tab[409]);}
  if(operatingMode=="50"){
    analogWrite(D3, tab[512]);}
  if(operatingMode=="60"){
    analogWrite(D3, tab[614]);}
  if(operatingMode=="70"){
    analogWrite(D3, tab[716]);}
  if(operatingMode=="80"){
    analogWrite(D3, tab[818]);}
  if(operatingMode=="90"){
    analogWrite(D3, tab[920]);}
}

void pulse(){ //światło pulsujące
  for (i=0; i<1024; i++){ //rozjaśnianie
    analogWrite(D3, tab[i]);
    Serial.println(tab[i]);
    delay(5);
  }
  for (i=1023; i>0; i--){ //ściemnianie
    analogWrite(D3, tab[i]);
    Serial.println(tab[i]);
    delay(5);
  }
}

void Morse(char param){ //w trakcie - alfabet Morse'a
  if(param=='A'){
    digitalWrite(D3, HIGH);
    delay(300); //czas trwania krótkiego sygnału
    digitalWrite(D3, LOW);
    delay(100); //przerwa między sygnałami
    digitalWrite(D3, HIGH);
    delay(1000); //czas trwania długiego
    digitalWrite(D3, LOW);
    }

  if(param=='B'){
    digitalWrite(D3, HIGH);
    delay(1000); //czas trwania długiego
    digitalWrite(D3, LOW);
    delay(100); //przerwa między sygnałami
    digitalWrite(D3, HIGH);
    delay(300); //czas trwania krótkiego sygnału
    digitalWrite(D3, LOW);
    delay(100); //przerwa między sygnałami
    digitalWrite(D3, HIGH);
    delay(300); //czas trwania krótkiego sygnału
    digitalWrite(D3, LOW);
    delay(100); //przerwa między sygnałami
    digitalWrite(D3, HIGH);
    delay(300); //czas trwania krótkiego sygnału
    digitalWrite(D3, LOW);
    }
  }



