
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
void echo_task(void *pv);    //������

#define TASK1_TASK_PRIO  2//�������ȼ�
#define TASK1_STK_SIZE	128 //�����ջ��С
TaskHandle_t Task1Task_Handler;   //������
void task1_task(void *pvParameters); //������

//�������ȼ�
#define LIST_TASK_PRIO		3
//�����ջ��С	
#define LIST_STK_SIZE 		128  
//������
TaskHandle_t ListTask_Handler;
//������
void list_task(void *pvParameters);


//����һ�������õ��б��3���б���
List_t TestList;  //�������б�
ListItem_t ListItem1;  //�������б���1
ListItem_t ListItem2;
ListItem_t ListItem3;


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
	
	xTaskCreate((TaskFunction_t   )echo_task,
							(const char*      )"echo_task",
							(uint16_t         )ECHO_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t      )ECHO_TASK_PRIO,
							(TaskHandle_t*    )&echoTask_Handler);
							
	    xTaskCreate((TaskFunction_t )list_task,     
                (const char*    )"list_task",   
                (uint16_t       )LIST_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LIST_TASK_PRIO,
                (TaskHandle_t*  )&ListTask_Handler); 
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����						
	vTaskDelete(StartTask_Handler);   //ɾ����ʼ����
	taskEXIT_CRITICAL();
}



void echo_task(void *pv)    //������
{
	char c1;
	while(1)
	{
		//printf("%s", scanf(""));
		c1 = fgetc(stdin);
		//if(c1 == 'j') printf("k\r\n");
		switch(c1)
		{
			case '4':
				fputc('4',stdout);
			break;
			case '3':
				fputc('3',stdout);
			break;
			case '2':

			break;
			case '1':

			break;
		}
	}
}


//list������
void list_task(void *pvParameters)
{
	//��һ������ʼ���б���б���
	vListInitialise(&TestList);
	vListInitialiseItem(&ListItem1);
	vListInitialiseItem(&ListItem2);
	vListInitialiseItem(&ListItem3);
	
	ListItem1.xItemValue=40;			//ListItem1�б���ֵΪ40
	ListItem2.xItemValue=60;			//ListItem2�б���ֵΪ60
	ListItem3.xItemValue=50;			//ListItem3�б���ֵΪ50
	
	//�ڶ�������ӡ�б�������б���ĵ�ַ
	printf("/*******************�б���б����ַ*******************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList                          %#x					\r\n",(int)&TestList);
	printf("TestList->xListEnd.xItemValue     %#x					\r\n",(int)TestList.xListEnd.xItemValue);
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd                %#x					\r\n",(int)(&TestList.xListEnd));
	printf("ListItem1                         %#x					\r\n",(int)&ListItem1);
	printf("ListItem2                         %#x					\r\n",(int)&ListItem2);
	printf("ListItem3                         %#x					\r\n",(int)&ListItem3);
	printf("/************************����**************************/\r\n");
	printf("����KEY_i������!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'i') delay_ms(10);					//�ȴ�KEY_UP������
	fputc('i',stdout);
	//�����������б�TestList����б���ListItem1����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem1);		//�����б���ListItem1
	printf("/******************����б���ListItem1*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_j������!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'j') delay_ms(10);					//�ȴ�KEY_UP������
	fputc('j',stdout);
	//���Ĳ������б�TestList����б���ListItem2����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem2);	//�����б���ListItem2
	printf("/******************����б���ListItem2*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_k������!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'k') delay_ms(10);					//�ȴ�KEY_UP������
	fputc('k',stdout);
	//���岽�����б�TestList����б���ListItem3����ͨ�����ڴ�ӡ����
	//�б����г�Ա����pxNext��pxPrevious��ֵ��ͨ��������ֵ�۲��б�
	//�����б��е����������
	vListInsert(&TestList,&ListItem3);	//�����б���ListItem3
	printf("/******************����б���ListItem3*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_l������!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'l') delay_ms(10);					//�ȴ�KEY_UP������
	fputc('l',stdout);
	//��������ɾ��ListItem2����ͨ�����ڴ�ӡ�����б����г�Ա����pxNext��
	//pxPrevious��ֵ��ͨ��������ֵ�۲��б������б��е����������
	uxListRemove(&ListItem2);						//ɾ��ListItem2
	printf("/******************ɾ���б���ListItem2*****************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************����**************************/\r\n");
	printf("����KEY_m������!\r\n\r\n\r\n");
	while(fgetc(stdin) == 'm') delay_ms(10);					//�ȴ�KEY_UP������
	fputc('m',stdout);
	//���߲���ɾ��ListItem2����ͨ�����ڴ�ӡ�����б����г�Ա����pxNext��
	//pxPrevious��ֵ��ͨ��������ֵ�۲��б������б��е����������
	TestList.pxIndex=TestList.pxIndex->pxNext;			//pxIndex�����һ�����pxIndex�ͻ�ָ��ListItem1��
	vListInsertEnd(&TestList,&ListItem2);				//�б�ĩβ����б���ListItem2
	printf("/***************��ĩβ����б���ListItem2***************/\r\n");
	printf("��Ŀ                              ��ַ				    \r\n");
	printf("TestList->pxIndex                 %#x					\r\n",(int)TestList.pxIndex);
	printf("TestList->xListEnd->pxNext        %#x					\r\n",(int)(TestList.xListEnd.pxNext));
	printf("ListItem2->pxNext                 %#x					\r\n",(int)(ListItem2.pxNext));
	printf("ListItem1->pxNext                 %#x					\r\n",(int)(ListItem1.pxNext));
	printf("ListItem3->pxNext                 %#x					\r\n",(int)(ListItem3.pxNext));
	printf("/*******************ǰ�������ӷָ���********************/\r\n");
	printf("TestList->xListEnd->pxPrevious    %#x					\r\n",(int)(TestList.xListEnd.pxPrevious));
	printf("ListItem2->pxPrevious             %#x					\r\n",(int)(ListItem2.pxPrevious));
	printf("ListItem1->pxPrevious             %#x					\r\n",(int)(ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious             %#x					\r\n",(int)(ListItem3.pxPrevious));
	printf("/************************����**************************/\r\n\r\n\r\n");
	while(1)
	{
		//LED1=!LED1;
		printf("Hello!\r\n");
        vTaskDelay(1000);                           //��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}







