//在使用 Qt 的 QTextEdit 控件时，调用 toHtml() 方法将返回当前文本编辑器中的内容，并将其转换为 HTML 格式的字符串。
//这个字符串包含了一些默认的 HTML 结构和样式信息，用来描述文本的格式和内容。具体而言，以下部分在调用 toHtml() 时会是一致的：

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
<html><head><meta name="qrichtext" content="1" /><style type="text/css">
p, li { white-space: pre-wrap; }
</style></head><body style=" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;">
<p style="margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"></p></body></html>

//段落 <p>：在默认情况下，QTextEdit 在初始化时会创建一个空的段落 <p> 元素，即使编辑器中没有实际的文本内容
//所以说，我们只需要获取段落p内的消息内容，在解析完内容之后再进行消息封装（如果有多个段落就有多个段落p），如下：

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd">
<html><head><meta name="qrichtext" content="1" /><style type="text/css">
p, li { white-space: pre-wrap; }
</style></head><body style=" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;">
<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">你好世界<br />你好世界</p>
<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><img src="qrc:/Resources/MainWindow/emotion/1.png" /></p>
<p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;">你好</p></body></html>

//<br />为html内换行符(xml写法，如果是html建议用<br>)，对应实际文本插入操作即软换行，即换行不换段
//可以解析QTextDocument，遍历文本块（文本块就是段落），纯文本以及图片可以用获取文本块内的片段（只要格式相同，就是同一个片段）进行区分