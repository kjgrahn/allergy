/* Copyright (C) 2021 Jörgen Grahn.
 * All rights reserved.
 */
#include <allergy/index.h>
#include <allergy/page.h>
#include <allergy/files...h>

#include <orchis.h>

namespace allergy {

    namespace page {

	using orchis::TC;

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

	void assert_drained(Files& f)
	{
	    std::string s;
	    orchis::assert_false(f.getline(s));
	}

	namespace helper {

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
	}

	unsigned count(const std::string& haystack,
		       const std::string& needle)
	{
	    size_t pos = 0;
	    unsigned n = 0;
	    while (true) {
		pos = haystack.find(needle, pos);
		if (pos == std::string::npos) break;
		pos += needle.size();
		n++;
	    }
	    return n;
	}

	/**
	 * True iff 'haystack' contains n non-overlapping copies of
	 * 'needle'.
	 */
	void assert_contains(const std::string& haystack,
			     const std::string& needle, unsigned n = 1)
	{
	    orchis::assert_eq(count(haystack, needle), n);
	}

	namespace front {

	    void simple(TC)
	    {
		std::stringstream ss;
		generate(ss, {"2020-01-01", "2020-01-01", "2020-02-01",
			      "2022-05-01", "2022-08-01"});
		const helper::Index ix {ss};
		const page::Frontpage page {ix.index};

		std::ostringstream os;

		page::Frontpage::Chunk chunk = page.begin();
		while (!chunk.end()) {
		    page.put(os, chunk++);
		}

		const auto html = os.str();
		assert_contains(html, "2022", 4);
		assert_contains(html, "2021", 2);
		assert_contains(html, "2020", 4);
		assert_contains(html, "2019", 0);

		assert_contains(html, "</table>");
		assert_contains(html, "</html>");
	    }
	}

	namespace year {

	    void simple(TC)
	    {
		std::stringstream ss;
		generate(ss, {"2021-01-01", "2021-01-01",
			      "2021-01-14",
			      "2021-02-28",
			      "2021-04-30",
			      "2021-12-24"});
		const helper::Index ix {ss};
		const page::Year page {ix.index, allergy::Year {"2021"}};

		std::ostringstream os;

		page::Year::Chunk chunk = page.begin();
		while (!chunk.end()) {
		    page.put(os, chunk++);
		}

		const auto html = os.str();
		assert_contains(html, "2021Q1");
		assert_contains(html, "2021Q2");
		assert_contains(html, "2021Q3");
		assert_contains(html, "2021Q4");

		assert_contains(html, "2021-01-01");
		assert_contains(html, "2021-01-14");
		assert_contains(html, "2021-02-28");
		assert_contains(html, "2021-04-30");
		assert_contains(html, "2021-12-24");

		// prev/next links
		assert_contains(html, "2020");
		assert_contains(html, "2022");

		assert_contains(html, "</table>", 4);
		assert_contains(html, "</html>");
	    }
	}

	namespace photo {

	    void simple(TC)
	    {
		std::stringstream ss;
		ss << "2021-04-18_0155.jpg\n"
		      "2021-04-18 14:49:08\n"
		      "blommande [vide], [Toltorpsdalen]\n"
		      "[Rosen & Sons]\n"
		      "{Salix}\n"
		      "\n"
		      "2021-04-18_0156.jpg\n"
		      "2021-04-18 14:49:09\n"
		      "ibid\n" ;
		const helper::Index ix {ss};
		const page::Photo page {ix.index, {2021, 4, 18}, Serial{"0156"}};

		std::ostringstream os;

		page::Photo::Chunk chunk = page.begin();
		while (!chunk.end()) {
		    page.put(os, chunk++);
		}

		const auto html = os.str();
		assert_contains(html, "<ul>");
		assert_contains(html, "<li>", 4);
		assert_contains(html, "<img class='photo'");
		assert_contains(html, "vide", 3);
		assert_contains(html, "Toltorpsdalen", 3);
		assert_contains(html, "Rosen &amp; Sons", 3);
		assert_contains(html, "Salix", 2);

		assert_contains(html, "</html>");
	    }
	}

	namespace keywords {

	    void simple(TC)
	    {
		std::stringstream ss;
		ss << "2021-04-18_0155.jpg\n"
		      "2021-04-18 14:49:08\n"
		      "blommande [vide], [Toltorpsdalen]\n"
		      "[Rosen & Sons]\n"
		      "{Salix}\n"
		      "\n"
		      "2021-04-18_0156.jpg\n"
		      "2021-04-18 14:49:09\n"
		      "ibid\n" ;
		const helper::Index ix {ss};
		const page::Words page {ix.index};

		std::ostringstream os;

		page::Words::Chunk chunk = page.begin();
		while (!chunk.end()) {
		    page.put(os, chunk++);
		}

		const auto html = os.str();
		assert_contains(html, "vide", 2);
		assert_contains(html, "Toltorpsdalen", 2);
		assert_contains(html, "Rosen &amp; Sons", 2);
		assert_contains(html, "Salix", 2);

		assert_contains(html, "</html>");
	    }
	}
    }
}
