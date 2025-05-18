//这些变量用来存储通过WebChannel与Qt对象建立的连接
var external0 = null;
var external_10001 = null;
var external_10002 = null;
var external_10003 = null;
var external_10004 = null;
var external_10005 = null;
var external_10006 = null;
var external_10007 = null;
var external_10008 = null;
var external_10009 = null;

/*
  这是一个扩展String原型的格式化函数，它允许使用带有占位符（如{{key}}）的模板字符串，并用传入的对象属性值替换相应的占位符
  
  arguments[0]获取的就是传入的msg
  假设json对象转成的消息为：msg = {"MSG":"你好"}  （把这个看成C++内的对象，msg为对象，"MSG"为数据成员，"你好"为数据成员的值）
  假设 temp = <div class="msg">{{MSG}}<span class="trigon"></span></div>
  temp调用format方法 var result = temp.format(msg);
  obj = msg，s = temp
  遍历对象属性：
  key = MSG
  创建正则表达式 new RegExp("\{\{" + "MSG" + "\}\}", "g") 相当于 /\{\{MSG\}\}/g
  使用正则表达式在字符串中查找 {{MSG}} 并替换为 obj["MSG"] 即 "你好"
  s变为<div class="msg">"你好"<span class="trigon"></span></div>
*/
String.prototype.format = function() {  
    if(arguments.length == 0) return this;  
    var obj = arguments[0];  
    var s = this;  
    for(var key in obj) {  
        s = s.replace(new RegExp("\\{\\{" + key + "\\}\\}", "g"), obj[key]);  
    }  
    return s;  
}  

/*
	通过QWebChannel与Qt的对象建立连接，并将这些对象存储到之前声明的变量中
	这段代码中，new QWebChannel() 是调用了 qwebchannel.js 中定义的构造函数，用于创建一个 QWebChannel 实例
	qt.webChannelTransport 作为参数传递给构造函数，表示传输渠道或者通信方式，用于与 Qt 应用程序进行通信
	匿名函数作为第二个参数，是在 QWebChannel 实例创建成功后执行的回调函数，接收一个 channel 参数，这个 channel 对象包含了从 Qt 应用程序暴露出的所有对象
	在回调函数中，通过 channel.objects 访问从 Qt 应用程序暴露出来的对象
	每个 external_ 开头的对象代表从 Qt 应用程序中暴露出来的一个具体对象，可以通过这些对象进行进一步的交互和调用
*/
new QWebChannel(qt.webChannelTransport,
	function(channel) {
		external0 = channel.objects.external0;
		external_10001 = channel.objects.external_10001;
external_10002 = channel.objects.external_10002;
external_10003 = channel.objects.external_10003;
external_10004 = channel.objects.external_10004;
external_10005 = channel.objects.external_10005;
external_10006 = channel.objects.external_10006;
external_10007 = channel.objects.external_10007;
external_10008 = channel.objects.external_10008;
external_10009 = channel.objects.external_10009;
	}
);

//这些函数用于处理从不同的Qt对象接收到的消息
//将msg格式化为HTML并添加到页面中的一个占位元素（#placeholder），然后滚动页面到底部以显示新消息。
function appendHtml0(msg){
	$("#placeholder").append(external0.msgRHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};

function recvHtml_10001(msg){
	$("#placeholder").append(external_10001.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
function recvHtml_10002(msg){
	$("#placeholder").append(external_10002.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
function recvHtml_10003(msg){
	$("#placeholder").append(external_10003.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
function recvHtml_10004(msg){
	$("#placeholder").append(external_10004.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
function recvHtml_10005(msg){
	$("#placeholder").append(external_10005.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
function recvHtml_10006(msg){
	$("#placeholder").append(external_10006.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
function recvHtml_10007(msg){
	$("#placeholder").append(external_10007.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
function recvHtml_10008(msg){
	$("#placeholder").append(external_10008.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
function recvHtml_10009(msg){
	$("#placeholder").append(external_10009.msgLHtmlTmpl.format(msg));
	window.scrollTo(0,document.body.scrollHeight); ;  
};
