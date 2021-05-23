/*
 * Copyright (C) 2020 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/index.h>
#include <allergy/files...h>
#include <split.h>
#include <join.h>

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
	 * which generates no warnings.
	 */
	struct Index {
	    explicit Index(std::istream& is)
		: files {is},
		  index {os, files},
		  all {index.all()}
	    {
		orchis::assert_true(index.valid());
		orchis::assert_eq(os.str(), "");
		assert_drained(files);
	    }

	    std::ostringstream os;
	    Files files;
	    const allergy::Index index;
	    const allergy::Index::Range all;
	};

	template <class It>
	void assert_entry(It it,
			  const std::string& file,
			  const std::string& timestamp,
			  const std::string& text = "")
	{
	    orchis::assert_eq(it->photo, Photo{file});
	    orchis::assert_eq(it->timestamp, Timestamp{timestamp});
	    orchis::assert_eq(it->text, text);
	}

	template <class It>
	void assert_end(It it, const Index& ix)
	{
	    orchis::assert_true(it==ix.all.end());
	}

	template <class It>
	void assert_end(It it, const allergy::Index::Range range)
	{
	    orchis::assert_true(it==range.end());
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
	    auto it = ix.all.begin();
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
				 "2020-05-16 14:12\n"
				 "\n"
				 "2020-05-16_0101.jpg\n"
				 "2020-05-16 14:13\n"
				 "\n"
				 "2020-05-16_0101.jpg\n"
				 "2020-05-16 14:14\n");
	    std::ostringstream err;
	    Files files {ss};
	    const allergy::Index ix {err, files};
	    orchis::assert_gt(err.str().size(), 0);
	    assert_drained(files);

	    const auto all = ix.all();
	    auto it = all.begin();
	    assert_entry(it++,
			 "2020-05-16_0100.jpg",
			 "2020-05-16 14:11");
	    assert_entry(it++,
			 "2020-05-16_0101.jpg",
			 "2020-05-16 14:13");
	    assert_end(it, all);
	}

	namespace query {

	    using orchis::assert_eq;
	    using orchis::assert_true;
	    using orchis::assert_false;

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

	    void generate(std::stringstream& ss,
			  const std::vector<std::string>& v)
	    {
		for (const auto& s: v) {
		    const auto v = split(s, 3);
		    const auto day = v.at(0);
		    const auto time = v.at(1);
		    const auto serial = v.at(2);
		    ss << day << '_' << serial << ".jpg\n"
		       << day << ' ' << time << '\n'
		       << "serial/" << serial << "\n\n";
		}
	    }

	    /* There's no public p.serial.
	     */
	    Serial serial_of(const Photo& p)
	    {
		std::ostringstream oss;
		oss << p;
		const auto s = oss.str();
		auto a = s.data();
		auto b = a + s.size();
		a = std::find(a, b, '_');
		a++;
		b = std::find(a, b, '.');
		return {a, b};
	    }

	    void assert_range(allergy::Index::Range r,
			      const std::string& serials)
	    {
		std::vector<Serial> ss;
		for (const auto& e : r) {
		    ss.push_back(serial_of(e.photo));
		}
		orchis::assert_eq(util::join(" ", begin(ss), end(ss)),
				  serials);
	    }

	    void has(TC)
	    {
		std::stringstream ss;
		generate(ss, {"2020-05-16 14:11 100",
			      "2020-05-16 14:11 101"});
		const Index ix {ss};

		assert_true(ix.index.has(Photo{"2020-05-16_100.jpg"}));
		assert_true(ix.index.has(Photo{"2020-05-16_101.jpg"}));

		assert_false(ix.index.has(Photo{"2020-05-16_102.jpg"}));
		assert_false(ix.index.has(Photo{""}));
	    }

	    void has_legacy(TC)
	    {
		std::stringstream ss;
		ss << "200516_100.jpg\n"
		      "2020-05-16 14:11\n"
		      "\n"
		      "200516_101.jpg\n"
		      "2020-05-16 14:11\n";
		const Index ix {ss};

		assert_true(ix.index.has(Photo{"2020-05-16_100.jpg"}));
		assert_true(ix.index.has(Photo{"2020-05-16_101.jpg"}));

		assert_false(ix.index.has(Photo{"2020-05-16_102.jpg"}));
		assert_false(ix.index.has(Photo{""}));
	    }

	    void get(TC)
	    {
		std::stringstream ss;
		generate(ss,
			 {"2020-05-16 14:11 100",
			  "2020-05-16 14:12 101"});
		const Index ix {ss};

		const auto e = ix.index.get(Photo{"2020-05-16_101.jpg"});
		assert_entry(&e,
			     "2020-05-16_101.jpg",
			     "2020-05-16 14:12",
			     "serial/101");
		try {
		    ix.index.get(Photo{"2020-05-16_102.jpg"});
		}
		catch (...) {
		    return;
		}
		throw orchis::Failure {};
	    }

	    void all(TC)
	    {
		std::stringstream ss;
		generate(ss, {
			"2021-05-06 12:00 06",
			"2020-09-26 12:01 05",
			"2020-08-02 12:00 02",
			"1980-08-01 12:00 01",
			"2020-08-02 12:00 03",
			"2020-09-26 12:00 04",
		    });
		const Index ix {ss};
		assert_range(ix.index.all(), "01 02 03 04 05 06");
	    }

	    void in_year(TC)
	    {
		std::stringstream ss;
		generate(ss, {
			"2021-05-06 12:00 06",
			"2020-09-26 12:01 05",
			"2020-08-02 12:00 02",
			"1980-08-01 12:00 01",
			"2020-08-02 12:00 03",
			"2020-09-26 12:00 04",
		    });
		const Index ix {ss};
		assert_range(ix.index.in(Year{"1979"}), "");
		assert_range(ix.index.in(Year{"1980"}), "01");
		assert_range(ix.index.in(Year{"2020"}), "02 03 04 05");
		assert_range(ix.index.in(Year{"2021"}), "06");
	    }

	    void in_june(TC)
	    {
		std::stringstream ss;
		generate(ss, {
			"2021-05-06 12:00 01",
			"2021-06-06 12:00 02",
			"2021-06-06 12:00 03",
			"2021-06-10 12:00 04",
			"2021-06-11 12:00 05",
			"2021-06-12 12:00 06",
		    });
		const Index ix {ss};
		assert_range(ix.index.in(Month{"2021-04"}), "");
		assert_range(ix.index.in(Month{"2021-05"}), "01");
		assert_range(ix.index.in(Month{"2021-06"}), "02 03 04 05 06");
		assert_range(ix.index.in(Month{"2021-07"}), "");
	    }

	    void on_day(TC)
	    {
		std::stringstream ss;
		generate(ss, {
			"2021-05-06 12:00 01",
			"2021-06-06 12:00 02",
			"2021-06-10 12:00 04",
			"2021-06-06 12:00 03",
			"2021-06-11 12:00 05",
			"2021-06-12 12:00 06",
		    });
		const Index ix {ss};
		assert_range(ix.index.on(Day{"2021-06-06"}), "02 03");
		assert_range(ix.index.on(Day{"2021-06-12"}), "06");
		assert_range(ix.index.on(Day{"2021-06-07"}), "");
		assert_range(ix.index.on(Day{"1970-01-01"}), "");
	    }
	}

	namespace navigation {

	    using query::generate;

	    const std::vector<std::string>& v = {
		"2018-01-01 12:00 01",
		"2021-05-06 12:00 02",
		"2021-05-06 12:00 03",
		"2021-05-07 12:00 04",
		"2021-05-08 12:00 05",
		"2021-05-08 12:00 06",
	    };

	    void years(TC)
	    {
		std::stringstream ss;
		generate(ss, v);
		const Index ix {ss};
		const auto& yy = ix.index.years;

		orchis::assert_eq(yy.begin, Year {"2018"});
		orchis::assert_eq(yy.end,   Year {"2022"});
	    }

	    void days(TC)
	    {
		std::stringstream ss;
		generate(ss, v);
		const Index ix {ss};

		Day d = ix.index.next(Day {"2017-12-24"});
		orchis::assert_eq(d, Day  {"2018-01-01"});
		d = ix.index.next(Day     {"2018-01-01"});
		orchis::assert_eq(d, Day  {"2021-05-06"});
		d = ix.index.next(Day     {"2021-05-06"});
		orchis::assert_eq(d, Day  {"2021-05-07"});
		d = ix.index.next(Day     {"2021-05-07"});
		orchis::assert_eq(d, Day  {"2021-05-08"});
		d = ix.index.next(Day     {"2021-05-08"});
		orchis::assert_false(d);

		d = ix.index.prev(Day     {"2048-01-01"});
		orchis::assert_eq(d, Day  {"2021-05-08"});
		d = ix.index.prev(Day     {"2021-05-08"});
		orchis::assert_eq(d, Day  {"2021-05-07"});
		d = ix.index.prev(Day     {"2021-05-07"});
		orchis::assert_eq(d, Day  {"2021-05-06"});
		d = ix.index.prev(Day     {"2021-05-06"});
		orchis::assert_eq(d, Day  {"2018-01-01"});
		d = ix.index.prev(Day     {"2018-01-01"});
		orchis::assert_false(d);
	    }

	    void photos(TC)
	    {
		std::stringstream ss;
		generate(ss, v);
		const Index ix {ss};

		Photo p = ix.index.next(Photo {"2021-05-08_05.jpg"});
		orchis::assert_eq(p, Photo {"2021-05-08_06.jpg"});
		p = ix.index.next(Photo {"2021-05-08_06.jpg"});
		orchis::assert_false(p);

		p = ix.index.prev(Photo {"2021-05-06_02.jpg"});
		orchis::assert_eq(p, Photo {"2018-01-01_01.jpg"});
		p = ix.index.prev(Photo {"2018-01-01_01.jpg"});
		orchis::assert_false(p);
	    }
	}
    }
}
