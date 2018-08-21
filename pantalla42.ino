//Alfredo Valdes
#include <SPI.h>
#include "epd4in2.h"
#include "epdpaint.h"
#include "imagedata.h"
#define COLORED     0
#define UNCOLORED   1
const char* Operador[6] = {"     AT&T", "     TELCEL", "     MOVISTAR", " =>  AT&T", " =>  TELCEL", " =>  MOVISTAR"};
unsigned char image[1500];
const char* Saldos[12] = {"     $20", "     $30", "     $50", "     $100", "     $150", "     $200",
                          " =>  $20", " =>  $30", " =>  $50", " =>  $100", " =>  $150", " =>  $200"
                         };
Paint paint(image, 400, 28);
Epd epd;
int _pantallaState = 0;
int _arrowPosition = 0;
int _arrowSaldoPosition = 0;
String operadora, saldo, dinero;
void setup() {
  Serial.begin(9600);
  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }
  epd.ClearFrame();
  epd.DisplayFrame();
  delay(3000);
  /* Deep sleep */
  //epd.Sleep();
}

void loop() {
  Serial.print("> ");
  // Read command
  char line[80];
  if (read_line(line, sizeof(line)) < 0) {
    Serial.println("Error: line too long");
    return; // skip command processing and try again on next iteration of loop
  }
  if (strcmp(line, "") == 0) {
    // Empty line: no command
  }
  else if (strcmp(line, "PantallaKelmek") == 0) {
    epd.ClearFrame();
    epd.DisplayFrame(gImage_KELMEK);
    _pantallaState = 0;
  }
  else if (strcmp(line, "PantallaOperador") == 0) {
    _pantallaState = 2;
    _arrowPosition = 0;
    epd.ClearFrame();
    pantalla_2();
    epd.DisplayFrameQuick();
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowPosition == 0) && (_pantallaState == 2)) {
    _arrowPosition = 2;
    pantalla_2();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowPosition);
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowPosition == 1) && (_pantallaState == 2)) {
    _arrowPosition = 0;
    pantalla_2();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowPosition);
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowPosition == 2) && (_pantallaState == 2)) {
    _arrowPosition = 1;
    pantalla_2();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowPosition == 0) && (_pantallaState == 2)) {
    _arrowPosition = 1;
    pantalla_2();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowPosition == 1) && (_pantallaState == 2)) {
    _arrowPosition = 2;
    pantalla_2();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowPosition == 2) && (_pantallaState == 2)) {
    _arrowPosition = 0;
    pantalla_2();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowPosition);
  }
  else if (strcmp(line, "PantallaNumero") == 0) {
    _pantallaState = 3;
    epd.ClearFrame();
    pantalla_3();
    epd.DisplayFrameQuick();
  }
  else if ((strcmp(line, "0") >= 0) && (_pantallaState == 3)) {
    if (strcmp(line, "OK") == 0) {
      Serial.println("aqui estoy mamando gacho");
      _pantallaState = 4;
    }
    else {
      String espacios = "       ";
      String cadena = line;
      operadora = espacios + cadena;
      char bufferCharOperadora[80];
      getValue(operadora, ',', 0).toCharArray(bufferCharOperadora, 80);
      epd.ClearFrame();
      pantalla_4(bufferCharOperadora);
      epd.DisplayFrameQuick();
    }
    /*
      if (getValue(operadora, ',', 1) == "NOTOK") {
      getValue(operadora, ',', 0).toCharArray(bufferCharOperadora, 80);
      //_pantallaState = 4;
      epd.ClearFrame();
      pantalla_4(bufferCharOperadora);
      epd.DisplayFrameQuick();
      }
      else if (getValue(operadora, ',', 1) == "OK") {
      getValue(operadora, ',', 0).toCharArray(bufferCharOperadora, 80);
      _pantallaState = 4;
      epd.ClearFrame();
      pantalla_4(bufferCharOperadora);
      epd.DisplayFrameQuick();
      }*/
  }
  else if (strcmp(line, "PantallaError") == 0) {
    epd.ClearFrame();
    epd.DisplayFrame(gImage_error);
  }
  else if (strcmp(line, "PantallaProcesando") == 0) {
    epd.ClearFrame();
    epd.DisplayFrame(gImage_procesando);
  }
  else if (strcmp(line, "PantallaSaldo") == 0) {
    _pantallaState = 5;
    _arrowSaldoPosition = 0;
    epd.ClearFrame();
    pantalla_5();
    epd.DisplayFrameQuick();
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowSaldoPosition == 0) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 5;
    pantalla_5();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowSaldoPosition == 1) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 0;
    pantalla_5();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowSaldoPosition == 2) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 1;
    pantalla_5();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowSaldoPosition == 3) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 2;
    pantalla_5();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowSaldoPosition == 4) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 3;
    pantalla_5();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Up") == 0) && (_arrowSaldoPosition == 5) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 4;
    pantalla_5();
    Serial.print("Arrow Up, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowSaldoPosition == 0) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 1;
    pantalla_5();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowSaldoPosition == 1) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 2;
    pantalla_5();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowSaldoPosition == 2) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 3;
    pantalla_5();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowSaldoPosition == 3) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 4;
    pantalla_5();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowSaldoPosition == 4) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 5;
    pantalla_5();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if ((strcmp(line, "Down") == 0) && (_arrowSaldoPosition == 5) && (_pantallaState == 5)) {
    _arrowSaldoPosition = 0;
    pantalla_5();
    Serial.print("Arrow Down, position ");
    Serial.println(_arrowSaldoPosition);
  }
  else if (strcmp(line, "PantallaMostrarSaldo") == 0) {
    String espacios = "       ";
    String cadena = line;
    saldo = espacios + cadena;
    char bufferCharSaldo[80];
    getValue(saldo, ',', 0).toCharArray(bufferCharSaldo, 80);
    char bufferCharOperadora[80];
    getValue(operadora, ',', 0).toCharArray(bufferCharOperadora, 80);
    _pantallaState = 6;
    epd.ClearFrame();
    pantalla_6(bufferCharOperadora, bufferCharSaldo);
    epd.DisplayFrameQuick();
  }
  else if (strcmp(line, "PantallaDinero") == 0) {
    _pantallaState = 7;
    epd.ClearFrame();
    pantalla_7();
    epd.DisplayFrameQuick();
  }
  else if ((strcmp(line, "0") >= 0) && (_pantallaState == 7)) {
    if (strcmp(line, "OK") == 0) {
      Serial.println("ya tengo tu lana");
      _pantallaState = 8;
    }
    else {
      String espacios = "          ";
      String cadena = line;
      dinero = espacios + cadena;
      char bufferCharDinero[80];
      getValue(dinero, ',', 0).toCharArray(bufferCharDinero, 80);
      epd.ClearFrame();
      pantalla_8(bufferCharDinero);
      epd.DisplayFrameQuick();
    }
  }
}

  String getValue(String data, char separator, int index)
  {
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
      if (data.charAt(i) == separator || i == maxIndex) {
        found++;
        strIndex[0] = strIndex[1] + 1;
        strIndex[1] = (i == maxIndex) ? i + 1 : i;
      }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
  }

  void sendTextToScreen(const char* text, int x, int y, int color, Paint paint, Epd epd, sFONT font) {
    paint.Clear(!color);
    paint.DrawStringAt(0, 0, text, &font, color);
    epd.SetPartialWindow(paint.GetImage(), x, y, paint.GetWidth(), paint.GetHeight());
  }

  int read_line(char* buffer, int bufsize)
  {
    for (int index = 0; index < bufsize; index++) {
      // Espera a que tengamos actividad en el puerto serial
      while (Serial.available() == 0) {
      }

      char ch = Serial.read(); // lee el siguiente caracter
      Serial.print(ch); // hazle eco, es opcional

      if (ch == '\n') {
        buffer[index] = 0; // llegamos al final de la cadena
        return index; // regresamos la longitud de la cadena
      }

      buffer[index] = ch; // agrega el caracter al buffer
    }
    char ch;
    do {
      // espera a que tengamos actividad en el puerto serial
      while (Serial.available() == 0) {
      }
      ch = Serial.read(); // lee y descarta el caracter
      Serial.print(ch); // hazle eco
    } while (ch != '\n');

    buffer[0] = 0; // vacia el buffer
    return -1; // indicamos que la cadena de input fue muy larga
  }

  void pantalla_2() {
    sendTextToScreen("Seleccione su operador:", 0, 0, COLORED, paint, epd, Font24);
    switch (_arrowPosition) {
      case 0:
        sendTextToScreen(Operador[3], 50, 70, COLORED, paint, epd, Font24);
        sendTextToScreen(Operador[1], 50, 100, COLORED, paint, epd, Font24);
        sendTextToScreen(Operador[2], 50, 130, COLORED, paint, epd, Font24);
        break;
      case 1:
        sendTextToScreen(Operador[0], 50, 70, COLORED, paint, epd, Font24);
        sendTextToScreen(Operador[4], 50, 100, COLORED, paint, epd, Font24);
        sendTextToScreen(Operador[2], 50, 130, COLORED, paint, epd, Font24);
        break;
      case 2:
        sendTextToScreen(Operador[0], 50, 70, COLORED, paint, epd, Font24);
        sendTextToScreen(Operador[1], 50, 100, COLORED, paint, epd, Font24);
        sendTextToScreen(Operador[5], 50, 130, COLORED, paint, epd, Font24);
        break;
      default:
        sendTextToScreen(Operador[0], 50, 70, COLORED, paint, epd, Font24);
        sendTextToScreen(Operador[1], 50, 100, COLORED, paint, epd, Font24);
        sendTextToScreen(Operador[2], 50, 130, COLORED, paint, epd, Font24);
        break;
    }
    sendTextToScreen("    Presione  Enter    ", 0, 210, COLORED, paint, epd, Font24);
    sendTextToScreen("    Para  continuar    ", 0, 240, COLORED, paint, epd, Font24);
    epd.DisplayFrameQuick();
  }
  void pantalla_3() {
    sendTextToScreen(Operador[_arrowPosition], 80, 0, COLORED, paint, epd, Font24);
    sendTextToScreen("   Ingrese su numero   ", 0, 120, COLORED, paint, epd, Font24);
    sendTextToScreen("   y presione  Enter   ", 0, 150, COLORED, paint, epd, Font24);
    /*if (strcmp(numero) > 0) {
      }*/
  }
  void pantalla_4(const char* numero) {
    String operadoraSeleccionada;
    switch (_arrowPosition) {
      case 0:
        operadoraSeleccionada = "         AT&T          ";
        break;
      case 1:
        operadoraSeleccionada = "        TELCEL         ";
        break;
      case 2:
        operadoraSeleccionada = "       MOVISTAR        ";
        break;
      default:
        break;
    }
    char bufferChar[80];
    getValue(operadoraSeleccionada, ',', 0).toCharArray(bufferChar, 80);
    sendTextToScreen("       Su numero       ", 0, 0, COLORED, paint, epd, Font24);
    sendTextToScreen(numero, 0, 50, UNCOLORED, paint, epd, Font24);
    sendTextToScreen("      Su operador      ", 0, 100, COLORED, paint, epd, Font24);
    sendTextToScreen(bufferChar, 0, 150, UNCOLORED, paint, epd, Font24);
    sendTextToScreen("   Enter : Continuar   ", 0, 200, COLORED, paint, epd, Font24);
    sendTextToScreen("   Cancel : Regresar   ", 0, 250, COLORED, paint, epd, Font24);
  }
  void pantalla_5() {
    sendTextToScreen("  Seleccione el Monto:", 0, 0, COLORED, paint, epd, Font24);
    switch (_arrowSaldoPosition) {
      case 0:
        sendTextToScreen(Saldos[6], 50, 50, COLORED, paint, epd, Font24);//20
        sendTextToScreen(Saldos[1], 50, 70, COLORED, paint, epd, Font24);//30
        sendTextToScreen(Saldos[2], 50, 90, COLORED, paint, epd, Font24);//50
        sendTextToScreen(Saldos[3], 50, 110, COLORED, paint, epd, Font24);//100
        sendTextToScreen(Saldos[4], 50, 130, COLORED, paint, epd, Font24);//150
        sendTextToScreen(Saldos[5], 50, 150, COLORED, paint, epd, Font24);//200
        break;
      case 1:
        sendTextToScreen(Saldos[0], 50, 50, COLORED, paint, epd, Font24);//20
        sendTextToScreen(Saldos[7], 50, 70, COLORED, paint, epd, Font24);//30
        sendTextToScreen(Saldos[2], 50, 90, COLORED, paint, epd, Font24);//50
        sendTextToScreen(Saldos[3], 50, 110, COLORED, paint, epd, Font24);//100
        sendTextToScreen(Saldos[4], 50, 130, COLORED, paint, epd, Font24);//150
        sendTextToScreen(Saldos[5], 50, 150, COLORED, paint, epd, Font24);//200
        break;
      case 2:
        sendTextToScreen(Saldos[0], 50, 50, COLORED, paint, epd, Font24);//20
        sendTextToScreen(Saldos[1], 50, 70, COLORED, paint, epd, Font24);//30
        sendTextToScreen(Saldos[8], 50, 90, COLORED, paint, epd, Font24);//50
        sendTextToScreen(Saldos[3], 50, 110, COLORED, paint, epd, Font24);//100
        sendTextToScreen(Saldos[4], 50, 130, COLORED, paint, epd, Font24);//150
        sendTextToScreen(Saldos[5], 50, 150, COLORED, paint, epd, Font24);//200
        break;
      case 3:
        sendTextToScreen(Saldos[0], 50, 50, COLORED, paint, epd, Font24);//20
        sendTextToScreen(Saldos[1], 50, 70, COLORED, paint, epd, Font24);//30
        sendTextToScreen(Saldos[2], 50, 90, COLORED, paint, epd, Font24);//50
        sendTextToScreen(Saldos[9], 50, 110, COLORED, paint, epd, Font24);//100
        sendTextToScreen(Saldos[4], 50, 130, COLORED, paint, epd, Font24);//150
        sendTextToScreen(Saldos[5], 50, 150, COLORED, paint, epd, Font24);//200
        break;
      case 4:
        sendTextToScreen(Saldos[0], 50, 50, COLORED, paint, epd, Font24);//20
        sendTextToScreen(Saldos[1], 50, 70, COLORED, paint, epd, Font24);//30
        sendTextToScreen(Saldos[2], 50, 90, COLORED, paint, epd, Font24);//50
        sendTextToScreen(Saldos[3], 50, 110, COLORED, paint, epd, Font24);//100
        sendTextToScreen(Saldos[10], 50, 130, COLORED, paint, epd, Font24);//150
        sendTextToScreen(Saldos[5], 50, 150, COLORED, paint, epd, Font24);//200
        break;
      case 5:
        sendTextToScreen(Saldos[0], 50, 50, COLORED, paint, epd, Font24);//20
        sendTextToScreen(Saldos[1], 50, 70, COLORED, paint, epd, Font24);//30
        sendTextToScreen(Saldos[2], 50, 90, COLORED, paint, epd, Font24);//50
        sendTextToScreen(Saldos[3], 50, 110, COLORED, paint, epd, Font24);//100
        sendTextToScreen(Saldos[4], 50, 130, COLORED, paint, epd, Font24);//150
        sendTextToScreen(Saldos[11], 50, 150, COLORED, paint, epd, Font24);//200
        break;
      default:
        sendTextToScreen(Saldos[0], 50, 50, COLORED, paint, epd, Font24);//20
        sendTextToScreen(Saldos[1], 50, 70, COLORED, paint, epd, Font24);//30
        sendTextToScreen(Saldos[2], 50, 90, COLORED, paint, epd, Font24);//50
        sendTextToScreen(Saldos[3], 50, 110, COLORED, paint, epd, Font24);//100
        sendTextToScreen(Saldos[4], 50, 130, COLORED, paint, epd, Font24);//150
        sendTextToScreen(Saldos[5], 50, 150, COLORED, paint, epd, Font24);//200
        break;
    }
    sendTextToScreen("    Presione  Enter    ", 0, 210, COLORED, paint, epd, Font24);
    sendTextToScreen("    Para  continuar    ", 0, 240, COLORED, paint, epd, Font24);
    epd.DisplayFrameQuick();
  }

  void pantalla_6(const char* numero, const char* saldo) {
    String operadoraSeleccionada;
    String saldoSeleccionado;
    switch (_arrowPosition) {
      case 0:
        operadoraSeleccionada = "         AT&T          ";
        break;
      case 1:
        operadoraSeleccionada = "        TELCEL         ";
        break;
      case 2:
        operadoraSeleccionada = "       MOVISTAR        ";
        break;
      default:
        break;
    }
    switch (_arrowSaldoPosition) {
      case 0:
        saldoSeleccionado = "         $ 20          ";
        break;
      case 1:
        saldoSeleccionado = "         $ 30          ";
        break;
      case 2:
        saldoSeleccionado = "         $ 50          ";
        break;
      case 3:
        saldoSeleccionado = "        $  100         ";
        break;
      case 4:
        saldoSeleccionado = "        $  150         ";
        break;
      case 5:
        saldoSeleccionado = "        $  200         ";
        break;
      default:
        break;
    }
    char bufferCharOperadora[80];
    char bufferCharSaldo[80];
    getValue(operadoraSeleccionada, ',', 0).toCharArray(bufferCharOperadora, 80);
    getValue(saldoSeleccionado, ',', 0).toCharArray(bufferCharSaldo, 80);
    sendTextToScreen("       Su numero       ", 0, 0, COLORED, paint, epd, Font24);
    sendTextToScreen(numero, 0, 30, UNCOLORED, paint, epd, Font24);
    sendTextToScreen("      Su operador      ", 0, 60, COLORED, paint, epd, Font24);
    sendTextToScreen(bufferCharOperadora, 0, 90, UNCOLORED, paint, epd, Font24);
    sendTextToScreen("       Su  Monto       ", 0, 120, COLORED, paint, epd, Font24);
    sendTextToScreen(bufferCharSaldo, 0, 150, UNCOLORED, paint, epd, Font24);
    sendTextToScreen("   Enter : Continuar   ", 0, 200, COLORED, paint, epd, Font24);
    sendTextToScreen("   Cancel : Regresar   ", 0, 250, COLORED, paint, epd, Font24);
  }
  void pantalla_7() {
    sendTextToScreen(Saldos[_arrowSaldoPosition], 80, 0, COLORED, paint, epd, Font24);
    sendTextToScreen("   Por Favor inserte   ", 0, 120, COLORED, paint, epd, Font24);
    sendTextToScreen("    el monto exacto.   ", 0, 150, COLORED, paint, epd, Font24);
    sendTextToScreen("     La maquina no     ", 0, 180, COLORED, paint, epd, Font24);
    sendTextToScreen("    regresa  cambio    ", 0, 210, COLORED, paint, epd, Font24);
    /*if (strcmp(numero) > 0) {
      }*/
  }
  void pantalla_8(const char* numero) {
    String operadoraSeleccionada;
    String saldoSeleccionado;
    switch (_arrowSaldoPosition) {
      case 0:
        saldoSeleccionado = "         $ 20          ";
        break;
      case 1:
        saldoSeleccionado = "         $ 30          ";
        break;
      case 2:
        saldoSeleccionado = "         $ 50          ";
        break;
      case 3:
        saldoSeleccionado = "        $  100         ";
        break;
      case 4:
        saldoSeleccionado = "        $  150         ";
        break;
      case 5:
        saldoSeleccionado = "        $  200         ";
        break;
      default:
        break;
    }
    char bufferChar[80];
    getValue(saldoSeleccionado, ',', 0).toCharArray(bufferChar, 80);
    sendTextToScreen("   Monto Seleccionado  ", 0, 0, COLORED, paint, epd, Font24);
    sendTextToScreen(bufferChar, 0, 50, UNCOLORED, paint, epd, Font24);
    sendTextToScreen("    Monto  Insertado   ", 0, 100, COLORED, paint, epd, Font24);
    sendTextToScreen(numero, 0, 150, UNCOLORED, paint, epd, Font24);
    sendTextToScreen("   Enter : Continuar   ", 0, 200, COLORED, paint, epd, Font24);
    sendTextToScreen("   Cancel : Regresar   ", 0, 250, COLORED, paint, epd, Font24);
  }
