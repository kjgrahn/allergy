/* Copyright (c) 2014 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "keys.h"

using allergy::Keys;


namespace {

    class TextStack {
    public:
	TextStack() : v(1) {}
	void put(char ch);
	void open();
	void close(char ch);

	void report(std::string& s,
		    std::vector<std::string>& bits);

    private:
	typedef std::vector<std::string> vs;
	vs v;
	vs done;
    };

    void TextStack::put(char ch)
    {
	for(vs::iterator i = v.begin(); i!=v.end(); i++) {
	    i->push_back(ch);
	}
    }

    void TextStack::open()
    {
	v.push_back("");
    }

    void TextStack::close(char ch)
    {
	if(v.size()==1) {
	    put(ch);
	}
	else {
	    done.push_back(v.back());
	    v.pop_back();
	}
    }

    void TextStack::report(std::string& s,
			   std::vector<std::string>& bits)
    {
	vs::iterator i = v.begin();
	std::swap(s, *i++);
	std::swap(bits, done);
	bits.insert(bits.end(), i, v.end());
    }
}


Keys::Keys(const std::string& str)
{
    const char* p = str.data();
    const char* const q = p + str.size();
    TextStack stack;
    while(p!=q) {
	if(*p=='[') {
	    stack.open();
	}
	else if(*p==']') {
	    stack.close(']');
	}
	else {
	    stack.put(*p);
	}
	p++;
    }
    stack.report(s, bits);
}

Keys::const_iterator Keys::begin() const
{
    return bits.begin();
}

Keys::const_iterator Keys::end() const
{
    return bits.end();
}

bool Keys::empty() const
{
    return bits.empty();
}
