#ifndef SPECIALIZED_PRIORITY_MAP_H
#define SPECIALIZED_PRIORITY_MAP_H

#include <node.h>
#include <node_object_wrap.h>

#include <map>
#include <vector>

namespace SpecializedObjects {

class PriorityMap : public node::ObjectWrap {
	public:
		static void Init(v8::Local<v8::Object>);
	private:
		PriorityMap();
		~PriorityMap();
		std::vector<std::string>sortWords(unsigned int);
		std::vector<char> sortChars(unsigned int);

		static void New(const v8::FunctionCallbackInfo<v8::Value>&);
		static void AddToKey(const v8::FunctionCallbackInfo<v8::Value>&);
		static void TopNWords(const v8::FunctionCallbackInfo<v8::Value>&);
		static void TopNChars(const v8::FunctionCallbackInfo<v8::Value>&);
		static void Count(const v8::FunctionCallbackInfo<v8::Value>&);

		static v8::Persistent<v8::Function> constructor;

		std::map<std::string, unsigned int> m_word_map;
		// Assuming chars (26) + periods + question marks + apostrophes
		std::map<char, unsigned int> m_char_map;
		unsigned int m_count;
		// Need this for custom sorting of priority vector/priority map
		/*struct {
        	bool operator()(std::pair<std::string, unsigned int> first, std::pair<std::string, unsigned int> second) {   
            	return first.second > second.second;
        	}   
    	} sortVectorPairs;*/
};

} // namespace SpecializedObjects

#endif
