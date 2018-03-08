
/*
 *  Ждёт от компьютера команду "команда [значения]"
 *  get amount:
 *    Считывает с датчика значение amount раз и отправляет каждое значение обратно на COM-порт
 *  
 */
#include <string.h>
const short TIMEOUT = 500; // пауза между измерениями.
const char ANALOGPINS[] = {3}; // пины для считывания значений.
const char VCCPINS[] = {8}; // пины для подачи питания.
const short MAXREQUESTSIZE = 6; // максимальный размер входящего запроса.
unsigned short moisture = 0; // считанное значение влажности от 0 до 1023.

char request[MAXREQUESTSIZE]; // входящий запрос.

const char SIZE = 2; // количество пинов, с которых считывются значения.

typedef struct answer {
  char pins[SIZE];
  int measurements[SIZE];
} answer;

const short NUMCOMMANDS = 1;
const char REQUESTLIST[NUMCOMMANDS][MAXREQUESTSIZE] = {"get"};
enum requestlist {GET};

int parseRequest(char request[]) {
  /*  Возвращает -1, если команда не найдена или номер команды в REQUESTLIST
  */
  for(int i = 0; i < NUMCOMMANDS; ++i){
    if (strcmp(REQUESTLIST[i], request) == 0) {
      return i;
    }
  }
  return -1;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(VCCPINS[0], OUTPUT);
  Serial.begin(9600);
  answer ans = {{'1', '2'}, {3, 4}};
}

void loop() {
  while (1)
    if (Serial.available() > 0) {
      // считывает sizeof(request) символов или символы до ' ', не вставляет '\0' и
      // оставляет нетронутой часть массива, в которую не производилась запись.
      request[Serial.readBytesUntil(' ', request, sizeof(request))] = '\0';
      switch (parseRequest(request)) {
        case GET:
          Serial.print("command GET:");
          int amount = Serial.parseInt();
          Serial.print(" ");
          Serial.println(amount);
          for (int i = 0; i < amount; ++i) {
            digitalWrite(VCCPINS[0], HIGH);
            moisture = analogRead(ANALOGPINS[0]);
            digitalWrite(VCCPINS[0], LOW);
            Serial.print(moisture);
            Serial.print('\n');
            delay(TIMEOUT);
          }
          break;
        
      }
    }
  
}

/* Заменить print на write */
