#pragma once
#include <Hazel.h>

/*
�����

�������ԣ�
    ���id
    ����
    ����
*/ 
class Player
{
public:
    Player();
private:
    std::string Name;
    int Score = 0;
    static int PlayerSerialID;
};