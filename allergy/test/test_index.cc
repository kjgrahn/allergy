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

	template <class It>
	void assert_entry(It it,
			  const std::string& file,
			  const std::string& timestamp,
			  const std::string& text)
	{
	    orchis::assert_eq(it->filename, file);
	    orchis::assert_eq(it->timestamp, Timestamp{timestamp});
	    orchis::assert_eq(it->text, text);
	}

	template <class It>
	void assert_end(It it, const Index& ix)
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
	    Files ff {ss};
	    const Index ix {ff};
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
	    Files ff {ss};
	    const Index ix {ff};
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
	    Files ff {ss};
	    const Index ix {ff};
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
	    Files ff {ss};
	    const Index ix {ff};
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
	    Files ff {ss};
	    const Index ix {ff};
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
	    Files ff {ss};
	    const Index ix {ff};
	    auto it = ix.begin();
	    assert_entry(it++,
			 "2020-05-16_0262.jpg",
			 "2020-05-16 14:11",
			 "foo\n"
			 " bar\n"
			 " baz");
	    assert_end(it, ix);
	}

	void trailing_space(TC)
	{
	    std::stringstream ss("2020-05-16_0262.jpg  \n"
				 "2020-05-16 14:11     \n"
				 "foobar\n");
	    Files ff {ss};
	    const Index ix {ff};
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
	    Files ff {ss};
	    const Index ix {ff};
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
	    Files ff {ss};
	    const Index ix {ff};
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
	    Files ff {ss};
	    const Index ix {ff};
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
    }
}
