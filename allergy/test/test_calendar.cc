/*
 * Copyright (C) 2020 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/calendar.h>

#include <orchis.h>

namespace {
}

namespace allergy {

    using orchis::TC;

    namespace year {

	using allergy::Year;

	void assert_invalid(const char* s)
	{
	    const Year val(s);
	    orchis::assert_false(val);
	}

	void simple(TC)
	{
	    const Year val("2020");
	    orchis::assert_eq(val, val);
	    orchis::assert_true(val);
	}

	void next(TC)
	{
	    Year val("2020");
	    orchis::assert_eq(val++, Year {"2020"});
	    orchis::assert_eq(val,   Year {"2021"});
	    orchis::assert_eq(val--, Year {"2021"});
	    orchis::assert_eq(val,   Year {"2020"});
	}

	void invalid(TC)
	{
	    const Year y;
	    orchis::assert_false(y);

	    assert_invalid("");
	    assert_invalid("  ");
	    assert_invalid("2020-01-01");
	    assert_invalid("20201");
	    assert_invalid("202");
	}

	void invalid2(TC)
	{
	    assert_invalid("202b");
	}
    }

    namespace month {

	using allergy::Month;

	void assert_invalid(const char* s)
	{
	    const Month val(s);
	    orchis::assert_false(val);
	}

	void simple(TC)
	{
	    const Month val("2020-11");
	    orchis::assert_eq(val, val);
	    orchis::assert_true(val);
	}

	void invalid(TC)
	{
	    const Month m;
	    orchis::assert_false(m);

	    assert_invalid("");
	    assert_invalid("  ");
	    assert_invalid("2020-11-12");
	    assert_invalid("2020-111");
	    assert_invalid("2020-1");
	    assert_invalid("2020-0b");
	}

	void invalid2(TC)
	{
	    assert_invalid("2020-00");
	    assert_invalid("2020-13");
	}
    }

    namespace day {

	using allergy::Day;

	void assert_invalid(const char* s)
	{
	    const Day val(s);
	    orchis::assert_false(val);
	    orchis::assert_false(val.mday());
	}

	void assert_invalid(unsigned yyyy, unsigned mm, unsigned dd)
	{
	    const Day val {yyyy, mm, dd};
	    orchis::assert_false(val);
	    orchis::assert_false(val.mday());
	}

	void simple(TC)
	{
	    const Day val("2020-11-07");
	    const Day val2(2020, 11, 07);
	    orchis::assert_eq(val, val2);
	    orchis::assert_true(val);
	    orchis::assert_eq(val2.mday(), 7);
	}

	void invalid(TC)
	{
	    const Day d;
	    orchis::assert_false(d);

	    assert_invalid("");
	    assert_invalid("  ");
	    assert_invalid("2020-11-07-23");
	    assert_invalid("2020-11-071");
	    assert_invalid("2020-11-0");
	    assert_invalid("2020:11:07");
	    assert_invalid("2020-11-ab");
	}

	void invalid2(TC)
	{
	    assert_invalid("2020-11-00");
	    assert_invalid("2020-02-30");
	    assert_invalid("2020-11-40");

	    assert_invalid(2020, 11, 00);
	    assert_invalid(2020, 02, 30);
	    assert_invalid(2020, 11, 40);
	}
    }

    namespace cal {

	void calendar(std::ostream& os, unsigned yyyy, unsigned mm,
		      unsigned count = 4)
	{
	    Calendar c {yyyy, mm, count};
	    std::array<Day, 7> week;
	    while (c.get(week)) {
		char pad = 0;
		for (const Day day: week) {
		    if (pad) os.put(pad);
		    pad = ' ';
		    char buf[3];
		    std::snprintf(buf, sizeof buf, "%2.0u", day.mday());
		    os << buf;
		}
		os.put('\n');
	    }
	}

	using orchis::assert_eq;

	void seventy(TC)
	{
	    std::ostringstream oss;
	    calendar(oss, 1970, 1);
	    assert_eq(oss.str(),
		      "          1  2  3  4\n"
		      " 5  6  7  8  9 10 11\n"
		      "12 13 14 15 16 17 18\n"
		      "19 20 21 22 23 24 25\n"
		      "26 27 28 29 30 31  1\n"
		      " 2  3  4  5  6  7  8\n"
		      " 9 10 11 12 13 14 15\n"
		      "16 17 18 19 20 21 22\n"
		      "23 24 25 26 27 28  1\n"
		      " 2  3  4  5  6  7  8\n"
		      " 9 10 11 12 13 14 15\n"
		      "16 17 18 19 20 21 22\n"
		      "23 24 25 26 27 28 29\n"
		      "30 31  1  2  3  4  5\n"
		      " 6  7  8  9 10 11 12\n"
		      "13 14 15 16 17 18 19\n"
		      "20 21 22 23 24 25 26\n"
		      "27 28 29 30         \n");
	}

	void twothousand(TC)
	{
	    std::ostringstream oss;
	    calendar(oss, 2000, 1);
	    assert_eq(oss.str(),
		      "                1  2\n"
		      " 3  4  5  6  7  8  9\n"
		      "10 11 12 13 14 15 16\n"
		      "17 18 19 20 21 22 23\n"
		      "24 25 26 27 28 29 30\n"
		      "31  1  2  3  4  5  6\n"
		      " 7  8  9 10 11 12 13\n"
		      "14 15 16 17 18 19 20\n"
		      "21 22 23 24 25 26 27\n"
		      "28 29  1  2  3  4  5\n"
		      " 6  7  8  9 10 11 12\n"
		      "13 14 15 16 17 18 19\n"
		      "20 21 22 23 24 25 26\n"
		      "27 28 29 30 31  1  2\n"
		      " 3  4  5  6  7  8  9\n"
		      "10 11 12 13 14 15 16\n"
		      "17 18 19 20 21 22 23\n"
		      "24 25 26 27 28 29 30\n");
	}

	void twentytwenty(TC)
	{
	    std::ostringstream oss;
	    calendar(oss, 2020, 9);
	    assert_eq(oss.str(),
		      "    1  2  3  4  5  6\n"
		      " 7  8  9 10 11 12 13\n"
		      "14 15 16 17 18 19 20\n"
		      "21 22 23 24 25 26 27\n"
		      "28 29 30  1  2  3  4\n"
		      " 5  6  7  8  9 10 11\n"
		      "12 13 14 15 16 17 18\n"
		      "19 20 21 22 23 24 25\n"
		      "26 27 28 29 30 31  1\n"
		      " 2  3  4  5  6  7  8\n"
		      " 9 10 11 12 13 14 15\n"
		      "16 17 18 19 20 21 22\n"
		      "23 24 25 26 27 28 29\n"
		      "30  1  2  3  4  5  6\n"
		      " 7  8  9 10 11 12 13\n"
		      "14 15 16 17 18 19 20\n"
		      "21 22 23 24 25 26 27\n"
		      "28 29 30 31         \n");
	}

	namespace days {

	    void mon(TC)
	    {
		std::ostringstream oss;
		calendar(oss, 1973, 10, 1);
		assert_eq(oss.str(),
			  " 1  2  3  4  5  6  7\n"
			  " 8  9 10 11 12 13 14\n"
			  "15 16 17 18 19 20 21\n"
			  "22 23 24 25 26 27 28\n"
			  "29 30 31            \n");
	    }

	    void tue(TC)
	    {
		std::ostringstream oss;
		calendar(oss, 1976, 6, 1);
		assert_eq(oss.str(),
			  "    1  2  3  4  5  6\n"
			  " 7  8  9 10 11 12 13\n"
			  "14 15 16 17 18 19 20\n"
			  "21 22 23 24 25 26 27\n"
			  "28 29 30            \n");
	    }

	    void wed(TC)
	    {
		std::ostringstream oss;
		calendar(oss, 1979, 8, 1);
		assert_eq(oss.str(),
			  "       1  2  3  4  5\n"
			  " 6  7  8  9 10 11 12\n"
			  "13 14 15 16 17 18 19\n"
			  "20 21 22 23 24 25 26\n"
			  "27 28 29 30 31      \n");
	    }

	    void thu(TC)
	    {
		std::ostringstream oss;
		calendar(oss, 1982, 4, 1);
		assert_eq(oss.str(),
			  "          1  2  3  4\n"
			  " 5  6  7  8  9 10 11\n"
			  "12 13 14 15 16 17 18\n"
			  "19 20 21 22 23 24 25\n"
			  "26 27 28 29 30      \n");
	    }

	    void fri(TC)
	    {
		std::ostringstream oss;
		calendar(oss, 2022, 4, 1);
		assert_eq(oss.str(),
			  "             1  2  3\n"
			  " 4  5  6  7  8  9 10\n"
			  "11 12 13 14 15 16 17\n"
			  "18 19 20 21 22 23 24\n"
			  "25 26 27 28 29 30   \n");
	    }

	    void sat(TC)
	    {
		std::ostringstream oss;
		calendar(oss, 2025, 11, 1);
		assert_eq(oss.str(),
			  "                1  2\n"
			  " 3  4  5  6  7  8  9\n"
			  "10 11 12 13 14 15 16\n"
			  "17 18 19 20 21 22 23\n"
			  "24 25 26 27 28 29 30\n");
	    }

	    void sun(TC)
	    {
		std::ostringstream oss;
		calendar(oss, 2028, 10, 1);
		assert_eq(oss.str(),
			  "                   1\n"
			  " 2  3  4  5  6  7  8\n"
			  " 9 10 11 12 13 14 15\n"
			  "16 17 18 19 20 21 22\n"
			  "23 24 25 26 27 28 29\n"
			  "30 31               \n");
	    }
	}
    }
}
