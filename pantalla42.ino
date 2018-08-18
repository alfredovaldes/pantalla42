#include <SPI.h>
#include "epd4in2.h"
#include "epdpaint.h"
#include "imagedata.h"
#define COLORED     0
#define UNCOLORED   1
const char* Operador[6] = {"     AT&T", "     TELCEL", "     MOVISTAR", " =>  AT&T", " =>  TELCEL", " =>  MOVISTAR"};
unsigned char image[1500];
Paint paint(image, 400, 28);
Epd epd;
int _pantallaState = 0;
int _arrowPosition = 0;
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
  else if (strcmp(line, "Pantalla1") == 0) {
    epd.ClearFrame();
    epd.DisplayFrame(gImage_KELMEK);
    _pantallaState = 0;
  }
  else if (strcmp(line, "Pantalla2") == 0) {
    _pantallaState = 2;
    _arrowPosition = 0;
    epd.ClearFrame();
    pantalla_2();
    epd.DisplayFrame();
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
  else if (strcmp(line, "Pantalla3") == 0) {
    _pantallaState = 3;
    epd.ClearFrame();
    pantalla_3();
    epd.DisplayFrame();
  }
  else if ((strcmp(line, "0") > 0) && (_pantallaState == 3)) {
    String myString;
    String espacios = "       ";
    String cadena = line;
    myString = espacios + cadena;
    char bufferChar[80];
    getValue(myString, ',', 0).toCharArray(bufferChar, 80);
    _pantallaState = 4;
    epd.ClearFrame();
    pantalla_4(bufferChar);
    epd.DisplayFrame();
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
  switch(_arrowPosition) {
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
/*else {
  String myString;
  myString=line;
  char bufferChar[80];
  getValue(myString, ',',0).toCharArray(bufferChar,80);
  sendTextToScreen(bufferChar,getValue(myString, ',',1).toInt(),getValue(myString, ',',2).toInt(), COLORED, paint, epd, Font16);
  epd.DisplayFrameQuick();
  Serial.print(line);
  }*/
