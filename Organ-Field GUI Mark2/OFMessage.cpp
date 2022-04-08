#include "OFMessage.h"

ExMessage MouseMessage{ NULL }; //储存相对窗口的鼠标消息;
void OFMessage::PeekWindowMouse_Tree() { while (true)peekmessage(&MouseMessage, EM_MOUSE); }

void OFMessage::PeekWindowMouse() { thread OFMT0(&OFMessage::PeekWindowMouse_Tree, this); OFMT0.detach(); }

ExMessage OFMessage::GetMouseMessage() { return MouseMessage; }