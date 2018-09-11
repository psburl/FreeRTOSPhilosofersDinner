# FreeRTOSPhilosofersDinner

Implementation of dining philosophers problem solve using only restricts [FreeRTOS](https://www.freertos.org/a00104.html) features like taks and queues on [Arduino Uno](https://store.arduino.cc/usa/arduino-uno-rev3) enviroment. 
### Details: 
This implementation uses 5 queues of 1 element to control 5 forks, where each queue represents 1 fork.<br/>
Each philosofer receiver two fork(queue) handlers and when he wakes up he tries to get your left side fork until he can. When he gets left side fork he tries to get right side fork until "xDelayTime" and if fail, retrive left side fork to table, else, eats and retrieves two forks to table.

## Dependencies: 
* [FreeRTOS v8.2.3](https://moodle-academico.uffs.edu.br/mod/resource/view.php?id=141223)

## Configurable Params 

* <b>xDelayTime:</b> It's declared on "philosofersDinner_Wesley-1511100022.ino" file at "philosofer" function. Represents the time philosofer stays thiking, eating or waiting for right side fork. 
