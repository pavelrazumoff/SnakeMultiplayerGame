#pragma once

#include <vector>

#include "EngineBasicTypes.h"
#include "Engine/EngineUtility.h"

#include "Engine/Network/SerializationUtility.h"

template<typename T>
class TArray
{
public:
	TArray() { MakePrimitiveType(); }
	~TArray() {}

	void Serialize(MemoryBitStream& stream)
	{
		uint32_t size = (uint32_t)data.size();
		stream.Serialize(size);

		if (stream.IsInput())
		{
			data.resize(size);
		}

		for (uint32_t i = 0; i < size; i++)
		{
			T value = data[i];
			SerializePrimitiveType(stream, reinterpret_cast<void*>(&value), primitiveType);
			if (stream.IsInput()) data[i] = value;
		}
	}

	EPrimitiveType GetPrimitiveType() const
	{
		return primitiveType;
	}

public:
	void Add(const T& item)
	{
		data.push_back(item);
	}

	void Remove(const T& item)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			if (data[i] == item)
			{
				data.erase(data.begin() + i);
				break;
			}
		}
	}

	void RemoveAt(uint32_t index)
	{
		data.erase(data.begin() + index);
	}

	size_t Size() const
	{
		return data.size();
	}

	T& operator[](uint32_t index)
	{
		return data[index];
	}

protected:
	void MakePrimitiveType()
	{
		if (std::is_same<T, int>::value)
		{
			primitiveType = EPrimitiveType::EPT_Int;
		}
		else if (std::is_same<T, uint32_t>::value)
		{
			primitiveType = EPrimitiveType::EPT_Int;
		}
		else if (std::is_same<T, float>::value)
		{
			primitiveType = EPrimitiveType::EPT_Float;
		}
		else if (std::is_same<T, bool>::value)
		{
			primitiveType = EPrimitiveType::EPT_Bool;
		}
		else if (std::is_same<T, std::string>::value)
		{
			primitiveType = EPrimitiveType::EPT_String;
		}
		else if (std::is_same<T, LV_VECTOR>::value)
		{
			primitiveType = EPrimitiveType::EPT_Vector2D;
		}
		else
			DebugEngineTrap();
	}

protected:
	std::vector<T> data;
	EPrimitiveType primitiveType;
};

bool SerializeArray(MemoryBitStream& stream, void* arrayData, EPrimitiveType arrayType);
