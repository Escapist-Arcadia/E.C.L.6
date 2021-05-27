#pragma once
#include<assert.h>
#include"EspString.hpp"
#include"EspArray.hpp"

class EspJsonValue;
class EspJsonMember;
class EspJsonParser;
class EspJsonObject;
class EspJsonArray;

enum class EspJsonValueType
{
	Value_Void = -1,
	Value_Boolean,
	Value_Null,
	Value_Number,
	Value_String,
	Value_Object,
	Value_Array
};
enum class EspJsonErrorCode
{
	NoError = 0,
	ERROR_INVALID_CHARACTER,
	ERROR_INVALID_NUMBER,
	ERROR_MISS_NUMBER,
	ERROR_NUMBER_TO_BIG,
	ERROR_MISS_QUOTE,
	ERROR_INVALID_ESCAPE_CHARACTER,
	ERROR_MISS_BRACE,
	ERROR_MISS_COLON,
	ERROR_MISS_COMMA,
	ERROR_MISS_BRACKET
};

class EspJsonValue
{
private:
	EspJsonValueType ValueType = EspJsonValueType::Value_Void;

public:
	EspJsonValueType GetValueType()const;
	const bool IsBoolean()const;
	const bool IsNull()const;
	const bool IsNumber()const;
	const bool IsString()const;
	const bool IsJsonObject()const;
	const bool IsJsonArray()const;


private:
	void* ValuePointer = NULL;

public:
	const bool IsVoid()const;
	void* GetValuePointer()const;
	bool GetBoolean()const;
	bool GetNull()const;
	double GetNumber()const;
	EspString GetString()const;
	EspJsonArray GetJsonArray()const;
	EspJsonObject GetJsonObject()const;

	void SetBoolean(const bool& BooleanValue);
	void SetNull();
	void SetNumber(const double& NumberValue);
	void SetString(const EspString& StringValue);
	void SetJsonObject(const EspJsonObject& JsonObject);
	void SetJsonArray(const EspJsonArray& JsonArray);

public:
	EspJsonValue();
	EspJsonValue(EspJsonValueType ValueType, void* ValuePointer);
	EspJsonValue(const EspJsonValue& JsonValue);
	void FreeValue();
	~EspJsonValue();
	EspJsonValue& operator=(const EspJsonValue& JsonValue);
};
class EspJsonMember
{
public:
	EspString Key;
	EspJsonValue Value;

	EspJsonMember();
	EspJsonMember(const EspString& Key, const EspJsonValue& Value);
	EspJsonMember(const EspJsonMember& JsonMember);
	EspJsonMember& operator=(const EspJsonMember& JsonMember);
};

class EspJsonObject
{
private:
	EspArray<EspJsonMember> JsonObject;

public:
	EspJsonObject();
	//EspJsonObject(const EspString& JsonString);
	EspJsonObject(const EspJsonObject& JsonObject);

	unsigned int IndexOf(const EspString& Key)const;
	bool HaveKey(const EspString& Key)const;
	unsigned int GetCount()const;
	bool IsEmpty()const;

	void AddBoolean(const EspString Key, const bool& BooleanValue);
	void AddNull(const EspString Key);
	void AddNumber(const EspString Key, const double& NumberValue);
	void AddString(const EspString Key, const EspString& StringValue);
	void AddJsonObject(const EspString Key, const EspJsonObject& JsonObject);
	void AddJsonArray(const EspString Key, const EspJsonArray& JsonObject);
	void AddMember(const EspString Key, const EspJsonValue& JsonValue);

	bool GetBoolean(const EspString& Key)const;
	bool GetNull(const EspString& Key);
	double GetNumber(const EspString& Key)const;
	EspString GetString(const EspString& Key)const;
	EspJsonObject GetJsonObject(const EspString& Key)const;
	EspJsonArray GetJsonArray(const EspString& Key)const;
	EspJsonValue& GetValue(const EspString& Key)const;

	const bool SetBoolean(const EspString& Key, const bool& BooleanValue);
	const bool SetNull(const EspString& Key);
	const bool SetNumber(const EspString& Key, const double& NumberValue);
	const bool SetString(const EspString& Key, const EspString& StringValue);
	const bool SetJsonObject(const EspString& Key, const EspJsonObject& JsonObject);
	const bool SetJsonArray(const EspString& Key, const EspJsonArray& JsonArray);

	void RemoveValue(const EspString& Key);
};
class EspJsonArray
{
private:
	EspArray<EspJsonValue> JsonArray;

public:
	EspJsonArray();
	EspJsonArray(const EspJsonArray& JsonArray);

	unsigned int GetCount()const;
	bool IsEmpty()const;

	void AddBoolean(const bool& BooleanValue);
	void AddNull();
	void AddNumber(const double& NumberValue);
	void AddString(const EspString& StringValue);
	void AddJsonObject(const EspJsonObject& JsonObject);
	void AddJsonArray(const EspJsonArray& JsonArray);
	void AddValue(const EspJsonValue& JsonValue);

	bool GetBoolean(unsigned int Index)const;
	bool GetNull(unsigned int Index);
	double GetNumber(unsigned int Index)const;
	EspString GetString(unsigned int Index)const;
	EspJsonObject GetJsonObject(unsigned int Index)const;
	EspJsonArray GetJsonArray(unsigned int Index)const;
	EspJsonValue& GetValue(unsigned int Index)const;

	const void SetBoolean(unsigned int Index, const bool& BooleanValue);
	const void SetNull(unsigned int Index);
	const void SetNumber(unsigned int Index, const double& NumberValue);
	const void SetString(unsigned int Index, const EspString& StringValue);
	const void SetJsonObject(unsigned int Index, const EspJsonObject& JsonObject);
	const void SetJsonArray(unsigned int Index, const EspJsonArray& JsonArray);

	void RemoveAt(const unsigned int Index);
};

class EspJsonParser
{
private:
	EspString JsonString;
	unsigned int ParsePos = 0;
	EspJsonErrorCode ErrorCode = EspJsonErrorCode::NoError;

	void CheckBlankSpace();
	const EspJsonValue ParseValue_Boolean_True();
	const EspJsonValue ParseValue_Boolean_False();
	const EspJsonValue ParseValue_Null();
	const EspJsonValue ParseValue_Number();
	const EspString ParseKey();
	const EspJsonValue ParseValue_String();
	const EspJsonValue ParseValue_Object();
	const EspJsonValue ParseValue_Array();

	const EspJsonValue ParseValue();
public:
	EspJsonParser();
	EspJsonParser(const EspString& JsonString);
	EspJsonObject Parse();
	const bool HasParseError()const;
	const EspJsonErrorCode GetParseError()const;
	const unsigned int GetParsePos()const;
};

EspJsonValueType EspJsonValue::GetValueType()const { return this->ValueType; }
const bool EspJsonValue::IsBoolean()const { return (this->ValueType == EspJsonValueType::Value_Boolean); }
const bool EspJsonValue::IsNull()const { return (this->ValueType == EspJsonValueType::Value_Null); }
const bool EspJsonValue::IsNumber()const { return (this->ValueType == EspJsonValueType::Value_Number); }
const bool EspJsonValue::IsString()const { return (this->ValueType == EspJsonValueType::Value_String); }
const bool EspJsonValue::IsJsonObject()const { return (this->ValueType == EspJsonValueType::Value_Object); }
const bool EspJsonValue::IsJsonArray()const { return (this->ValueType == EspJsonValueType::Value_Array); }
const bool EspJsonValue::IsVoid()const { return ((this->ValueType == EspJsonValueType::Value_Void) || (this->ValuePointer == NULL)); }
void* EspJsonValue::GetValuePointer()const { return this->ValuePointer; }
bool EspJsonValue::GetBoolean()const
{
	assert(this->ValueType == EspJsonValueType::Value_Boolean && this->ValuePointer != NULL);
	return *(bool*)this->ValuePointer;
}
bool EspJsonValue::GetNull()const
{
	assert(this->ValueType == EspJsonValueType::Value_Null && this->ValuePointer != NULL);
	return true;
}
double EspJsonValue::GetNumber()const
{
	assert(this->ValueType == EspJsonValueType::Value_Number && this->ValuePointer != NULL);
	return *(double*)this->ValuePointer;
}
EspString EspJsonValue::GetString()const
{
	assert(this->ValueType == EspJsonValueType::Value_String && this->ValuePointer != NULL);
	return *(EspString*)this->ValuePointer;
}
EspJsonArray EspJsonValue::GetJsonArray()const
{
	assert(this->ValueType == EspJsonValueType::Value_Array && this->ValuePointer != NULL);
	return *(EspJsonArray*)this->ValuePointer;
}
EspJsonObject EspJsonValue::GetJsonObject()const
{
	assert(this->ValueType == EspJsonValueType::Value_Object && this->ValuePointer != NULL);
	return *(EspJsonObject*)this->ValuePointer;
}
void EspJsonValue::SetBoolean(const bool& BooleanValue)
{
	FreeValue();
	this->ValueType = EspJsonValueType::Value_Boolean;
	this->ValuePointer = new bool(BooleanValue);
}
void EspJsonValue::SetNull()
{
	FreeValue();
	this->ValueType = EspJsonValueType::Value_Null;
	this->ValuePointer = NULL;
}
void EspJsonValue::SetNumber(const double& NumberValue)
{
	FreeValue();
	this->ValueType = EspJsonValueType::Value_Number;
	this->ValuePointer = new double(NumberValue);
}
void EspJsonValue::SetString(const EspString& StringValue)
{
	FreeValue();
	this->ValueType = EspJsonValueType::Value_String;
	this->ValuePointer = new EspString(StringValue);
}
void EspJsonValue::SetJsonObject(const EspJsonObject& JsonObject)
{
	FreeValue();
	this->ValueType = EspJsonValueType::Value_Object;
	this->ValuePointer = new EspJsonObject(JsonObject);
}
void EspJsonValue::SetJsonArray(const EspJsonArray& JsonArray)
{
	FreeValue();
	this->ValueType = EspJsonValueType::Value_Array;
	this->ValuePointer = new EspJsonArray(JsonArray);
}


EspJsonValue::EspJsonValue() {}
EspJsonValue::EspJsonValue(EspJsonValueType ValueType, void* ValuePointer)
{
	this->ValueType = ValueType;
	this->ValuePointer = ValuePointer;
}
EspJsonValue::EspJsonValue(const EspJsonValue& JsonValue)
{
	this->ValueType = JsonValue.ValueType;
	switch (JsonValue.ValueType)
	{
	case EspJsonValueType::Value_Boolean: this->ValuePointer = new bool(JsonValue.GetBoolean()); break;
	case EspJsonValueType::Value_Null:this->ValuePointer = NULL; break;
	case EspJsonValueType::Value_Number: this->ValuePointer = new double(JsonValue.GetNumber()); break;
	case EspJsonValueType::Value_String: this->ValuePointer = new EspString(JsonValue.GetString()); break;
	case EspJsonValueType::Value_Object: this->ValuePointer = new EspJsonObject(JsonValue.GetJsonObject()); break;
	case EspJsonValueType::Value_Array: this->ValuePointer = new EspJsonArray(JsonValue.GetJsonArray()); break;
	}
}
void EspJsonValue::FreeValue()
{
	if (!IsVoid())
		switch (GetValueType())
		{
		case EspJsonValueType::Value_Boolean:delete (bool*)this->ValuePointer; this->ValuePointer = NULL; break;
		case EspJsonValueType::Value_Null:this->ValuePointer = NULL; break;
		case EspJsonValueType::Value_Number:delete (double*)this->ValuePointer; this->ValuePointer = NULL; break;
		case EspJsonValueType::Value_String:delete (EspString*)this->ValuePointer; this->ValuePointer = NULL; break;
		case EspJsonValueType::Value_Object:delete (EspJsonObject*)this->ValuePointer; this->ValuePointer = NULL; break;
		case EspJsonValueType::Value_Array:delete (EspJsonArray*)this->ValuePointer; this->ValuePointer = NULL; break;
		}
}
EspJsonValue::~EspJsonValue() { this->FreeValue(); }
EspJsonValue& EspJsonValue::operator=(const EspJsonValue& JsonValue)
{
	FreeValue();
	this->ValueType = JsonValue.ValueType;
	switch (JsonValue.ValueType)
	{
	case EspJsonValueType::Value_Boolean: this->ValuePointer = new bool(JsonValue.GetBoolean()); break;
	case EspJsonValueType::Value_Null:this->ValuePointer = NULL; break;
	case EspJsonValueType::Value_Number: this->ValuePointer = new double(JsonValue.GetNumber()); break;
	case EspJsonValueType::Value_String: this->ValuePointer = new EspString(JsonValue.GetString()); break;
	case EspJsonValueType::Value_Object: this->ValuePointer = new EspJsonObject(JsonValue.GetJsonObject()); break;
	case EspJsonValueType::Value_Array: this->ValuePointer = new EspJsonArray(JsonValue.GetJsonArray()); break;
	}
	return *this;
}

EspJsonMember::EspJsonMember() {}
EspJsonMember::EspJsonMember(const EspString& Key, const EspJsonValue& Value)
{
	this->Key = Key;
	this->Value = Value;
}
EspJsonMember::EspJsonMember(const EspJsonMember& JsonMember)
{
	this->Key = JsonMember.Key;
	this->Value = JsonMember.Value;
}
EspJsonMember& EspJsonMember::operator=(const EspJsonMember& JsonMember)
{
	this->Key = JsonMember.Key;
	this->Value = JsonMember.Value;
	return *this;
}

EspJsonObject::EspJsonObject() {}
//EspJsonObject(const EspString& JsonString);
EspJsonObject::EspJsonObject(const EspJsonObject& JsonObject)
{
	this->JsonObject.AddArray(JsonObject.JsonObject);
}
unsigned int EspJsonObject::IndexOf(const EspString& Key)const
{
	if (JsonObject.IsEmpty() || Key.IsEmpty())
		return -1;
	for (unsigned int TimeNum = 0; TimeNum < JsonObject.GetCount(); TimeNum++)
		if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
			return TimeNum;
	return -1;
}
bool EspJsonObject::HaveKey(const EspString& Key)const { return this->IndexOf(Key) != -1; }
unsigned int EspJsonObject::GetCount()const { return JsonObject.GetCount(); }
bool EspJsonObject::IsEmpty()const { return JsonObject.IsEmptyOrNull(); }
void EspJsonObject::AddBoolean(const EspString Key, const bool& BooleanValue)
{
	if (!Key.IsEmpty())
		this->JsonObject.AddElement({ Key, EspJsonValue(EspJsonValueType::Value_Boolean,new bool(BooleanValue)) });
}
void EspJsonObject::AddNull(const EspString Key)
{
	if (!Key.IsEmpty())
		this->JsonObject.AddElement({ Key,EspJsonValue(EspJsonValueType::Value_Null,NULL) });
}
void EspJsonObject::AddNumber(const EspString Key, const double& NumberValue)
{
	if (!Key.IsEmpty())
		this->JsonObject.AddElement({ Key, EspJsonValue(EspJsonValueType::Value_Number,new double(NumberValue)) });
}
void EspJsonObject::AddString(const EspString Key, const EspString& StringValue)
{
	if (!Key.IsEmpty())
		this->JsonObject.AddElement({ Key, EspJsonValue(EspJsonValueType::Value_String,new EspString(StringValue)) });
}
void EspJsonObject::AddJsonObject(const EspString Key, const EspJsonObject& JsonObject)
{
	if (!Key.IsEmpty())
		this->JsonObject.AddElement({ Key, EspJsonValue(EspJsonValueType::Value_Object,new EspJsonObject(JsonObject)) });
}
void EspJsonObject::AddJsonArray(const EspString Key, const EspJsonArray& JsonObject)
{
	if (!Key.IsEmpty())
		this->JsonObject.AddElement({ Key, EspJsonValue(EspJsonValueType::Value_Array,new EspJsonArray(JsonObject)) });
}
void EspJsonObject::AddMember(const EspString Key, const EspJsonValue& JsonValue)
{
	if (!Key.IsEmpty())
		this->JsonObject.AddElement(EspJsonMember(Key, JsonValue));
}

bool EspJsonObject::GetBoolean(const EspString& Key)const
{
	if (!(Key.IsEmpty() || JsonObject.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key) && this->JsonObject.GetElementAt(TimeNum).Value.IsBoolean())
				return this->JsonObject.GetElementAt(TimeNum).Value.GetBoolean();
}
bool EspJsonObject::GetNull(const EspString& Key)
{
	if (!(Key.IsEmpty() || JsonObject.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key) && this->JsonObject.GetElementAt(TimeNum).Value.IsNull())
				return true;
	return false;
}
double EspJsonObject::GetNumber(const EspString& Key)const
{
	if (!(Key.IsEmpty() || JsonObject.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key) && this->JsonObject.GetElementAt(TimeNum).Value.IsNumber())
				return this->JsonObject.GetElementAt(TimeNum).Value.GetNumber();
}
EspString EspJsonObject::GetString(const EspString& Key)const
{
	if (!(Key.IsEmpty() || JsonObject.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key) && this->JsonObject.GetElementAt(TimeNum).Value.IsString())
				return this->JsonObject.GetElementAt(TimeNum).Value.GetString();
}
EspJsonObject EspJsonObject::GetJsonObject(const EspString& Key)const
{
	if (!(Key.IsEmpty() || JsonObject.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key) && this->JsonObject.GetElementAt(TimeNum).Value.IsJsonObject())
				return this->JsonObject.GetElementAt(TimeNum).Value.GetJsonObject();
}
EspJsonArray EspJsonObject::GetJsonArray(const EspString& Key)const
{
	if (!(Key.IsEmpty() || JsonObject.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key) && this->JsonObject.GetElementAt(TimeNum).Value.IsJsonArray())
				return this->JsonObject.GetElementAt(TimeNum).Value.GetJsonArray();
}
EspJsonValue& EspJsonObject::GetValue(const EspString& Key)const
{
	if (!(Key.IsEmpty() || JsonObject.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
				return this->JsonObject.GetElementAt(TimeNum).Value;
}


const bool EspJsonObject::SetBoolean(const EspString& Key, const bool& BooleanValue)
{
	if (!(this->JsonObject.IsEmpty() || Key.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
			{
				this->JsonObject.GetElementAt(TimeNum).Value.SetBoolean(BooleanValue);
				return true;
			}
	return false;
}
const bool EspJsonObject::SetNull(const EspString& Key)
{
	if (!(this->JsonObject.IsEmpty() || Key.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
			{
				this->JsonObject.GetElementAt(TimeNum).Value.SetNull();
				return true;
			}
	return false;
}
const bool EspJsonObject::SetNumber(const EspString& Key, const double& NumberValue)
{
	if (!(this->JsonObject.IsEmpty() || Key.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
			{
				this->JsonObject.GetElementAt(TimeNum).Value.SetNumber(NumberValue);
				return true;
			}
	return false;
}
const bool EspJsonObject::SetString(const EspString& Key, const EspString& StringValue)
{
	if (!(this->JsonObject.IsEmpty() || Key.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
			{
				this->JsonObject.GetElementAt(TimeNum).Value.SetString(StringValue);
				return true;
			}
	return false;
}
const bool EspJsonObject::SetJsonObject(const EspString& Key, const EspJsonObject& JsonObject)
{
	if (!(this->JsonObject.IsEmpty() || Key.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
			{
				this->JsonObject.GetElementAt(TimeNum).Value.SetJsonObject(JsonObject);
				return true;
			}
	return false;
}
const bool EspJsonObject::SetJsonArray(const EspString& Key, const EspJsonArray& JsonArray)
{
	if (!(this->JsonObject.IsEmpty() || Key.IsEmpty()))
		for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
			if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
			{
				this->JsonObject.GetElementAt(TimeNum).Value.SetJsonArray(JsonArray);
				return true;
			}
	return false;
}

void EspJsonObject::RemoveValue(const EspString& Key)
{
	if (this->JsonObject.IsEmpty() || Key.IsEmpty())return;
	for (unsigned int TimeNum = 0; TimeNum < this->JsonObject.GetCount(); TimeNum++)
		if (this->JsonObject.GetElementAt(TimeNum).Key.Compare(Key))
		{
			this->JsonObject.DeleteElement(TimeNum);
			return;
		}
}

EspJsonArray::EspJsonArray() {}
EspJsonArray::EspJsonArray(const EspJsonArray& JsonArray)
{
	this->JsonArray.AddArray(JsonArray.JsonArray);
}

unsigned int EspJsonArray::GetCount()const { return this->JsonArray.GetCount(); }
bool EspJsonArray::IsEmpty()const { return this->JsonArray.IsEmpty(); }

void EspJsonArray::AddBoolean(const bool& BooleanValue) { this->JsonArray.AddElement(EspJsonValue(EspJsonValueType::Value_Boolean, new bool(BooleanValue))); }
void EspJsonArray::AddNull() { this->JsonArray.AddElement(EspJsonValue(EspJsonValueType::Value_Null, NULL)); }
void EspJsonArray::AddNumber(const double& NumberValue) { this->JsonArray.AddElement(EspJsonValue(EspJsonValueType::Value_Number, new double(NumberValue))); }
void EspJsonArray::AddString(const EspString& StringValue) { this->JsonArray.AddElement(EspJsonValue(EspJsonValueType::Value_String, new EspString(StringValue))); }
void EspJsonArray::AddJsonObject(const EspJsonObject& JsonObject) { this->JsonArray.AddElement(EspJsonValue(EspJsonValueType::Value_Object, new EspJsonObject(JsonObject))); }
void EspJsonArray::AddJsonArray(const EspJsonArray& JsonArray) { this->JsonArray.AddElement(EspJsonValue(EspJsonValueType::Value_Array, new EspJsonArray(JsonArray))); }
void EspJsonArray::AddValue(const EspJsonValue& JsonValue) { this->JsonArray.AddElement(JsonValue); }

bool EspJsonArray::GetBoolean(unsigned int Index)const
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		return this->JsonArray.GetElementAt(Index).GetBoolean();
}
bool EspJsonArray::GetNull(unsigned int Index)
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		return this->JsonArray.GetElementAt(Index).GetNull();
}
double EspJsonArray::GetNumber(unsigned int Index)const
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		return this->JsonArray.GetElementAt(Index).GetNumber();
}
EspString EspJsonArray::GetString(unsigned int Index)const
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		return this->JsonArray.GetElementAt(Index).GetString();
}
EspJsonObject EspJsonArray::GetJsonObject(unsigned int Index)const
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		return this->JsonArray.GetElementAt(Index).GetJsonObject();
}
EspJsonArray EspJsonArray::GetJsonArray(unsigned int Index)const
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		return this->JsonArray.GetElementAt(Index).GetJsonArray();
}
EspJsonValue& EspJsonArray::GetValue(unsigned int Index)const
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		return this->JsonArray.GetElementAt(Index);
}

const void EspJsonArray::SetBoolean(unsigned int Index, const bool& BooleanValue)
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		this->JsonArray.GetElementAt(Index).SetBoolean(BooleanValue);
}
const void EspJsonArray::SetNull(unsigned int Index)
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		this->JsonArray.GetElementAt(Index).SetNull();
}
const void EspJsonArray::SetNumber(unsigned int Index, const double& NumberValue)
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		this->JsonArray.GetElementAt(Index).SetNumber(NumberValue);
}
const void EspJsonArray::SetString(unsigned int Index, const EspString& StringValue)
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		this->JsonArray.GetElementAt(Index).SetString(StringValue);
}
const void EspJsonArray::SetJsonObject(unsigned int Index, const EspJsonObject& JsonObject)
{
	if (Index < JsonArray.GetCount() && !JsonArray.IsEmptyOrNull())
		this->JsonArray.GetElementAt(Index).SetJsonObject(JsonObject);
}
const void EspJsonArray::SetJsonArray(unsigned int Index, const EspJsonArray& JsonArray)
{
	if (Index < this->JsonArray.GetCount() && !this->JsonArray.IsEmptyOrNull())
		this->JsonArray.GetElementAt(Index).SetJsonArray(JsonArray);
}

void EspJsonArray::RemoveAt(const unsigned int Index)
{
	this->JsonArray.DeleteElement(Index, 1);
}

void EspJsonParser::CheckBlankSpace()
{
	while (this->JsonString.GetCharAt(ParsePos) == ' ' ||
		this->JsonString.GetCharAt(ParsePos) == '\r' ||
		this->JsonString.GetCharAt(ParsePos) == '\n' ||
		this->JsonString.GetCharAt(ParsePos) == '\t')
		ParsePos++;
}
const EspJsonValue EspJsonParser::ParseValue_Boolean_True()
{
	CheckBlankSpace();
	if (this->JsonString.GetCharAt(ParsePos) == 't' &&
		this->JsonString.GetCharAt(ParsePos + 1) == 'r' &&
		this->JsonString.GetCharAt(ParsePos + 2) == 'u' &&
		this->JsonString.GetCharAt(ParsePos + 3) == 'e')
	{
		ParsePos += 4;
		return EspJsonValue(EspJsonValueType::Value_Boolean, new bool(true));
	}
	else
		this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_CHARACTER;
}
const EspJsonValue EspJsonParser::ParseValue_Boolean_False()
{
	CheckBlankSpace();
	if (this->JsonString.GetCharAt(ParsePos) == 'f' &&
		this->JsonString.GetCharAt(ParsePos + 1) == 'a' &&
		this->JsonString.GetCharAt(ParsePos + 2) == 'l' &&
		this->JsonString.GetCharAt(ParsePos + 3) == 's' &&
		this->JsonString.GetCharAt(ParsePos + 4) == 'e')
	{
		ParsePos += 5;
		return EspJsonValue(EspJsonValueType::Value_Boolean, new bool(false));
	}
	else
		this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_CHARACTER;
}
const EspJsonValue EspJsonParser::ParseValue_Null()
{
	CheckBlankSpace();
	if (this->JsonString.GetCharAt(ParsePos) == 'n' &&
		this->JsonString.GetCharAt(ParsePos + 1) == 'u' &&
		this->JsonString.GetCharAt(ParsePos + 2) == 'l' &&
		this->JsonString.GetCharAt(ParsePos + 3) == 'l')
	{
		ParsePos += 4;
		return EspJsonValue(EspJsonValueType::Value_Null, NULL);
	}
	else
		this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_CHARACTER;
}
const EspJsonValue EspJsonParser::ParseValue_Number()
{
	CheckBlankSpace();
	EspString NumberValue;
	if (this->JsonString.GetCharAt(ParsePos) == '-')
	{
		NumberValue.Append('-');
		ParsePos++;
	}
	if (this->JsonString.GetCharAt(ParsePos) == '0')
	{
		NumberValue.Append('0');
		ParsePos++;
	}
	else
	{
		if (!(this->JsonString.GetCharAt(ParsePos) >= '1' && this->JsonString.GetCharAt(ParsePos) <= '9'))
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_NUMBER;
			return EspJsonValue();
		}
		while (this->JsonString.GetCharAt(ParsePos) >= '0' && this->JsonString.GetCharAt(ParsePos) <= '9')
		{
			NumberValue.Append(this->JsonString.GetCharAt(ParsePos));
			ParsePos++;
		}
	}
	if (this->JsonString.GetCharAt(ParsePos) == '.')
	{
		NumberValue.Append('.');
		ParsePos++;
		if (!(this->JsonString.GetCharAt(ParsePos) >= '0' && this->JsonString.GetCharAt(ParsePos) <= '9'))
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_NUMBER;
			return EspJsonValue();
		}
		while (this->JsonString.GetCharAt(ParsePos) >= '0' && this->JsonString.GetCharAt(ParsePos) <= '9')
		{
			NumberValue.Append(this->JsonString.GetCharAt(ParsePos));
			ParsePos++;
		}
	}
	if (this->JsonString.GetCharAt(ParsePos) == 'e' || this->JsonString.GetCharAt(ParsePos) == 'E')
	{
		NumberValue.Append(this->JsonString.GetCharAt(ParsePos));
		ParsePos++;
		if (this->JsonString.GetCharAt(ParsePos) == '+' || this->JsonString.GetCharAt(ParsePos) == '-')
		{
			NumberValue.Append(this->JsonString.GetCharAt(ParsePos));
			ParsePos++;
		}
		if (!(this->JsonString.GetCharAt(ParsePos) >= '0' && this->JsonString.GetCharAt(ParsePos) <= '9'))
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_NUMBER;
			return EspJsonValue();
		}
		while (this->JsonString.GetCharAt(ParsePos) >= '0' && this->JsonString.GetCharAt(ParsePos) <= '9')
		{
			NumberValue.Append(this->JsonString.GetCharAt(ParsePos));
			ParsePos++;
		}
	}
	EspJsonValue Value = EspJsonValue(EspJsonValueType::Value_Number, new double(::strtod(NumberValue.GetAnsiStr(), NULL)));
	return Value;
}
const EspString EspJsonParser::ParseKey()
{
	if (this->JsonString.GetCharAt(ParsePos) != '"')
	{
		this->ErrorCode = EspJsonErrorCode::ERROR_MISS_QUOTE;
		return EspString();;
	}
	EspString String;
	while (true)
	{
		ParsePos++;
		if (this->JsonString.GetCharAt(ParsePos) == '\\')
		{
			ParsePos++;
			switch (this->JsonString.GetCharAt(ParsePos))
			{
			case'"':String.Append('"'); break;
			case'\\':String.Append('\\'); break;
			case'/':String.Append('/'); break;
			case'b':String.Append('\b'); break;
			case'f':String.Append('\f'); break;
			case'n':String.Append('\n'); break;
			case'r':String.Append('\r'); break;
			case't':String.Append('\t'); break;
			case'u':
			{

				break;
			}
			default:
				this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_ESCAPE_CHARACTER;
				return EspString();;
			}
		}
		else if (this->JsonString.GetCharAt(ParsePos) == '"')
		{
			ParsePos++;
			return String;
		}
		else if (this->JsonString.GetCharAt(ParsePos) == '\r' || this->JsonString.GetCharAt(ParsePos) == '\n')
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_CHARACTER;
			return EspString();;
		}
		else
			String.Append(this->JsonString.GetCharAt(ParsePos));
	}
}
const EspJsonValue EspJsonParser::ParseValue_String()
{
	if (this->JsonString.GetCharAt(ParsePos) != '"')
	{
		this->ErrorCode = EspJsonErrorCode::ERROR_MISS_QUOTE;
		return EspJsonValue();
	}
	EspString String;
	while (true)
	{
		ParsePos++;
		if (this->JsonString.GetCharAt(ParsePos) == '\\')
		{
			ParsePos++;
			switch (this->JsonString.GetCharAt(ParsePos))
			{
			case'"':String.Append('"'); break;
			case'\\':String.Append('\\'); break;
			case'/':String.Append('/'); break;
			case'b':String.Append('\b'); break;
			case'f':String.Append('\f'); break;
			case'n':String.Append('\n'); break;
			case'r':String.Append('\r'); break;
			case't':String.Append('\t'); break;
			case'u':
			{

				break;
			}
			default:
				this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_ESCAPE_CHARACTER;
				return EspJsonValue();
			}
		}
		else if (this->JsonString.GetCharAt(ParsePos) == '"')
		{
			ParsePos++;
			return EspJsonValue(EspJsonValueType::Value_String, new EspString(String));
		}
		else if (this->JsonString.GetCharAt(ParsePos) == '\r' || this->JsonString.GetCharAt(ParsePos) == '\n')
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_CHARACTER;
			return EspJsonValue();
		}
		else
			String.Append(this->JsonString.GetCharAt(ParsePos));
	}
}
const EspJsonValue EspJsonParser::ParseValue_Object()
{
	if (this->JsonString.GetCharAt(ParsePos) != '{')
	{
		this->ErrorCode = EspJsonErrorCode::ERROR_MISS_BRACE;
		return EspJsonValue();
	}
	CheckBlankSpace();
	ParsePos++;
	if (this->JsonString.GetCharAt(ParsePos) == '}')
	{
		ParsePos++;
		return EspJsonValue(EspJsonValueType::Value_Object, new EspJsonObject());
	}
	EspJsonObject JsonObject;
	while (true)
	{
		CheckBlankSpace();
		if (this->JsonString.GetCharAt(ParsePos) != '"')
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_MISS_QUOTE;
			return EspJsonValue();
		}
		const EspString	Key = ParseKey();
		if (this->ErrorCode != EspJsonErrorCode::NoError)
			return EspJsonValue();
		CheckBlankSpace();
		if (this->JsonString.GetCharAt(ParsePos) != ':')
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_MISS_COLON;
			return EspJsonValue();
		}
		ParsePos++;
		CheckBlankSpace();
		const EspJsonValue Value = ParseValue();
		if (this->ErrorCode != EspJsonErrorCode::NoError)
			return EspJsonValue();
		CheckBlankSpace();
		JsonObject.AddMember(Key, Value);
		if (this->JsonString.GetCharAt(ParsePos) == ',')
			ParsePos++;
		else if (this->JsonString.GetCharAt(ParsePos) == '}')
		{
			ParsePos++;
			return EspJsonValue(EspJsonValueType::Value_Object, new EspJsonObject(JsonObject));
		}
		else
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_MISS_COMMA;
			return EspJsonValue();
		}
	}
}
const EspJsonValue EspJsonParser::ParseValue_Array()
{
	if (this->JsonString.GetCharAt(ParsePos) != '[')
	{
		this->ErrorCode = EspJsonErrorCode::ERROR_MISS_BRACKET;
		return EspJsonValue();
	}
	CheckBlankSpace();
	ParsePos++;
	if (this->JsonString.GetCharAt(ParsePos) == ']')
	{
		ParsePos++;
		return EspJsonValue(EspJsonValueType::Value_Array, new EspJsonArray());
	}
	EspJsonArray JsonArray;
	while (true)
	{
		CheckBlankSpace();
		EspJsonErrorCode ErrorCode;
		const EspJsonValue Value = ParseValue();
		if (this->ErrorCode != EspJsonErrorCode::NoError)
			return EspJsonValue();
		CheckBlankSpace();
		JsonArray.AddValue(Value);
		if (this->JsonString.GetCharAt(ParsePos) == ',')
			ParsePos++;
		else if (this->JsonString.GetCharAt(ParsePos) == ']')
		{
			ParsePos++;
			return EspJsonValue(EspJsonValueType::Value_Array, new EspJsonArray(JsonArray));
		}
		else
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_MISS_COMMA;
			return EspJsonValue();
		}
	}
}
const EspJsonValue EspJsonParser::ParseValue()
{
	if (this->ErrorCode == EspJsonErrorCode::NoError)
		switch (this->JsonString.GetCharAt(this->ParsePos))
		{
		case 't':return ParseValue_Boolean_True(); break;
		case 'f':return ParseValue_Boolean_False(); break;
		case 'n':return ParseValue_Null(); break;
		case '-':case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8':case '9':return ParseValue_Number(); break;
		case '"':return ParseValue_String(); break;
		case '{':return ParseValue_Object(); break;
		case '[':return ParseValue_Array(); break;
		default:
			this->ErrorCode = EspJsonErrorCode::ERROR_INVALID_CHARACTER;
			return EspJsonValue();
		}
}
EspJsonParser::EspJsonParser() {}
EspJsonParser::EspJsonParser(const EspString& JsonString) { this->JsonString = JsonString; }
EspJsonObject EspJsonParser::Parse()
{
	if (this->JsonString.IsEmpty())
		return EspJsonObject();
	CheckBlankSpace();
	if (this->JsonString.GetCharAt(ParsePos) != '{')
	{
		this->ErrorCode = EspJsonErrorCode::ERROR_MISS_BRACE;
		return EspJsonObject();
	}
	CheckBlankSpace();
	ParsePos++;
	if (this->JsonString.GetCharAt(ParsePos) == '}')
		return EspJsonObject();
	EspJsonObject JsonObject;
	while (true)
	{
		CheckBlankSpace();
		if (this->JsonString.GetCharAt(ParsePos) != '"')
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_MISS_QUOTE;
			return EspJsonObject();
		}
		const EspString Key = ParseKey();
		if (this->ErrorCode != EspJsonErrorCode::NoError)
			return EspJsonObject();
		CheckBlankSpace();
		if (this->JsonString.GetCharAt(ParsePos) != ':')
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_MISS_COLON;
			return EspJsonObject();
		}
		ParsePos++;
		CheckBlankSpace();
		const EspJsonValue Value = ParseValue();
		if (this->ErrorCode != EspJsonErrorCode::NoError)
			return EspJsonObject();
		JsonObject.AddMember(Key, Value);
		if (this->JsonString.GetCharAt(ParsePos) == ',')
			ParsePos++;
		else if (this->JsonString.GetCharAt(ParsePos) == '}')
			return JsonObject;
		else
		{
			this->ErrorCode = EspJsonErrorCode::ERROR_MISS_COMMA;
			return EspJsonObject();
		}
	}
}
const bool EspJsonParser::HasParseError()const { return this->ErrorCode != EspJsonErrorCode::NoError; }
const EspJsonErrorCode EspJsonParser::GetParseError()const { return this->ErrorCode; }
const unsigned int EspJsonParser::GetParsePos()const { return this->ParsePos; }