#include "http_head.h"
#include <stdio.h>
#include "string.h"
#include "webpage.h"
#include "STM32_usart.h"
#include "EN_students.h"
/***********************************
file��http_head.c
���ߣ������
ʱ�䣺2019/9/19-
���������������ѧǶ��ʽϵͳʵ����ʹ��
***********************************/
#define POST_REAL_DATA_STRING				"RealData"					//ʵʱ�����ϱ�
#define POST_API_URL						    "/api/RTU_DataInterface"	//�ӿڵ�ַ

// |      http����ʽ                |               ����
// |          GET 									  |    ͨ�����������������ȡĳ����Դ
// |          HEAD                    |    ������GET��������Ӧ����в�������Ӧ�壬ֻ����Э����Ϣ���ײ���ͨ�����ڲ�����Դ�Ƿ���ڻ����Ƿ��޸�
// |          POST                    |    �ͻ�����������ύ����(֧��HTML�ı�����)�����ܻᵼ���µ���Դ�Ľ�������������Դ���޸�
// |          PUT                     |    �ӿͻ�������������͵�����ȡ��ָ�����ĵ�������(ȫ��ȡ��)
// |          PATCH                   |    �ͻ�������������͵�����ȡ��ָ�����ĵ�������(����ȡ��)
// |          TRACE                   |    ���Կͻ��������������ԭʼ�����ģ�����"�ػ�"���
// |          OPTIONS                 |    �����������ȡ������֧�ֵĸ��ֹ��ܣ�����ѯ�ʷ�����֧��ʲô���͵�HTTP������һ���������ܲ���
// |          DELETE                  |    ���������ɾ��ָ������Դ

st_http_request  request;
unsigned char http_type;
unsigned char http_Buff[2048]={0};
char tx_buf[MAX_URI_SIZE]={0};
//POST�����httpͷ��Ϣ,
//����1�����ͷ�ʽ������2��API��ַ������3��������IP������4���������ݳ���  ����5��post�����µı���
static const char *const HTTP_HEAD = 	     
{
	"%s %s HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded; charset=UTF-8\r\nX-Requested-With: XMLHttpRequest\r\nHost: %s\r\nContent-Length: %d\r\nExpect: 100-continue\r\nConnection: close\r\n\r\n%s"
};

/************************************
������:http_Request_message
������HTTP������
���룺mode(����ģʽ):"GET","HEAD","POST","PUT","PATCH","TRACE","OPTIONS","DELETE",(����д�ڿ�ͷ��)
			API(API��ַ)
			IP(������IP��ַ��Ŀ�Ķ�)
			buff(http post�������ݰ�)
�����int   (���ر��ĳ���)
������http_Request_message("POST","/v1.0/device/15738/sensor/27051/datapoints","192.168.1.1","123");//
***********************************/
int http_Request_message(char* mode,char* API,char* IP,char* buff)   //HTTP������ѹ����http_Buff�����ֻҪͨ��tcpЭ�鷢�;���
{
	int i = strlen(buff);
	memset(http_Buff,0,sizeof(http_Buff));
	sprintf(http_Buff,HTTP_HEAD,mode,API,IP,i,buff);
	return strlen(http_Buff);
}
/************************************
������:make_http_response_head
������HTTP������Ӧͷ��
���룺buf (������Ӧͷ��������)
			type (�ļ�����)
			len	 (�������ݳ���)
�������
������make_http_response_head(http_Buff,request.URI,);//
***********************************/
void make_http_response_head(unsigned char * buf,char type,uint32_t len)
{
  char * head;
  char tmp[10];
  
  memset(buf,0x00,MAX_URI_SIZE); 
  //printf("make http response\r\n");
  //
  /* �ļ�����*/
  if 	    (type == PTYPE_HTML)  head = RES_HTMLHEAD_OK;
  else if (type == PTYPE_GIF)	  head = RES_GIFHEAD_OK;
  else if (type == PTYPE_TEXT)	head = RES_TEXTHEAD_OK;
  else if (type == PTYPE_JPEG)	head = RES_JPEGHEAD_OK;
  else if (type == PTYPE_FLASH)	head = RES_FLASHHEAD_OK;
  else if (type == PTYPE_MPEG)	head = RES_MPEGHEAD_OK;
  else if (type == PTYPE_PDF)	  head = RES_PDFHEAD_OK;

  sprintf(tmp,"%ld", len);	
  strcpy((char*)buf, head);   //����ͷ��
  strcat((char*)buf, tmp);		//���ر��ĳ���
  strcat((char*)buf, "\r\n\r\n"); //���ر��Ľ�����־
  //printf("%s\r\n", buf);
}
/************************************
������:http_Message_parsing
���������Ľ���
���룺s (������Ӧͷ��������)
			buff (�ļ�����)
			request	 (Ӧ����Ϣ�ṹ��ָ��)
�������
������http_Message_parsing(0,http_Buff,&request);//
***********************************/
int http_Message_parsing(SOCKET s,unsigned char * buff,st_http_request * request)//���Ľ���
{
	int size = 0,i = 0;
	char * nexttok = 0;
	size = Read_SOCK_Data_Buffer(s, buff);//���뱨������
	nexttok = strtok((char *)buff," ");   //�ֽ��ַ���Ϊһ���ַ�����
	if(!nexttok)
	{
		request->METHOD = METHOD_ERR;
		return 0;
	}
	if(!strcmp(nexttok,"GET") || !strcmp(nexttok,"get"))
	{
		request->METHOD = METHOD_GET;
		nexttok = strtok(NULL," ");//����URI
	}
	else if(!strcpy(nexttok,"POST") || !strcpy(nexttok,"post"))
	{
    nexttok = strtok(NULL," ");//����URI
    request->METHOD = METHOD_POST;
	}
	else if(!strcmp(nexttok, "HEAD") || !strcmp(nexttok,"head"))
	{
		request->METHOD = METHOD_HEAD;
    nexttok = strtok(NULL," ");//����URI
	}
	else if(!strcpy(nexttok,"PUT") || !strcpy(nexttok,"put"))
	{
		request->METHOD = METHOD_ERR;//����Ӧ����
	}
	else if(!strcpy(nexttok,"PATCH") || !strcpy(nexttok,"patch"))
	{
		request->METHOD = METHOD_ERR;//����Ӧ����
	}
	else if(!strcpy(nexttok,"TRACE") || !strcpy(nexttok,"trace"))
	{
		request->METHOD = METHOD_ERR;//����Ӧ����
	}
	else if(!strcpy(nexttok,"OPTIONS") || !strcpy(nexttok,"options"))
	{
		request->METHOD = METHOD_OPTIONS;//��Ӧ����
	}
	else if(!strcpy(nexttok,"DELETE") || !strcpy(nexttok,"delete"))
	{
		request->METHOD = METHOD_ERR;//����Ӧ����
	}
	else
	{
		request->METHOD = METHOD_ERR;//����
	}
	if(!nexttok)
  {
    request->METHOD = METHOD_ERR;//����
  			
    return 0;
  }
	strcpy(request->URI,nexttok);//����URI
	return size;
}
//find_http_uri_type(&http_type,request->URI);
void find_http_uri_type(unsigned char * type, char * buf) //��ѯҪ���͵��ı�����
{
  /* Decide type according to extention*/
  if 	    (strstr(buf, ".pl"))				                  *type = PTYPE_PL;
  else if (strstr(buf, ".html") || strstr(buf,".htm"))	*type = PTYPE_HTML;
  else if (strstr(buf, ".gif"))				                  *type = PTYPE_GIF;
  else if (strstr(buf, ".text") || strstr(buf,".txt"))	*type = PTYPE_TEXT;
  else if (strstr(buf, ".jpeg") || strstr(buf,".jpg"))	*type = PTYPE_JPEG;
  else if (strstr(buf, ".swf")) 			                  *type = PTYPE_FLASH;
  else if (strstr(buf, ".mpeg") || strstr(buf,".mpg"))	*type = PTYPE_MPEG;
  else if (strstr(buf, ".pdf")) 			                  *type = PTYPE_PDF;
  else if (strstr(buf, ".cgi")  || strstr(buf,".CGI"))	*type = PTYPE_CGI;
  else if (strstr(buf, ".js")   || strstr(buf,".JS"))	  *type = PTYPE_TEXT;	
  else if (strstr(buf, ".xml")  || strstr(buf,".XML"))	*type = PTYPE_HTML;
  else 							                                    *type = PTYPE_ERR;
}
/*****************************************
������:unsigned char get_http_information(st_http_request * request)  //
����������http get�����е�����
���룺request  (Ӧ����Ϣ�ṹ��ָ��)
�������
������unsigned char get_http_information(st_http_request * request)
*****************************************/
unsigned char get_http_information(st_http_request * request)//����get��uri��������
{
	char * nexttok = 0;
	if(!request->URI)return 0;
	nexttok = strtok((char *)request->URI,"?");//����  ��ǰΪURL
	
	nexttok = strtok(NULL," ");//��������
	if(!nexttok)//�޸�������
	{
		request->get_info = 0;//��ָ��ط�
		return 0;
	}
	request->get_info = nexttok;//get_info���get����ָ��
	*(request->get_info - 1) = '\0';
	return 1;
}
/************************************
������:void HTTP_POST_information(st_http_request * request)  // ����if(request.METHOD == METHOD_ERR)�ڲ���Ӧ���Ŀ������޸�
������http post�������ݽ���
���룺request   (Ӧ����Ϣ�ṹ��ָ��)
�������
������HTTP_feedback()
***********************************/
void HTTP_POST_information(st_http_request * request)
{
	char * nexttok = 0;
	nexttok = strtok((char *)request->URI,"\r\n\r\n");
	nexttok = strtok(NULL,"\0");
	printf("POST������������Ϊ:%s\n",nexttok);
	
}

char mimajiexi(st_http_request * request)
{
	char * nexttok = 0;
	int i;
	nexttok = strtok((char *)request->get_info,"username=");
	request->name = nexttok;
	nexttok = strtok(NULL,"&password=");
	request->Password = nexttok;
	*(request->name + 12) = '\0';//�˺ų��� 12
	printf("�˺ţ�%s\n���룺%s\n",request->name,request->Password);
	for(i=0;i<people_number;i++)
	{
		if(!strcmp(request->name,students[i].name)&&!strcmp(request->Password,students[i].Password))
		{
			return 1;
		}
	}
	return 0;
}
/************************************
������:void HTTP_feedback()  // ����if(request.METHOD == METHOD_ERR)�ڲ���Ӧ���Ŀ������޸�
������httpӦ����Ӧ
���룺��
�������
������HTTP_feedback()
***********************************/
void HTTP_feedback()
{
		int size = 0;
		size = http_Message_parsing(0,http_Buff,&request);//���벢��������
		find_http_uri_type(&request.TYPE,request.URI);//����������ļ�����
		printf("���յ��ı�������Ϊ:");
		USART_Sent_nbit(USART1,http_Buff,size);
		printf("������������Ϊ��%d\n",request.METHOD);
		printf("���������ļ�����Ϊ��%d\n",request.TYPE);
		if(request.METHOD == METHOD_ERR)//������
		{
			memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//��շ�������
			memcpy(Tx_Buffer,ERROR_REQUEST_PAGE,sizeof(ERROR_REQUEST_PAGE));//ѹ�������
			Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//���ͱ���
			printf("���󱨸棺400!\n");
			socket_close(0);//�Ͽ�����
		}
		else if(request.METHOD == METHOD_GET)//GET����
		{
			if(get_http_information(&request))//����get��uri��������
			{
				printf("get����Я������Ϊ��%s\n",request.get_info);//��ӡ��������
			}
			printf("urlΪ��%s\n",request.URI);
			if(!strcmp(request.URI,"/index.htm") || !strcmp(request.URI,"/") || !strcmp(request.URI,"/index.html"))//��ҳ��Ϣ
			{
				request.TYPE = PTYPE_HTML;
				memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//��շ�������
				make_http_response_head(Tx_Buffer,request.TYPE,strlen(INDEX_HTML));//������Ӧ����ͷ��
				strcat((char *)Tx_Buffer,INDEX_HTML);
				Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//���ͱ���
			}
			else if(!strcmp(request.URI,"/w5500.js"))//��̬��Ӧ
			{
				memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//��շ�������
				memset(tx_buf,0,MAX_URI_SIZE);//�������
				sprintf(tx_buf,IP_message_show,POST_VER,
				Phy_Addr[0],Phy_Addr[1],Phy_Addr[2],Phy_Addr[3],Phy_Addr[4],Phy_Addr[5],
				IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3],
				Gateway_IP[0],Gateway_IP[1],Gateway_IP[2],Gateway_IP[3],
				Sub_Mask[0],Sub_Mask[1],Sub_Mask[2],Sub_Mask[3]);
				sprintf(Tx_Buffer,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
				Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//���ͱ���
				printf("����js���ݣ�\n");
				socket_close(0);//�Ͽ�����
			}
			else if(!strcmp(request.URI,"/config.html"))//w5500������Ϣ
			{
				memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//��շ�������
				make_http_response_head(Tx_Buffer,request.TYPE,strlen(CONFIG_HTML));//������Ӧ����ͷ��
				strcat((char *)Tx_Buffer,CONFIG_HTML);
				Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//���ͱ���
				socket_close(0);//�Ͽ�����
			}
			else if(!strcmp(request.URI,"/open.html"))//����
			{
				if(mimajiexi(&request))
				{
					memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//��շ�������
					make_http_response_head(Tx_Buffer,request.TYPE,strlen(OPEN_HTML));//������Ӧ����ͷ��
					strcat((char *)Tx_Buffer,OPEN_HTML);
					Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//���ͱ���
				}
				else
				{
					memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//��շ�������
					make_http_response_head(Tx_Buffer,request.TYPE,strlen(INDEX_HTML));//������Ӧ����ͷ��
					strcat((char *)Tx_Buffer,INDEX_HTML);
					Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//���ͱ���
				}
				if(!strcmp(request.get_info,"1"))//������Ϣ
				{
					gpio_SetIO(DOOR_OPEN);
				}
			}
		}
		else if(request.METHOD == METHOD_POST)//POST����
		{
			memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//��շ�������
			HTTP_POST_information(&request);//����post�����ڲ�����
		}
		else if(request.METHOD == METHOD_OPTIONS)//OPTIONS����
		{
			memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//��շ�������
			
		}
}
