#include "Player.h"

int Player::PlayerSerialID = 0;

Player::Player()
{
    Name = "Player" + std::to_string(PlayerSerialID);
    ++PlayerSerialID;
}