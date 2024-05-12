#include "SerializationUtility.h"

#include "Engine/EngineTypes/Array.h"

bool SerializePrimitiveType(MemoryBitStream& stream, void* pData, EPrimitiveType type, EPrimitiveType subtype)
{
	switch (type)
	{
		case EPrimitiveType::EPT_Int:
			stream.Serialize(*reinterpret_cast<int*>(pData));
			return true;
		case EPrimitiveType::EPT_Float:
			stream.Serialize(*reinterpret_cast<float*>(pData));
			return true;
		case EPrimitiveType::EPT_Bool:
			stream.Serialize(*reinterpret_cast<bool*>(pData));
			return true;
		case EPrimitiveType::EPT_String:
			stream.Serialize(*reinterpret_cast<std::string*>(pData));
			return true;
		case EPrimitiveType::EPT_Vector2D:
			{
				auto vec2d = reinterpret_cast<LV_VECTOR*>(pData);
				if (vec2d)
					stream.Serialize(vec2d->x, vec2d->y);
			}
			return true;
		case EPrimitiveType::EPT_Array:
			return SerializeArray(stream, pData, subtype);
		default:
			break;
	}

	return false;
}
