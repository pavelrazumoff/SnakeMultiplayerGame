#include "Array.h"

bool SerializeArray(MemoryBitStream& stream, void* arrayData, EPrimitiveType arrayType)
{
	switch (arrayType)
	{
		case EPrimitiveType::EPT_Int:
			if (auto _array = reinterpret_cast<TArray<int>*>(arrayData)) { _array->Serialize(stream); return true; }
			break;
		case EPrimitiveType::EPT_Float:
			if (auto _array = reinterpret_cast<TArray<float>*>(arrayData)) { _array->Serialize(stream); return true; }
			break;
		case EPrimitiveType::EPT_Bool:
			if (auto _array = reinterpret_cast<TArray<bool>*>(arrayData)) { _array->Serialize(stream); return true; }
			break;
		case EPrimitiveType::EPT_String:
			if (auto _array = reinterpret_cast<TArray<std::string>*>(arrayData)) { _array->Serialize(stream); return true; }
			break;
		case EPrimitiveType::EPT_Vector2D:
			if (auto _array = reinterpret_cast<TArray<LV_VECTOR>*>(arrayData)) { _array->Serialize(stream); return true; }
			break;
		case EPrimitiveType::EPT_Array:
			DebugEngineTrap(); // TODO.
			break;
		default:
			break;
	}

	return false;
}
