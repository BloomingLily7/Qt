/*
使用json对象封装：   
 属性描述：
	"type" : 1表示单聊，0表示群聊
	"send" : 发送者
	"recv" : 接收者（群号或个人账号）
	"what" : "msg"为普通消息，"file"为文件消息
	
 普通消息：
	"msg" 数组: 顺序封装内容，每个段落（文本块）为一个json数组（数组内是多个json对象）
	
 文件消息：
	"file" 对象，"name"属性为文件名，"len"属性为文件长度，"content"属性为文件内容
*/

//单聊，发送对象为10001，接收对象为10002，发送内容为两个段落：
/*
	你好(1.png)世界
	你好
*/
{
	"type" : "1",                       
	"send" : "10001", 
	"recv"  :  "10002",
	"what" : "msg",
	"msg" : 
	[
		[ {"txt" : "你好"}, { "img", "1"} , {"txt" : "世界"} ],
		[ {"txt" : "你好"} ]
	]
}

//单聊，发送对象为10001，接收对象为10002，发送内容为test.txt文件，文件长度为12，文件内容为Hello World.
{
	"type" : "1",                       
	"send" : "10001", 
	"recv"  :  "10002",
	"what" : "file",
	"file" : 
	{
		"name" : "test.txt"
		"len" : "12"
		"content" : "Hello World."
	}
}

/*
组装msg：
	txt内容不变，img改为 <img src='qrc:/Resources/MainWindow/emotion/1.png'/>     //不设置图片宽高则显示图片本身的宽高
组装完成：
	你好<img src='qrc:/Resources/MainWindow/emotion/1.png'/>世界<br>你好          //如果有多个段落的话，每个段落文本后面补一个<br>，使实际显示有换行效果

接下来把这个组装好的字串直接替换到userLeft/Right.html里的 {{MSG}} 即可
*/

/*
写入file：
	打开文件对话框获取保存路径，对用户选择的（路径+文件名）对应写入即可
*/