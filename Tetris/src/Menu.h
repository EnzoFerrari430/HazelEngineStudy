#pragma once


enum class MenuItem
{
    Root,  //虚拟的root菜单
    Start,
    ModeChoose,
    SpecialThanks,
    SpecialThanksText,

    ModeNormal,         //普通模式
    ModeDarkLight,      //暗光模式
    ModeCooperation,    //合作模式
    ModeVersus          //对抗模式
};

//菜单的类型
enum class ItemType
{
    Label,
    Button,
    Text
};

