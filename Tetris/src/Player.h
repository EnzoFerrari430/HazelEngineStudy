#pragma once
#include <Hazel.h>

/*
玩家类

包含属性：
    玩家id
    控制
    分数
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