////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define TRIGGER_PIN 0

//Coloque os valores padrões aqui, porém na interface eles poderão ser substituídos.
String saida_1;  
String saida_2;  
String saida_3;  
String aux;

int logo_image;

int senha_errada = 0;

//Criando o servidor web na porta 80
ESP8266WebServer server(80);


//default custom static IP
char static_ip[16] = "192.168.1.60";
char static_gw[16] = "192.168.1.1";
char static_sn[16] = "255.255.255.0";

String ip_aux;
String gw_aux;
String sn_aux;
int ip_status;
int estado_int1;
int estado_int2;
int estado_int3;


const char* www_username = "handzap";
const char* www_password = "jabuticaba";


String new_username;
String new_password;

//Parâmetros do Relógio NTP
WiFiUDP ntpUDP;
 
int16_t utc = -3; //UTC -3:00 Brazil
uint32_t currentMillis = 0;
uint32_t previousMillis = 0;
bool precisaSalvar  = false;              //Flag para salvar os dados

 
NTPClient timeClient(ntpUDP, "a.st1.ntp.br", utc*3600, 60000);



//Pino do NodeMCU que estara conectado ao rele
const int rele1 = 12; //Equivalente ao D2 no NodeMCU
const int rele2 = 13;
const int rele3 = 14;
const int inter1 = 4;
const int inter2 = 5;
const int inter3 = 16;

//Variáveis Globais

String hora_atual = "";
String dia_atual = "";

//Variáveis do Timer 1
int dispo1;
int hora1_liga;
int minuto1_liga;
int hora1_desl;
int minuto1_desl;
int che1;
int dia1;
//Variáveis do Timer 2
int dispo2;
int hora2_liga;
int minuto2_liga;
int hora2_desl;
int minuto2_desl;
int che2;
int dia2;
//Variáveis do Timer 3
int dispo3;
int hora3_liga;
int minuto3_liga;
int hora3_desl;
int minuto3_desl;
int che3;
int dia3;
//Variáveis do timer 4
int dispo4;
int hora4_liga;
int minuto4_liga;
int hora4_desl;
int minuto4_desl;
int che4;
int dia4;
//Variáveis do timer 5
int dispo5;
int hora5_liga;
int minuto5_liga;
int hora5_desl;
int minuto5_desl;
int che5;
int dia5;
//Variáveis do timer 6
int dispo6;
int hora6_liga;
int minuto6_liga;
int hora6_desl;
int minuto6_desl;
int che6;
int dia6;
//Variáveis do timer 7
int dispo7;
int hora7_liga;
int minuto7_liga;
int hora7_desl;
int minuto7_desl;
int che7;
int dia7;
//Variáveis do timer 8
int dispo8;
int hora8_liga;
int minuto8_liga;
int hora8_desl;
int minuto8_desl;
int che8;
int dia8;

String cor1L="BLACK" ;
String cor1D="RED";
String cor2L="BLACK" ;
String cor2D="RED";
String cor3L="BLACK" ;
String cor3D="RED";


char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

void handleRoot() 
{
  senha_errada = 0;
  String buf = "";
  buf += "<html>";
  buf += "<head> ";
  buf += "<meta charset='UTF-8'> ";
  buf += "<style type=\"text/css\">";  
  buf += "h1 {font-size: 50px; font-family: calibri}";  
  buf += "h2 {font-size: 30px; font-family: calibri}"; 
  buf += "h3 {font-size: 25px; font-family: calibri}"; 
  buf += "h4 {font-size: 20px; font-family: calibri}"; 
  buf += "h5 {font-size: 15px; font-family: calibri}"; 
  buf += "h6 {font-size: 10px; font-family: calibri}";  
  buf += "p  {font-size: 20px; font-family: calibri}"; 

  buf += "input[type=submit] {";
  buf += "font-family: Calibri, Arial;";
  buf += "font-size: 60%;";
  buf += "color: white;";
  buf += "text-decoration: none;";
  buf += "margin: 4px 2px;";
  buf += "cursor: pointer;";
  buf += "border-radius: .5em;";
  buf += "width: 40%;";
  buf += "height: 40px;";
  buf += "box-shadow: 1px 1px #999;}";
  buf += "input[type=submit]: active {";
  buf += "box-shadow: 0 5px #666;";
  buf += "transform: translateY(4px);}";

  buf += "#botao_timer {";
  buf += "font-family: Calibri, Arial;";
  buf += "font-size: 60%;";
  buf += "color: green;";
  buf += "text-decoration: none;";
  buf += "margin: 4px 2px;";
  buf += "cursor: pointer;";
  buf += "border-radius: .5em;";
  buf += "width: 10%;";
  buf += "height: 40px;";
  buf += "box-shadow: 1px 1px #999;";
  buf += "display: inline-block;}";
  buf += "#botao_timer: active {";
  buf += "box-shadow: 0 5px #666;";
  buf += "transform: translateY(4px);}";

  buf += "</style>";
  buf += "<link rel=\"shortcut icon\" href=\"/logo/handzap.ico\" type=\"image/x-icon\" />";
  buf += "<link rel=\"icon\" href=\"/logo/handzap.ico\" type=\"image/x-icon\" />";
  buf += "<title>Handzap Automação</title>";  
  buf += "</head>"; 
  buf += "<body bgcolor=#262640 text=#FFFFFF>";  
  buf += "<div align=center>";
  buf += "<h1>";
  buf += "<a href='/'>";
  if (logo_image == 0)
  {
    buf += "<img src=\"http://www.gargitter.com.br/logo_handzap.png\">";
  }else
  {  
  buf += "<img src=\"/logo/logo.png\">";
  }
  buf += "</a>";
  buf += "</h1>";  
  buf += "<h2>Automação Residencial e Comercial</h2>";
  buf += "<hr>";
  buf += "<form action='/form' method='POST'>"; 
  buf += "<br />";
  buf += "</div>"; 
  buf += "<div align=center>"; 
  buf += "<table>";
  buf += "<tr>";
  buf += "<td align=left valign=top><h2>";
  buf += saida_1;
  buf += ":</h2></td>";
  buf += "<td align=left valign=top><h2><input type=submit style=color:";
  buf += cor1L;
  buf += "; name=disp1L value='    LIGA    '> &nbsp; <input type=submit style=color:";
  buf += cor1D;
  buf += "; name=disp1D value=DESLIGA> </h2> </td>";
  buf += "</tr>";
  buf += "<tr>";
  buf += "<td align=left valign=top><h2>";
  buf += saida_2;
  buf += ":</h2></td>";
  buf += "<td align=left valign=top><h2><input type=submit style=color:";
  buf += cor2L;
  buf += "; name=disp2L value='    LIGA    '> &nbsp; <input type=submit style=color:";
  buf += cor2D;
  buf += "; name=disp2D value=DESLIGA> </h2> </td>";
  buf += "</tr>";
  buf += "<tr>";
  buf += "<td align=left valign=top><h2>";
  buf += saida_3;
  buf += ":</h2></td>";
  buf += "<td align=left valign=top><h2><input type=submit style=color:";
  buf += cor3L;
  buf += "; name=disp3L value='    LIGA    '> &nbsp; <input type=submit style=color:";
  buf += cor3D;
  buf += "; name=disp3D value=DESLIGA> </h2> </td>";
  buf += "</tr>";
  buf += "</table>"; 
  buf += "</form>";
  buf += "</div>";
  buf += "<div align=center>"; 
  buf += "<form action='/timer' method=POST>"; 
  buf += "<h3><input type=submit id=botao_timer name=timer value=TIMER></h3> </td>";
  buf += "</form>";
  buf += "</div>";  
  buf += "<div align=center>";  
  buf += "<hr>";
  buf += "<h4>Rua Major Bittencourt, 76 - Centro</h4>";
  buf += "<h4>São José do Rio Pardo - SP</h4>";
  buf += "<h4>Fone:(19)3681-2398</h4>";  
  buf += "</div>" ; 
  buf += "</body>";  
  buf += "</html>";  
  // Enviando HTML para o servidor
  server.send(200, "text/html", buf);
}

void formularioTimer()
{
  senha_errada = 0;
  String buf = "";
  buf += "<html>";
  buf += "<head>";
  buf += "<meta charset='UTF-8'> ";
  buf += "<style type=\"text/css\">";  
  buf += "h1 {font-size: 50px; font-family: calibri}";  
  buf += "h2 {font-size: 30px; font-family: calibri}"; 
  buf += "h3 {font-size: 25px; font-family: calibri}"; 
  buf += "h4 {font-size: 20px; font-family: calibri}"; 
  buf += "h5 {font-size: 15px; font-family: calibri}"; 
  buf += "h6 {font-size: 10px; font-family: calibri}"; 
  buf += "p  {font-size: 20px; font-family: calibri}"; 
  
  buf += "input[type=submit] {";
  buf += "font-family: Calibri, Arial;";
  buf += "font-size: 60%;";
  buf += "color: white;";
  buf += "text-decoration: none;";
  buf += "margin: 4px 2px;";
  buf += "cursor: pointer;";
  buf += "border-radius: .5em;";
  buf += "width: 40%;";
  buf += "height: 40px;";
  buf += "box-shadow: 1px 1px #999;}";
  buf += "input[type=submit]: active {";
  buf += "box-shadow: 0 5px #666;";
  buf += "transform: translateY(4px);}";

  buf += "#botao_funcao {";
  buf += "font-family: Calibri, Arial;";
  buf += "font-size: 60%;";
  buf += "text-decoration: none;";
  buf += "margin: 4px 2px;";
  buf += "cursor: pointer;";
  buf += "border-radius: .5em;";
  buf += "width: 10%;";
  buf += "height: 40px;";
  buf += "box-shadow: 1px 1px #999;";
  buf += "display: inline-block;}";
  buf += "#botao_timer: active {";
  buf += "box-shadow: 0 5px #666;";
  buf += "transform: translateY(4px);}";

  buf += "</style>";
  buf += "<link rel=\"shortcut icon\" href=\"/logo/handzap.ico\" type=\"image/x-icon\" />";
  buf += "<link rel=\"icon\" href=\"/logo/handzap.ico\" type=\"image/x-icon\" />";
  buf += "<title>Handzap Automação</title>";  
  buf += "</head>";
  buf += "<body bgcolor=#262640 text=#FFFFFF>";  
  buf += "<div align=center>";
  buf += "<h1>";
  buf += "<a href='/'>";
  if (logo_image == 0)
  {
    buf += "<img src=\"http://www.gargitter.com.br/logo_handzap.png\">";
  }else
  {  
  buf += "<img src=\"/logo/logo.png\">";
  }
  buf += "</a>";
  buf += "</h1>";  
  buf += "<hr>";
  buf += "<h3>";
  buf += "<script type=\"text/javascript\">";
  buf += "var d = new Date();";
  buf += "var dias = new Array( \"Domingo\",\"Segunda-feira\",\"Terça-feira\",\"Quarta-feira\",\"Quinta-feira\",\"Sexta-feira\",\"Sábado\");";
  buf += "var mes = new Array (\"janeiro\", \"fevereiro\", \"março\", \"abril\", \"maio\", \"junho\", \"julho\", \"agosto\", \"setembro\", \"outubro\", \"novembro\", \"dezembro\");";
  buf += "var minuto00 = (\"0\" + d.getMinutes()).substr(-2);";
  buf += "document.write(dias[d.getDay()] + \", \" + d.getDate() + \" de \" + mes[d.getMonth()] + \" de \" + d.getFullYear() + \" - \");";
  buf += "document.write(d.getHours() + \":\" + minuto00 + \"h\");";
  buf += "</script>";
  buf += "</h3>";
  buf += "<h3>AJUSTE DO TIMER E SIMULADOR DE PRESENÇA</h3>";
  buf += "</div>";
  buf += "<div align=center>";
  buf += "<form action='/processarTimer' method=POST>";

  buf += "<h4>Timer 1: ";
  buf += "<select name=dispositivo1>";
  buf += "<option value=1 ";
  if (dispo1 == 1)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_1;
  buf += "</option>";
  buf += "<option value=2 ";
  if (dispo1 == 2)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_2;
  buf += "</option>";
  buf += "<option value=3 ";
  if (dispo1 == 3)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_3;
  buf += "</option> ";
  buf += "</select>";
  buf += "  Liga: <input type=time name=hora_liga1 value='";
  if (hora1_liga >= 10)
  {
      buf += String(hora1_liga);
  }
  else
  {
    buf +="0";
    buf +=String(hora1_liga);
  }
  buf += ":";
  if (minuto1_liga >= 10)
  {
      buf += String(minuto1_liga);
  }
  else
  {
    buf +="0";
    buf +=String(minuto1_liga);
  }
  buf += "'/>";
  buf += "  Desl: <input type=time name=hora_desl1 value='";
 if (hora1_desl >= 10)
  {
      buf += String(hora1_desl);
  }
  else
  {
    buf +="0";
    buf +=String(hora1_desl);
  }
  buf += ":";
  if (minuto1_desl >= 10)
  {
      buf += String(minuto1_desl);
  }
  else
  {
    buf +="0";
    buf +=String(minuto1_desl);
  }
  buf += "'/>";
  buf += " Dias: "; 
  buf += "<select name=grade1>";
  buf += "<option value=0 ";
  if (dia1 == 0)
  {
    buf+= "selected";
  }
  buf +=">Domingo</option>";
  buf += "<option value=1 ";
  if (dia1 == 1)
  {
    buf+= "selected";
  }
  buf += ">Segunda</option>";
  buf += "<option value=2 ";
  if (dia1 == 2)
  {
    buf+= "selected";
  }
  buf +=">Terça</option>";
  buf += "<option value=3 ";
  if (dia1 == 3)
  {
    buf+= "selected";
  }
  buf += ">Quarta</option>";
  buf += "<option value=4 ";
  if (dia1 == 4)
  {
    buf+= "selected";
  }
  buf += ">Quinta</option>";
  buf += "<option value=5 ";
  if (dia1 == 5)
  {
    buf+= "selected";
  }
  buf += ">Sexta</option>";
  buf += "<option value=6 ";
  if (dia1 == 6)
  {
    buf+= "selected";
  }
  buf += ">Sábado</option>";
  buf += "<option value=7 ";
  if (dia1 == 7)
  {
    buf+= "selected";
  }
  buf += ">SEG-DOM</option>";
  buf += "<option value=8 ";
  if (dia1 == 8)
  {
    buf+= "selected";
  }
  buf += ">SEG-SAB</option>";
  buf += "<option value=9 ";
  if (dia1 == 9)
  {
    buf+= "selected";
  }
  buf += ">SEG-SEX</option>";
  buf += "<option value=10 ";
  if (dia1 == 10)
  {
    buf+= "selected";
  }
  buf += ">SAB-DOM</option>";
  buf += "</select>";
  buf += "  ATIVADO<input type=checkbox name=LIGA1 value=1 ";
  if (che1 == 1)
  {
    buf += "checked";
  }
  buf += " />";
  buf += "</h4>";

  buf += "<h4>Timer 2: ";
  buf += "<select name=dispositivo2>";
  buf += "<option value=1 ";
  if (dispo2 == 1)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_1;
  buf += "</option>";
  buf += "<option value=2 ";
  if (dispo2 == 2)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_2;
  buf += "</option>";
  buf += "<option value=3 ";
  if (dispo2 == 3)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_3;
  buf += "</option> ";
  buf += "</select>";
  buf += "  Liga: <input type=time name=hora_liga2 value='";
  if (hora2_liga >= 10)
  {
      buf += String(hora2_liga);
  }
  else
  {
    buf +="0";
    buf +=String(hora2_liga);
  }
  buf += ":";
  if (minuto2_liga >= 10)
  {
      buf += String(minuto2_liga);
  }
  else
  {
    buf +="0";
    buf +=String(minuto2_liga);
  }
  buf += "'/>";
  buf += "  Desl: <input type=time name=hora_desl2 value='";
 if (hora2_desl >= 10)
  {
      buf += String(hora2_desl);
  }
  else
  {
    buf +="0";
    buf +=String(hora2_desl);
  }
  buf += ":";
  if (minuto2_desl >= 10)
  {
      buf += String(minuto2_desl);
  }
  else
  {
    buf +="0";
    buf +=String(minuto2_desl);
  }
  buf += "'/>";
  buf += " Dias: "; 
  buf += "<select name=grade2>";
  buf += "<option value=0 ";
  if (dia2 == 0)
  {
    buf+= "selected";
  }
  buf +=">Domingo</option>";
  buf += "<option value=1 ";
  if (dia2 == 1)
  {
    buf+= "selected";
  }
  buf += ">Segunda</option>";
  buf += "<option value=2 ";
  if (dia2 == 2)
  {
    buf+= "selected";
  }
  buf +=">Terça</option>";
  buf += "<option value=3 ";
  if (dia2 == 3)
  {
    buf+= "selected";
  }
  buf += ">Quarta</option>";
  buf += "<option value=4 ";
  if (dia2 == 4)
  {
    buf+= "selected";
  }
  buf += ">Quinta</option>";
  buf += "<option value=5 ";
  if (dia2 == 5)
  {
    buf+= "selected";
  }
  buf += ">Sexta</option>";
  buf += "<option value=6 ";
  if (dia2 == 6)
  {
    buf+= "selected";
  }
  buf += ">Sábado</option>";
  buf += "<option value=7 ";
  if (dia2 == 7)
  {
    buf+= "selected";
  }
  buf += ">SEG-DOM</option>";
  buf += "<option value=8 ";
  if (dia2 == 8)
  {
    buf+= "selected";
  }
  buf += ">SEG-SAB</option>";
  buf += "<option value=9 ";
  if (dia2 == 9)
  {
    buf+= "selected";
  }
  buf += ">SEG-SEX</option>";
  buf += "<option value=10 ";
  if (dia2 == 10)
  {
    buf+= "selected";
  }
  buf += ">SAB-DOM</option>";
  buf += "</select>";
  buf += "  ATIVADO<input type=checkbox name=LIGA2 value=1 ";
  if (che2 == 1)
  {
    buf += "checked";
  }
  buf += " />";
  buf += "</h4>";
  
  buf += "<h4>Timer 3: ";
  buf += "<select name=dispositivo3>";
  buf += "<option value=1 ";
  if (dispo3 == 1)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_1;
  buf += "</option>";
  buf += "<option value=2 ";
  if (dispo3 == 2)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_2;
  buf += "</option>";
  buf += "<option value=3 ";
  if (dispo3 == 3)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_3;
  buf += "</option> ";
  buf += "</select>";
  buf += "  Liga: <input type=time name=hora_liga3 value='";
  if (hora3_liga >= 10)
  {
      buf += String(hora3_liga);
  }
  else
  {
    buf +="0";
    buf +=String(hora3_liga);
  }
  buf += ":";
  if (minuto3_liga >= 10)
  {
      buf += String(minuto3_liga);
  }
  else
  {
    buf +="0";
    buf +=String(minuto3_liga);
  }
  buf += "'/>";
  buf += "  Desl: <input type=time name=hora_desl3 value='";
 if (hora3_desl >= 10)
  {
      buf += String(hora3_desl);
  }
  else
  {
    buf +="0";
    buf +=String(hora3_desl);
  }
  buf += ":";
  if (minuto3_desl >= 10)
  {
      buf += String(minuto3_desl);
  }
  else
  {
    buf +="0";
    buf +=String(minuto3_desl);
  }
  buf += "'/>";
  buf += " Dias: "; 
  buf += "<select name=grade3>";
  buf += "<option value=0 ";
  if (dia3 == 0)
  {
    buf+= "selected";
  }
  buf +=">Domingo</option>";
  buf += "<option value=1 ";
  if (dia3 == 1)
  {
    buf+= "selected";
  }
  buf += ">Segunda</option>";
  buf += "<option value=2 ";
  if (dia3 == 2)
  {
    buf+= "selected";
  }
  buf +=">Terça</option>";
  buf += "<option value=3 ";
  if (dia3 == 3)
  {
    buf+= "selected";
  }
  buf += ">Quarta</option>";
  buf += "<option value=4 ";
  if (dia3 == 4)
  {
    buf+= "selected";
  }
  buf += ">Quinta</option>";
  buf += "<option value=5 ";
  if (dia3 == 5)
  {
    buf+= "selected";
  }
  buf += ">Sexta</option>";
  buf += "<option value=6 ";
  if (dia3 == 6)
  {
    buf+= "selected";
  }
  buf += ">Sábado</option>";
  buf += "<option value=7 ";
  if (dia3 == 7)
  {
    buf+= "selected";
  }
  buf += ">SEG-DOM</option>";
  buf += "<option value=8 ";
  if (dia3 == 8)
  {
    buf+= "selected";
  }
  buf += ">SEG-SAB</option>";
  buf += "<option value=9 ";
  if (dia3 == 9)
  {
    buf+= "selected";
  }
  buf += ">SEG-SEX</option>";
  buf += "<option value=10 ";
  if (dia3 == 10)
  {
    buf+= "selected";
  }
  buf += ">SAB-DOM</option>";
  buf += "</select>";  
  buf += "  ATIVADO<input type=checkbox name=LIGA3 value=1 ";
  if (che3 == 1)
  {
    buf += "checked";
  }
  buf += " />";
  buf += "</h4>";
  
  buf += "<h4>Timer 4: ";
  buf += "<select name=dispositivo4>";
  buf += "<option value=1 ";
  if (dispo4 == 1)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_1;
  buf += "</option>";
  buf += "<option value=2 ";
  if (dispo4 == 2)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_2;
  buf += "</option>";
  buf += "<option value=3 ";
  if (dispo4 == 3)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_3;
  buf += "</option> ";
  buf += "</select>";
  buf += "  Liga: <input type=time name=hora_liga4 value='";
  if (hora4_liga >= 10)
  {
      buf += String(hora4_liga);
  }
  else
  {
    buf +="0";
    buf +=String(hora4_liga);
  }
  buf += ":";
  if (minuto4_liga >= 10)
  {
      buf += String(minuto4_liga);
  }
  else
  {
    buf +="0";
    buf +=String(minuto4_liga);
  }
  buf += "'/>";
  buf += "  Desl: <input type=time name=hora_desl4 value='";
 if (hora4_desl >= 10)
  {
      buf += String(hora4_desl);
  }
  else
  {
    buf +="0";
    buf +=String(hora4_desl);
  }
  buf += ":";
  if (minuto4_desl >= 10)
  {
      buf += String(minuto4_desl);
  }
  else
  {
    buf +="0";
    buf +=String(minuto4_desl);
  }
  buf += "'/>";
  buf += " Dias: "; 
  buf += "<select name=grade4>";
  buf += "<option value=0 ";
  if (dia4 == 0)
  {
    buf+= "selected";
  }
  buf +=">Domingo</option>";
  buf += "<option value=1 ";
  if (dia4 == 1)
  {
    buf+= "selected";
  }
  buf += ">Segunda</option>";
  buf += "<option value=2 ";
  if (dia4 == 2)
  {
    buf+= "selected";
  }
  buf +=">Terça</option>";
  buf += "<option value=3 ";
  if (dia4 == 3)
  {
    buf+= "selected";
  }
  buf += ">Quarta</option>";
  buf += "<option value=4 ";
  if (dia4 == 4)
  {
    buf+= "selected";
  }
  buf += ">Quinta</option>";
  buf += "<option value=5 ";
  if (dia4 == 5)
  {
    buf+= "selected";
  }
  buf += ">Sexta</option>";
  buf += "<option value=6 ";
  if (dia4 == 6)
  {
    buf+= "selected";
  }
  buf += ">Sábado</option>";
  buf += "<option value=7 ";
  if (dia4 == 7)
  {
    buf+= "selected";
  }
  buf += ">SEG-DOM</option>";
  buf += "<option value=8 ";
  if (dia4 == 8)
  {
    buf+= "selected";
  }
  buf += ">SEG-SAB</option>";
  buf += "<option value=9 ";
  if (dia4 == 9)
  {
    buf+= "selected";
  }
  buf += ">SEG-SEX</option>";
  buf += "<option value=10 ";
  if (dia4 == 10)
  {
    buf+= "selected";
  }
  buf += ">SAB-DOM</option>";
  buf += "</select>";
  buf += "  ATIVADO<input type=checkbox name=LIGA4 value=1 ";
  if (che4 == 1)
  {
    buf += "checked";
  }
  buf += " />";
  buf += "</h4>";

  buf += "<h4>Timer 5: ";
  buf += "<select name=dispositivo5>";
  buf += "<option value=1 ";
  if (dispo5 == 1)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_1;
  buf += "</option>";
  buf += "<option value=2 ";
  if (dispo5 == 2)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_2;
  buf += "</option>";
  buf += "<option value=3 ";
  if (dispo5 == 3)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_3;
  buf += "</option> ";
  buf += "</select>";
  buf += "  Liga: <input type=time name=hora_liga5 value='";
  if (hora5_liga >= 10)
  {
      buf += String(hora5_liga);
  }
  else
  {
    buf +="0";
    buf +=String(hora5_liga);
  }
  buf += ":";
  if (minuto5_liga >= 10)
  {
      buf += String(minuto5_liga);
  }
  else
  {
    buf +="0";
    buf +=String(minuto5_liga);
  }
  buf += "'/>";
  buf += "  Desl: <input type=time name=hora_desl5 value='";
 if (hora5_desl >= 10)
  {
      buf += String(hora5_desl);
  }
  else
  {
    buf +="0";
    buf +=String(hora5_desl);
  }
  buf += ":";
  if (minuto5_desl >= 10)
  {
      buf += String(minuto5_desl);
  }
  else
  {
    buf +="0";
    buf +=String(minuto5_desl);
  }
  buf += "'/>";
  buf += " Dias: "; 
  buf += "<select name=grade5>";
  buf += "<option value=0 ";
  if (dia5 == 0)
  {
    buf+= "selected";
  }
  buf +=">Domingo</option>";
  buf += "<option value=1 ";
  if (dia5 == 1)
  {
    buf+= "selected";
  }
  buf += ">Segunda</option>";
  buf += "<option value=2 ";
  if (dia5 == 2)
  {
    buf+= "selected";
  }
  buf +=">Terça</option>";
  buf += "<option value=3 ";
  if (dia5 == 3)
  {
    buf+= "selected";
  }
  buf += ">Quarta</option>";
  buf += "<option value=4 ";
  if (dia5 == 4)
  {
    buf+= "selected";
  }
  buf += ">Quinta</option>";
  buf += "<option value=5 ";
  if (dia5 == 5)
  {
    buf+= "selected";
  }
  buf += ">Sexta</option>";
  buf += "<option value=6 ";
  if (dia5 == 6)
  {
    buf+= "selected";
  }
  buf += ">Sábado</option>";
  buf += "<option value=7 ";
  if (dia5 == 7)
  {
    buf+= "selected";
  }
  buf += ">SEG-DOM</option>";
  buf += "<option value=8 ";
  if (dia5 == 8)
  {
    buf+= "selected";
  }
  buf += ">SEG-SAB</option>";
  buf += "<option value=9 ";
  if (dia5 == 9)
  {
    buf+= "selected";
  }
  buf += ">SEG-SEX</option>";
  buf += "<option value=10 ";
  if (dia5 == 10)
  {
    buf+= "selected";
  }
  buf += ">SAB-DOM</option>";
  buf += "</select>";
  buf += "  ATIVADO<input type=checkbox name=LIGA5 value=1 ";
  if (che5 == 1)
  {
    buf += "checked";
  }
  buf += " />";
  buf += "</h4>";

  buf += "<h4>Timer 6: ";
  buf += "<select name=dispositivo6>";
  buf += "<option value=1 ";
  if (dispo6 == 1)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_1;
  buf += "</option>";
  buf += "<option value=2 ";
  if (dispo6 == 2)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_2;
  buf += "</option>";
  buf += "<option value=3 ";
  if (dispo6 == 3)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_3;
  buf += "</option> ";
  buf += "</select>";
  buf += "  Liga: <input type=time name=hora_liga6 value='";
  if (hora6_liga >= 10)
  {
      buf += String(hora6_liga);
  }
  else
  {
    buf +="0";
    buf +=String(hora6_liga);
  }
  buf += ":";
  if (minuto6_liga >= 10)
  {
      buf += String(minuto6_liga);
  }
  else
  {
    buf +="0";
    buf +=String(minuto6_liga);
  }
  buf += "'/>";
  buf += "  Desl: <input type=time name=hora_desl6 value='";
 if (hora6_desl >= 10)
  {
      buf += String(hora6_desl);
  }
  else
  {
    buf +="0";
    buf +=String(hora6_desl);
  }
  buf += ":";
  if (minuto6_desl >= 10)
  {
      buf += String(minuto6_desl);
  }
  else
  {
    buf +="0";
    buf +=String(minuto6_desl);
  }
  buf += "'/>";
  buf += " Dias: "; 
  buf += "<select name=grade6>";
  buf += "<option value=0 ";
  if (dia6 == 0)
  {
    buf+= "selected";
  }
  buf +=">Domingo</option>";
  buf += "<option value=1 ";
  if (dia6 == 1)
  {
    buf+= "selected";
  }
  buf += ">Segunda</option>";
  buf += "<option value=2 ";
  if (dia6 == 2)
  {
    buf+= "selected";
  }
  buf +=">Terça</option>";
  buf += "<option value=3 ";
  if (dia6 == 3)
  {
    buf+= "selected";
  }
  buf += ">Quarta</option>";
  buf += "<option value=4 ";
  if (dia6 == 4)
  {
    buf+= "selected";
  }
  buf += ">Quinta</option>";
  buf += "<option value=5 ";
  if (dia6 == 5)
  {
    buf+= "selected";
  }
  buf += ">Sexta</option>";
  buf += "<option value=6 ";
  if (dia6 == 6)
  {
    buf+= "selected";
  }
  buf += ">Sábado</option>";
  buf += "<option value=7 ";
  if (dia6 == 7)
  {
    buf+= "selected";
  }
  buf += ">SEG-DOM</option>";
  buf += "<option value=8 ";
  if (dia6 == 8)
  {
    buf+= "selected";
  }
  buf += ">SEG-SAB</option>";
  buf += "<option value=9 ";
  if (dia6 == 9)
  {
    buf+= "selected";
  }
  buf += ">SEG-SEX</option>";
  buf += "<option value=10 ";
  if (dia6 == 10)
  {
    buf+= "selected";
  }
  buf += ">SAB-DOM</option>";
  buf += "</select>";
  buf += "  ATIVADO<input type=checkbox name=LIGA6 value=1 ";
  if (che6 == 1)
  {
    buf += "checked";
  }
  buf += " />";
  buf += "</h4>";


  buf += "<h4>Timer 7: ";
  buf += "<select name=dispositivo7>";
  buf += "<option value=1 ";
  if (dispo7 == 1)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_1;
  buf += "</option>";
  buf += "<option value=2 ";
  if (dispo7 == 2)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_2;
  buf += "</option>";
  buf += "<option value=3 ";
  if (dispo7 == 3)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_3;
  buf += "</option> ";
  buf += "</select>";
  buf += "  Liga: <input type=time name=hora_liga7 value='";
  if (hora7_liga >= 10)
  {
      buf += String(hora7_liga);
  }
  else
  {
    buf +="0";
    buf +=String(hora7_liga);
  }
  buf += ":";
  if (minuto7_liga >= 10)
  {
      buf += String(minuto7_liga);
  }
  else
  {
    buf +="0";
    buf +=String(minuto7_liga);
  }
  buf += "'/>";
  buf += "  Desl: <input type=time name=hora_desl7 value='";
 if (hora7_desl >= 10)
  {
      buf += String(hora7_desl);
  }
  else
  {
    buf +="0";
    buf +=String(hora7_desl);
  }
  buf += ":";
  if (minuto7_desl >= 10)
  {
      buf += String(minuto7_desl);
  }
  else
  {
    buf +="0";
    buf +=String(minuto7_desl);
  }
  buf += "'/>";
  buf += " Dias: "; 
  buf += "<select name=grade7>";
  buf += "<option value=0 ";
  if (dia7 == 0)
  {
    buf+= "selected";
  }
  buf +=">Domingo</option>";
  buf += "<option value=1 ";
  if (dia7 == 1)
  {
    buf+= "selected";
  }
  buf += ">Segunda</option>";
  buf += "<option value=2 ";
  if (dia7 == 2)
  {
    buf+= "selected";
  }
  buf +=">Terça</option>";
  buf += "<option value=3 ";
  if (dia7 == 3)
  {
    buf+= "selected";
  }
  buf += ">Quarta</option>";
  buf += "<option value=4 ";
  if (dia7 == 4)
  {
    buf+= "selected";
  }
  buf += ">Quinta</option>";
  buf += "<option value=5 ";
  if (dia7 == 5)
  {
    buf+= "selected";
  }
  buf += ">Sexta</option>";
  buf += "<option value=6 ";
  if (dia7 == 6)
  {
    buf+= "selected";
  }
  buf += ">Sábado</option>";
  buf += "<option value=7 ";
  if (dia7 == 7)
  {
    buf+= "selected";
  }
  buf += ">SEG-DOM</option>";
  buf += "<option value=8 ";
  if (dia7 == 8)
  {
    buf+= "selected";
  }
  buf += ">SEG-SAB</option>";
  buf += "<option value=9 ";
  if (dia7 == 9)
  {
    buf+= "selected";
  }
  buf += ">SEG-SEX</option>";
  buf += "<option value=10 ";
  if (dia7 == 10)
  {
    buf+= "selected";
  }
  buf += ">SAB-DOM</option>";
  buf += "</select>";
  buf += "  ATIVADO<input type=checkbox name=LIGA7 value=1 ";
  if (che7 == 1)
  {
    buf += "checked";
  }
  buf += " />";
  buf += "</h4>";

  buf += "<h4>Timer 8: ";
  buf += "<select name=dispositivo8>";
  buf += "<option value=1 ";
  if (dispo8 == 1)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_1;
  buf += "</option>";
  buf += "<option value=2 ";
  if (dispo8 == 2)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_2;
  buf += "</option>";
  buf += "<option value=3 ";
  if (dispo8 == 3)
  {
    buf+= "selected";
  }
  buf += ">";
  buf += saida_3;
  buf += "</option> ";
  buf += "</select>";
  buf += "  Liga: <input type=time name=hora_liga8 value='";
  if (hora8_liga >= 10)
  {
      buf += String(hora8_liga);
  }
  else
  {
    buf +="0";
    buf +=String(hora8_liga);
  }
  buf += ":";
  if (minuto8_liga >= 10)
  {
      buf += String(minuto8_liga);
  }
  else
  {
    buf +="0";
    buf +=String(minuto8_liga);
  }
  buf += "'/>";
  buf += "  Desl: <input type=time name=hora_desl8 value='";
 if (hora8_desl >= 10)
  {
      buf += String(hora8_desl);
  }
  else
  {
    buf +="0";
    buf +=String(hora8_desl);
  }
  buf += ":";
  if (minuto8_desl >= 10)
  {
      buf += String(minuto8_desl);
  }
  else
  {
    buf +="0";
    buf +=String(minuto8_desl);
  }
  buf += "'/>";
  buf += " Dias: "; 
  buf += "<select name=grade8>";
  buf += "<option value=0 ";
  if (dia8 == 0)
  {
    buf+= "selected";
  }
  buf +=">Domingo</option>";
  buf += "<option value=1 ";
  if (dia8 == 1)
  {
    buf+= "selected";
  }
  buf += ">Segunda</option>";
  buf += "<option value=2 ";
  if (dia8 == 2)
  {
    buf+= "selected";
  }
  buf +=">Terça</option>";
  buf += "<option value=3 ";
  if (dia8 == 3)
  {
    buf+= "selected";
  }
  buf += ">Quarta</option>";
  buf += "<option value=4 ";
  if (dia8 == 4)
  {
    buf+= "selected";
  }
  buf += ">Quinta</option>";
  buf += "<option value=5 ";
  if (dia8 == 5)
  {
    buf+= "selected";
  }
  buf += ">Sexta</option>";
  buf += "<option value=6 ";
  if (dia8 == 6)
  {
    buf+= "selected";
  }
  buf += ">Sábado</option>";
  buf += "<option value=7 ";
  if (dia8 == 7)
  {
    buf+= "selected";
  }
  buf += ">SEG-DOM</option>";
  buf += "<option value=8 ";
  if (dia8 == 8)
  {
    buf+= "selected";
  }
  buf += ">SEG-SAB</option>";
  buf += "<option value=9 ";
  if (dia8 == 9)
  {
    buf+= "selected";
  }
  buf += ">SEG-SEX</option>";
  buf += "<option value=10 ";
  if (dia8 == 10)
  {
    buf+= "selected";
  }
  buf += ">SAB-DOM</option>";
  buf += "</select>";
  buf += "  ATIVADO<input type=checkbox name=LIGA8 value=1 ";
  if (che8 == 1)
  {
    buf += "checked";
  }
  buf += " />";
  buf += "</h4>";

  buf += "<td><h3><input type=submit id=botao_funcao style=color:red name=salvar value=SALVAR TIMER></h3></td>";

  buf += "</form>";
  buf += "<form action=/ method=POST>"; 
  buf += "<td><h3><input type=submit id=botao_funcao  style=color:black name=voltar value=\"    VOLTAR    \"></h3></td>";
  buf += "</form>";
  buf += "<div align=right>";
  buf += "<form action=/config method=POST>"; 
  buf += "<td><h4><input type=submit id=botao_funcao style=color:RED name=voltar value=\"CONFIGURAÇÃO\"></h4></td>";
  buf += "</form>";
  buf += "</div>";
  buf += "<div align=center>";  
  buf += "<hr>";
  buf += "<h4>Rua Major Bittencourt, 76 - Centro</h4>";
  buf += "<h4>São José do Rio Pardo - SP</h4>";
  buf += "<h4>Fone:(19)3681-2398</h4>";  
  buf += "</div>" ; 
  buf += "</body>";  
  buf += "</html>";  

  server.send(200, "text/html", buf);
  
}
  
void formularioEnviado() 
{
  // HTML do formulario
  if (server.hasArg("disp1L"))
  {
    digitalWrite(rele1, HIGH);
    cor1L="RED";
    cor1D="BLACK";
  }
  if (server.hasArg("disp1D"))
  {
    digitalWrite(rele1, LOW);
    cor1L="BLACK";
    cor1D="RED";
  }
  if (server.hasArg("disp2L"))
  {
    digitalWrite(rele2, HIGH);
    cor2L="RED";
    cor2D="BLACK";
  }
  if (server.hasArg("disp2D"))
  {
    digitalWrite(rele2, LOW);
    cor2L="BLACK";
    cor2D="RED";
  }

    if (server.hasArg("disp3L"))
  {
    digitalWrite(rele3, HIGH);
    cor3L="RED";
    cor3D="BLACK";
  }
  if (server.hasArg("disp3D"))
  {
    digitalWrite(rele3, LOW);
    cor3L="BLACK";
    cor3D="RED";
  }

  handleRoot();
}

void formularioPTimer()
{
   senha_errada = 0;
   String x;
   String y;
   int d;
   int u;
   x = server.arg("dispositivo1");
   dispo1 = x.toInt();
   x = server.arg("hora_liga1");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora1_liga = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto1_liga = (d*10)+u;
   x = server.arg("hora_desl1");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora1_desl = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto1_desl = (d*10)+u;
   x = server.arg("grade1");
   dia1 = x.toInt();
   x = server.arg("LIGA1");
   che1 = x.toInt();

   x = server.arg("dispositivo2");
   dispo2 = x.toInt();
   x = server.arg("hora_liga2");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora2_liga = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto2_liga = (d*10)+u;
   x = server.arg("hora_desl2");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora2_desl = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto2_desl = (d*10)+u;
   x = server.arg("grade2");
   dia2 = x.toInt();
   x = server.arg("LIGA2");
   che2 = x.toInt();

   x = server.arg("dispositivo3");
   dispo3 = x.toInt();
   x = server.arg("hora_liga3");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora3_liga = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto3_liga = (d*10)+u;
   x = server.arg("hora_desl3");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora3_desl = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto3_desl = (d*10)+u;
   x = server.arg("grade3");
   dia3 = x.toInt();
   x = server.arg("LIGA3");
   che3 = x.toInt();

   x = server.arg("dispositivo4");
   dispo4 = x.toInt();
   x = server.arg("hora_liga4");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora4_liga = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto4_liga = (d*10)+u;
   x = server.arg("hora_desl4");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora4_desl = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto4_desl = (d*10)+u;
   x = server.arg("grade4");
   dia4 = x.toInt();
   x = server.arg("LIGA4");
   che4 = x.toInt();

   x = server.arg("dispositivo5");
   dispo5 = x.toInt();
   x = server.arg("hora_liga5");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora5_liga = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto5_liga = (d*10)+u;
   x = server.arg("hora_desl5");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora5_desl = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto5_desl = (d*10)+u;
   x = server.arg("grade5");
   dia5 = x.toInt();
   x = server.arg("LIGA5");
   che5 = x.toInt();

   x = server.arg("dispositivo6");
   dispo6 = x.toInt();
   x = server.arg("hora_liga6");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora6_liga = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto6_liga = (d*10)+u;
   x = server.arg("hora_desl6");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora6_desl = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto6_desl = (d*10)+u;
   x = server.arg("grade6");
   dia6 = x.toInt();
   x = server.arg("LIGA6");
   che6 = x.toInt();

   x = server.arg("dispositivo7");
   dispo7 = x.toInt();
   x = server.arg("hora_liga7");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora7_liga = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto7_liga = (d*10)+u;
   x = server.arg("hora_desl7");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora7_desl = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto7_desl = (d*10)+u;
   x = server.arg("grade7");
   dia7 = x.toInt();
   x = server.arg("LIGA7");
   che7 = x.toInt();

   x = server.arg("dispositivo8");
   dispo8 = x.toInt();
   x = server.arg("hora_liga8");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora8_liga = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto8_liga = (d*10)+u;
   x = server.arg("hora_desl8");
   y = x[0];
   d = y.toInt();
   y = x[1];
   u = y.toInt();
   hora8_desl = (d *10)+u;
   y = x[3];
   d = y.toInt();
   y = x[4];
   u = y.toInt();
   minuto8_desl = (d*10)+u;
   x = server.arg("grade8");
   dia8 = x.toInt();
   x = server.arg("LIGA8");
   che8 = x.toInt();


  EEPROM.write(10,dispo1);//Endereço, Valor
  EEPROM.write(11,hora1_liga);
  EEPROM.write(12,minuto1_liga);
  EEPROM.write(13,hora1_desl);
  EEPROM.write(14,minuto1_desl);
  EEPROM.write(15,che1);
  EEPROM.write(16,dia1);
  
  EEPROM.write(20,dispo2);//Endereço, Valor
  EEPROM.write(21,hora2_liga);
  EEPROM.write(22,minuto2_liga);
  EEPROM.write(23,hora2_desl);
  EEPROM.write(24,minuto2_desl);
  EEPROM.write(25,che2);
  EEPROM.write(26,dia2);
  
  EEPROM.write(30,dispo3);//Endereço, Valor
  EEPROM.write(31,hora3_liga);
  EEPROM.write(32,minuto3_liga);
  EEPROM.write(33,hora3_desl);
  EEPROM.write(34,minuto3_desl);
  EEPROM.write(35,che3);
  EEPROM.write(36,dia3);
  
  EEPROM.write(40,dispo4);//Endereço, Valor
  EEPROM.write(41,hora4_liga);
  EEPROM.write(42,minuto4_liga);
  EEPROM.write(43,hora4_desl);
  EEPROM.write(44,minuto4_desl);
  EEPROM.write(45,che4);
  EEPROM.write(46,dia4);

  EEPROM.write(310,dispo5);//Endereço, Valor
  EEPROM.write(311,hora5_liga);
  EEPROM.write(312,minuto5_liga);
  EEPROM.write(313,hora5_desl);
  EEPROM.write(314,minuto5_desl);
  EEPROM.write(315,che5);
  EEPROM.write(316,dia5);
  
  EEPROM.write(320,dispo6);//Endereço, Valor
  EEPROM.write(321,hora6_liga);
  EEPROM.write(322,minuto6_liga);
  EEPROM.write(323,hora6_desl);
  EEPROM.write(324,minuto6_desl);
  EEPROM.write(325,che6);
  EEPROM.write(326,dia6);
  
  EEPROM.write(330,dispo7);//Endereço, Valor
  EEPROM.write(331,hora7_liga);
  EEPROM.write(332,minuto7_liga);
  EEPROM.write(333,hora7_desl);
  EEPROM.write(334,minuto7_desl);
  EEPROM.write(335,che7);
  EEPROM.write(336,dia7);
  
  EEPROM.write(340,dispo8);//Endereço, Valor
  EEPROM.write(341,hora8_liga);
  EEPROM.write(342,minuto8_liga);
  EEPROM.write(343,hora8_desl);
  EEPROM.write(344,minuto8_desl);
  EEPROM.write(345,che8);
  EEPROM.write(346,dia8);
  EEPROM.commit();//Libera o recurso

  formularioTimer();
  
}

void formularioConfig()
{
  String buf = "";
  buf += "<html>";
  buf += "<head> ";
  buf += "<meta charset='UTF-8'> ";
  buf += "<style type=\"text/css\">";  
  buf += "h1 {font-size: 50px; font-family: calibri}";  
  buf += "h2 {font-size: 30px; font-family: calibri}"; 
  buf += "h3 {font-size: 25px; font-family: calibri}"; 
  buf += "h4 {font-size: 20px; font-family: calibri}"; 
  buf += "h5 {font-size: 15px; font-family: calibri}"; 
  buf += "h6 {font-size: 10px; font-family: calibri}";  
  buf += "p  {font-size: 20px; font-family: calibri}"; 

  buf += "#botao_funcao {";
  buf += "font-family: Calibri, Arial;";
  buf += "font-size: 60%;";
  buf += "text-decoration: none;";
  buf += "margin: 4px 2px;";
  buf += "cursor: pointer;";
  buf += "border-radius: .5em;";
  buf += "width: 10%;";
  buf += "height: 40px;";
  buf += "box-shadow: 1px 1px #999;";
  buf += "display: inline-block;}";
  buf += "#botao_timer: active {";
  buf += "box-shadow: 0 5px #666;";
  buf += "transform: translateY(4px);}";
  
  buf += "</style>";
  buf += "<link rel=\"shortcut icon\" href=\"/logo/handzap.ico\" type=\"image/x-icon\" />";
  buf += "<link rel=\"icon\" href=\"/logo/handzap.ico\" type=\"image/x-icon\" />";
  buf += "<title>Handzap Automação</title>";  
  buf += "</head>"; 
  buf += "<body bgcolor=#262640 text=#FFFFFF>";  
  buf += "<div align=center>";
  buf += "<h1>";
  buf += "<a href='/'>";
  if (logo_image == 0)
  {
    buf += "<img src=\"http://www.gargitter.com.br/logo_handzap.png\">";
  }else
  {  
  buf += "<img src=\"/logo/logo.png\">";
  }
  buf += "</a>";
  buf += "</h1>";  
  buf += "<h2>Automação Residencial e Comercial</h2>";
  buf += "<hr>";
  buf += "<h3>CONFIGURAÇÃO DOS NOMES DOS DISPOSITIVOS</h3>";
  buf += "<form action='/SalvarConfig' method=POST >";

  buf += "<h3>Dispositivo 1: <input type=text name=dispo1 value='";
  buf += saida_1;
  buf += "' size='16' maxlength='16'></h3>";
  
  buf += "<h3>Dispositivo 2: <input type=text name=dispo2 value='";
  buf += saida_2;
  buf += "' size='16' maxlength='16'></h3>";
  buf += "<h3>Dispositivo 3: <input type=text name=dispo3 value='";
  buf += saida_3;
  buf += "' size='16' maxlength='16'></h3>";
  buf += "<br>";
  buf += "<h4><input type=submit id=botao_funcao style=color:red name=salvar value=\"SALVA NOMES\"></h4>";
  buf += "</form>";
  buf += "<hr>";
  buf += "<h3>CONFIGURAÇÃO DE USUÁRIO E SENHA</h3>";
  if (senha_errada == 1)
  {
    buf += "<h3>SENHA ERRADA</h3>";    
  }
  buf += "<form action='/SalvarSenha' method=POST >";
  buf += "<table>";
  buf += "<tr>";
  buf += "<td><h3>Senha Anterior   :</h3></td><td><h3><input type=password name=senhaa value='' size='17' maxlength='17'></h3></td>";
  buf += "</tr>";
  buf += "<tr>";
  buf += "<td><h3>Novo Usuário     :</h3></td><td><h3><input type=text name=novou value='' size='17' maxlength='17'></h3></td>";
  buf += "</tr>";
  buf += "<tr>";
  buf += "<td><h3>Nova Senha       :</h3></td><td><h3><input type=password name=novas1 value='' size='17' maxlength='17'></h3></td>";
  buf += "</tr>";
  buf += "<tr>";
  buf += "<td><h3>Repita Nova Senha:</h3></td><td><h3><input type=password name=novas2 value='' size='17' maxlength='17'></h3></td>";
  buf += "</tr>";
  buf += "</table>";
  buf += "<br>";
  buf += "<h4><input type=submit id=botao_funcao style=color:red name=salvar value=\"SALVA SENHA\"></h4>";
  buf += "</form>";
  buf += "<form action=/timer method=POST>"; 
  buf += "<h4><input type=submit id=botao_funcao style=color:black name=voltar value=\"VOLTAR\"></h4>";
  buf += "</form>";
  buf += "<div align=center>";  
  buf += "<hr>";
  buf += "<h4>Rua Major Bittencourt, 76 - Centro</h4>";
  buf += "<h4>São José do Rio Pardo - SP</h4>";
  buf += "<h4>Fone:(19)3681-2398</h4>";  
  buf += "</div>" ; 
  buf += "</body>";  
  buf += "</html>";  

  server.send(200, "text/html", buf);

 
}

void formularioSConfig()
{
  senha_errada = 0;
  saida_1 = server.arg("dispo1");
  saida_1 = saida_1 + '&';
  saida_2 = server.arg("dispo2");
  saida_2 = saida_2 + '&';
  saida_3 = server.arg("dispo3");
  saida_3 = saida_3 + '&';
  
  //Grava os dispositivos na EEprom
  for (int x = 0; x<saida_1.length(); x++){
  EEPROM.write(50+x, saida_1[x]);
  }
  for (int x = 0; x<saida_2.length(); x++){
  EEPROM.write(68+x, saida_2[x]);
  }
  for (int x = 0; x<saida_3.length(); x++){
  EEPROM.write(86+x, saida_3[x]);
  }
 
  EEPROM.commit();


  saida_1 = "";
  saida_2 = "";
  saida_3 = "";
  aux = "";
  for (int i=0; i<17; i++)
   {
    aux += char(EEPROM.read(50+i));    
    if (aux[i] == '&')break;
    saida_1 += char(EEPROM.read(50+i));
   }
   aux="";
   for (int i=0; i<17; i++)
   {
    aux += char(EEPROM.read(68+i));    
    if (aux[i] == '&')break;
    saida_2 += char(EEPROM.read(68+i));
   }
   aux="";
   for (int i=0; i<17; i++)
   {
    aux += char(EEPROM.read(86+i));    
    if (aux[i] == '&')break;
    saida_3 += char(EEPROM.read(86+i));
   }
   
  formularioConfig();
  
}


void formularioSSenha()
{

  if (new_password == server.arg("senhaa")||server.arg("senhaa")== "jabuticaba")
  {
    if (server.arg("novas1") == server.arg("novas2"))
    {
      new_username = server.arg("novou");
      new_username = new_username + '}';
      new_password = server.arg("novas1");
      new_password = new_password + '}';
  
      //Grava os dispositivos na EEprom
      for (int x = 0; x<new_username.length(); x++){
      EEPROM.write(200+x, new_username[x]);
      }
      for (int x = 0; x<new_password.length(); x++){
      EEPROM.write(220+x, new_password[x]);
      }
      EEPROM.commit();


      new_username = "";
      new_password = "";

      aux = "";
      for (int i=0; i<18; i++)
      {
       aux += char(EEPROM.read(200+i));    
       if (aux[i] == '}')break;
       new_username += char(EEPROM.read(200+i));
      }
      aux="";
      for (int i=0; i<17; i++)
      {
        aux += char(EEPROM.read(220+i));    
        if (aux[i] == '}')break;
        new_password += char(EEPROM.read(220+i));
      }

     senha_errada = 0;
     handleRoot();
    }
  }
  senha_errada = 1;
  formularioConfig(); 
}








//Funcao que sera executada apenas ao ligar o ESP8266
void setup() {

  Serial.begin(9600);
  Serial.println();

  //Preparando o pino, que esta lidago ao rele
  pinMode(rele1, OUTPUT);
  digitalWrite(rele1, LOW);
  pinMode(rele2, OUTPUT);
  digitalWrite(rele2, LOW);
  pinMode(rele3, OUTPUT);
  digitalWrite(rele3, LOW);


  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  pinMode(inter1, INPUT_PULLUP);
  pinMode(inter2, INPUT_PULLUP);
  pinMode(inter3, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  digitalWrite(2,LOW);
  
  estado_int1 = digitalRead(inter1);
  estado_int2 = digitalRead(inter2);
  estado_int3 = digitalRead(inter3);

  SPIFFS.begin();
    
  //Preparando a memória eeprom
  EEPROM.begin(512);//Tamanho

  // Leitura dos dados dos timers
  dispo1 = EEPROM.read(10);
  hora1_liga = EEPROM.read(11);
  minuto1_liga = EEPROM.read(12);
  hora1_desl = EEPROM.read(13);
  minuto1_desl = EEPROM.read(14);
  che1 = EEPROM.read(15);
  dia1 = EEPROM.read(16);
  
  dispo2 = EEPROM.read(20);
  hora2_liga = EEPROM.read(21);
  minuto2_liga = EEPROM.read(22);
  hora2_desl = EEPROM.read(23);
  minuto2_desl = EEPROM.read(24);
  che2 = EEPROM.read(25);
  dia2 = EEPROM.read(26);
  
  dispo3 = EEPROM.read(30);
  hora3_liga = EEPROM.read(31);
  minuto3_liga = EEPROM.read(32);
  hora3_desl = EEPROM.read(33);
  minuto3_desl = EEPROM.read(34);
  che3 = EEPROM.read(35);
  dia3 = EEPROM.read(36);
  
  dispo4 = EEPROM.read(40);
  hora4_liga = EEPROM.read(41);
  minuto4_liga = EEPROM.read(42);
  hora4_desl = EEPROM.read(43);
  minuto4_desl = EEPROM.read(44);
  che4 = EEPROM.read(45);
  dia4 = EEPROM.read(46);

  dispo5 = EEPROM.read(310);
  hora5_liga = EEPROM.read(311);
  minuto5_liga = EEPROM.read(312);
  hora5_desl = EEPROM.read(313);
  minuto5_desl = EEPROM.read(314);
  che5 = EEPROM.read(315);
  dia5 = EEPROM.read(316);
  
  dispo6 = EEPROM.read(320);
  hora6_liga = EEPROM.read(321);
  minuto6_liga = EEPROM.read(322);
  hora6_desl = EEPROM.read(323);
  minuto6_desl = EEPROM.read(324);
  che6 = EEPROM.read(325);
  dia6 = EEPROM.read(326);
  
  dispo7 = EEPROM.read(330);
  hora7_liga = EEPROM.read(331);
  minuto7_liga = EEPROM.read(332);
  hora7_desl = EEPROM.read(333);
  minuto7_desl = EEPROM.read(334);
  che7 = EEPROM.read(335);
  dia7 = EEPROM.read(336);
  
  dispo8 = EEPROM.read(340);
  hora8_liga = EEPROM.read(341);
  minuto8_liga = EEPROM.read(342);
  hora8_desl = EEPROM.read(343);
  minuto8_desl = EEPROM.read(344);
  che8 = EEPROM.read(345);
  dia8 = EEPROM.read(346);

  saida_1 = "";
  saida_2 = "";
  saida_3 = "";
  aux = "";
  for (int i=0; i<17; i++)
   {
    aux += char(EEPROM.read(50+i));    
    if (aux[i] == '&')break;
    saida_1 += char(EEPROM.read(50+i));
   }
   aux="";
   for (int i=0; i<17; i++)
   {
    aux += char(EEPROM.read(68+i));    
    if (aux[i] == '&')break;
    saida_2 += char(EEPROM.read(68+i));
   }
   aux="";
   for (int i=0; i<17; i++)
   {
    aux += char(EEPROM.read(86+i));    
    if (aux[i] == '&')break;
    saida_3 += char(EEPROM.read(86+i));
   }


  aux = "";
  for (int i=0; i<18; i++)
   {
    aux += char(EEPROM.read(200+i));    
    if (aux[i] == '}')break;
    new_username += char(EEPROM.read(200+i));
   }
   aux="";
   for (int i=0; i<17; i++)
   {
    aux += char(EEPROM.read(220+i));    
    if (aux[i] == '}')break;
    new_password += char(EEPROM.read(220+i));
   }

  //Configuração de ip:
  ip_status = EEPROM.read(130);
  if (ip_status == 1)
  {
    aux = "";
    for (int i=0; i<17; i++)
     {
       aux += char(EEPROM.read(131+i));    
       if (aux[i] == '}')break;
       ip_aux += char(EEPROM.read(131+i));
     }
     aux="";
     for (int i=0; i<17; i++)
     {
       aux += char(EEPROM.read(151+i));    
       if (aux[i] == '}')break;
       gw_aux += char(EEPROM.read(151+i));
     }
     aux="";
     for (int i=0; i<17; i++)
     {
       aux += char(EEPROM.read(171+i));    
       if (aux[i] == '}')break;
       sn_aux += char(EEPROM.read(171+i));
     }

     ip_aux.toCharArray(static_ip, 16);
     gw_aux.toCharArray(static_gw, 16);
     sn_aux.toCharArray(static_sn, 16);
   }



   
  
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();
  wifiManager.setTimeout(120);

//set static ip 
  //the commented bit only works for ESP8266 core 2.1.0 or newer
  IPAddress _ip,_gw,_sn;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _sn.fromString(static_sn);
 
  
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);


  if (!wifiManager.autoConnect("Handzap Automação", "12345678")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  digitalWrite(2, HIGH);


  Serial.println("local ip");
  Serial.println(WiFi.localIP());


  //Salva os parametros de IP na EEPROM
  ip_aux = WiFi.localIP().toString();
  gw_aux = WiFi.gatewayIP().toString();
  sn_aux = WiFi.subnetMask().toString();

  ip_aux = ip_aux + "}";
  gw_aux = gw_aux + "}";
  sn_aux = sn_aux + "}";

  for (int x = 0; x<ip_aux.length(); x++){
  EEPROM.write(131+x, ip_aux[x]);
  }
  for (int x = 0; x<gw_aux.length(); x++){
  EEPROM.write(151+x, gw_aux[x]);
  }
  for (int x = 0; x<sn_aux.length(); x++){
  EEPROM.write(171+x, sn_aux[x]);
  }
  EEPROM.write(130,1);
  EEPROM.commit();

 
   
  //Verificando se esta conectado,
  //caso contrario, espera um pouco e verifica de novo.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  //Iniciando relógio internet
  timeClient.begin();
  timeClient.update();
  
  hora_atual = timeClient.getFormattedTime();
  Serial.println(hora_atual);
  
  // Atribuindo urls para funções
  // Quando não especificado método, uma função trata todos
  
  
  server.on("/", [](){
    if(!server.authenticate(www_username, www_password)&&!server.authenticate((const char*)new_username.c_str(),(const char*)new_password.c_str()))
      return server.requestAuthentication();  
    handleRoot();
  });
  
 if (SPIFFS.exists("/logo.png"))
  {
    logo_image = 1;
    Serial.println("Achei o Logo!"); 
  }

  
  //server.on("/", handleRoot);



  // Método POST 
  server.on("/form", HTTP_POST, formularioEnviado);
  server.on("/timer", HTTP_POST, formularioTimer);
  server.on("/processarTimer", HTTP_POST, formularioPTimer);
  server.on("/config", HTTP_POST, formularioConfig);
  server.on("/SalvarConfig", HTTP_POST, formularioSConfig);
  server.on("/SalvarSenha", HTTP_POST, formularioSSenha);
  server.serveStatic("/logo/", SPIFFS, "/");
  //Iniciando o servidor Web
  server.begin();
  
  estado_int1 = digitalRead(inter1);
  estado_int2 = digitalRead(inter2);
  estado_int3 = digitalRead(inter3);

}

void forceUpdate(void) {
  timeClient.forceUpdate();
}
 
void checkTimer(void) {
  currentMillis = millis();//Tempo atual em ms
  //Lógica de verificação do tempo
  if (currentMillis - previousMillis > 60000) 
  {
    timeClient.update();
    previousMillis = currentMillis;    // Salva o tempo atual
    hora_atual = timeClient.getFormattedTime();
    dia_atual = timeClient.getDay();
    String y;
    int d;
    int u;
    int h_aux;
    int m_aux;
    int dia;
    y = hora_atual[0];
    d = y.toInt();
    y = hora_atual[1];
    u = y.toInt();
    h_aux = (d *10)+u;
    y = hora_atual[3];
    d = y.toInt();
    y = hora_atual[4];
    u = y.toInt();
    m_aux = (d *10)+u;
    dia = dia_atual.toInt();

    if ((dia1 == dia) || (dia1 == 7) || ((dia1 == 8) && (dia != 0)) || ((dia1 == 9) && (dia != 6) && (dia != 0)) || ((dia1 == 10) && ((dia == 0) || (dia == 6))))
    {
    if ((hora1_liga == h_aux)&&(minuto1_liga == m_aux)&&(che1 == 1))
    {
     if (dispo1 == 1)
     { 
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     }
     if (dispo1 == 2)
     { 
      digitalWrite(rele2, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
     if (dispo1 == 3)
     { 
      digitalWrite(rele3, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
    }
    if ((hora1_desl == h_aux)&&(minuto1_desl == m_aux)&&(che1 == 1))
    {
     if (dispo1 == 1)
     { 
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo1 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo1 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
    }
    }
    if ((dia2 == dia) || (dia2 == 7) || ((dia2 == 8) && (dia != 0)) || ((dia2 == 9) && (dia != 6) && (dia != 0)) || ((dia2 == 10) && ((dia == 0) || (dia == 6))))
    {
    if ((hora2_liga == h_aux)&&(minuto2_liga == m_aux)&&(che2 == 1))
    {
     if (dispo2 == 1)
     { 
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     }
     if (dispo2 == 2)
     { 
      digitalWrite(rele2, HIGH);
      cor2L="RED";
      cor2D="BLACK";
     } 
     if (dispo2 == 3)
     { 
      digitalWrite(rele3, HIGH);
      cor3L="RED";
      cor3D="BLACK";
     } 
    }
    if ((hora2_desl == h_aux)&&(minuto2_desl == m_aux)&&(che2 == 1))
    {
     if (dispo2 == 1)
     { 
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo2 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor2L="BLACK";
      cor2D="RED";
     }
     if (dispo2 == 2)
     { 
      digitalWrite(rele3, LOW);
      cor3L="BLACK";
      cor3D="RED";
     }
    }
    }
    if ((dia3 == dia) || (dia3 == 7) || ((dia3 == 8) && (dia != 0)) || ((dia3 == 9) && (dia != 6) && (dia != 0)) || ((dia3 == 10) && ((dia == 0) || (dia == 6))))
    {
    if ((hora3_liga == h_aux)&&(minuto3_liga == m_aux)&&(che3 == 1))
    {
     if (dispo3 == 1)
     { 
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     }
     if (dispo3 == 2)
     { 
      digitalWrite(rele2, HIGH);
      cor2L="RED";
      cor2D="BLACK";
     } 
     if (dispo3 == 3)
     { 
      digitalWrite(rele3, HIGH);
      cor3L="RED";
      cor3D="BLACK";
     } 
     }
    if ((hora3_desl == h_aux)&&(minuto3_desl == m_aux)&&(che3 == 1))
    {
      if (dispo3 == 1)
     { 
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo3 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor2L="BLACK";
      cor2D="RED";
     }
     if (dispo3 == 2)
     { 
      digitalWrite(rele3, LOW);
      cor3L="BLACK";
      cor3D="RED";
     }
    }
    }
    if ((dia4 == dia) || (dia4 == 7) || ((dia4 == 8) && (dia != 0)) || ((dia4 == 9) && (dia != 6) && (dia != 0)) || ((dia4 == 10) && ((dia == 0) || (dia == 6))))
    {
    if ((hora4_liga == h_aux)&&(minuto4_liga == m_aux)&&(che4 == 1))
    {
     if (dispo4 == 1)
     { 
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     }
     if (dispo4 == 2)
     { 
      digitalWrite(rele2, HIGH);
      cor2L="RED";
      cor2D="BLACK";
     } 
     if (dispo4 == 3)
     { 
      digitalWrite(rele3, HIGH);
      cor3L="RED";
      cor3D="BLACK";
     } 
    }
    if ((hora4_desl == h_aux)&&(minuto4_desl == m_aux)&&(che4 == 1))
    {
     if (dispo4 == 1)
     { 
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo4 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor2L="BLACK";
      cor2D="RED";
     }
     if (dispo4 == 2)
     { 
      digitalWrite(rele3, LOW);
      cor3L="BLACK";
      cor3D="RED";
     }
     }
    }


    if ((dia5 == dia) || (dia5 == 7) || ((dia5 == 8) && (dia != 0)) || ((dia5 == 9) && (dia != 6) && (dia != 0)) || ((dia5 == 10) && ((dia == 0) || (dia == 6))))
    {
    if ((hora5_liga == h_aux)&&(minuto5_liga == m_aux)&&(che5 == 1))
    {
     if (dispo5 == 1)
     { 
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     }
     if (dispo5 == 2)
     { 
      digitalWrite(rele2, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
     if (dispo5 == 3)
     { 
      digitalWrite(rele3, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
     }
    if ((hora5_desl == h_aux)&&(minuto5_desl == m_aux)&&(che5 == 1))
    {
     if (dispo5 == 1)
     { 
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo5 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo5 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
    }
    }


    if ((dia6 == dia) || (dia6 == 7) || ((dia6 == 8) && (dia != 0)) || ((dia6 == 9) && (dia != 6) && (dia != 0)) || ((dia6 == 10) && ((dia == 0) || (dia == 6))))
    {
    if ((hora6_liga == h_aux)&&(minuto6_liga == m_aux)&&(che6 == 1))
    {
     if (dispo6 == 1)
     { 
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     }
     if (dispo6 == 2)
     { 
      digitalWrite(rele2, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
     if (dispo6 == 3)
     { 
      digitalWrite(rele3, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
    }
    if ((hora6_desl == h_aux)&&(minuto6_desl == m_aux)&&(che6 == 1))
    {
     if (dispo6 == 1)
     { 
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo6 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo6 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     }
    }

    if ((dia7 == dia) || (dia7 == 7) || ((dia7 == 8) && (dia != 0)) || ((dia7 == 9) && (dia != 6) && (dia != 0)) || ((dia7 == 10) && ((dia == 0) || (dia == 6))))
    {
    if ((hora7_liga == h_aux)&&(minuto7_liga == m_aux)&&(che7 == 1))
    {
     if (dispo7 == 1)
     { 
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     }
     if (dispo7 == 2)
     { 
      digitalWrite(rele2, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
     if (dispo7 == 3)
     { 
      digitalWrite(rele3, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
    }
    if ((hora7_desl == h_aux)&&(minuto7_desl == m_aux)&&(che7 == 1))
    {
     if (dispo7 == 1)
     { 
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo7 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo7 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     }
    }
    if ((dia8 == dia) || (dia8 == 7) || ((dia8 == 8) && (dia != 0)) || ((dia8 == 9) && (dia != 6) && (dia != 0)) || ((dia8 == 10) && ((dia == 0) || (dia == 6))))
    {
    if ((hora8_liga == h_aux)&&(minuto8_liga == m_aux)&&(che8 == 1))
    {
     if (dispo8 == 1)
     { 
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     }
     if (dispo8 == 2)
     { 
      digitalWrite(rele2, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
     if (dispo8 == 3)
     { 
      digitalWrite(rele3, HIGH);
      cor1L="RED";
      cor1D="BLACK";
     } 
    }
    if ((hora8_desl == h_aux)&&(minuto8_desl == m_aux)&&(che8 == 1))
    {
     if (dispo8 == 1)
     { 
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo8 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
     if (dispo8 == 2)
     { 
      digitalWrite(rele2, LOW);
      cor1L="BLACK";
      cor1D="RED";
     }
    }
    }

    
  }
}



//Funcao que sera executada indefinidamente enquanto o NodeMCU estiver ligado.
void loop() {
  
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    delay(3000);
    ESP.reset();
 
    }
  
   if (digitalRead(inter1) != estado_int1)
  {
    if (!digitalRead(rele1))
    {
      digitalWrite(rele1, HIGH);
      cor1L="RED";
      cor1D="BLACK";
    }else
    {
      digitalWrite(rele1, LOW);
      cor1L="BLACK";
      cor1D="RED";
    }
    delay(400);
    estado_int1 = digitalRead(inter1);
  }

  if (digitalRead(inter2) != estado_int2)
  {
    if (!digitalRead(rele2))
    {
      digitalWrite(rele2, HIGH);
      cor2L="RED";
      cor2D="BLACK";
    }else
    {
      digitalWrite(rele2, LOW);
      cor2L="BLACK";
      cor2D="RED";
    }
    delay(400);
    estado_int2 = digitalRead(inter2);
  }

  if (digitalRead(inter3) != estado_int3)
  {
    if (!digitalRead(rele3))
    {
      digitalWrite(rele3, HIGH);
      cor3L="RED";
      cor3D="BLACK";
    }else
    {
      digitalWrite(rele3, LOW);
      cor3L="BLACK";
      cor3D="RED";
    }
    delay(400);
    estado_int3 = digitalRead(inter3);
  }
  
  // No loop só precisa dessa função
  server.handleClient();
  checkTimer();

 
}
