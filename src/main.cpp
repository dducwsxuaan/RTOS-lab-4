#include "scheduler.h"
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



void TIMER_ISR(void *pvParameters){
  while(1){
    SCH_Update();
    vTaskDelay(100);
  }
}

#define LED_PIN 48
#define D3 6
#define D4 7
#define D5 8
#define D6 9

void red_green_on(){
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, LOW);
}

void red_yellow_on(){
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, LOW);
  digitalWrite(D6, HIGH);
}

void green_red_on(){
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
}

void yellow_red_on(){
  digitalWrite(D3, LOW);
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
}

void setup(){
  xTaskCreate(TIMER_ISR, "TIMER_ISR", 2048, NULL, 2, NULL);
    // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  SCH_Init();
  //SCH_Add_Task(task, 0, 100);
  SCH_Add_Task(red_green_on, 0, 100);
  SCH_Add_Task(red_yellow_on, 30, 100);
  SCH_Add_Task(green_red_on, 50, 100);
  SCH_Add_Task(yellow_red_on, 80, 100);

}

void loop() {
  SCH_Dispatch_Tasks();
}
