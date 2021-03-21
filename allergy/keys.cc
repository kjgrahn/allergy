/* Copyright (c) 2014, 2021 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "keys.h"

using allergy::Keys;


namespace {

    /**
     * State machine for parsing Keys strings.  At any point in time
     * we're either:
     * - just copying
     * - copying, and copying N levels of [keys]
     * - not copying, and copying a {key}
     */
    class TextStack {
    public:
	TextStack() : v(1) {}
	void put(char ch);
	void report(std::string& s,
		    std::vector<std::string>& bits);

    private:
	std::vector<std::string> v;
	std::vector<std::string> done;
	bool curly = false;
    };

    void TextStack::put(const char ch)
    {
	const bool square = v.size() > 1;

	auto pop = [this] () {
	    done.push_back(v.back());
	    v.pop_back();
	};

	if (curly) {
	    if (ch=='}') {
		pop();
		curly = false;
	    }
	    else {
		v[1].push_back(ch);
	    }
	}
	else if (ch=='[') {
	    v.push_back("");
	}
	else if (square && ch==']') {
	    pop();
	}
	else if (!square && ch=='{') {
	    v.push_back("");
	    curly = true;
	}
	else {
	    for(std::string& s : v) s.push_back(ch);
	}
    }

    void TextStack::report(std::string& s,
			   std::vector<std::string>& bits)
    {
	auto i = v.begin();
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
    for (char ch: str) stack.put(ch);
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
