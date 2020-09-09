#include "player.h"



bool Player::Get_Death(){return this->is_dead;}

bool Player::Get_Leader(){return this->is_leader;}

bool Player::Get_Openeye(){return this->is_openeye;}

void Player::Change_Death(){this->is_dead=true;}

void Player::Change_Leader(){this->is_leader=true;}

void Player::Change_Openeye(){this->is_openeye=!this->Get_Openeye();}

