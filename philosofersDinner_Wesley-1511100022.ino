/* Federal University of Fronteira Sul - 2018
  Wesley Burlani

  Implementation of philosofer's dinner on FreeRTOS 
  using only queue structure to control parallel flow

  Details: this implementation uses 5 queues of 1 element
  to control 5 forks, where each queue represents 1 fork

  each philosofer receiver two fork(queue) handlers. When he wakes up
  he trie to get your left side fork until he can. Qhen he gets left side fork
  he tries to get right side fork until"xDelayTime" and if fail, retrive left 
  side fork to table, else, eats and retrieves two forks to table.
*/

#include "FreeRTOS_AVR.h"
#include "basic_io_avr.h"

QueueHandle_t xForks[5];
String busy = String("busy");

typedef struct param{
  const char* id;
  QueueHandle_t* ptrLeftSideForkHandler;
  QueueHandle_t* ptrRightSideForkHandler;
}params;

void philosopher(void *pvParameters);

void setup() {

  __malloc_heap_end = (char*)RAMEND;
  Serial.begin(9600);

  params parameters[] = {
    { "1", &xForks[1], &xForks[0] },
    { "2", &xForks[0], &xForks[1] },
    { "3", &xForks[1], &xForks[2] },
    { "4", &xForks[2], &xForks[3] },
    { "5", &xForks[3], &xForks[4] }
  };

  BaseType_t xStatus;

  for(int i=0; i< 5; i++){

    xForks[i] = xQueueCreate( 1, sizeof( String * ) );
    xStatus = xQueueSendToBack( xForks[i], &busy, 0 );

    if( xStatus != pdPASS ){
      
      String strSendToQueueErr = String("Could not send to the queue ");
      strSendToQueueErr.concat(String(i));
      strSendToQueueErr.concat("\n");
      vPrintString( strSendToQueueErr.c_str() );
    }

    xTaskCreate( philosopher, String(i).c_str(), 125, &parameters[i], 1, NULL );
  }

  vTaskStartScheduler();

  for( ;; );
}

void loop() {
   
}

void philosopher(void *pvParameters){

  //set the philosofer actions delay time
  TickType_t xDelayTime = pdMS_TO_TICKS(100);

  //receive tasks parameters
  params* parameters = (params*)pvParameters;
  const char* id = parameters->id;
  QueueHandle_t* ptrLeftSideForkHandler = parameters->ptrLeftSideForkHandler;
  QueueHandle_t* ptrRightSideForkHandler = parameters->ptrRightSideForkHandler;
  
  //build message to thinking state
  String strThinkingMessage = String(id);
  strThinkingMessage.concat(" is thinking\n");
  const char* thinkingMessage = strThinkingMessage.c_str();

  //build message to woke up state
  String strWokeUpMessage = String(id);
  strWokeUpMessage.concat(" woke up\n");
  const char* wokeUpMessage = strWokeUpMessage.c_str();

  //build message to eating state
  String strEatingMessage = String(id);
  strEatingMessage.concat(" is eating\n");
  const char* eatingMessage = strEatingMessage.c_str();

  portBASE_TYPE xStatus;
  String* ptrLeftFork;
  String* ptrRigthFork;
  
  for( ;; )
  {
    // philosofer is thinking..
    vPrintString( thinkingMessage );
    //time to wait when pihlosofer is thinking
    vTaskDelay( xDelayTime );
    //philosofer woke up
    vPrintString( wokeUpMessage );
    //philosofer tries to get first fork 
    xStatus = xQueueReceive( *ptrLeftSideForkHandler, &ptrLeftFork, portMAX_DELAY );
    //verify if philosofer gets the first fork
    if( xStatus == pdPASS )
    {
      // philosofer tries to get second fork
      xStatus =  xQueueReceive( *ptrRightSideForkHandler, &ptrRigthFork, xDelayTime);
      // if philosofer can't to get second fork, he retuns first fork to table
      if(xStatus == errQUEUE_EMPTY)
        xQueueSendToBack( *ptrLeftSideForkHandler, &ptrLeftFork, 0 );
      else{
        // he gets second fork and start eating
        vPrintString( eatingMessage );
        // time that philosofer is eating
        vTaskDelay( xDelayTime );
        // philosofer end eating state and return two forks to table.
        xQueueSendToBack( *ptrLeftSideForkHandler, &ptrLeftFork, 0 );
        xQueueSendToBack( *ptrRightSideForkHandler, &ptrRigthFork, 0 );
      } 
    }
  }
}
