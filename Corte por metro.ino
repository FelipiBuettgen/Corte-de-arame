/*
Programa feito por Felipi Buettgen 10/02/2023.

materiais para fazer a maquina de arame por metro


* um arduino uno
* uma cnc shield
* um lcd 20x4
* 2 drivers a4988
* 2 motores de passo nema 17
* 1 rotary encoder com switch

///
conecte a shield no arduino e configure os drivers
os drivers no lugar do eixo X e Y
X eixo que leva o arame
Y eixo que corta o arame no final do ciclo


no eixo Z vocÊ vai conectar o Rotary Encoder
Vou fornecer uma foto pra vocês entender a ligação do encoder que é a unica coisa diferente
o resto é tranquilo


utilizei um conjunto de um alimentador de impressora 3D que é bem seguro pra transportar o arame


Link do extruzor
nesse kit não vai o motor
https://produto.mercadolivre.com.br/MLB-1681093746-extrusora-mk8-p-ender-engrenagem-simples-3-cr-10-cr-10s-_JM#is_advertising=true&position=1&search_layout=grid&type=pad&tracking_id=3aa07118-306e-46ca-80b7-3d4e801bd742&is_advertising=true&ad_domain=VQCATCORE_LST&ad_position=1&ad_click_id=MjNhN2M5ZTctNGJjYi00ZTVlLTk4NmEtMWYxMWQyMmFjMGQ0

*/




#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x26, 20, 4);
#define botao 9
#define pinP 3
#define pinP2 2
#define pinD 5
#define pinD2 6
#define clk 7
#define dt 4

int espera = 600;
int pclk;
int pdt;
int quant;
int cm;
int ml;
int col;
byte seta[8] = {
  B00100,
  B01110,
  B11111,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110
};


void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.createChar(1, seta);
  Serial.println("Olá Mundo");
  pinMode(pinP, OUTPUT);
  pinMode(pinP2, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinD2, OUTPUT);
  digitalWrite(pinD2, 1);
  //// Motor1
  pinMode(botao, INPUT_PULLUP);
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  lcd.backlight();
  lcd.clear();
  qtd();
}

void loop() {
}


void qtd() {
  int i = 0;
  int selec = 1;
  int ant;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Quantidade: 1");
  while (i != 1) {
    pclk = digitalRead(clk);
    if (digitalRead(dt) != pclk) {
      if (pclk != 1) {
        delay(1);
        selec++;
      } else {
        delay(1);
        selec--;
      }
      ant = pclk;
      if (selec < 0) selec = 1;
      lcd.setCursor(12, 0);
      lcd.print("      ");
      lcd.setCursor(12, 0);
      lcd.print(selec);
    }

    if (digitalRead(botao) == 0) {
      delay(500);
      quant = selec;
      i = 1;
    }
  }
  tamanho();
}

void tamanho() {
  int i = 0;
  int selec = 1;
  int ant;
  lcd.setCursor(0, 1);
  lcd.print("Centimetros: 1,");
  while (i != 1) {
    pclk = digitalRead(clk);
    if (pclk != ant) {
      if (digitalRead(dt) != pclk) {
        selec++;
      } else {
        selec--;
      }
      ant = pclk;
      if (selec < 0) selec = 0;
      lcd.setCursor(13, 1);
      lcd.print("            ");
      lcd.setCursor(13, 1);
      lcd.print(selec);
      lcd.print(",");
    }
    if (digitalRead(botao) == 0) {
      delay(500);
      cm = selec;
      if(cm < 999) col = 17;
      if(cm < 99) col = 16;
      if(cm < 9) col = 15;
      i = 1;
    }
  }
  mili();
}
void mili() {
  int i = 0;
  int selec = 0;
  int ant;

  while (i != 1) {
    pclk = digitalRead(clk);
    if (pclk != ant) {
      if (digitalRead(dt) != pclk) {

        selec++;
      } else {

        selec--;
      }
      ant = pclk;
      if (selec < 0) selec = 0;
      if (selec > 9) selec = 9;
      lcd.setCursor(col, 1);
      lcd.print("           ");
      lcd.setCursor(col, 1);
      lcd.print(selec);
      lcd.print("mm");
    }
    if (digitalRead(botao) == 0) {
      delay(500);
      ml = selec;
      i = 1;
    }
  }
  girar(quant, cm, ml);
}
void girar(int qt, int cm, int ml) {
  const int pulsos = ((cm * 10) + ml) * 48 / qt;
  Serial.println(pulsos);
  int ps = pulsos;
  int ant;
  bool lk;
  unsigned long temp = 0;
  unsigned long tempo = 0;
  int ant2;
  int quantidade = qt;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Quantidade: ");
  lcd.print(qt);
  lcd.setCursor(0, 1);
  lcd.print("Tamanho: ");
  lcd.print(cm);
  lcd.print(",");
  lcd.print(ml);
  lcd.print("cm");
  lcd.setCursor(2, 2);
  lcd.print("Play");
  lcd.setCursor(10, 2);
  lcd.print("Reset");

  while (true) {
    // tempo = millis();
    // while (digitalRead(botao) == 0) {
    //   temp = millis();
    //   Serial.print("TEMP: ");
    //   Serial.println(temp);
    //   Serial.print("TEMPO: ");
    //   Serial.println(tempo);
    // }
    // if (temp > tempo + 2000) {
    //   qtd();
    // }
    // if (temp > 0 && temp <= tempo + 1000) {
    pclk = digitalRead(clk);
    if (pclk != ant) {
      if (digitalRead(dt) != pclk) {
        lk = 0;
      } else {
        lk = 1;
      }
      ant = pclk;
    }
    if (lk == 1) {
      lcd.setCursor(12, 3);
      lcd.print("  ");
      lcd.setCursor(3, 3);
      lcd.write(1);
    }
    if (lk == 0) {
      lcd.setCursor(3, 3);
      lcd.print("  ");
      lcd.setCursor(12, 3);
      lcd.write(1);
    }
  if (digitalRead(botao) == 0) {
    if (lk == 0) {
      delay(200);
      qtd();
    } else {
      lcd.setCursor(0, 2);
      lcd.print("                   ");
      lcd.setCursor(0, 3);
      lcd.print("                   ");
      while (true) {
        for (quantidade; quantidade > 0; quantidade--) {
          lcd.setCursor(0, 2);
          lcd.print("Falta cortar:     ");
          lcd.setCursor(14, 2);
          lcd.print(quantidade - 1);
          while (ps > 0) {
            digitalWrite(pinP, 1);
            delayMicroseconds(espera);
            digitalWrite(pinP, 0);
            delayMicroseconds(espera);
            ps--;
          }
          ps = pulsos;
          for (int i = 200; i > 0; i--) {
            digitalWrite(pinP2, 1);
            delayMicroseconds(espera);
            digitalWrite(pinP2, 0);
            delayMicroseconds(espera);
          }
        }
        qtd();
      }
    }
  }
}
}
