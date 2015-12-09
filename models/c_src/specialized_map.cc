#include "specialized_map.h"

#include <string>
#include <vector>

namespace SpecializedObjects {

using v8::Array;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Handle;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Undefined;
using v8::Value;

Persistent<Function> PriorityMap::constructor;


/*
 * Helper functions / main part of class
 * */

bool cmp_word(const std::pair<std::string, int>  &p1, const std::pair<std::string, int> &p2)
{
    return p1.second > p2.second;
}

bool cmp_char(const std::pair<char, int>  &p1, const std::pair<char, int> &p2)
{
    return p1.second > p2.second;
}

PriorityMap::PriorityMap() : 
	m_word_map(std::map<std::string, unsigned int>()),
	m_char_map(std::map<char, unsigned int>()),
	m_count(0){
	
}

std::vector<std::string> PriorityMap::sortWords(unsigned int size) {
	std::vector<std::string> return_vec;
	std::vector<std::pair<std::string, int> > organizational_vector;

	copy(m_word_map.begin(), m_word_map.end(), back_inserter(organizational_vector));

	sort(organizational_vector.begin(), organizational_vector.end(), cmp_word);

	unsigned int iter_len = size > organizational_vector.size() ? organizational_vector.size() : size;
	for (unsigned int i = 0; i < iter_len; i++){
		return_vec.push_back(organizational_vector[i].first);
	}
	return return_vec;
}

std::vector<char> PriorityMap::sortChars(unsigned int size) {
	std::vector<char> return_vec;
	std::vector<std::pair<char, int> > organizational_vector;

	copy(m_char_map.begin(), m_char_map.end(), back_inserter(organizational_vector));

	sort(organizational_vector.begin(), organizational_vector.end(), cmp_char);
	unsigned int iter_len = size > organizational_vector.size() ? organizational_vector.size() : size;
	for (unsigned int i = 0; i < iter_len; i++){
		return_vec.push_back(organizational_vector[i].first);
	}
	return return_vec;
}

PriorityMap::~PriorityMap() {

}

void PriorityMap::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "PriorityMap"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "addToKey", AddToKey);
  NODE_SET_PROTOTYPE_METHOD(tpl, "topNWords", TopNWords);
  NODE_SET_PROTOTYPE_METHOD(tpl, "topNChars", TopNChars);
  NODE_SET_PROTOTYPE_METHOD(tpl, "count", Count);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "PriorityMap"),
               tpl->GetFunction());
}

void PriorityMap::New(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new PriorityMap(...)`
    PriorityMap* obj = new PriorityMap();
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `PriorityMap(...)`, turn into construct call.
    const int argc = 0;
    Local<Value> argv[argc] = { };
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    args.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

void PriorityMap::AddToKey(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  PriorityMap* obj = ObjectWrap::Unwrap<PriorityMap>(args.Holder());
  std::string key = "";
  if (!(args[0]->IsUndefined())) {
  	  v8::String::Utf8Value param1(args[0]->ToString());
  	  key = *param1;
  	  if (!key.empty()){
	  	obj->m_word_map[key]++;
	  	obj->m_count++;
  	  	args.GetReturnValue().Set(Number::New(isolate, obj->m_word_map[key]));
  	  	for (size_t i = 0; i < key.length(); i++) 
		  obj->m_char_map[key[i]]++;
	  }
	  return;
  }
  // If you don't give me a string, you're not getting a valid answer back
  args.GetReturnValue().Set(v8::False(isolate));
}

void PriorityMap::TopNWords(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();

  PriorityMap* obj = ObjectWrap::Unwrap<PriorityMap>(args.Holder());
  int n = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
  
  std::vector<std::string> topNWords = obj->sortWords(static_cast<unsigned int>(n));
  unsigned int iter_size = topNWords.size() < static_cast<unsigned int>(n) ? topNWords.size() : static_cast<unsigned int>(n);
  Handle<Array> array = Array::New(isolate, iter_size);
  for (size_t i = 0; i < iter_size; i++)
	array->Set(i, v8::String::NewFromUtf8(isolate, topNWords[i].c_str()));

  args.GetReturnValue().Set(array);
}

void PriorityMap::TopNChars(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();

  PriorityMap* obj = ObjectWrap::Unwrap<PriorityMap>(args.Holder());
  int n = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
  
  std::vector<char> topNChars = obj->sortChars(n);
  unsigned int iter_size = topNChars.size() < static_cast<unsigned int>(n) ? topNChars.size() : static_cast<unsigned int>(n);
  Handle<Array> array = Array::New(isolate, iter_size);
  for (size_t i = 0; i < iter_size; i++)
  	  //For some reason, I need to convert to string from char back to char...v8's problem
	array->Set(i, v8::String::NewFromUtf8(isolate, std::string(1, topNChars[i]).c_str()));

  args.GetReturnValue().Set(array);
}

void PriorityMap::Count(const v8::FunctionCallbackInfo<v8::Value>& args) {
  Isolate* isolate = args.GetIsolate();

  PriorityMap* obj = ObjectWrap::Unwrap<PriorityMap>(args.Holder());
  
  args.GetReturnValue().Set(Number::New(isolate, obj->m_count));
}

}
