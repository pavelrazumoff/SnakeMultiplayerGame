#pragma once

#include "Serialization/MemoryBitStream.h"
#include "ReflectionSystem/MemberVariable.h"

bool SerializePrimitiveType(MemoryBitStream& stream, void* pData, EPrimitiveType type, EPrimitiveType subtype = EPrimitiveType::EPT_None);
