#include "OFMessage.h"

ExMessage Message{ NULL }; //储存相对窗口的鼠标消息;
void OFMessage::PeekOFMessage_Tree() { while (true) { Message = getmessage(EM_MOUSE | EM_KEY | EM_CHAR | EM_WINDOW); } }

void OFMessage::PeekOFMessageUP() { thread OFMT0(&OFMessage::PeekOFMessage_Tree, this); OFMT0.detach(); }

ExMessage OFMessage::PeekOFMessage() { return Message; }