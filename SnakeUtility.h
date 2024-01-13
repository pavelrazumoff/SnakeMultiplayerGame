#pragma once

#include "Engine/CollisionDetection/CollisionTypes.h"

namespace SnakeUtility {

inline constexpr uint16_t BodyCollisionPrimitiveType = static_cast<uint16_t>(CollisionPrimitiveType::Custom) + 1;

} // namespace SnakeUtility
