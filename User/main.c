
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

#define START_TASK_PRIO		1		     //�������ȼ�
#define START_STK_SIZE 		256				//�����ջ��С
TaskHandle_t StartTask_Handler;			//������
void start_task(void *pvParameters);//������


#define ECHO_TASK_PRIO 2   //�������ȼ�
#define ECHO_STK_SIZE 256	//�����ջ
TaskHandle_t echoTask_Handler;       //������
void echo_task(void *pvParameters);    //������

#define TASK1_TASK_PRIO  2//�������ȼ�
#define TASK1_STK_SIZE	256 //�����ջ��С
TaskHandle_t Task1Task_Handler;   //������
void task1_task(void *pvParameters); //������

//������Ϣ���е�����
#define KEYMSG_Q_NUM 	1   //������Ϣ���е����� 
#define MESSAGE_Q_NUM	4	//�������ݵ���Ϣ���е�����
QueueHandle_t Key_Queue;   //������Ϣ���о��
QueueHandle_t Message_Queue; //��Ϣ���о��
void check_msg_queue(void)
{
}

int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�ж����ȼ����� ����4
	
	USART1_init(115200);
    delay_init(); 
    led_init();
	xTaskCreate((TaskFunction_t )start_task,    //������
							 (const char*   )"start_task",  //��������
							 (uint16_t      )START_STK_SIZE, //�����ջ��С
							 (void*         )NULL, 						//������������Ĳ���
							 (UBaseType_t   )START_TASK_PRIO, //�������ȼ�
							 (TaskHandle_t* )&StartTask_Handler); //������
	
    vTaskStartScheduler();        
    return 0;
	
}

void start_task(void *pvParameters)
//��ʼ����������
{	
	taskENTER_CRITICAL();           //�����ٽ���

	//������ϢKey_Queue
	Key_Queue = xQueueCreate(KEYMSG_Q_NUM,sizeof(u8));
	//������ϢMessage_Queue,��������Ǵ��ڽ��ջ���������
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
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����						
	vTaskDelete(StartTask_Handler);   //ɾ����ʼ����
	taskEXIT_CRITICAL();
}



void echo_task(void *pv)    //������
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
	//while(fgetc(stdin) == 'i') delay_ms(10);					//�ȴ�KEY_UP������
	//fputc('i',stdout);
	//printf("Hello!\r\n");
	while(1)
	{
		//LED1=!LED1;
		
        //vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
       // printf("i = %d, Key_Queue = 0\r\n", Key_Queue);
		if(Key_Queue !=0)
		{
			if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))
			{
				//printf("�������");
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
					printf("�������:3");
				break;
				case '4':
					printf("�������");
					fputc(i,stdout);
				break;
				}
			}
			vTaskDelay(500); 
			printf("key = %c\r\n", key);
		}
		
	}
}







