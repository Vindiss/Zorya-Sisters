#include "Capacity/DoubleJump.h"
#include "Engine.h"
#include "Modules/SceneModule.h"
#include "Components/Player.h"

DoubleJump::~DoubleJump() {
	GetCapacityOwner()->GetComponent<Player>()->SetCanDoubleJump(false);
}

void DoubleJump::SetDoubleJump(bool _can_double_jump) {
	GetCapacityOwner()->GetComponent<Player>()->SetCanDoubleJump(_can_double_jump);
}