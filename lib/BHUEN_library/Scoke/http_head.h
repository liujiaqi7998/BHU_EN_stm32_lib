#ifndef __http_head_H
#define __http_head_H

#include "W5500.h"

#define DOOR_OPEN A2      //门锁开门

#define POST_VER							      "1.00"						//  http版本号,此处必须是浮点或整数
#define HTTP_SERVER_PORT	80		   /**< Http server well-known port number */

/* HTTP Method */
#define		METHOD_ERR		  0        /**< Error    Method.  */
#define		METHOD_GET		  1		     /**< GET      Method.  */
#define		METHOD_HEAD		  2		     /**< HEAD     Method.  */
#define		METHOD_POST		  3		     /**< POST     Method.  */
#define   METHOD_PUT      4		     /**< PUT      Method.  */
#define   METHOD_PATCH    5		     /**< PATCH    Method.  */
#define   METHOD_TRACE    6		     /**< TRACE    Method.  */
#define   METHOD_OPTIONS  7		     /**< OPTIONS  Method.  */
#define   METHOD_DELETE   8		     /**< DELETE   Method.  */

/* HTTP GET Method */
#define		PTYPE_ERR		    0		     /**< Error file. */
#define		PTYPE_HTML		  1		     /**< HTML file.  */
#define		PTYPE_GIF		    2		     /**< GIF file.   */
#define		PTYPE_TEXT		  3		     /**< TEXT file.  */
#define		PTYPE_JPEG		  4		     /**< JPEG file.  */
#define		PTYPE_FLASH	    5		     /**< FLASH file. */
#define		PTYPE_MPEG		  6		     /**< MPEG file.  */
#define		PTYPE_PDF		    7		     /**< PDF file.   */
#define 	PTYPE_CGI		    8		     /**< CGI         */
#define   PTYPE_PL        9        //pl



/* HTML Doc. for ERROR */
#define ERROR_HTML_PAGE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 78\r\n\r\n<HTML>\r\n<BODY>\r\nSorry, the page you requested was not found.\r\n</BODY>\r\n</HTML>\r\n\0"
//static char  ERROR_HTML_PAGE[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 78\r\n\r\n<HTML>\r\n<BODY>\r\nSorry, the page you requested was not found.\r\n</BODY>\r\n</HTML>\r\n\0";

#define ERROR_REQUEST_PAGE "HTTP/1.1 400 OK\r\nContent-Type: text/html\r\nContent-Length: 50\r\n\r\n<HTML>\r\n<BODY>\r\nInvalid request.\r\n</BODY>\r\n</HTML>\r\n\0"
//static char ERROR_REQUEST_PAGE[] = "HTTP/1.1 400 OK\r\nContent-Type: text/html\r\nContent-Length: 50\r\n\r\n<HTML>\r\n<BODY>\r\nInvalid request.\r\n</BODY>\r\n</HTML>\r\n\0";

#define RETURN_CGI_PAGE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 59\r\n\r\n<html><head><title>iWeb - Configuration</title></head><BODY>CGI command was executed.</BODY></HTML>\0"


/* Response header for HTML*/
#define RES_HTMLHEAD_OK	"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
//static PROGMEM char RES_HTMLHEAD_OK[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
/* Response head for TEXT */
#define RES_TEXTHEAD_OK	"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "

/* Response head for GIF */
#define RES_GIFHEAD_OK	"HTTP/1.1 200 OK\r\nContent-Type: image/gif\r\nContent-Length: "

/* Response head for JPEG */
#define RES_JPEGHEAD_OK	"HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: "		

/* Response head for FLASH */
#define RES_FLASHHEAD_OK "HTTP/1.1 200 OK\r\nContent-Type: application/x-shockwave-flash\r\nContent-Length: "
//static PROGMEM char RES_FLASHHEAD_OK[] = "HTTP/1.1 200 OK\r\nContent-Type: application/x-shockwave-flash\r\nContent-Length: ";

/* Response head for MPEG */
#define RES_MPEGHEAD_OK "HTTP/1.1 200 OK\r\nContent-Type: video/mpeg\r\nContent-Length: "	

/* Response head for PDF */
#define RES_PDFHEAD_OK "HTTP/1.1 200 OK\r\nContent-Type: application/pdf\r\nContent-Length: "

//digital I/O out put control result response
#define DOUT_RES_1  "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 1\r\n\r\n1"
#define DOUT_RES_0  "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 1\r\n\r\n0"

#define MAX_URI_SIZE	2048

//IP 信息展示
#define IP_message_show "settingsCallback({\"ver\":\"%s\",\
                \"mac\":\"%02X:%02X:%02X:%02X:%02X:%02X\",\
                \"ip\":\"%d.%d.%d.%d\",\
                \"gw\":\"%d.%d.%d.%d\",\
                \"sub\":\"%d.%d.%d.%d\",\
                });"

extern unsigned char http_Buff[2048];//http BUFF区

typedef struct _st_http_request
{
  unsigned char	METHOD;						/**< request method(METHOD_GET...). */
  unsigned char	TYPE;						/**< request type(PTYPE_HTML...).   */
  char	URI[MAX_URI_SIZE];				/**< request file name.             */
	char * get_info;
	char * name;
	char * Password;
}st_http_request;

extern st_http_request  request;

extern unsigned char http_type;//


int http_Request_message(char* mode,char* API,char* IP,char* buff);//TCP client报文请求


int http_Message_parsing(SOCKET s,unsigned char * buff,st_http_request* request);//TCP Server 请求报文解析
void find_http_uri_type(unsigned char * type, char * buf);//分析请求的文件类型
unsigned char get_http_information(st_http_request * request);//解析get里uri带的数据
void make_http_response_head(unsigned char * buf,char type,uint32_t len);//HTTP报文响应头部
void HTTP_feedback();//http应答响应
#endif


