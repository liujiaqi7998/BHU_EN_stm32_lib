#ifndef __WEBPAGE_H
#define __WEBPAGE_H

/***********************************************************/
//��ҳ
/***********************************************************/
#define INDEX_HTML "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<meta http-equiv='Content-Type' content='text/html; charset=GB2312'>"\
"<title>EN1409.com</title>"\
"<link rel='icon' href='data:;base64,='>"\
"</head>"\
"<body>"\
"<form name='user' action='/open.html' method='get'>"\
"�û���:<input type='text' name='username'><br>"\
"��&nbsp;&nbsp;&nbsp;��:<input type='password' name='password'><br>"\
"<input type='submit' value='�� ¼'>"\
"</form>"\
"</body>"\
"</html>"\
/***********************************************************/
//����ҳ��
/***********************************************************/
#define OPEN_HTML "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<meta http-equiv='Content-Type' content='text/html; charset=GB2312'>"\
"<title>EN1409.com</title>"\
"<link rel='icon' href='data:;base64,='>"\
"<script type='text/javascript'>"\
"function f1() {"\
"var pwdBtn = document.getElementById('pwdBtnID');"\
"var url = 'http://liaolingfeng.zicp.io:80/open.html?1';"\
"var request = new XMLHttpRequest();"\
"request.open('GET', url);"\
"request.send("");"\
"pwdBtn.value = '�ұ�����!';"\
"alert('���ųɹ���');"\
"};"\
"</script>"\
"</head>"\
"<body>"\
"<p><input id='pwdBtnID' type='button' onclick='f1()' value='���ҿ���' style='height:60px;width:100px;'/></p>"\
"<a href='http://liaolingfeng.zicp.io:80/config.html'>�������������ò鿴</a>"\
"</body>"\
"</html>"\
/***********************************************************/
//w5500����ҳ��
/***********************************************************/
#define CONFIG_HTML "<!DOCTYPE html>"\
"<html>"\
"<head>"\
"<title>EN����������</title>"\
"<link rel='icon' href='data:;base64,='>"\
"<meta http-equiv='Content-Type' content='text/html; charset=GB2312'/>"\
"<style type='text/css'>"\
"body {text-align:left; background-color:#c0deed;font-family:Verdana;}"\
"#main {margin-right:auto;margin-left:auto;margin-top:30px;}"\
"label{display:inline-block;width:150px;}"\
"#main h3{color:#66b3ff; text-decoration:underline;}"\
"</style>"\
"<script>"\
"function $(id) { return document.getElementById(id); };"\
"function settingsCallback(o) {"\
"if ($('txtVer')) $('txtVer').value = o.ver;"\
"if ($('txtMac')) $('txtMac').value = o.mac;"\
"if ($('txtIp')) $('txtIp').value = o.ip;"\
"if ($('txtSub')) $('txtSub').value = o.sub;"\
"if ($('txtGw')) $('txtGw').value = o.gw;"\
"};"\
"</script>"\
"</head>"\
"<body>"\
"<div id='main'>"\
"<div style='background:snow; display:block;padding:10px 20px;'>"\
"<h3>�����������</h3>"\
"<form id='frmSetting' method='POST' action='config.cgi'>"\
"<p><label for='txtIp'>�̼��汾��:</label><input type='text' id='txtVer' name='ver' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>MAC��ַ:</label><input type='text' id='txtMac' name='mac' size='16' disabled='disabled' /></p>"\
"<p><label for='txtIp'>IP��ַ:</label><input type='text' id='txtIp' name='ip' size='16' /></p>"\
"<p><label for='txtSub'>��������:</label><input type='text' id='txtSub' name='sub' size='16' /></p>"\
"<p><label for='txtGw'>Ĭ������:</label><input type='text' id='txtGw' name='gw' size='16' /></p>"\
"</form>"\
"</div>"\
"</div>"\
"<div style='margin:5px 5px;'>"\
"&copy;Copyright 2019/9/26 by EN-�����"\
"</div>"\
"<script type='text/javascript' src='w5500.js'></script>"\
"</body>"\
"</html>"

#endif





