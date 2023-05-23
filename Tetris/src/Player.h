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

    // �߼�����
    void OnUpdate(Hazel::Timestep ts);
    // ��Ⱦ����
    void OnRenderer();
private:

private:
    std::string Name;
    int Score = 0;

    float DeltaTime = 0.3f;
    bool Rotate = false;


    static int PlayerSerialID;
};