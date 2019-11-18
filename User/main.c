
#include "stm32f10x.h"
#include "misc.h"
#include "led.h"
#include "usart.h"
#include "stdio.h"
//#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "delay.h"
#include "stm32f10x_tim.h"

#define START_TASK_PRIO		1		     //任务优先级
#define START_STK_SIZE 		256				//任务堆栈大小
TaskHandle_t StartTask_Handler;			//任务句柄
void start_task(void *pvParameters);//任务函数


#define ECHO_TASK_PRIO 2   //任务优先级
#define ECHO_STK_SIZE 256	//任务堆栈
TaskHandle_t echoTask_Handler;       //任务句柄
void echo_task(void *pvParameters);    //任务函数

#define TASK1_TASK_PRIO  2//任务优先级
#define TASK1_STK_SIZE	256 //任务堆栈大小
TaskHandle_t Task1Task_Handler;   //任务句柄
void task1_task(void *pvParameters); //任务函数

//按键消息队列的数量
#define KEYMSG_Q_NUM 	1   //按键消息队列的数量 
#define MESSAGE_Q_NUM	4	//发送数据的消息队列的数量
QueueHandle_t Key_Queue;   //按键消息队列句柄
QueueHandle_t Message_Queue; //消息队列句柄
void check_msg_queue(void)
{
}

int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //中断优先级分组 分组4
	
	USART1_init(115200);
    delay_init(); 
    led_init();
	xTaskCreate((TaskFunction_t )start_task,    //任务函数
							 (const char*   )"start_task",  //任务名称
							 (uint16_t      )START_STK_SIZE, //任务堆栈大小
							 (void*         )NULL, 						//传达给任务函数的参数
							 (UBaseType_t   )START_TASK_PRIO, //任务优先级
							 (TaskHandle_t* )&StartTask_Handler); //任务句柄
	
    vTaskStartScheduler();        
    return 0;
	
}

void start_task(void *pvParameters)
//开始任务任务函数
{	
	taskENTER_CRITICAL();           //进入临界区

	//创建消息Key_Queue
	Key_Queue = xQueueCreate(KEYMSG_Q_NUM,sizeof(u8));
	//创建消息Message_Queue,队列项长度是串口接收缓冲区长度
	Message_Queue = xQueueCreate(MESSAGE_Q_NUM, USART_REC_LEN);
	xTaskCreate((TaskFunction_t   )echo_task,
							(const char*      )"echo_task",
							(uint16_t         )ECHO_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t      )ECHO_TASK_PRIO,
							(TaskHandle_t*    )&echoTask_Handler);
							
	    xTaskCreate((TaskFunction_t )task1_task,     
                (const char*    )"task1_task",   
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_TASK_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler); 
    vTaskDelete(StartTask_Handler); //删除开始任务						
	vTaskDelete(StartTask_Handler);   //删除开始任务
	taskEXIT_CRITICAL();
}



void echo_task(void *pv)    //任务函数
{
	u8 key, i=0;
	BaseType_t err;
	char c1;
	while(1)
	{
		//printf("%s", scanf(""));
		c1 = fgetc(stdin);
		//if(c1 == 'j') printf("k\r\n");
		switch(c1)
		{
			case '4':
				//fputc('4',stdout);
				key = c1;
				xQueueSend(Key_Queue, &key, 10);
			break;
			case '3':
				fputc('3',stdout);
				key = c1;
				xQueueSend(Key_Queue, &key, 10);
			break;
			case '2':
				fputc('2',stdout);
			break;
			case '1':
				fputc('1',stdout);
			break;
		}
	}
}


//
void task1_task(void *pvParameters)
{
	u8 num, key, i;
	printf("Hello3!\r\n");
	//while(fgetc(stdin) == 'i') delay_ms(10);					//等待KEY_UP键按下
	//fputc('i',stdout);
	//printf("Hello!\r\n");
	while(1)
	{
		//LED1=!LED1;
		
        //vTaskDelay(1000);                           //延时1s，也就是1000个时钟节拍	
       // printf("i = %d, Key_Queue = 0\r\n", Key_Queue);
		if(Key_Queue !=0)
		{
			if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))
			{
				//printf("输出队列");
				i = key;
				switch(key)
				{
				case '1':
					fputc(i,stdout);
				break;
				case '2':
					fputc(i,stdout);
				break;
				case '3':
					//fputc(i,stdout);
					printf("队列输出:3");
				break;
				case '4':
					printf("输出队列");
					fputc(i,stdout);
				break;
				}
			}
			vTaskDelay(500); 
			printf("key = %c\r\n", key);
		}
		
	}
}







