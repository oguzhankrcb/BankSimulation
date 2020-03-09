#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>


#define EFTMIN 3
#define EFTMAX 10
#define CREDITMIN 10
#define CREDITMAX 20
#define BILLPAYMIN 2
#define BILLPAYMAX 12
#define DRAWMONEYMIN 1
#define DRAWMONEYMAX 7

HANDLE Gise1 = 0;
HANDLE Gise2 = 0;
HANDLE Gise3 = 0;
HANDLE GiseControlHnd = 0;
HANDLE ClientCreatorHnd = 0;


int lastclientno = 0;
int second = 0;
int minute = 0;
int hour = 0;
int mostclientinonetime = 0;
int mostwaitedclient = 0;
int averagewaitingtime = 0;
int simulationtime = 0;
int simulation_onoff = 0;
int gise1clientcount = 0;
int gise1servetime = 0;
int gise2clientcount = 0;
int gise2servetime = 0;
int gise3clientcount = 0;
int gise3servetime = 0;

typedef struct Nodes{
	int clientno;
	char operation;
	int time;
	int waittime;
	struct Nodes * next;
}Node;


typedef struct Queues{
	int queuelen;
	Node * front;
	Node * tail;
}Queue;

typedef struct thargs{
	int time;
	char op;
	int clientno;
	int waitingtime;
}ThArgs;

Queue bankqueue;

int CreateOperationTime(int operation);
void InsertClientToQueue(Queue * pqueue, int time, char op);
void Gise1MakeJob(void* args);
void Gise2MakeJob(void* args);
void Gise3MakeJob(void* args);
void GiseControl();
void ClientTimer();
void ClientCreator();
int CreateOperation();


void InsertClientToQueue(Queue * pqueue, int time, char op)
{
	
	Node * newnode = (Node*)malloc(sizeof(Node));
	

	if (pqueue->queuelen == 0)
	{
		
		lastclientno++; 
		newnode->clientno = lastclientno;
		newnode->operation = op;
		newnode->time = time;
		newnode->waittime = 0;
		newnode->next = NULL;
		pqueue->front = newnode;
		pqueue->tail = newnode;
		pqueue->queuelen++;
		printf("Musteri%d Saat %02d:%02d:%02d'de bankaya geldi.\r\n", lastclientno, hour,minute,second);
		return;
	}
	lastclientno++;
	printf("Musteri%d Saat %02d:%02d:%02d'de bankaya geldi.\r\n", lastclientno, hour, minute, second);
	newnode->clientno = lastclientno;
	newnode->operation = op;
	newnode->time = time;
	newnode->waittime = 0;
	newnode->next = NULL;
	pqueue->tail->next = newnode;
	pqueue->tail = newnode;
	pqueue->queuelen++;
}



void GiseControl()
{
	while (1)
	{
		Sleep(1000);
		if (bankqueue.queuelen > mostclientinonetime)
			mostclientinonetime = bankqueue.queuelen;

		if (bankqueue.queuelen >= 1)
		{
			if (Gise1 == NULL)
			{
				Node* temp = bankqueue.front;
				bankqueue.front = bankqueue.front->next;
				bankqueue.queuelen--;
				ThArgs arg;
				arg.op = temp->operation;
				arg.time = temp->time;
				arg.clientno = temp->clientno;
				arg.waitingtime = temp->waittime;
				Gise1 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Gise1MakeJob, &arg, NULL, NULL);
				free(temp);
			}
			else if (Gise2 == NULL)
			{
				Node* temp = bankqueue.front;
				bankqueue.front = bankqueue.front->next;
				bankqueue.queuelen--;
				ThArgs arg;
				arg.op = temp->operation;
				arg.time = temp->time;
				arg.clientno = temp->clientno;
				arg.waitingtime = temp->waittime;
				Gise2 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Gise2MakeJob, &arg, NULL, NULL);
				free(temp);
			}
			else if (Gise3 == NULL)
			{
				Node* temp = bankqueue.front;
				bankqueue.front = bankqueue.front->next;
				bankqueue.queuelen--;
				ThArgs arg;
				arg.op = temp->operation;
				arg.time = temp->time;
				arg.clientno = temp->clientno;
				arg.waitingtime = temp->waittime;
				Gise3 = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Gise3MakeJob, &arg, NULL, NULL);
				free(temp);
			}

		}
		
	}
	
}

void ClientTimer()
{
	while (1)
	{
		Sleep(1000);
		if (bankqueue.queuelen >= 1)
		{
			Node * temp = bankqueue.front;
			int i = 0;
			while (temp != NULL)
			{
				temp->waittime++;
				if (temp->waittime > mostwaitedclient)
					mostwaitedclient = temp->waittime;
				temp = temp->next;
			}
		}
	}
}

void Gise1MakeJob(void* args)
{
	ThArgs arg = *(ThArgs*)args;
	int operationtime = arg.time * 60;
	averagewaitingtime += arg.waitingtime;
	char islem[15];
	if (arg.op == 'E')
		strcpy(islem, "EFT");
	else if (arg.op == 'C')
		strcpy(islem, "CREDIT");
	else if (arg.op == 'B')
		strcpy(islem, "BILL");
	else if (arg.op == 'D')
		strcpy(islem, "DRAW");
	printf("Musteri%d'e Gise1'de Saat %02d:%02d:%02d'de %s hizmeti verilmeye baslandi.\r\n", arg.clientno, hour, minute, second, islem);
	while (operationtime)
	{
		Sleep(1000);
		operationtime--;
		gise1servetime++;
	}
	printf("Musteri%d'in Gise1'de Saat %02d:%02d:%02d'de %s islemleri tamamlandi\r\n", arg.clientno, hour, minute, second, islem);


	gise1clientcount++;
	Gise1 = NULL;


}

void Gise2MakeJob(void* args)
{
	ThArgs arg = *(ThArgs*)args;
	int operationtime = arg.time * 60;
	averagewaitingtime += arg.waitingtime;
	char islem[15];
	if (arg.op == 'E')
		strcpy(islem, "EFT");
	else if (arg.op == 'C')
		strcpy(islem, "CREDIT");
	else if (arg.op == 'B')
		strcpy(islem, "BILL");
	else if (arg.op == 'D')
		strcpy(islem, "DRAW");
	printf("Musteri%d'e Gise2'de Saat %02d:%02d:%02d'de %s hizmeti verilmeye baslandi.\r\n", arg.clientno, hour, minute, second, islem);
	while (operationtime)
	{
		Sleep(1000);
		operationtime--;
		gise2servetime++;
	}
	printf("Musteri%d'in Gise2'de Saat %02d:%02d:%02d'de %s islemleri tamamlandi\r\n", arg.clientno, hour, minute, second, islem);
	gise2clientcount++;
	Gise2 = NULL;
}

void Gise3MakeJob(void* args)
{
	ThArgs arg = *(ThArgs*)args;
	int operationtime = arg.time * 60;
	averagewaitingtime += arg.waitingtime;
	char islem[15];
	if (arg.op == 'E')
		strcpy(islem, "EFT");
	else if (arg.op == 'C')
		strcpy(islem, "CREDIT");
	else if (arg.op == 'B')
		strcpy(islem, "BILL");
	else if (arg.op == 'D')
		strcpy(islem, "DRAW");
	printf("Musteri%d'e Gise3'de Saat %02d:%02d:%02d'de %s hizmeti verilmeye baslandi.\r\n", arg.clientno, hour, minute, second, islem);
	while (operationtime)
	{
		Sleep(1000);
		operationtime--;
		gise3servetime++;
	}
	printf("Musteri%d'in Gise3'de Saat %02d:%02d:%02d'de %s islemleri tamamlandi\r\n", arg.clientno, hour, minute, second, islem);
	gise3clientcount++;
	Gise3 = NULL;
}




int CreateOperationTime(int operation)
{

	switch (operation)
	{
	case 0:
		return (EFTMIN + rand() % (EFTMAX + 1 - EFTMIN));
		break;
	case 1:
		return (CREDITMIN + rand() % (CREDITMAX + 1 - CREDITMIN));
		break;
	case 2:
		return (BILLPAYMIN + rand() % (BILLPAYMAX + 1 - BILLPAYMIN));
		break;
	case 3:
		return (DRAWMONEYMIN + rand() % (DRAWMONEYMAX + 1 - DRAWMONEYMIN));
		break;
	default:
		printf("Gecersiz islem program sonlaniyor...");
		exit(0);
		break;
	}

	return 0;
}

int CreateOperation()
{
	return (rand() % 4);
}

void ClientCreator(){
	srand(time(NULL));
	while (1)
	{
		int RandomSleepTime = 1 + (rand() % (5 * 60));
		Sleep(RandomSleepTime * 1000);

		switch (CreateOperation())
		{
		case 0:
			InsertClientToQueue(&bankqueue, CreateOperationTime(0), 'E');
			break;
		case 1:
			InsertClientToQueue(&bankqueue, CreateOperationTime(1), 'C');
			break;
		case 2:
			InsertClientToQueue(&bankqueue, CreateOperationTime(2), 'B');
			break;
		case 3:
			InsertClientToQueue(&bankqueue, CreateOperationTime(3), 'D');
			break;
		default:
			printf("Belirlenmemis operasyon.");
			exit(0);
			break;
		}

	}

}

void WaitForFinishAndPrintResults()
{
	while (1)
	{
		Sleep(1000);
		if (simulation_onoff == 0 && Gise1 == NULL && Gise2 == NULL && Gise3 == NULL)
		{
			printf("Simulation has been ended :\r\n");
			printf("Most Waited Person Waited : %d Seconds In The Queue\r\n", mostwaitedclient);
			printf("Most Person In One Time : %d In The Queue\r\n", mostclientinonetime);
			printf("Average Time Of Waiting Persons : %d Seconds\r\n", averagewaitingtime / lastclientno);
			printf("Gise 1 Total Clients : %d\r\nGise 1 Average Serve Time : %d Seconds\r\n", gise1clientcount, gise1servetime/gise1clientcount);
			printf("Gise 2 Total Clients : %d\r\nGise 2 Average Serve Time : %d Seconds\r\n", gise2clientcount, gise2servetime/gise2clientcount);
			printf("Gise 3 Total Clients : %d\r\nGise 3 Average Serve Time : %d Seconds\r\n", gise3clientcount, gise3servetime /gise3clientcount);
			break;
		}
	}

	system("pause");
}

void Timer()
{
	while (1)
	{
		Sleep(1000);
		second++;
		if (simulationtime == minute + (hour * 60))
		{
			simulation_onoff = 0;
			if (ClientCreatorHnd != NULL)
				TerminateThread(ClientCreatorHnd, 0);
		}
			
		if (second == 60)
		{
			second = 0;
			minute++;
		}
		if (minute == 60)
		{
			minute = 0;
			hour++;
		}
	}

}



int main()
{
	simulation_onoff = 1;
	printf("How much time do you want to give for simulation(min) : ");
	scanf("%d", &simulationtime);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Timer, NULL, NULL, NULL);
	ClientCreatorHnd = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientCreator, NULL, NULL, NULL);
	GiseControlHnd = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GiseControl, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientTimer, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WaitForFinishAndPrintResults, NULL, NULL, NULL);
	while (1)
	{
		Sleep(1000);
	}
	return 0;
}
