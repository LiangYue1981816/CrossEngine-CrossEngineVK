/*
www.sourceforge.net/projects/tinyxml
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include <ctype.h>
#include <stdarg.h>
#include "tinyxml.h"

FILE* TiXmlFOpen(const char* filename, const char* mode);

bool TiXmlBase::condenseWhiteSpace = true;

// Microsoft compiler security
FILE* TiXmlFOpen(const char* filename, const char* mode)
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
	FILE* fp = 0;
	errno_t err = fopen_s(&fp, filename, mode);
	if (!err && fp)
		return fp;
	return 0;
#else
	return fopen( filename, mode );
#endif
}

void TiXmlBase::EncodeString(const TIXML_STRING& str, TIXML_STRING* outString)
{
	int i = 0;

	while (i < (int)str.length())
	{
		unsigned char c = (unsigned char)str[i];

		if (c == '&'
			&& i < ((int)str.length() - 2)
			&& str[i + 1] == '#'
			&& str[i + 2] == 'x')
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while (i < (int)str.length() - 1)
			{
				outString->append(str.c_str() + i, 1);
				++i;
				if (str[i] == ';')
					break;
			}
		}
		else if (c == '&')
		{
			outString->append(entity[0].str, entity[0].strLength);
			++i;
		}
		else if (c == '<')
		{
			outString->append(entity[1].str, entity[1].strLength);
			++i;
		}
		else if (c == '>')
		{
			outString->append(entity[2].str, entity[2].strLength);
			++i;
		}
		else if (c == '\"')
		{
			outString->append(entity[3].str, entity[3].strLength);
			++i;
		}
		else if (c == '\'')
		{
			outString->append(entity[4].str, entity[4].strLength);
			++i;
		}
		else if (c < 32)
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[32];

#if defined(TIXML_SNPRINTF)		
			TIXML_SNPRINTF(buf, sizeof(buf), "&#x%02X;", (unsigned)(c & 0xff));
#else
			sprintf( buf, "&#x%02X;", (unsigned) ( c & 0xff ) );
#endif		

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append(buf, (int)strlen(buf));
			++i;
		}
		else
		{
			//char realc = (char) c;
			//outString->append( &realc, 1 );
			*outString += (char)c;	// somewhat more efficient function call.
			++i;
		}
	}
}


TiXmlNode::TiXmlNode(NodeType _type) : TiXmlBase()
{
	parent = 0;
	type = _type;
	firstChild = 0;
	lastChild = 0;
	prev = 0;
	next = 0;
}


TiXmlNode::~TiXmlNode()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while (node)
	{
		temp = node;
		node = node->next;
		delete temp;
	}
}


void TiXmlNode::CopyTo(TiXmlNode* target) const
{
	target->SetValue(value.c_str());
	target->userData = userData;
	target->location = location;
}


void TiXmlNode::Clear()
{
	TiXmlNode* node = firstChild;
	TiXmlNode* temp = 0;

	while (node)
	{
		temp = node;
		node = node->next;
		delete temp;
	}

	firstChild = 0;
	lastChild = 0;
}


TiXmlNode* TiXmlNode::LinkEndChild(TiXmlNode* node)
{
	assert(node->parent == 0 || node->parent == this);
	assert(node->GetDocument() == 0 || node->GetDocument() == this->GetDocument());

	if (node->Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		delete node;
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}

	node->parent = this;

	node->prev = lastChild;
	node->next = 0;

	if (lastChild)
		lastChild->next = node;
	else
		firstChild = node;			// it was an empty list.

	lastChild = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertEndChild(const TiXmlNode& addThis)
{
	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}
	TiXmlNode* node = addThis.Clone();
	if (!node)
		return 0;

	return LinkEndChild(node);
}


TiXmlNode* TiXmlNode::InsertBeforeChild(TiXmlNode* beforeThis, const TiXmlNode& addThis)
{
	if (!beforeThis || beforeThis->parent != this) {
		return 0;
	}
	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}

	TiXmlNode* node = addThis.Clone();
	if (!node)
		return 0;
	node->parent = this;

	node->next = beforeThis;
	node->prev = beforeThis->prev;
	if (beforeThis->prev)
	{
		beforeThis->prev->next = node;
	}
	else
	{
		assert(firstChild == beforeThis);
		firstChild = node;
	}
	beforeThis->prev = node;
	return node;
}


TiXmlNode* TiXmlNode::InsertAfterChild(TiXmlNode* afterThis, const TiXmlNode& addThis)
{
	if (!afterThis || afterThis->parent != this) {
		return 0;
	}
	if (addThis.Type() == TiXmlNode::TINYXML_DOCUMENT)
	{
		if (GetDocument())
			GetDocument()->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}

	TiXmlNode* node = addThis.Clone();
	if (!node)
		return 0;
	node->parent = this;

	node->prev = afterThis;
	node->next = afterThis->next;
	if (afterThis->next)
	{
		afterThis->next->prev = node;
	}
	else
	{
		assert(lastChild == afterThis);
		lastChild = node;
	}
	afterThis->next = node;
	return node;
}


TiXmlNode* TiXmlNode::ReplaceChild(TiXmlNode* replaceThis, const TiXmlNode& withThis)
{
	if (!replaceThis)
		return 0;

	if (replaceThis->parent != this)
		return 0;

	if (withThis.ToDocument()) {
		// A document can never be a child.	Thanks to Noam.
		TiXmlDocument* document = GetDocument();
		if (document)
			document->SetError(TIXML_ERROR_DOCUMENT_TOP_ONLY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return 0;
	}

	TiXmlNode* node = withThis.Clone();
	if (!node)
		return 0;

	node->next = replaceThis->next;
	node->prev = replaceThis->prev;

	if (replaceThis->next)
		replaceThis->next->prev = node;
	else
		lastChild = node;

	if (replaceThis->prev)
		replaceThis->prev->next = node;
	else
		firstChild = node;

	delete replaceThis;
	node->parent = this;
	return node;
}


bool TiXmlNode::RemoveChild(TiXmlNode* removeThis)
{
	if (!removeThis) {
		return false;
	}

	if (removeThis->parent != this)
	{
		assert(0);
		return false;
	}

	if (removeThis->next)
		removeThis->next->prev = removeThis->prev;
	else
		lastChild = removeThis->prev;

	if (removeThis->prev)
		removeThis->prev->next = removeThis->next;
	else
		firstChild = removeThis->next;

	delete removeThis;
	return true;
}

const TiXmlNode* TiXmlNode::FirstChild(const char * _value) const
{
	const TiXmlNode* node;
	for (node = firstChild; node; node = node->next)
	{
		if (strcmp(node->Value(), _value) == 0)
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::LastChild(const char * _value) const
{
	const TiXmlNode* node;
	for (node = lastChild; node; node = node->prev)
	{
		if (strcmp(node->Value(), _value) == 0)
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::IterateChildren(const TiXmlNode* previous) const
{
	if (!previous)
	{
		return FirstChild();
	}
	else
	{
		assert(previous->parent == this);
		return previous->NextSibling();
	}
}


const TiXmlNode* TiXmlNode::IterateChildren(const char * val, const TiXmlNode* previous) const
{
	if (!previous)
	{
		return FirstChild(val);
	}
	else
	{
		assert(previous->parent == this);
		return previous->NextSibling(val);
	}
}


const TiXmlNode* TiXmlNode::NextSibling(const char * _value) const
{
	const TiXmlNode* node;
	for (node = next; node; node = node->next)
	{
		if (strcmp(node->Value(), _value) == 0)
			return node;
	}
	return 0;
}


const TiXmlNode* TiXmlNode::PreviousSibling(const char * _value) const
{
	const TiXmlNode* node;
	for (node = prev; node; node = node->prev)
	{
		if (strcmp(node->Value(), _value) == 0)
			return node;
	}
	return 0;
}


void TiXmlElement::RemoveAttribute(const char * name)
{
	TiXmlAttribute* node = attributeSet.Find(name);

	if (node)
	{
		attributeSet.Remove(node);
		delete node;
	}
}

const TiXmlElement* TiXmlNode::FirstChildElement() const
{
	const TiXmlNode* node;

	for (node = FirstChild();
		node;
		node = node->NextSibling())
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::FirstChildElement(const char * _value) const
{
	const TiXmlNode* node;

	for (node = FirstChild(_value);
		node;
		node = node->NextSibling(_value))
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::NextSiblingElement() const
{
	const TiXmlNode* node;

	for (node = NextSibling();
		node;
		node = node->NextSibling())
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}


const TiXmlElement* TiXmlNode::NextSiblingElement(const char * _value) const
{
	const TiXmlNode* node;

	for (node = NextSibling(_value);
		node;
		node = node->NextSibling(_value))
	{
		if (node->ToElement())
			return node->ToElement();
	}
	return 0;
}


const TiXmlDocument* TiXmlNode::GetDocument() const
{
	const TiXmlNode* node;

	for (node = this; node; node = node->parent)
	{
		if (node->ToDocument())
			return node->ToDocument();
	}
	return 0;
}


TiXmlElement::TiXmlElement(const char * _value)
	: TiXmlNode(TiXmlNode::TINYXML_ELEMENT)
{
	firstChild = lastChild = 0;
	value = _value;
}


TiXmlElement::TiXmlElement(const TiXmlElement& copy)
	: TiXmlNode(TiXmlNode::TINYXML_ELEMENT)
{
	firstChild = lastChild = 0;
	copy.CopyTo(this);
}


TiXmlElement& TiXmlElement::operator=(const TiXmlElement& base)
{
	ClearThis();
	base.CopyTo(this);
	return *this;
}


TiXmlElement::~TiXmlElement()
{
	ClearThis();
}


void TiXmlElement::ClearThis()
{
	Clear();
	while (attributeSet.First())
	{
		TiXmlAttribute* node = attributeSet.First();
		attributeSet.Remove(node);
		delete node;
	}
}


const char* TiXmlElement::AttributeString(const char* name) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	const char* result = 0;

	if (attrib) {
		result = attrib->Value();
	}

	return result;
}


int TiXmlElement::AttributeBool(const char* name) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	int result = 0;

	if (attrib) {
		attrib->QueryBoolValue(&result);
	}

	return result;
}


int TiXmlElement::AttributeInt1(const char* name) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	int result = 0;

	if (attrib) {
		attrib->QueryIntValue1(&result);
	}

	return result;
}


int* TiXmlElement::AttributeInt2(const char* name, int *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(int) * 2);

	if (attrib) {
		attrib->QueryIntValue2(&result[0], &result[1]);
	}

	return result;
}


int* TiXmlElement::AttributeInt3(const char* name, int *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(int) * 3);

	if (attrib) {
		attrib->QueryIntValue3(&result[0], &result[1], &result[2]);
	}

	return result;
}


int* TiXmlElement::AttributeInt4(const char* name, int *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(int) * 4);

	if (attrib) {
		attrib->QueryIntValue4(&result[0], &result[1], &result[2], &result[3]);
	}

	return result;
}


float TiXmlElement::AttributeFloat1(const char* name) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	float result = 0.0f;

	if (attrib) {
		attrib->QueryFloatValue1(&result);
	}

	return result;
}


float* TiXmlElement::AttributeFloat2(const char* name, float *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(float) * 2);

	if (attrib) {
		attrib->QueryFloatValue2(&result[0], &result[1]);
	}

	return result;
}


float* TiXmlElement::AttributeFloat3(const char* name, float *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(float) * 3);

	if (attrib) {
		attrib->QueryFloatValue3(&result[0], &result[1], &result[2]);
	}

	return result;
}


float* TiXmlElement::AttributeFloat4(const char* name, float *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(float) * 4);

	if (attrib) {
		attrib->QueryFloatValue4(&result[0], &result[1], &result[2], &result[3]);
	}

	return result;
}


double TiXmlElement::AttributeDouble1(const char* name) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	double result = 0.0;

	if (attrib) {
		attrib->QueryDoubleValue1(&result);
	}

	return result;
}


double* TiXmlElement::AttributeDouble2(const char* name, double *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(double) * 2);

	if (attrib) {
		attrib->QueryDoubleValue2(&result[0], &result[1]);
	}

	return result;
}


double* TiXmlElement::AttributeDouble3(const char* name, double *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(double) * 3);

	if (attrib) {
		attrib->QueryDoubleValue3(&result[0], &result[1], &result[2]);
	}

	return result;
}


double* TiXmlElement::AttributeDouble4(const char* name, double *result) const
{
	const TiXmlAttribute* attrib = attributeSet.Find(name);
	memset(result, 0, sizeof(double) * 4);

	if (attrib) {
		attrib->QueryDoubleValue4(&result[0], &result[1], &result[2], &result[3]);
	}

	return result;
}


void TiXmlElement::SetAttributeBool(const char * name, int val)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetBoolValue(val);
	}
}


void TiXmlElement::SetAttributeInt1(const char * name, int val1)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetIntValue1(val1);
	}
}


void TiXmlElement::SetAttributeInt2(const char * name, int val1, int val2)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetIntValue2(val1, val2);
	}
}

void TiXmlElement::SetAttributeInt3(const char * name, int val1, int val2, int val3)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetIntValue3(val1, val2, val3);
	}
}

void TiXmlElement::SetAttributeInt4(const char * name, int val1, int val2, int val3, int val4)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetIntValue4(val1, val2, val3, val4);
	}
}


void TiXmlElement::SetAttributeFloat1(const char * name, float val1)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetFloatValue1(val1);
	}
}

void TiXmlElement::SetAttributeFloat2(const char * name, float val1, float val2)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetFloatValue2(val1, val2);
	}
}

void TiXmlElement::SetAttributeFloat3(const char * name, float val1, float val2, float val3)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetFloatValue3(val1, val2, val3);
	}
}

void TiXmlElement::SetAttributeFloat4(const char * name, float val1, float val2, float val3, float val4)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetFloatValue4(val1, val2, val3, val4);
	}
}


void TiXmlElement::SetAttributeDouble1(const char * name, double val1)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetDoubleValue1(val1);
	}
}

void TiXmlElement::SetAttributeDouble2(const char * name, double val1, double val2)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetDoubleValue2(val1, val2);
	}
}

void TiXmlElement::SetAttributeDouble3(const char * name, double val1, double val2, double val3)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetDoubleValue3(val1, val2, val3);
	}
}

void TiXmlElement::SetAttributeDouble4(const char * name, double val1, double val2, double val3, double val4)
{
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		attrib->SetDoubleValue4(val1, val2, val3, val4);
	}
}


void TiXmlElement::SetAttributeString(const char* name, const char * format, ...)
{
	static char value[1024 * 1024];
	TiXmlAttribute* attrib = attributeSet.FindOrCreate(name);
	if (attrib) {
		va_list vaList;
		va_start(vaList, format);
		vsprintf(value, format, vaList);
		va_end(vaList);
		attrib->SetValue(value);
	}
}


void TiXmlElement::Print(FILE* cfile, int depth) const
{
	int i;
	assert(cfile);
	for (i = 0; i < depth; i++) {
		fprintf(cfile, "    ");
	}

	fprintf(cfile, "<%s", value.c_str());

	const TiXmlAttribute* attrib;
	for (attrib = attributeSet.First(); attrib; attrib = attrib->Next())
	{
		fprintf(cfile, " ");
		attrib->Print(cfile, depth);
	}

	// There are 3 different formatting approaches:
	// 1) An element without children is printed as a <foo /> node
	// 2) An element with only a text child is printed as <foo> text </foo>
	// 3) An element with children is printed on multiple lines.
	TiXmlNode* node;
	if (!firstChild)
	{
		fprintf(cfile, " />");
	}
	else if (firstChild == lastChild && firstChild->ToText())
	{
		fprintf(cfile, ">");
		firstChild->Print(cfile, depth + 1);
		fprintf(cfile, "</%s>", value.c_str());
	}
	else
	{
		fprintf(cfile, ">");

		for (node = firstChild; node; node = node->NextSibling())
		{
			if (!node->ToText())
			{
				fprintf(cfile, "\n");
			}
			node->Print(cfile, depth + 1);
		}
		fprintf(cfile, "\n");
		for (i = 0; i < depth; ++i) {
			fprintf(cfile, "    ");
		}
		fprintf(cfile, "</%s>", value.c_str());
	}
}


void TiXmlElement::CopyTo(TiXmlElement* target) const
{
	// superclass:
	TiXmlNode::CopyTo(target);

	// Element class: 
	// Clone the attributes, then clone the children.
	const TiXmlAttribute* attribute = 0;
	for (attribute = attributeSet.First();
		attribute;
		attribute = attribute->Next())
	{
		target->SetAttributeString(attribute->Name(), attribute->Value());
	}

	TiXmlNode* node = 0;
	for (node = firstChild; node; node = node->NextSibling())
	{
		target->LinkEndChild(node->Clone());
	}
}

bool TiXmlElement::Accept(TiXmlVisitor* visitor) const
{
	if (visitor->VisitEnter(*this, attributeSet.First()))
	{
		for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
		{
			if (!node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit(*this);
}


TiXmlNode* TiXmlElement::Clone() const
{
	TiXmlElement* clone = new TiXmlElement(Value());
	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}


const char* TiXmlElement::GetText() const
{
	const TiXmlNode* child = this->FirstChild();
	if (child) {
		const TiXmlText* childText = child->ToText();
		if (childText) {
			return childText->Value();
		}
	}
	return 0;
}


TiXmlDocument::TiXmlDocument() : TiXmlNode(TiXmlNode::TINYXML_DOCUMENT)
{
	tabsize = 4;
	useMicrosoftBOM = false;
	ClearError();
}

TiXmlDocument::TiXmlDocument(const char * documentName) : TiXmlNode(TiXmlNode::TINYXML_DOCUMENT)
{
	tabsize = 4;
	useMicrosoftBOM = false;
	value = documentName;
	ClearError();
}


TiXmlDocument::TiXmlDocument(const TiXmlDocument& copy) : TiXmlNode(TiXmlNode::TINYXML_DOCUMENT)
{
	copy.CopyTo(this);
}


TiXmlDocument& TiXmlDocument::operator=(const TiXmlDocument& copy)
{
	Clear();
	copy.CopyTo(this);
	return *this;
}


bool TiXmlDocument::LoadFile(TiXmlEncoding encoding)
{
	return LoadFile(Value(), encoding);
}


bool TiXmlDocument::SaveFile() const
{
	return SaveFile(Value());
}


bool TiXmlDocument::LoadFile(const char* _filename, TiXmlEncoding encoding)
{
	TIXML_STRING filename(_filename);
	value = filename;

	// reading in binary mode so that tinyxml can normalize the EOL
	FILE* file = TiXmlFOpen(value.c_str(), "rb");

	if (file)
	{
		bool result = LoadFile(file, encoding);
		fclose(file);
		return result;
	}
	else
	{
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}
}


bool TiXmlDocument::LoadFile(FILE* file, TiXmlEncoding encoding)
{
	if (!file)
	{
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	// Delete the existing data:
	Clear();
	location.Clear();

	// Get the file size, so we can pre-allocate the string. HUGE speed impact.
	long length = 0;
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Strange case, but good to handle up front.
	if (length <= 0)
	{
		SetError(TIXML_ERROR_DOCUMENT_EMPTY, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	// Subtle bug here. TinyXml did use fgets. But from the XML spec:
	// 2.11 End-of-Line Handling
	// <snip>
	// <quote>
	// ...the XML processor MUST behave as if it normalized all line breaks in external 
	// parsed entities (including the document entity) on input, before parsing, by translating 
	// both the two-character sequence #xD #xA and any #xD that is not followed by #xA to 
	// a single #xA character.
	// </quote>
	//
	// It is not clear fgets does that, and certainly isn't clear it works cross platform. 
	// Generally, you expect fgets to translate from the convention of the OS to the c/unix
	// convention, and not work generally.

	/*
	while( fgets( buf, sizeof(buf), file ) )
	{
	data += buf;
	}
	*/

	char* buf = new char[length + 1];
	buf[0] = 0;

	if (fread(buf, length, 1, file) != 1) {
		delete[] buf;
		SetError(TIXML_ERROR_OPENING_FILE, 0, 0, TIXML_ENCODING_UNKNOWN);
		return false;
	}

	bool result = LoadFile(buf, length, encoding);
	delete[] buf;
	return result;
}


bool TiXmlDocument::LoadFile(char* text, long length, TiXmlEncoding encoding)
{
	// Process the buffer in place to normalize new lines. (See comment above.)
	// Copies from the 'p' to 'q' pointer, where p can advance faster if
	// a newline-carriage return is hit.
	//
	// Wikipedia:
	// Systems based on ASCII or a compatible character set use either LF  (Line feed, '\n', 0x0A, 10 in decimal) or 
	// CR (Carriage return, '\r', 0x0D, 13 in decimal) individually, or CR followed by LF (CR+LF, 0x0D 0x0A)...
	//		* LF:    Multics, Unix and Unix-like systems (GNU/Linux, AIX, Xenix, Mac OS X, FreeBSD, etc.), BeOS, Amiga, RISC OS, and others
	//		* CR+LF: DEC RT-11 and most other early non-Unix, non-IBM OSes, CP/M, MP/M, DOS, OS/2, Microsoft Windows, Symbian OS
	//		* CR:    Commodore 8-bit machines, Apple II family, Mac OS up to version 9 and OS-9

	char* buf = new char[length + 1];
	memcpy(buf, text, length);

	const char* p = buf;	// the read head
	char* q = buf;			// the write head
	const char CR = 0x0d;
	const char LF = 0x0a;

	buf[length] = 0;
	while (*p) {
		assert(p < (buf + length));
		assert(q <= (buf + length));
		assert(q <= p);

		if (*p == CR) {
			*q++ = LF;
			p++;
			if (*p == LF) {		// check for CR+LF (and skip LF)
				p++;
			}
		}
		else {
			*q++ = *p++;
		}
	}
	assert(q <= (buf + length));
	*q = 0;

	Parse(buf, 0, encoding);

	delete[] buf;
	return !Error();
}


bool TiXmlDocument::SaveFile(const char * filename) const
{
	// The old c stuff lives on...
	FILE* fp = TiXmlFOpen(filename, "w");
	if (fp)
	{
		bool result = SaveFile(fp);
		fclose(fp);
		return result;
	}
	return false;
}


bool TiXmlDocument::SaveFile(FILE* fp) const
{
	if (useMicrosoftBOM)
	{
		const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
		const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
		const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;

		fputc(TIXML_UTF_LEAD_0, fp);
		fputc(TIXML_UTF_LEAD_1, fp);
		fputc(TIXML_UTF_LEAD_2, fp);
	}
	Print(fp, 0);
	return (ferror(fp) == 0);
}


void TiXmlDocument::CopyTo(TiXmlDocument* target) const
{
	TiXmlNode::CopyTo(target);

	target->error = error;
	target->errorId = errorId;
	target->errorDesc = errorDesc;
	target->tabsize = tabsize;
	target->errorLocation = errorLocation;
	target->useMicrosoftBOM = useMicrosoftBOM;

	TiXmlNode* node = 0;
	for (node = firstChild; node; node = node->NextSibling())
	{
		target->LinkEndChild(node->Clone());
	}
}


TiXmlNode* TiXmlDocument::Clone() const
{
	TiXmlDocument* clone = new TiXmlDocument();
	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}


void TiXmlDocument::Print(FILE* cfile, int depth) const
{
	assert(cfile);
	for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
	{
		node->Print(cfile, depth);
		fprintf(cfile, "\n");
	}
}


bool TiXmlDocument::Accept(TiXmlVisitor* visitor) const
{
	if (visitor->VisitEnter(*this))
	{
		for (const TiXmlNode* node = FirstChild(); node; node = node->NextSibling())
		{
			if (!node->Accept(visitor))
				break;
		}
	}
	return visitor->VisitExit(*this);
}


const TiXmlAttribute* TiXmlAttribute::Next() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (next->value.empty() && next->name.empty())
		return 0;
	return next;
}

/*
TiXmlAttribute* TiXmlAttribute::Next()
{
// We are using knowledge of the sentinel. The sentinel
// have a value or name.
if ( next->value.empty() && next->name.empty() )
return 0;
return next;
}
*/

const TiXmlAttribute* TiXmlAttribute::Previous() const
{
	// We are using knowledge of the sentinel. The sentinel
	// have a value or name.
	if (prev->value.empty() && prev->name.empty())
		return 0;
	return prev;
}

/*
TiXmlAttribute* TiXmlAttribute::Previous()
{
// We are using knowledge of the sentinel. The sentinel
// have a value or name.
if ( prev->value.empty() && prev->name.empty() )
return 0;
return prev;
}
*/

void TiXmlAttribute::Print(FILE* cfile, int /*depth*/, TIXML_STRING* str) const
{
	TIXML_STRING n, v;

	EncodeString(name, &n);
	EncodeString(value, &v);

	if (value.find('\"') == TIXML_STRING::npos) {
		if (cfile) {
			fprintf(cfile, "%s=\"%s\"", n.c_str(), v.c_str());
		}
		if (str) {
			(*str) += n; (*str) += "=\""; (*str) += v; (*str) += "\"";
		}
	}
	else {
		if (cfile) {
			fprintf(cfile, "%s='%s'", n.c_str(), v.c_str());
		}
		if (str) {
			(*str) += n; (*str) += "='"; (*str) += v; (*str) += "'";
		}
	}
}


int TiXmlAttribute::QueryBoolValue(int* _value) const
{
	if (stricmp(value.c_str(), "1") == 0) {
		*_value = 1;
		return TIXML_SUCCESS;
	}

	if (stricmp(value.c_str(), "yes") == 0) {
		*_value = 1;
		return TIXML_SUCCESS;
	}

	if (stricmp(value.c_str(), "true") == 0) {
		*_value = 1;
		return TIXML_SUCCESS;
	}

	if (stricmp(value.c_str(), "0") == 0) {
		*_value = 0;
		return TIXML_SUCCESS;
	}

	if (stricmp(value.c_str(), "no") == 0) {
		*_value = 0;
		return TIXML_SUCCESS;
	}

	if (stricmp(value.c_str(), "false") == 0) {
		*_value = 0;
		return TIXML_SUCCESS;
	}

	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryIntValue1(int* ival1) const
{
	if (TIXML_SSCANF(value.c_str(), "%d", ival1) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryIntValue2(int* ival1, int* ival2) const
{
	if (TIXML_SSCANF(value.c_str(), "%d %d", ival1, ival2) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryIntValue3(int* ival1, int* ival2, int* ival3) const
{
	if (TIXML_SSCANF(value.c_str(), "%d %d %d", ival1, ival2, ival3) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryIntValue4(int* ival1, int* ival2, int* ival3, int* ival4) const
{
	if (TIXML_SSCANF(value.c_str(), "%d %d %d %d", ival1, ival2, ival3, ival4) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryFloatValue1(float* fval1) const
{
	if (TIXML_SSCANF(value.c_str(), "%f", fval1) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryFloatValue2(float* fval1, float *fval2) const
{
	if (TIXML_SSCANF(value.c_str(), "%f %f", fval1, fval2) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryFloatValue3(float* fval1, float *fval2, float *fval3) const
{
	if (TIXML_SSCANF(value.c_str(), "%f %f %f", fval1, fval2, fval3) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryFloatValue4(float* fval1, float *fval2, float *fval3, float *fval4) const
{
	if (TIXML_SSCANF(value.c_str(), "%f %f %f %f", fval1, fval2, fval3, fval4) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryDoubleValue1(double* dval1) const
{
	if (TIXML_SSCANF(value.c_str(), "%lf", dval1) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryDoubleValue2(double* dval1, double* dval2) const
{
	if (TIXML_SSCANF(value.c_str(), "%lf %lf", dval1, dval2) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryDoubleValue3(double* dval1, double* dval2, double* dval3) const
{
	if (TIXML_SSCANF(value.c_str(), "%lf %lf %lf", dval1, dval2, dval3) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

int TiXmlAttribute::QueryDoubleValue4(double* dval1, double* dval2, double* dval3, double* dval4) const
{
	if (TIXML_SSCANF(value.c_str(), "%lf %lf %lf %lf", dval1, dval2, dval3, dval4) == 1)
		return TIXML_SUCCESS;
	return TIXML_WRONG_TYPE;
}

void TiXmlAttribute::SetBoolValue(int _value)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	if (_value) {
		TIXML_SNPRINTF(buf, sizeof(buf), "true");
	}
	else {
		TIXML_SNPRINTF(buf, sizeof(buf), "false");
	}
#else
	if (_value) {
		sprintf(buf, sizeof(buf), "true");
	}
	else {
		sprintf(buf, sizeof(buf), "false");
	}
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetIntValue1(int _value1)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%d", _value1);
#else
	sprintf (buf, "%d", _value1);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetIntValue2(int _value1, int _value2)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%d %d", _value1, _value2);
#else
	sprintf(buf, "%d %d", _value1, _value2);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetIntValue3(int _value1, int _value2, int _value3)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%d %d %d", _value1, _value2, _value3);
#else
	sprintf(buf, "%d %d %d", _value1, _value2, _value3);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetIntValue4(int _value1, int _value2, int _value3, int _value4)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%d %d %d %d", _value1, _value2, _value3, _value4);
#else
	sprintf(buf, "%d %d %d %d", _value1, _value2, _value3, _value4);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetFloatValue1(float _value1)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%f", _value1);
#else
	sprintf(buf, "%f", _value1);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetFloatValue2(float _value1, float _value2)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%f %f", _value1, _value2);
#else
	sprintf(buf, "%f %f", _value1, _value2);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetFloatValue3(float _value1, float _value2, float _value3)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%f %f %f", _value1, _value2, _value3);
#else
	sprintf(buf, "%f %f %f", _value1, _value2, _value3);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetFloatValue4(float _value1, float _value2, float _value3, float _value4)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)		
	TIXML_SNPRINTF(buf, sizeof(buf), "%f %f %f %f", _value1, _value2, _value3, _value4);
#else
	sprintf(buf, "%f %f %f %f", _value1, _value2, _value3, _value4);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetDoubleValue1(double _value1)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)
	TIXML_SNPRINTF(buf, sizeof(buf), "%g", _value1);
#else
	sprintf (buf, "%g", _value1);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetDoubleValue2(double _value1, double _value2)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)
	TIXML_SNPRINTF(buf, sizeof(buf), "%g %g", _value1, _value2);
#else
	sprintf(buf, "%g %g", _value1, _value2);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetDoubleValue3(double _value1, double _value2, double _value3)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)
	TIXML_SNPRINTF(buf, sizeof(buf), "%g %g %g", _value1, _value2, _value3);
#else
	sprintf(buf, "%g %g %g", _value1, _value2, _value3);
#endif
	SetValue(buf);
}

void TiXmlAttribute::SetDoubleValue4(double _value1, double _value2, double _value3, double _value4)
{
	char buf[256];
#if defined(TIXML_SNPRINTF)
	TIXML_SNPRINTF(buf, sizeof(buf), "%g %g %g %g", _value1, _value2, _value3, _value4);
#else
	sprintf(buf, "%g %g %g %g", _value1, _value2, _value3, _value4);
#endif
	SetValue(buf);
}


TiXmlComment::TiXmlComment(const TiXmlComment& copy) : TiXmlNode(TiXmlNode::TINYXML_COMMENT)
{
	copy.CopyTo(this);
}


TiXmlComment& TiXmlComment::operator=(const TiXmlComment& base)
{
	Clear();
	base.CopyTo(this);
	return *this;
}


void TiXmlComment::Print(FILE* cfile, int depth) const
{
	assert(cfile);
	for (int i = 0; i < depth; i++)
	{
		fprintf(cfile, "    ");
	}
	fprintf(cfile, "<!--%s-->", value.c_str());
}


void TiXmlComment::CopyTo(TiXmlComment* target) const
{
	TiXmlNode::CopyTo(target);
}


bool TiXmlComment::Accept(TiXmlVisitor* visitor) const
{
	return visitor->Visit(*this);
}


TiXmlNode* TiXmlComment::Clone() const
{
	TiXmlComment* clone = new TiXmlComment();

	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}


void TiXmlText::Print(FILE* cfile, int depth) const
{
	assert(cfile);
	if (cdata)
	{
		int i;
		fprintf(cfile, "\n");
		for (i = 0; i < depth; i++) {
			fprintf(cfile, "    ");
		}
		fprintf(cfile, "<![CDATA[%s]]>\n", value.c_str());	// unformatted output
	}
	else
	{
		TIXML_STRING buffer;
		EncodeString(value, &buffer);
		fprintf(cfile, "%s", buffer.c_str());
	}
}


void TiXmlText::CopyTo(TiXmlText* target) const
{
	TiXmlNode::CopyTo(target);
	target->cdata = cdata;
}


bool TiXmlText::Accept(TiXmlVisitor* visitor) const
{
	return visitor->Visit(*this);
}


TiXmlNode* TiXmlText::Clone() const
{
	TiXmlText* clone = 0;
	clone = new TiXmlText("");

	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}


TiXmlDeclaration::TiXmlDeclaration(const char * _version,
	const char * _encoding,
	const char * _standalone)
	: TiXmlNode(TiXmlNode::TINYXML_DECLARATION)
{
	version = _version;
	encoding = _encoding;
	standalone = _standalone;
}


TiXmlDeclaration::TiXmlDeclaration(const TiXmlDeclaration& copy)
	: TiXmlNode(TiXmlNode::TINYXML_DECLARATION)
{
	copy.CopyTo(this);
}


TiXmlDeclaration& TiXmlDeclaration::operator=(const TiXmlDeclaration& copy)
{
	Clear();
	copy.CopyTo(this);
	return *this;
}


void TiXmlDeclaration::Print(FILE* cfile, int /*depth*/, TIXML_STRING* str) const
{
	if (cfile) fprintf(cfile, "<?xml ");
	if (str)	 (*str) += "<?xml ";

	if (!version.empty()) {
		if (cfile) fprintf(cfile, "version=\"%s\" ", version.c_str());
		if (str) { (*str) += "version=\""; (*str) += version; (*str) += "\" "; }
	}
	if (!encoding.empty()) {
		if (cfile) fprintf(cfile, "encoding=\"%s\" ", encoding.c_str());
		if (str) { (*str) += "encoding=\""; (*str) += encoding; (*str) += "\" "; }
	}
	if (!standalone.empty()) {
		if (cfile) fprintf(cfile, "standalone=\"%s\" ", standalone.c_str());
		if (str) { (*str) += "standalone=\""; (*str) += standalone; (*str) += "\" "; }
	}
	if (cfile) fprintf(cfile, "?>");
	if (str)	 (*str) += "?>";
}


void TiXmlDeclaration::CopyTo(TiXmlDeclaration* target) const
{
	TiXmlNode::CopyTo(target);

	target->version = version;
	target->encoding = encoding;
	target->standalone = standalone;
}


bool TiXmlDeclaration::Accept(TiXmlVisitor* visitor) const
{
	return visitor->Visit(*this);
}


TiXmlNode* TiXmlDeclaration::Clone() const
{
	TiXmlDeclaration* clone = new TiXmlDeclaration();

	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}


void TiXmlUnknown::Print(FILE* cfile, int depth) const
{
	for (int i = 0; i < depth; i++)
		fprintf(cfile, "    ");
	fprintf(cfile, "<%s>", value.c_str());
}


void TiXmlUnknown::CopyTo(TiXmlUnknown* target) const
{
	TiXmlNode::CopyTo(target);
}


bool TiXmlUnknown::Accept(TiXmlVisitor* visitor) const
{
	return visitor->Visit(*this);
}


TiXmlNode* TiXmlUnknown::Clone() const
{
	TiXmlUnknown* clone = new TiXmlUnknown();

	if (!clone)
		return 0;

	CopyTo(clone);
	return clone;
}


TiXmlAttributeSet::TiXmlAttributeSet()
{
	sentinel.next = &sentinel;
	sentinel.prev = &sentinel;
}


TiXmlAttributeSet::~TiXmlAttributeSet()
{
	assert(sentinel.next == &sentinel);
	assert(sentinel.prev == &sentinel);
}


void TiXmlAttributeSet::Add(TiXmlAttribute* addMe)
{
	assert(!Find(addMe->Name()));	// Shouldn't be multiply adding to the set.

	addMe->next = &sentinel;
	addMe->prev = sentinel.prev;

	sentinel.prev->next = addMe;
	sentinel.prev = addMe;
}

void TiXmlAttributeSet::Remove(TiXmlAttribute* removeMe)
{
	TiXmlAttribute* node;

	for (node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (node == removeMe)
		{
			node->prev->next = node->next;
			node->next->prev = node->prev;
			node->next = 0;
			node->prev = 0;
			return;
		}
	}
	assert(0);		// we tried to remove a non-linked attribute.
}


TiXmlAttribute* TiXmlAttributeSet::Find(const char* name) const
{
	for (TiXmlAttribute* node = sentinel.next; node != &sentinel; node = node->next)
	{
		if (strcmp(node->name.c_str(), name) == 0)
			return node;
	}
	return 0;
}


TiXmlAttribute* TiXmlAttributeSet::FindOrCreate(const char* _name)
{
	TiXmlAttribute* attrib = Find(_name);
	if (!attrib) {
		attrib = new TiXmlAttribute();
		Add(attrib);
		attrib->SetName(_name);
	}
	return attrib;
}


TiXmlHandle TiXmlHandle::FirstChild() const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild();
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}


TiXmlHandle TiXmlHandle::FirstChild(const char * value) const
{
	if (node)
	{
		TiXmlNode* child = node->FirstChild(value);
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}


TiXmlHandle TiXmlHandle::FirstChildElement() const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement();
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}


TiXmlHandle TiXmlHandle::FirstChildElement(const char * value) const
{
	if (node)
	{
		TiXmlElement* child = node->FirstChildElement(value);
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}


TiXmlHandle TiXmlHandle::Child(int count) const
{
	if (node)
	{
		int i;
		TiXmlNode* child = node->FirstChild();
		for (i = 0;
			child && i < count;
			child = child->NextSibling(), ++i)
		{
			// nothing
		}
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}


TiXmlHandle TiXmlHandle::Child(const char* value, int count) const
{
	if (node)
	{
		int i;
		TiXmlNode* child = node->FirstChild(value);
		for (i = 0;
			child && i < count;
			child = child->NextSibling(value), ++i)
		{
			// nothing
		}
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}


TiXmlHandle TiXmlHandle::ChildElement(int count) const
{
	if (node)
	{
		int i;
		TiXmlElement* child = node->FirstChildElement();
		for (i = 0;
			child && i < count;
			child = child->NextSiblingElement(), ++i)
		{
			// nothing
		}
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}


TiXmlHandle TiXmlHandle::ChildElement(const char* value, int count) const
{
	if (node)
	{
		int i;
		TiXmlElement* child = node->FirstChildElement(value);
		for (i = 0;
			child && i < count;
			child = child->NextSiblingElement(value), ++i)
		{
			// nothing
		}
		if (child)
			return TiXmlHandle(child);
	}
	return TiXmlHandle(0);
}


bool TiXmlPrinter::VisitEnter(const TiXmlDocument&)
{
	return true;
}

bool TiXmlPrinter::VisitExit(const TiXmlDocument&)
{
	return true;
}

bool TiXmlPrinter::VisitEnter(const TiXmlElement& element, const TiXmlAttribute* firstAttribute)
{
	DoIndent();
	buffer += "<";
	buffer += element.Value();

	for (const TiXmlAttribute* attrib = firstAttribute; attrib; attrib = attrib->Next())
	{
		buffer += " ";
		attrib->Print(0, 0, &buffer);
	}

	if (!element.FirstChild())
	{
		buffer += " />";
		DoLineBreak();
	}
	else
	{
		buffer += ">";
		if (element.FirstChild()->ToText()
			&& element.LastChild() == element.FirstChild()
			&& element.FirstChild()->ToText()->CDATA() == false)
		{
			simpleTextPrint = true;
			// no DoLineBreak()!
		}
		else
		{
			DoLineBreak();
		}
	}
	++depth;
	return true;
}


bool TiXmlPrinter::VisitExit(const TiXmlElement& element)
{
	--depth;
	if (!element.FirstChild())
	{
		// nothing.
	}
	else
	{
		if (simpleTextPrint)
		{
			simpleTextPrint = false;
		}
		else
		{
			DoIndent();
		}
		buffer += "</";
		buffer += element.Value();
		buffer += ">";
		DoLineBreak();
	}
	return true;
}


bool TiXmlPrinter::Visit(const TiXmlText& text)
{
	if (text.CDATA())
	{
		DoIndent();
		buffer += "<![CDATA[";
		buffer += text.Value();
		buffer += "]]>";
		DoLineBreak();
	}
	else if (simpleTextPrint)
	{
		TIXML_STRING str;
		TiXmlBase::EncodeString(text.ValueTStr(), &str);
		buffer += str;
	}
	else
	{
		DoIndent();
		TIXML_STRING str;
		TiXmlBase::EncodeString(text.ValueTStr(), &str);
		buffer += str;
		DoLineBreak();
	}
	return true;
}


bool TiXmlPrinter::Visit(const TiXmlDeclaration& declaration)
{
	DoIndent();
	declaration.Print(0, 0, &buffer);
	DoLineBreak();
	return true;
}


bool TiXmlPrinter::Visit(const TiXmlComment& comment)
{
	DoIndent();
	buffer += "<!--";
	buffer += comment.Value();
	buffer += "-->";
	DoLineBreak();
	return true;
}


bool TiXmlPrinter::Visit(const TiXmlUnknown& unknown)
{
	DoIndent();
	buffer += "<";
	buffer += unknown.Value();
	buffer += ">";
	DoLineBreak();
	return true;
}

