#pragma once
#include<assert.h>
#include"EspString.hpp"
#include"EspArray.hpp"
#include<Windows.h>
enum class EspJsonValueType { Value_Void, Value_Null, Value_Boolean, Value_Number, Value_String, Value_Object, Value_Array };
enum class EspJsonErrorCode
{
	Error_NoError,
	Error_Invalid_Character,
	Error_Invalid_Escape_Character,
	Error_Invalid_Number,
	Error_Miss_Quote,
	Error_Miss_Comma,
	Error_Miss_Colon,
	Error_Miss_Brace,
	Error_Miss_Bracket
};
class EspJsonValue;
class EspJsonMember;
class EspJsonParser;
class EspJsonObject;
class EspJsonArray;
class EspJsonValue
{
private:
	EspJsonValueType ValueType = EspJsonValueType::Value_Void;
	void* ValuePointer = nullptr;

public:
	EspJsonValue() {}
	EspJsonValue(EspJsonValueType ValueType, void* ValuePointer);
	EspJsonValue(const bool& BooleanValue);
	EspJsonValue(const double& NumberValue);
	EspJsonValue(const EspString& StringValue);
	EspJsonValue(const EspJsonObject& JsonObject);
	EspJsonValue(const EspJsonArray& JsonArray);
	EspJsonValue(const EspJsonValue& NewValue);
	~EspJsonValue()
	{
		this->PreFreeValue();
	}

	const bool IsVoid()const { return this->ValueType == EspJsonValueType::Value_Void; }
	const bool IsNull()const { return this->ValueType == EspJsonValueType::Value_Null; }
	const bool IsBoolean()const { return this->ValueType == EspJsonValueType::Value_Boolean; }
	const bool IsNumber()const { return this->ValueType == EspJsonValueType::Value_Number; }
	const bool IsString()const { return this->ValueType == EspJsonValueType::Value_String; }
	const bool IsObject()const { return this->ValueType == EspJsonValueType::Value_Object; }
	const bool IsArray()const { return this->ValueType == EspJsonValueType::Value_Array; }
	EspJsonValueType GetValueType()const { return this->ValueType; }

	const bool& GetBoolean()const;
	const double& GetNumber()const;
	const EspString& GetString()const;
	EspJsonObject& GetJsonObject();
	EspJsonArray& GetJsonArray();
	void SetBoolean(const bool& NewValue);
	void SetNull();
	void SetNumber(const double& NewValue);
	void SetString(const EspString& NewValue);
	void SetJsonObject(const EspJsonObject& JsonObject);
	void SetJsonArray(const EspJsonArray& JsonArray);
	void PreFreeValue()const;
	const EspJsonValue& operator=(const EspJsonValue& NewValue);
	EspJsonValue& operator[](const EspString& Key);
	EspJsonValue& operator[](const unsigned int Index);
};
class EspJsonMember
{
private:
	EspString Key;
	EspJsonValue Value;
public:
	EspJsonMember() {}
	EspJsonMember(const EspString& Key, const EspJsonValue& Value)
	{
		this->Key = Key;
		this->Value = Value;
	}
	EspJsonMember(const EspJsonMember& Member)
	{
		this->Key = Member.Key;
		this->Value = Member.Value;
	}
	EspString& GetKey() { return Key; }
	void SetKey(const EspString& Key) { this->Key = Key; }
	EspJsonValue& GetValue() { return Value; }
	void SetValue(const EspJsonValue& Value) { this->Value = Value; }
	//	const EspJsonMember& operator=(const EspJsonMember& NewMember)
};

class EspJsonObject
{
private:
	EspArray<EspJsonMember> JsonObject;
public:
	EspJsonObject() {}
	EspJsonObject(const EspJsonObject& JsonObject)
	{
		for (unsigned int TimeNum = 0; TimeNum < JsonObject.JsonObject.GetCount(); TimeNum++)
			this->JsonObject.AddElement(JsonObject.JsonObject.GetElementAt(TimeNum));
	}
	void AddMember(const EspJsonMember& JsonMember) { JsonObject.AddElement(JsonMember); }
	EspJsonValue& GetValue(const EspString& Key)
	{
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).GetKey().Compare(Key))
				return JsonObject.GetElementAt(TimeNum).GetValue();
	}
	EspJsonMember& GetMember(const unsigned int Index)
	{
		return this->JsonObject.GetElementAt(Index);
	}
	void SetValue(const EspString& Key, const EspJsonValue& Value)
	{
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).GetKey().Compare(Key))
				JsonObject.GetElementAt(TimeNum).SetValue(Value);
	}
	void DeleteMember(const EspString& Key)
	{
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).GetKey().Compare(Key))
			{
				this->JsonObject.DeleteElement(TimeNum, 1);
				return;
			}
	}
	void DeleteAll() { this->JsonObject.Empty(); }
	unsigned int GetCount()const { return this->JsonObject.GetCount(); }
	EspJsonValue& operator[](const EspString& Key) { return this->GetValue(Key); }

	EspString Synthesize()const;
};
class EspJsonArray
{
private:
	EspArray<EspJsonValue> JsonArray;
public:
	EspJsonArray() {}
	EspJsonArray(const EspJsonArray& JsonArray)
	{
		for (unsigned int TimeNum = 0; TimeNum < JsonArray.JsonArray.GetCount(); TimeNum++)
			this->JsonArray.AddElement(JsonArray.JsonArray.GetElementAt(TimeNum));
	}
	void AddValue(const EspJsonValue& JsonValue) { this->JsonArray.AddElement(JsonValue); }
	EspJsonValue& GetValue(const unsigned int Index) { return this->JsonArray.GetElementAt(Index); }
	void SetValue(const unsigned int Index, const EspJsonValue& JsonValue) { this->JsonArray.SetElementAt(Index, JsonValue); }
	void DeleteValue(const unsigned int Index) { this->JsonArray.DeleteElement(Index, 1); }
	void DeleteAll() { this->JsonArray.Empty(); }
	unsigned int GetCount()const { return this->JsonArray.GetCount(); }
	EspJsonValue& operator[](const unsigned int Index) { return this->GetValue(Index); }

	EspString Synthesize()const;
};

class EspJsonParser
{
private:
	EspString JsonString;
	unsigned int ParsePos = 0;
	EspJsonErrorCode ErrorCode = EspJsonErrorCode::Error_NoError;

	void ParseValue_Boolean_True(EspJsonValue& JsonValue)
	{
		//std::cout << "[" << this->ParsePos << "]";
		if (this->JsonString.GetCharAt(this->ParsePos++) == 't' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'r' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'u' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'e')
			JsonValue.SetBoolean(true);
		else
			this->ErrorCode = EspJsonErrorCode::Error_Invalid_Character;
		//std::cout << "Value: " << "[Boolean] " << "true" << std::endl;
	}
	void ParseValue_Boolean_False(EspJsonValue& JsonValue)
	{
		//std::cout << "[" << this->ParsePos << "]";
		if (this->JsonString.GetCharAt(this->ParsePos++) == 'f' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'a' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'l' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 's' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'e')
			JsonValue.SetBoolean(false);
		else
			this->ErrorCode = EspJsonErrorCode::Error_Invalid_Character;
		//std::cout << "Value: " << "[Boolean] " << "false" << std::endl;
	}
	void ParseValue_Null(EspJsonValue& JsonValue)
	{
		//std::cout << "[" << this->ParsePos << "]";
		if (this->JsonString.GetCharAt(this->ParsePos++) == 'n' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'u' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'l' &&
			this->JsonString.GetCharAt(this->ParsePos++) == 'l')
			JsonValue.SetNull();
		else
			this->ErrorCode = EspJsonErrorCode::Error_Invalid_Character;
		//std::cout << "Value: " << "[Null] " << "null" << std::endl;
	}
	void ParseValue_Number(EspJsonValue& JsonValue)
	{
		//std::cout << "[" << this->ParsePos << "]";
		EspString NumberValue;
		if (this->JsonString.GetCharAt(this->ParsePos) == '-')
		{
			NumberValue.Append('-');
			this->ParsePos++;
		}
		if (this->JsonString.GetCharAt(this->ParsePos) == '0')
		{
			NumberValue.Append('0');
			this->ParsePos++;
		}
		else
		{
			if (!(this->JsonString.GetCharAt(this->ParsePos) >= '1' && this->JsonString.GetCharAt(this->ParsePos) <= '9'))
			{
				this->ErrorCode = EspJsonErrorCode::Error_Invalid_Number;
				return;
			}
			while (this->JsonString.GetCharAt(this->ParsePos) >= '0' && this->JsonString.GetCharAt(this->ParsePos) <= '9')
			{
				NumberValue.Append(this->JsonString.GetCharAt(this->ParsePos));
				this->ParsePos++;
			}
		}
		if (this->JsonString.GetCharAt(this->ParsePos) == '.')
		{
			NumberValue.Append('.');
			this->ParsePos++;
			if (!(this->JsonString.GetCharAt(this->ParsePos) >= '0' && this->JsonString.GetCharAt(this->ParsePos) <= '9'))
			{
				this->ErrorCode = EspJsonErrorCode::Error_Invalid_Number;
			}
			while (this->JsonString.GetCharAt(this->ParsePos) >= '0' && this->JsonString.GetCharAt(this->ParsePos) <= '9')
			{
				NumberValue.Append(this->JsonString.GetCharAt(this->ParsePos));
				this->ParsePos++;
			}
		}
		if (this->JsonString.GetCharAt(this->ParsePos) == 'e' || this->JsonString.GetCharAt(this->ParsePos) == 'E')
		{
			NumberValue.Append(this->JsonString.GetCharAt(this->ParsePos));
			this->ParsePos++;
			if (this->JsonString.GetCharAt(this->ParsePos) == '+' || this->JsonString.GetCharAt(this->ParsePos) == '-')
			{
				NumberValue.Append(this->JsonString.GetCharAt(this->ParsePos));
				this->ParsePos++;
			}
			if (!(this->JsonString.GetCharAt(this->ParsePos) >= '0' && this->JsonString.GetCharAt(this->ParsePos) <= '9'))
			{
				this->ErrorCode = EspJsonErrorCode::Error_Invalid_Number;
				return;
			}
			while (this->JsonString.GetCharAt(this->ParsePos) >= '0' && this->JsonString.GetCharAt(this->ParsePos) <= '9')
			{
				NumberValue.Append(this->JsonString.GetCharAt(this->ParsePos));
				this->ParsePos++;
			}
		}
		JsonValue.SetNumber(::strtod(NumberValue.GetAnsiStr(), NULL));
		//std::cout << "Value: " << "[Number] " << NumberValue.GetAnsiStr() << std::endl;
	}
	void ParseKey(EspString& Key)
	{
		//std::cout << "[" << this->ParsePos << "]";
		if (this->JsonString.GetCharAt(this->ParsePos) != '"')
		{
			this->ErrorCode = EspJsonErrorCode::Error_Miss_Quote;
			return;
		}
		Key.Empty();
		while (true)
		{
			this->ParsePos++;
			if (this->JsonString.GetCharAt(this->ParsePos) == '\\')
			{
				this->ParsePos++;
				switch (this->JsonString.GetCharAt(this->ParsePos))
				{
				case'"':Key.Append('"'); break;
				case'\\':Key.Append('\\'); break;
				case'/':Key.Append('/'); break;
				case'b':Key.Append('\b'); break;
				case'f':Key.Append('\f'); break;
				case'n':Key.Append('\n'); break;
				case'r':Key.Append('\r'); break;
				case't':Key.Append('\t'); break;
				case'u':
				{

					break;
				}
				default:
					this->ErrorCode = EspJsonErrorCode::Error_Invalid_Escape_Character;
					return;
				}
			}
			else if (this->JsonString.GetCharAt(this->ParsePos) == '"')
			{
				this->ParsePos++;
				//std::cout << "Key: " << Key << std::endl;
				return;
			}
			else if (this->JsonString.GetCharAt(this->ParsePos) == '\r' || this->JsonString.GetCharAt(this->ParsePos) == '\n')
			{
				this->ErrorCode = EspJsonErrorCode::Error_Invalid_Character;
				return;
			}
			else
				Key.Append(this->JsonString.GetCharAt(this->ParsePos));
		}
	}
	void ParseValue_String(EspJsonValue& JsonValue)
	{
		//std::cout << "[" << this->ParsePos << "]";
		if (this->JsonString.GetCharAt(this->ParsePos) != '"')
		{
			this->ErrorCode = EspJsonErrorCode::Error_Miss_Quote;
			return;
		}
		EspString StringValue;
		while (true)
		{
			this->ParsePos++;
			if (this->JsonString.GetCharAt(this->ParsePos) == '\\')
			{
				this->ParsePos++;
				switch (this->JsonString.GetCharAt(this->ParsePos))
				{
				case'"':StringValue.Append('"'); break;
				case'\\':StringValue.Append('\\'); break;
				case'/':StringValue.Append('/'); break;
				case'b':StringValue.Append('\b'); break;
				case'f':StringValue.Append('\f'); break;
				case'n':StringValue.Append('\n'); break;
				case'r':StringValue.Append('\r'); break;
				case't':StringValue.Append('\t'); break;
				case'u':
				{

					break;
				}
				default:
					this->ErrorCode = EspJsonErrorCode::Error_Invalid_Escape_Character;
					return;
				}
			}
			else if (this->JsonString.GetCharAt(this->ParsePos) == '"')
			{
				this->ParsePos++;
				JsonValue.SetString(StringValue);
				//std::cout << "Value: " << "[String] " << (StringValue.IsEmpty()?"":StringValue.GetAnsiStr()) << std::endl;
				return;
			}
			else if (this->JsonString.GetCharAt(this->ParsePos) == '\r' || this->JsonString.GetCharAt(this->ParsePos) == '\n')
			{
				this->ErrorCode = EspJsonErrorCode::Error_Invalid_Character;
				return;
			}
			else
				StringValue.Append(this->JsonString.GetCharAt(this->ParsePos));
		}
	}
	void ParseValue_JsonObject(EspJsonValue& JsonValue)
	{
		//std::cout << "[" << this->ParsePos << "]";
		if (this->JsonString.GetCharAt(this->ParsePos) != '{')
		{
			this->ErrorCode = EspJsonErrorCode::Error_Miss_Brace;
			return;
		}
		this->ParsePos++;
		if (this->JsonString.GetCharAt(this->ParsePos) == '}')
		{
			this->ParsePos++;
			JsonValue.SetJsonObject(EspJsonObject());
			return;
		}
		EspJsonObject JsonObject;
		//std::cout << "Value: " << "[Object] " << std::endl;
		while (true)
		{
			if (this->JsonString.GetCharAt(this->ParsePos) != '"')
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Quote;
				return;
			}
			EspJsonMember JsonMember;
			this->ParseKey(JsonMember.GetKey());
			if (this->ErrorCode != EspJsonErrorCode::Error_NoError)
				return;
			//this->ParsePos++;
			if (this->JsonString.GetCharAt(this->ParsePos) != ':')
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Colon;
				return;
			}
			this->ParsePos++;
			this->ParseValue(JsonMember.GetValue());
			if (this->ErrorCode != EspJsonErrorCode::Error_NoError)
				return;
			JsonObject.AddMember(JsonMember);
			if (this->JsonString.GetCharAt(this->ParsePos) == ',')
				this->ParsePos++;
			else if (this->JsonString.GetCharAt(this->ParsePos) == '}')
			{
				this->ParsePos++;
				this->ErrorCode = EspJsonErrorCode::Error_NoError;
				JsonValue.SetJsonObject(JsonObject);
				return;
			}
			else
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Comma;
				return;
			}
		}
	}
	void ParseValue_JsonArray(EspJsonValue& JsonValue)
	{
		//std::cout << "[" << this->ParsePos << "]";
		if (this->JsonString.GetCharAt(this->ParsePos) != '[')
		{
			this->ErrorCode = EspJsonErrorCode::Error_Miss_Bracket;
			return;
		}
		this->ParsePos++;
		if (this->JsonString.GetCharAt(this->ParsePos) == ']')
		{
			this->ParsePos++;
			JsonValue.SetJsonArray(EspJsonArray());
			return;
		}
		EspJsonArray JsonArray;
		//std::cout << "Value: " << "[Array] " <<  std::endl;
		while (true)
		{
			EspJsonValue SubValue;
			this->ParseValue(SubValue);
			if (this->ErrorCode != EspJsonErrorCode::Error_NoError)
				return;
			JsonArray.AddValue(SubValue);
			if (this->JsonString.GetCharAt(this->ParsePos) == ',')
				this->ParsePos++;
			else if (this->JsonString.GetCharAt(this->ParsePos) == ']')
			{
				this->ParsePos++;
				JsonValue.SetJsonArray(JsonArray);
				return;
			}
			else {
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Comma;
				return;
			}
		}
	}
	void ParseValue(EspJsonValue& JsonValue)
	{
		if (this->ErrorCode == EspJsonErrorCode::Error_NoError)
		{
			switch (this->JsonString.GetCharAt(this->ParsePos))
			{
			case 't':this->ParseValue_Boolean_True(JsonValue); break;
			case 'f':this->ParseValue_Boolean_False(JsonValue); break;
			case 'n':this->ParseValue_Null(JsonValue); break;
			case '0':case '1':case '2':case '3':case '4':case '5':
			case '6':case '7':case '8':case '9':case '-':
				this->ParseValue_Number(JsonValue); break;
			case '"':this->ParseValue_String(JsonValue); break;
			case '{':this->ParseValue_JsonObject(JsonValue); break;
			case '[':this->ParseValue_JsonArray(JsonValue); break;
			default:
				this->ErrorCode = EspJsonErrorCode::Error_Invalid_Character;
				break;
			}
		}
	}
public:
	EspJsonParser(const EspString& JsonString) { this->JsonString = JsonString; }
	EspJsonObject Parse()
	{
		if (this->JsonString.GetCharAt(this->ParsePos) != '{')
		{
			this->ErrorCode = EspJsonErrorCode::Error_Miss_Brace;
			return EspJsonObject();
		}
		this->ParsePos++;
		if (this->JsonString.GetCharAt(this->ParsePos) == '}')
			return EspJsonObject();
		EspJsonObject JsonObject;
		while (true)
		{
			if (this->JsonString.GetCharAt(this->ParsePos) != '"')
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Quote;
				return EspJsonObject();
			}
			EspJsonMember JsonMember;
			this->ParseKey(JsonMember.GetKey());
			if (this->ErrorCode != EspJsonErrorCode::Error_NoError)
				return EspJsonObject();
			//this->ParsePos++;
			if (this->JsonString.GetCharAt(this->ParsePos) != ':')
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Colon;
				return EspJsonObject();
			}
			this->ParsePos++;
			this->ParseValue(JsonMember.GetValue());
			if (this->ErrorCode != EspJsonErrorCode::Error_NoError)
				return EspJsonObject();
			JsonObject.AddMember(JsonMember);
			if (this->JsonString.GetCharAt(this->ParsePos) == ',')
				this->ParsePos++;
			else if (this->JsonString.GetCharAt(this->ParsePos) == '}')
			{
				this->ErrorCode = EspJsonErrorCode::Error_NoError;
				return JsonObject;
			}
			else
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Comma;
				return EspJsonObject();
			}
		}
	}
	void Parse(EspJsonObject& JsonObject)
	{
		if (this->JsonString.GetCharAt(this->ParsePos) != '{')
		{
			this->ErrorCode = EspJsonErrorCode::Error_Miss_Brace;
			JsonObject = EspJsonObject();
			return;
		}
		this->ParsePos++;
		if (this->JsonString.GetCharAt(this->ParsePos) == '}')
		{
			JsonObject = EspJsonObject();
			return;
		}
		while (true)
		{
			if (this->JsonString.GetCharAt(this->ParsePos) != '"')
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Quote;
				JsonObject.DeleteAll();
				JsonObject = EspJsonObject();
				return;
			}
			EspJsonMember JsonMember;
			this->ParseKey(JsonMember.GetKey());
			if (this->ErrorCode != EspJsonErrorCode::Error_NoError)
			{
				JsonObject = EspJsonObject();
				return;
			}			//this->ParsePos++;
			if (this->JsonString.GetCharAt(this->ParsePos) != ':')
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Colon;
				JsonObject.DeleteAll();
				JsonObject = EspJsonObject();
				return;
			}
			this->ParsePos++;
			this->ParseValue(JsonMember.GetValue());
			if (this->ErrorCode != EspJsonErrorCode::Error_NoError)
			{
				JsonObject = EspJsonObject();
				return;
			}
			JsonObject.AddMember(JsonMember);
			if (this->JsonString.GetCharAt(this->ParsePos) == ',')
				this->ParsePos++;
			else if (this->JsonString.GetCharAt(this->ParsePos) == '}')
			{
				this->ErrorCode = EspJsonErrorCode::Error_NoError;
				return;
			}
			else
			{
				this->ErrorCode = EspJsonErrorCode::Error_Miss_Comma;
				JsonObject.DeleteAll();
				JsonObject = EspJsonObject();
				return;
			}
		}
	}
	const EspJsonErrorCode& GetErrorCode()const { return this->ErrorCode; }
	const unsigned int& GetParsePos()const { return this->ParsePos; }

};

EspJsonValue::EspJsonValue(EspJsonValueType ValueType, void* ValuePointer)
{
	switch (this->ValueType = ValueType)
	{
	case EspJsonValueType::Value_Boolean:this->ValuePointer = new bool(*(bool*)ValuePointer); break;
	case EspJsonValueType::Value_Number:this->ValuePointer = new double(*(double*)ValuePointer); break;
	case EspJsonValueType::Value_String:this->ValuePointer = new EspString(*(EspString*)ValuePointer); break;
	case EspJsonValueType::Value_Object:this->ValuePointer = new EspJsonObject(*(EspJsonObject*)ValuePointer); break;
	case EspJsonValueType::Value_Array:this->ValuePointer = new EspJsonArray(*(EspJsonArray*)ValuePointer); break;
	}
}
EspJsonValue::EspJsonValue(const bool& BooleanValue) { this->ValueType = EspJsonValueType::Value_Boolean; this->ValuePointer = new bool(BooleanValue); }
EspJsonValue::EspJsonValue(const double& NumberValue) { this->ValueType = EspJsonValueType::Value_Number; this->ValuePointer = new double(NumberValue); }
EspJsonValue::EspJsonValue(const EspString& StringValue) { this->ValueType = EspJsonValueType::Value_String; this->ValuePointer = new EspString(StringValue); }
EspJsonValue::EspJsonValue(const EspJsonObject& JsonObject) { this->ValueType = EspJsonValueType::Value_Object; this->ValuePointer = new EspJsonObject(JsonObject); }
EspJsonValue::EspJsonValue(const EspJsonArray& JsonArray) { this->ValueType = EspJsonValueType::Value_Array; this->ValuePointer = new EspJsonArray(JsonArray); }

EspJsonValue::EspJsonValue(const EspJsonValue& NewValue)
{
	switch (this->ValueType = NewValue.ValueType)
	{
	case EspJsonValueType::Value_Boolean:this->ValuePointer = new bool(*(bool*)NewValue.ValuePointer); break;
	case EspJsonValueType::Value_Number:this->ValuePointer = new double(*(double*)NewValue.ValuePointer); break;
	case EspJsonValueType::Value_String:this->ValuePointer = new EspString(*(EspString*)NewValue.ValuePointer); break;
	case EspJsonValueType::Value_Object:this->ValuePointer = new EspJsonObject(*(EspJsonObject*)NewValue.ValuePointer); break;
	case EspJsonValueType::Value_Array:this->ValuePointer = new EspJsonArray(*(EspJsonArray*)NewValue.ValuePointer); break;
	}
}
const bool& EspJsonValue::GetBoolean()const
{
	assert(this->ValueType == EspJsonValueType::Value_Boolean && this->ValuePointer != nullptr);
	return *(bool*)ValuePointer;
}
const double& EspJsonValue::GetNumber()const
{
	assert(this->ValueType == EspJsonValueType::Value_Number && this->ValuePointer != nullptr);
	return *(double*)this->ValuePointer;
}
const EspString& EspJsonValue::GetString()const
{
	assert(this->ValueType == EspJsonValueType::Value_String && this->ValuePointer != nullptr);
	return *(EspString*)this->ValuePointer;
}
EspJsonObject& EspJsonValue::GetJsonObject()
{
	assert(this->ValueType == EspJsonValueType::Value_Object && this->ValuePointer != nullptr);
	return *(EspJsonObject*)this->ValuePointer;
}
EspJsonArray& EspJsonValue::GetJsonArray()
{
	assert(this->ValueType == EspJsonValueType::Value_Array && this->ValuePointer != nullptr);
	return *(EspJsonArray*)this->ValuePointer;
}
void EspJsonValue::SetBoolean(const bool& NewValue)
{
	this->PreFreeValue();
	this->ValueType = EspJsonValueType::Value_Boolean;
	this->ValuePointer = new bool(NewValue);
}
void EspJsonValue::SetNull()
{
	this->PreFreeValue();
	this->ValueType = EspJsonValueType::Value_Null;
}
void EspJsonValue::SetNumber(const double& NewValue)
{
	this->PreFreeValue();
	this->ValueType = EspJsonValueType::Value_Number;
	this->ValuePointer = new double(NewValue);
}
void EspJsonValue::SetString(const EspString& NewValue)
{
	this->PreFreeValue();
	this->ValueType = EspJsonValueType::Value_String;
	this->ValuePointer = new EspString(NewValue);
}
void EspJsonValue::SetJsonObject(const EspJsonObject& JsonObject)
{
	this->PreFreeValue();
	this->ValueType = EspJsonValueType::Value_Object;
	this->ValuePointer = new EspJsonObject(JsonObject);
}
void EspJsonValue::SetJsonArray(const EspJsonArray& JsonArray)
{
	this->PreFreeValue();
	this->ValueType = EspJsonValueType::Value_Array;
	this->ValuePointer = new EspJsonArray(JsonArray);
}
void EspJsonValue::PreFreeValue()const
{
	if (this->ValuePointer != nullptr)
		switch (this->ValueType)
		{
		case EspJsonValueType::Value_Boolean:delete (bool*)ValuePointer; break;
		case EspJsonValueType::Value_Number:delete (double*)ValuePointer; break;
		case EspJsonValueType::Value_String:delete (EspString*)ValuePointer; break;
		case EspJsonValueType::Value_Object:delete (EspJsonObject*)ValuePointer; break;
		case EspJsonValueType::Value_Array:delete (EspJsonArray*)ValuePointer; break;
		}
}
const EspJsonValue& EspJsonValue::operator=(const EspJsonValue& NewValue)
{
	this->PreFreeValue();
	switch (this->ValueType = NewValue.ValueType)
	{
	case EspJsonValueType::Value_Boolean:this->ValuePointer = new bool(*(bool*)NewValue.ValuePointer); break;
	case EspJsonValueType::Value_Number:this->ValuePointer = new double(*(double*)NewValue.ValuePointer); break;
	case EspJsonValueType::Value_String:this->ValuePointer = new EspString(*(EspString*)NewValue.ValuePointer); break;
	case EspJsonValueType::Value_Object:this->ValuePointer = new EspJsonObject(*(EspJsonObject*)NewValue.ValuePointer); break;
	case EspJsonValueType::Value_Array:this->ValuePointer = new EspJsonArray(*(EspJsonArray*)NewValue.ValuePointer); break;
	}
	return *this;
}
EspJsonValue& EspJsonValue::operator[](const EspString& Key) { return this->GetJsonObject().GetValue(Key); }
EspJsonValue& EspJsonValue::operator[](const unsigned int Index) { return this->GetJsonArray().GetValue(Index); }

EspString EspJsonObject::Synthesize()const
{
	if (this->JsonObject.IsEmptyOrNull())return EspString("{}");
	EspString Result("{");
	for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
	{
		//Key
		Result.Append('"').Append(this->JsonObject.GetElementAt(TimeNum).GetKey()).Append('"').Append(':');
		//Value
		switch (this->JsonObject.GetElementAt(TimeNum).GetValue().GetValueType())
		{
		case EspJsonValueType::Value_Boolean:this->JsonObject.GetElementAt(TimeNum).GetValue().GetBoolean() ? Result.Append("true") : Result.Append("false"); break;
		case EspJsonValueType::Value_Null:Result.Append("null"); break;
		case EspJsonValueType::Value_Number:Result.Append(EspString::ToString(this->JsonObject.GetElementAt(TimeNum).GetValue().GetNumber(), 20)); break;
		case EspJsonValueType::Value_String:Result.Append('"').Append(this->JsonObject.GetElementAt(TimeNum).GetValue().GetString()).Append('"'); break;
		case EspJsonValueType::Value_Object: Result.Append(this->JsonObject.GetElementAt(TimeNum).GetValue().GetJsonObject().Synthesize()); break;
		case EspJsonValueType::Value_Array:Result.Append(this->JsonObject.GetElementAt(TimeNum).GetValue().GetJsonArray().Synthesize()); break;
		}
		if (TimeNum != this->JsonObject.GetCount() - 1)
			Result.Append(',');
	}
	return Result.Append('}');
}
EspString EspJsonArray::Synthesize()const
{
	if (this->JsonArray.IsEmptyOrNull())return EspString("{}");
	EspString Result("[");
	for (unsigned int TimeNum = 0; TimeNum < this->JsonArray.GetCount(); TimeNum++)
	{
		//Value
		switch (this->JsonArray.GetElementAt(TimeNum).GetValueType())
		{
		case EspJsonValueType::Value_Boolean:this->JsonArray.GetElementAt(TimeNum).GetBoolean() ? Result.Append("true") : Result.Append("false"); break;
		case EspJsonValueType::Value_Null:Result.Append("null"); break;
		case EspJsonValueType::Value_Number:Result.Append(EspString::ToString(this->JsonArray.GetElementAt(TimeNum).GetNumber(), 20)); break;
		case EspJsonValueType::Value_String:Result.Append('"').Append(this->JsonArray.GetElementAt(TimeNum).GetString()).Append('"'); break;
		case EspJsonValueType::Value_Object: Result.Append(this->JsonArray.GetElementAt(TimeNum).GetJsonObject().Synthesize()); break;
		case EspJsonValueType::Value_Array:Result.Append(this->JsonArray.GetElementAt(TimeNum).GetJsonArray().Synthesize()); break;
		}
		if (TimeNum != this->JsonArray.GetCount() - 1)
			Result.Append(',');
	}
	return Result.Append(']');
}
