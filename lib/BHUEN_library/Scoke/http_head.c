#include "http_head.h"
#include <stdio.h>
#include "string.h"
#include "webpage.h"
#include "STM32_usart.h"
#include "EN_students.h"
/***********************************
file：http_head.c
作者：廖凌峰
时间：2019/9/19-
本代码仅供北华大学嵌入式系统实验室使用
***********************************/
#define POST_REAL_DATA_STRING				"RealData"					//实时数据上报
#define POST_API_URL						    "/api/RTU_DataInterface"	//接口地址

// |      http请求方式                |               描述
// |          GET 									  |    通常用于请求服务器获取某个资源
// |          HEAD                    |    类似于GET，但是响应结果中不包含响应体，只包含协议信息和首部，通常用于测试资源是否存在或者是否被修改
// |          POST                    |    客户端向服务器提交数据(支持HTML的表单数据)，可能会导致新的资源的建立或者已有资源的修改
// |          PUT                     |    从客户端向服务器传送的数据取代指定的文档的内容(全部取代)
// |          PATCH                   |    客户端向服务器传送的数据取代指定的文档的内容(部分取代)
// |          TRACE                   |    回显客户端请求服务器的原始请求报文，用于"回环"诊断
// |          OPTIONS                 |    请求服务器获取服务器支持的各种功能，可以询问服务器支持什么类型的HTTP方法，一般用于性能测试
// |          DELETE                  |    请求服务器删除指定的资源

st_http_request  request;
unsigned char http_type;
unsigned char http_Buff[2048]={0};
char tx_buf[MAX_URI_SIZE]={0};
//POST请求的http头信息,
//参数1：发送方式，参数2：API地址，参数3：服务器IP，参数4：后续数据长度  参数5：post请求下的报文
static const char *const HTTP_HEAD = 	     
{
	"%s %s HTTP/1.1\r\nContent-Type: application/x-www-form-urlencoded; charset=UTF-8\r\nX-Requested-With: XMLHttpRequest\r\nHost: %s\r\nContent-Length: %d\r\nExpect: 100-continue\r\nConnection: close\r\n\r\n%s"
};

/************************************
函数名:http_Request_message
描述：HTTP请求报文
输入：mode(发送模式):"GET","HEAD","POST","PUT","PATCH","TRACE","OPTIONS","DELETE",(详情写在开头了)
			API(API地址)
			IP(服务器IP地址，目的端)
			buff(http post请求数据包)
输出：int   (返回报文长度)
样例：http_Request_message("POST","/v1.0/device/15738/sensor/27051/datapoints","192.168.1.1","123");//
***********************************/
int http_Request_message(char* mode,char* API,char* IP,char* buff)   //HTTP请求报文压缩到http_Buff数组里，只要通过tcp协议发送就行
{
	int i = strlen(buff);
	memset(http_Buff,0,sizeof(http_Buff));
	sprintf(http_Buff,HTTP_HEAD,mode,API,IP,i,buff);
	return strlen(http_Buff);
}
/************************************
函数名:make_http_response_head
描述：HTTP报文响应头部
输入：buf (报文响应头部缓存区)
			type (文件类型)
			len	 (报文数据长度)
输出：无
样例：make_http_response_head(http_Buff,request.URI,);//
***********************************/
void make_http_response_head(unsigned char * buf,char type,uint32_t len)
{
  char * head;
  char tmp[10];
  
  memset(buf,0x00,MAX_URI_SIZE); 
  //printf("make http response\r\n");
  //
  /* 文件类型*/
  if 	    (type == PTYPE_HTML)  head = RES_HTMLHEAD_OK;
  else if (type == PTYPE_GIF)	  head = RES_GIFHEAD_OK;
  else if (type == PTYPE_TEXT)	head = RES_TEXTHEAD_OK;
  else if (type == PTYPE_JPEG)	head = RES_JPEGHEAD_OK;
  else if (type == PTYPE_FLASH)	head = RES_FLASHHEAD_OK;
  else if (type == PTYPE_MPEG)	head = RES_MPEGHEAD_OK;
  else if (type == PTYPE_PDF)	  head = RES_PDFHEAD_OK;

  sprintf(tmp,"%ld", len);	
  strcpy((char*)buf, head);   //加载头部
  strcat((char*)buf, tmp);		//加载报文长度
  strcat((char*)buf, "\r\n\r\n"); //加载报文结束标志
  //printf("%s\r\n", buf);
}
/************************************
函数名:http_Message_parsing
描述：报文解析
输入：s (报文响应头部缓存区)
			buff (文件类型)
			request	 (应答信息结构体指针)
输出：无
样例：http_Message_parsing(0,http_Buff,&request);//
***********************************/
int http_Message_parsing(SOCKET s,unsigned char * buff,st_http_request * request)//报文解析
{
	int size = 0,i = 0;
	char * nexttok = 0;
	size = Read_SOCK_Data_Buffer(s, buff);//读入报文数据
	nexttok = strtok((char *)buff," ");   //分解字符串为一组字符串。
	if(!nexttok)
	{
		request->METHOD = METHOD_ERR;
		return 0;
	}
	if(!strcmp(nexttok,"GET") || !strcmp(nexttok,"get"))
	{
		request->METHOD = METHOD_GET;
		nexttok = strtok(NULL," ");//查找URI
	}
	else if(!strcpy(nexttok,"POST") || !strcpy(nexttok,"post"))
	{
    nexttok = strtok(NULL," ");//查找URI
    request->METHOD = METHOD_POST;
	}
	else if(!strcmp(nexttok, "HEAD") || !strcmp(nexttok,"head"))
	{
		request->METHOD = METHOD_HEAD;
    nexttok = strtok(NULL," ");//查找URI
	}
	else if(!strcpy(nexttok,"PUT") || !strcpy(nexttok,"put"))
	{
		request->METHOD = METHOD_ERR;//不响应此类
	}
	else if(!strcpy(nexttok,"PATCH") || !strcpy(nexttok,"patch"))
	{
		request->METHOD = METHOD_ERR;//不响应此类
	}
	else if(!strcpy(nexttok,"TRACE") || !strcpy(nexttok,"trace"))
	{
		request->METHOD = METHOD_ERR;//不响应此类
	}
	else if(!strcpy(nexttok,"OPTIONS") || !strcpy(nexttok,"options"))
	{
		request->METHOD = METHOD_OPTIONS;//响应此类
	}
	else if(!strcpy(nexttok,"DELETE") || !strcpy(nexttok,"delete"))
	{
		request->METHOD = METHOD_ERR;//不响应此类
	}
	else
	{
		request->METHOD = METHOD_ERR;//报错
	}
	if(!nexttok)
  {
    request->METHOD = METHOD_ERR;//报错
  			
    return 0;
  }
	strcpy(request->URI,nexttok);//拷贝URI
	return size;
}
//find_http_uri_type(&http_type,request->URI);
void find_http_uri_type(unsigned char * type, char * buf) //查询要发送的文本类型
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
函数名:unsigned char get_http_information(st_http_request * request)  //
描述：解析http get请求中的数据
输入：request  (应答信息结构体指针)
输出：无
样例：unsigned char get_http_information(st_http_request * request)
*****************************************/
unsigned char get_http_information(st_http_request * request)//解析get里uri带的数据
{
	char * nexttok = 0;
	if(!request->URI)return 0;
	nexttok = strtok((char *)request->URI,"?");//解析  ？前为URL
	
	nexttok = strtok(NULL," ");//查找内容
	if(!nexttok)//无附带内容
	{
		request->get_info = 0;//空指针回返
		return 0;
	}
	request->get_info = nexttok;//get_info存放get数据指针
	*(request->get_info - 1) = '\0';
	return 1;
}
/************************************
函数名:void HTTP_POST_information(st_http_request * request)  // 类似if(request.METHOD == METHOD_ERR)内部响应报文可自行修改
描述：http post请求数据解析
输入：request   (应答信息结构体指针)
输出：无
样例：HTTP_feedback()
***********************************/
void HTTP_POST_information(st_http_request * request)
{
	char * nexttok = 0;
	nexttok = strtok((char *)request->URI,"\r\n\r\n");
	nexttok = strtok(NULL,"\0");
	printf("POST请求所带数据为:%s\n",nexttok);
	
}

char mimajiexi(st_http_request * request)
{
	char * nexttok = 0;
	int i;
	nexttok = strtok((char *)request->get_info,"username=");
	request->name = nexttok;
	nexttok = strtok(NULL,"&password=");
	request->Password = nexttok;
	*(request->name + 12) = '\0';//账号长度 12
	printf("账号：%s\n密码：%s\n",request->name,request->Password);
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
函数名:void HTTP_feedback()  // 类似if(request.METHOD == METHOD_ERR)内部响应报文可自行修改
描述：http应答响应
输入：无
输出：无
样例：HTTP_feedback()
***********************************/
void HTTP_feedback()
{
		int size = 0;
		size = http_Message_parsing(0,http_Buff,&request);//读入并解析报文
		find_http_uri_type(&request.TYPE,request.URI);//分析请求的文件类型
		printf("接收到的报文数据为:");
		USART_Sent_nbit(USART1,http_Buff,size);
		printf("报文请求类型为：%d\n",request.METHOD);
		printf("报文请求文件类型为：%d\n",request.TYPE);
		if(request.METHOD == METHOD_ERR)//有问题
		{
			memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//清空发送数组
			memcpy(Tx_Buffer,ERROR_REQUEST_PAGE,sizeof(ERROR_REQUEST_PAGE));//压入错误报文
			Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//发送报文
			printf("错误报告：400!\n");
			socket_close(0);//断开连接
		}
		else if(request.METHOD == METHOD_GET)//GET请求
		{
			if(get_http_information(&request))//解析get里uri带的数据
			{
				printf("get请求携带数据为：%s\n",request.get_info);//打印请求数据
			}
			printf("url为：%s\n",request.URI);
			if(!strcmp(request.URI,"/index.htm") || !strcmp(request.URI,"/") || !strcmp(request.URI,"/index.html"))//主页信息
			{
				request.TYPE = PTYPE_HTML;
				memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//清空发送数组
				make_http_response_head(Tx_Buffer,request.TYPE,strlen(INDEX_HTML));//制作响应报文头部
				strcat((char *)Tx_Buffer,INDEX_HTML);
				Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//发送报文
			}
			else if(!strcmp(request.URI,"/w5500.js"))//动态响应
			{
				memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//清空发送数组
				memset(tx_buf,0,MAX_URI_SIZE);//清空数组
				sprintf(tx_buf,IP_message_show,POST_VER,
				Phy_Addr[0],Phy_Addr[1],Phy_Addr[2],Phy_Addr[3],Phy_Addr[4],Phy_Addr[5],
				IP_Addr[0],IP_Addr[1],IP_Addr[2],IP_Addr[3],
				Gateway_IP[0],Gateway_IP[1],Gateway_IP[2],Gateway_IP[3],
				Sub_Mask[0],Sub_Mask[1],Sub_Mask[2],Sub_Mask[3]);
				sprintf(Tx_Buffer,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length:%d\r\n\r\n%s",strlen(tx_buf),tx_buf);
				Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//发送报文
				printf("发送js数据！\n");
				socket_close(0);//断开连接
			}
			else if(!strcmp(request.URI,"/config.html"))//w5500配置信息
			{
				memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//清空发送数组
				make_http_response_head(Tx_Buffer,request.TYPE,strlen(CONFIG_HTML));//制作响应报文头部
				strcat((char *)Tx_Buffer,CONFIG_HTML);
				Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//发送报文
				socket_close(0);//断开连接
			}
			else if(!strcmp(request.URI,"/open.html"))//开门
			{
				if(mimajiexi(&request))
				{
					memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//清空发送数组
					make_http_response_head(Tx_Buffer,request.TYPE,strlen(OPEN_HTML));//制作响应报文头部
					strcat((char *)Tx_Buffer,OPEN_HTML);
					Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//发送报文
				}
				else
				{
					memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//清空发送数组
					make_http_response_head(Tx_Buffer,request.TYPE,strlen(INDEX_HTML));//制作响应报文头部
					strcat((char *)Tx_Buffer,INDEX_HTML);
					Write_SOCK_Data_Buffer(0, Tx_Buffer, sizeof(Tx_Buffer));//发送报文
				}
				if(!strcmp(request.get_info,"1"))//开门信息
				{
					gpio_SetIO(DOOR_OPEN);
				}
			}
		}
		else if(request.METHOD == METHOD_POST)//POST请求
		{
			memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//清空发送数组
			HTTP_POST_information(&request);//解析post请求内部报文
		}
		else if(request.METHOD == METHOD_OPTIONS)//OPTIONS请求
		{
			memset(Tx_Buffer,0x00,sizeof(Tx_Buffer));//清空发送数组
			
		}
}
