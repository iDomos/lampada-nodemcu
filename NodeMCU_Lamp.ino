#include <ESP8266WiFi.h>

//Nome da sua rede Wifi
const char* ssid = "LIVINGLAB";

//Senha da rede
const char* password = "livinglab-sebrae";

//IP do ESP (para voce acessar pelo browser - voce TEM que mudar este IP tambem)
IPAddress ip(192, 168, 43, 50);

//IP do roteador da sua rede wifi
IPAddress gateway(172, 50, 4, 1);

//Mascara de rede da sua rede wifi
IPAddress subnet(255, 255, 255, 0);

//Criando o servidor web na porta 80
WiFiServer server(80);

/*
Equivalencia das saidas Digitais entre nodeMCU e ESP8266 (na IDE do Arduino)
NodeMCU – ESP8266
D0 = 16;
D1 = 5;
D2 = 4;
D3 = 0;
D4 = 2;
D5 = 14;
D6 = 12;
D7 = 13;
D8 = 15;
D9 = 3;
D10 = 1;
*/

//Pino do NodeMCU que estara conectado ao rele
const int pin = 4; //Equivalente ao D2 no NodeMCU

//Funcao que sera executada apenas ao ligar o ESP8266
void setup() {
  //Preparando o pino, que esta lidago ao rele
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);

  //Conectando a rede Wifi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);


  //Verificando se esta conectado,
  //caso contrario, espera um pouco e verifica de novo.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Iniciando o servidor Web
  server.begin();
}

//Funcao que sera executada indefinidamente enquanto o NodeMCU estiver ligado.
void loop() {
  //Verificando se o servidor esta pronto.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Verificando se o servidor recebeu alguma requisicao
  while (!client.available()) {
    delay(1);
  }

  //Obtendo a requisicao vinda do browser
  String req = client.readStringUntil('\r');

  //Sugestao dada por Enrico Orlando
  if(req == "GET /favicon.ico HTTP/1.1"){
      req = client.readStringUntil('\r');
  }

  client.flush();

  //Iniciando o buffer que ira conter a pagina HTML que sera enviada para o browser.
  String buf = "";

  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<head> ";
  buf += "<meta charset='UTF-8'> ";
  buf += "<meta http-equiv='cache-control' content='max-age=0' /> ";
  buf += "<meta http-equiv='cache-control' content='no-cache' /> ";
  buf += "<meta http-equiv='expires' content='0' /> ";
  buf += "<meta http-equiv='expires' content='Tue, 01 Jan 1980 1:00:00 GMT' /> ";
  buf += "<meta http-equiv='pragma' content='no-cache' /> ";
  buf += "<title>Automa&ccedil;&atilde;o Residencial</title> ";
  buf += "<style> ";
  buf += "body{font-family:Open Sans; color:#555555;} ";
  buf += "h1{font-size:24px; font-weight:normal; margin:0.4em 0;} ";
  buf += ".container { width: 100%; margin: 0 auto; } ";
  buf += ".container .row { float: left; clear: both; width: 100%; } ";
  buf += ".container .col { float: left; margin: 0 0 1.2em; padding-right: 1.2em; padding-left: 1.2em; } ";
  buf += ".container .col.four, .container .col.twelve { width: 100%; } ";
  buf += "@media screen and (min-width: 767px) { ";
  buf += ".container{width: 100%; max-width: 1080px; margin: 0 auto;} ";
  buf += ".container .row{width:100%; float:left; clear:both;} ";
  buf += ".container .col{float: left; margin: 0 0 1em; padding-right: .5em; padding-left: .5em;} ";
  buf += ".container .col.four { width: 50%; } ";
  buf += ".container .col.tweleve { width: 100%; } ";
  buf += "} ";
  buf += "* {-moz-box-sizing: border-box; -webkit-box-sizing: border-box; box-sizing: border-box;} ";
  buf += "a{text-decoration:none;} ";
  buf += ".btn {font-size: 18px; white-space:nowrap; width:100%; padding:.8em 1.5em; font-family: Open Sans, Helvetica,Arial,sans-serif; ";
  buf += "line-height:18px; display: inline-block;zoom: 1; color: #fff; text-align: center; position:relative; ";
  buf += "-webkit-transition: border .25s linear, color .25s linear, background-color .25s linear; ";
  buf += "transition: border .25s linear, color .25s linear, background-color .25s linear;} ";
  buf += ".btn.btn-sea{background-color: #08bc9a; border-color: #08bc9a; -webkit-box-shadow: 0 3px 0 #088d74; box-shadow: 0 3px 0 #088d74;} ";
  buf += ".btn.btn-sea:hover{background-color:#01a183;} ";
  buf += ".btn.btn-sea:active{ top: 3px; outline: none; -webkit-box-shadow: none; box-shadow: none;} ";
  buf += "</style> ";
  buf += "</head> ";
  buf += "<body> ";
  buf += "<div class='container'> ";
  buf += "<div class='row'> ";
  buf += "<div class='col twelve'> ";
  buf += "<p align='center'><font size='10'>Controle de l&acirc;mpadas</font></p> ";
  buf += "</div> ";
  buf += "</div> ";
  buf += "<div class='row'> ";
  buf += "<div class='col four'> ";
  buf += "<a href='?f=on1' class='btn btn-sea'>Ligar 1</a> ";
  buf += "<a href='?f=on2' class='btn btn-sea' style='margin-top: 10px'>Ligar 2</a> ";
  buf += "<a href='?f=on3' class='btn btn-sea' style='margin-top: 10px'>Ligar 3</a> ";
  buf += "<a href='?f=on4' class='btn btn-sea' style='margin-top: 10px'>Ligar 4</a> ";
  buf += "<a href='?f=on5' class='btn btn-sea' style='margin-top: 10px'>Ligar 5</a> ";
  buf += "</div> ";
  buf += "<div class='col four'> ";
  buf += "<a href='?f=off1' class='btn btn-sea'>Desligar 1</a> ";
  buf += "<a href='?f=off2' class='btn btn-sea' style='margin-top: 10px'>Desligar 2</a> ";
  buf += "<a href='?f=off3' class='btn btn-sea' style='margin-top: 10px'>Desligar 3</a> ";
  buf += "<a href='?f=off4' class='btn btn-sea' style='margin-top: 10px'>Desligar 4</a> ";
  buf += "<a href='?f=off5' class='btn btn-sea' style='margin-top: 10px'>Desligar 5</a> ";
  buf += "</div> ";
  buf += "</div> ";
  buf += "</div> ";
  buf += "</body> ";
  buf += "</html> ";

  //Enviando para o browser a 'pagina' criada.
  client.print(buf);
  client.flush();

  /*
  Equivalencia das saidas Digitais entre nodeMCU e ESP8266 (na IDE do Arduino)
  NodeMCU – ESP8266
  D0 = 16;
  D1 = 5;
  D2 = 4;
  D3 = 0;
  D4 = 2;
  D5 = 14;
  D6 = 12;
  D7 = 13;
  D8 = 15;
  D9 = 3;
  D10 = 1;
  */

  //Analisando a requisicao recebida para decidir se liga ou desliga a lampada
  if (req.indexOf("on1") != -1) {
    Serial.print("on1 chamado");
    digitalWrite(16, HIGH);
  } else if (req.indexOf("on2") != -1) {
    Serial.print("on2 chamado");
    digitalWrite(5, HIGH);
  } else if (req.indexOf("on3") != -1) {
    Serial.print("on3 chamado");
    digitalWrite(4, HIGH);
  } else if (req.indexOf("on4") != -1) {
    Serial.print("on4 chamado");
    digitalWrite(0, HIGH);
  } else if (req.indexOf("on5") != -1) {
    Serial.print("on5 chamado");
    digitalWrite(2, HIGH);
  } else if (req.indexOf("off1") != -1) {
    Serial.print("off1 chamado");
    digitalWrite(16, LOW);
  } else if (req.indexOf("off2") != -1) {
    Serial.print("off2 chamado");
    digitalWrite(5, LOW);
  } else if (req.indexOf("off3") != -1) {
    Serial.print("off3 chamado");
    digitalWrite(4, LOW);
  } else if (req.indexOf("off4") != -1) {
    Serial.print("off4 chamado");
    digitalWrite(0, LOW);
  } else if (req.indexOf("off5") != -1) {
    Serial.print("off5 chamado");
    digitalWrite(2, LOW);
  } else {
    //Requisicao invalida!
    client.stop();
  }
}
