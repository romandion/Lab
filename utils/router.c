/*Routine.c    TC2.0 WinXp*/
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#define ROUTNUM 7

typedef struct
{
  int dis;
  int from;
}RoutNode;

RoutNode data[ROUTNUM][ROUTNUM];/*·�ɱ�*/

void InitData(FILE* pfile);/*�������ļ���ȡ���ݣ���ʼ��·�ɱ�*/
void OutputRoutData();/*������е�·�ɱ�*/
void Communication(int recv, int send);/*send����recv�㷢���Լ���·�ɱ�*/
void Exchange();/*���нڵ����һ�����ݽ���,����·�ɱ�*/

void main()
{
  int start,end;
  int i;
  FILE *pfile;
  pfile = fopen("data.txt", "r");
  if (pfile == NULL)
  {
    printf("data file failed to open, press any key to quit.\n");
    getch();
    return;
  }
  else
    InitData(pfile);
  fclose(pfile);
  printf("\nInitial State:\n");
  OutputRoutData();
  for (i=0; i<ROUTNUM; i++)
  {
   Exchange();
  }
  printf("\nState Exchanged:\n");
  OutputRoutData();

  printf("input the start node(%d-%d) : ", 0, ROUTNUM-1);
  scanf("%d", &start);
  printf("input the end node(%d-%d) : ", 0, ROUTNUM-1);
  scanf("%d", &end);
  if (start==end || start<0 || start>6 || end<0 || end>6)
  {
    printf("\nwrong input, press any key to quit.\n");
    getch();
    return;
  }
  else
  {
   int cur = start;
   int total = 0;
   if (data[start][end].dis < 0)
   {
    printf("No routine find!\n");
    getch();
    return;
   }

   printf("%c->", cur+65);
   while (data[cur][end].from >= 0)
   {
    total += data[cur][data[cur][end].from].dis;
    printf("%c->",data[cur][end].from+65);
    cur = data[cur][end].from;
   }
   total += data[cur][end].dis;
   printf("%c\ntotal distance=%d", end+65, total);
   getch();
   return;
  }
}

void InitData(FILE* pfile)
{
  char num[10];
  int i=0;
  char c;
  int m,n;
  fseek(pfile, 0, 0);
  for (m=0; !feof(pfile)&&m<7; m++)
  {
    for (n=0; !feof(pfile)&&n<7; n++)
    {
      while (!feof(pfile))
      {
        c = fgetc(pfile);
        if (c==',')/*����һ������*/
        {
          num[i] = '\0';
    data[m][n].dis = atoi(num);
    data[m][n].from = -1;
          i = 0;
          break;
        }
        else if ((c>='0' && c<='9') || c=='-')/*����������ֻ����*/
        {
          num[i++] = c;
        }
      }
    }
  }
}

void OutputRoutData()
{
 int i,j;
 printf("   ");
 for(i=0; i<ROUTNUM; i++)
 {
  printf("<  %d   >", i);
 }
 printf("\n");
 for (i=0; i<ROUTNUM; i++)
 {
  printf("%d  ", i);
  for (j=0; j<ROUTNUM; j++)
  {
   if (data[i][j].dis < 0)/*�����·��*/
    printf("-    ");
   else
    printf("%-5d", data[i][j].dis);
   if (data[i][j].from < 0)/*���δ���������ڵ�*/
    printf("-  ");
   else
    printf("%c  ", data[i][j].from+65);
  }
  printf("\n");
 }
}

void Communication(int recv, int send)
{
 int i;
 for (i=0; i<ROUTNUM; i++)
 {
  if (data[send][i].dis > 0)/*���send�ڵ㵽i�Žڵ���·��*/
  {
   if (data[recv][i].dis < 0)/*���recv��i�Žڵ���·��*/
   {
    data[recv][i].dis = data[send][i].dis + data[recv][send].dis;
    data[recv][i].from = send;
   }
   else if (data[recv][i].dis > data[send][i].dis + data[recv][send].dis)/*�������·������·��Զ*/
   {
    data[recv][i].dis = data[send][i].dis + data[recv][send].dis;
    data[recv][i].from = send;
   }
  }
 }
}

void Exchange()
{
 int i,j;
 for (i=0; i<ROUTNUM; i++)
 {
  for (j=0; j<ROUTNUM; j++)
  {
   if (data[i][j].dis > 0)/*��������ڵ�֮����·��*/
   {
    Communication(j,i);/*��i�Žڵ��·�ɱ��͸�j�Žڵ�*/
   }
  }
 }
}

/*data.txt*/
 0, 2,-1,-1, 8,-1, 5,
 2, 0,-1, 5,-1,-1,-1,
-1,-1, 0,-1,-1, 9,-1,
-1, 5,-1, 0,-1,-1,-1,
 8,-1,-1,-1, 0,-1, 7,
-1,-1, 9,-1,-1, 0, 3,
 5,-1,-1,-1, 7, 3, 0,
