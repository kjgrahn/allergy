/*
 * Copyright (C) 2020 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/index.h>
#include <allergy/files...h>

#include <orchis.h>

namespace allergy {

    namespace index {

	using orchis::TC;

	void assert_drained(Files& f)
	{
	    std::string s;
	    orchis::assert_false(f.getline(s));
	}

	/* Convenience wrapper around a valid allergy::Index
	 * which generated no warnings.
	 */
	struct Index {
	    explicit Index(std::istream& is)
		: files {is},
		  index {os, files}
	    {
		orchis::assert_true(index.valid());
		orchis::assert_eq(os.str(), "");
		assert_drained(files);
	    }

	    allergy::Index::iterator begin() const { return index.begin(); }
	    allergy::Index::iterator end()   const { return index.end(); }

	    std::ostringstream os;
	    Files files;
	    const allergy::Index index;
	};

	template <class It>
	void assert_entry(It it,
			  const std::string& file,
			  const std::string& timestamp,
			  const std::string& text = "")
	{
	    orchis::assert_eq(it->filename, Photo{file});
	    orchis::assert_eq(it->timestamp, Timestamp{timestamp});
	    orchis::assert_eq(it->text, text);
	}

	template <class It>
	void assert_end(It it, const Index& ix)
	{
	    orchis::assert_true(it==ix.end());
	}

	template <class It>
	void assert_end(It it, const allergy::Index& ix)
	{
	    orchis::assert_true(it==ix.end());
	}

	void simple(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg\n"
				 "2020-05-16 14:11\n"
				 "foobar\n"
				 "\n"
				 "2020-05-16_0263.jpg\n"
				 "2020-05-16 14:12\n"
				 "bar\n");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "foobar");
	    assert_entry(it++,
			 "2020-05-16_0263.jpg",
			 "2020-05-16 14:12",
			 "bar");
	    assert_end(it, ix);
	}

	void no_description(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg\n"
				 "2020-05-16 14:11\n"
				 "\n"
				 "2020-05-16_0263.jpg\n"
				 "2020-05-16 14:12\n"
				 "\n");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "");
	    assert_entry(it++,
			 "2020-05-16_0263.jpg",
			 "2020-05-16 14:12",
			 "");
	    assert_end(it, ix);
	}

	void no_time(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg\n"
				 "\n"
				 "2020-05-16_0263.jpg\n"
				 "foo");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "",
			 "");
	    assert_entry(it++,
			 "2020-05-16_0263.jpg",
			 "",
			 "foo");
	    assert_end(it, ix);
	}

	void ibid(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg\n"
				 "2020-05-16 14:11\n"
				 "foobar\n"
				 "\n"
				 "2020-05-16_0263.jpg\n"
				 "2020-05-16 14:12\n"
				 "ibid\n");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "foobar");
	    assert_entry(it++,
			 "2020-05-16_0263.jpg",
			 "2020-05-16 14:12",
			 "foobar");
	    assert_end(it, ix);
	}

	void empty(TC)
	{
	    std::stringstream ss("# nothing here\n");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_end(it, ix);
	}

	void multiline(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg\n"
				 "2020-05-16 14:11\n"
				 "foo\n"
				 " bar\n"
				 " baz\n");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "foo\n"
			 "bar\n"
			 "baz");
	    assert_end(it, ix);
	}

	void trailing_space(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg  \n"
				 "2020-05-16 14:11     \n"
				 "foobar\n");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "foobar");
	    assert_end(it, ix);
	}

	void comments(TC)
	{
	    std::stringstream ss("# bar\n"
				 "# bar\n"
				 "\n"
				 "# bar\n"
				 "2020-05-16_0262.jpg\n"
				 "# bar\n"
				 "2020-05-16 14:11\n"
				 "# bar\n"
				 "foobar\n"
				 "# bar\n"
				 "\n"
				 "# bar\n");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "foobar");
	    assert_end(it, ix);
	}

	void no_eol_at_eof(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg\n"
				 "2020-05-16 14:11\n"
				 "foobar");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "foobar");
	    assert_end(it, ix);
	}

	void keys(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg\n"
				 "2020-05-16 14:11\n"
				 "[[foo][bar]]\n"
				 "\n"
				 "2020-05-16_0263.jpg\n"
				 "2020-05-16 14:12\n"
				 "ibid\n");
	    const Index ix {ss};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "foobar");
	    assert_entry(it++,
			 "2020-05-16_0263.jpg",
			 "2020-05-16 14:12",
			 "foobar");
	    assert_end(it, ix);
	}

	void duplicate(TC)
	{
	    std::stringstream ss("2020-05-16_0100.jpg\n"
				 "2020-05-16 14:11\n"
				 "\n"
				 "2020-05-16_0100.jpg\n"
				 "2020-05-16 14:12\n");
	    std::ostringstream err;
	    Files files {ss};
	    const allergy::Index ix {err, files};
	    orchis::assert_gt(err.str().size(), 0);
	    assert_drained(files);

	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0100.jpg",
			 "2020-05-16 14:11");
	    assert_entry(it++,
			 "2020-05-16_0100.jpg",
			 "2020-05-16 14:12");
	    assert_end(it, ix);
	}

	namespace query {

	    void generate(std::stringstream& ss,
			  const std::vector<std::string>& v)
	    {
		unsigned serial = 100;
		for (const auto& s : v) {
		    ss << s.substr(0, 10) << '_' << serial << ".jpg\n"
		       << s << "\n\n";
		    serial++;
		}
	    }

	    using orchis::assert_eq;

	    void assert_entries(const std::vector<Entry>& ee,
				const std::vector<std::string>& tt)
	    {
		assert_eq(ee.size(), tt.size());
		auto it = begin(tt);
		for (const auto& e: ee) {
		    assert_eq(e.timestamp, Timestamp{*it});
		    it++;
		}
	    }

	    void has(TC)
	    {
		using orchis::assert_true;
		using orchis::assert_false;

		std::stringstream ss;
		generate(ss,
			 {"2020-05-16 14:11",
			  "2020-05-16 14:11"});
		const Index ix {ss};

		assert_true(ix.index.has(Photo{"2020-05-16_100.jpg"}));
		assert_true(ix.index.has(Photo{"2020-05-16_101.jpg"}));

		assert_false(ix.index.has(Photo{"2020-05-16_102.jpg"}));
		assert_false(ix.index.has(Photo{""}));
	    }

	    void all(TC)
	    {
		std::stringstream ss;
		generate(ss,
			 {"2020-05-16 14:11",
			  "2020-05-16 14:12"});
		const Index ix {ss};
		assert_entries(ix.index.all(),
			       {"2020-05-16 14:11",
				"2020-05-16 14:12"});
	    }

	    void year(TC)
	    {
		std::stringstream ss;
		generate(ss,
			 {"2020-05-16 12:00",
			  "2020-12-31 12:00",
			  "2020-01-01 12:00",
			  "2021-01-01 12:00",
			  "2019-01-01 12:00"});
		const Index ix {ss};
		assert_entries(ix.index.in(Year{"2020"}),
			 {"2020-01-01 12:00",
			  "2020-05-16 12:00",
			  "2020-12-31 12:00"});
	    }

	    void month(TC)
	    {
		std::stringstream ss;
		generate(ss,
			 {"2020-07-01 12:00",
			  "2020-08-02 12:00",
			  "1980-08-01 12:00",
			  "2020-08-01 12:01",
			  "2020-08-01 12:00",
			  "2020-09-01 12:00"});
		const Index ix {ss};
		assert_entries(ix.index.in(Month{"2020-08"}),
			       {"2020-08-01 12:00",
				"2020-08-01 12:01",
				"2020-08-02 12:00"});
	    }

	    void day(TC)
	    {
		std::stringstream ss;
		generate(ss,
			 {"2020-07-01 12:00",
			  "2020-09-26 12:01",
			  "2020-08-02 12:00",
			  "2020-09-26 12:00",
			  "2020-09-25 12:00",
			  "2020-09-24 12:00",
			  "2020-09-26 11:59",
			  "1980-08-01 12:00"});
		const Index ix {ss};
		assert_entries(ix.index.on(Day{"2020-09-26"}),
			       {"2020-09-26 11:59",
				"2020-09-26 12:00",
				"2020-09-26 12:01"});
	    }

	    void key(TC)
	    {
		std::stringstream ss("2020-09-26_0001.jpg\n"
				     "2020-09-26 14:23\n"
				     "[foo][bar]\n"
				     "\n"
				     "2020-09-26_0002.jpg\n"
				     "2020-09-26 14:24\n"
				     "ibid\n"
				     "\n"
				     "2020-09-26_0003.jpg\n"
				     "2020-09-26 14:25\n"
				     "[bar][baz]\n");
		const Index ix {ss};
		assert_entries(ix.index.key(Key{"foo"}),
			       {"2020-09-26 14:23",
				"2020-09-26 14:24"});
		assert_entries(ix.index.key(Key{"bar"}),
			       {"2020-09-26 14:23",
				"2020-09-26 14:24",
				"2020-09-26 14:25"});
		assert_entries(ix.index.key(Key{"baz"}),
			       {"2020-09-26 14:25"});
		assert_entries(ix.index.key(Key{"bat"}), {});
	    }
	}
    }
}
