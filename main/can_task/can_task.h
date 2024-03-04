#ifndef CAN_TASK_H
#define CAN_TASK_H
#define TX_GPIO_NUM 1
#define RX_GPIO_NUM 3



void can_task_init(void);   // Initialize CAN
void can_receive_task(void *pvParameter);   // CAN task

#endif // CAN_TASK_H
