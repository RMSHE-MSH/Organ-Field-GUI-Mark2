#include "OFMessage.h"

ExMessage MouseMessage{ NULL }; //������Դ��ڵ������Ϣ;
void OFMessage::PeekWindowMouse_Tree() { while (true)peekmessage(&MouseMessage, EM_MOUSE); }

void OFMessage::PeekWindowMouse() { thread OFMT0(&OFMessage::PeekWindowMouse_Tree, this); OFMT0.detach(); }

ExMessage OFMessage::GetMouseMessage() { return MouseMessage; }