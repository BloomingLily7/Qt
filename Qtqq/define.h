#pragma once

#define QRC_QSS(x) ":/Resources/QSS/" + x + ".css"

#define INI_PATH(f) QApplication::applicationDirPath() + "/ini/" + f

#define APP_ICON(i) ":/Resources/MainWindow/app/" + i + ".png"

#define GROUP_ID_LENGTH 4

#define IMG_PATH(p) ":/Resources/MainWindow/emotion/" + p +".png"

#define IMG_SRC "<img src = '%1'/>"
#define IMG_QRC "qrc:/Resources/MainWindow/emotion/"
#define IMG_PNG ".png"

#define HTML_PATH(h) ":/Resources/MainWindow/MsgHtml/" + h + ".html"

#define MSG_LENGTH 2048

#define IMG "img"
#define TXT "txt"

#define BR "<br>"

#define MYSELF "0"

#define TYPE "type"
#define TYPE_GROUP "0"
#define TYPE_SINGLE "1"
#define SEND "send"
#define RECV "recv"
#define WHAT "what"
#define WHAT_MSG "msg"
#define WHAT_FILE "file"

#define FILE_NAME "name"
#define FILE_LENGTH "length"
#define FILE_CONTENT "content"

enum ButtonType
{
	MIN_CLOSE_BUTTON = 0,		//显示最小化以及关闭按钮
	MIN_MAX_CLOSE_BUTTON = 1,	//最小化、最大化、关闭按钮
	CLOSE_BUTTON = 2			//仅关闭按钮
};