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

    // 逻辑部分
    void OnUpdate(Hazel::Timestep ts);
    // 渲染部分
    void OnRenderer();
private:

private:
    std::string Name;
    int Score = 0;

    float DeltaTime = 0.3f;
    bool Rotate = false;


    static int PlayerSerialID;
};