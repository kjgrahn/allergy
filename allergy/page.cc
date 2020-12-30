/* Copyright (c) 2020 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "page.h"

#include "index.h"
#include "calendar.h"

#include <iostream>
#include <cstdio>
#include <numeric>
#include <algorithm>
#include <vector>
#include <array>

namespace {

    struct Years {
	explicit Years(const allergy::Index& ix);

	struct Entry {
	    explicit Entry(allergy::Year yyyy) : yyyy {yyyy} {}
	    allergy::Year yyyy;
	    std::array<unsigned, 12> msum;
	};

	std::array<unsigned, 12> msum;
	std::vector<Entry> years;
    };

    Years::Years(const allergy::Index& ix)
    {
	msum.fill(0);

	for (allergy::Year y = ix.years.begin; y < ix.years.end; y++) {

	    years.emplace_back(y);
	    auto p = begin(years.back().msum);
	    auto sp = begin(msum);

	    for (const allergy::Month m : y.months()) {
		const unsigned n = ix.in(m).size();
		*p++ = n;
		*sp++ += n;
	    }
	}
	std::reverse(begin(years), end(years));
    }

    template <class Cont>
    unsigned sum(const Cont& c)
    {
	return std::accumulate(std::begin(c), std::end(c), 0u);
    }

    std::ostream& hput(std::ostream& os, unsigned n)
    {
	if (!n) return os << "&ndash;";
	return os << n;
    }

    std::ostream& hput(std::ostream& os, allergy::Year yyyy)
    {
	return os << "<a href='/" << yyyy << "'>" << yyyy << "</a>";
    }

    std::ostream& hput(std::ostream& os, allergy::Month month, unsigned n)
    {
	if (!n) return os << "&ndash;";

	return os << "<a href='/" << month << "'>" << n << "</a>";
    }

    template <class Title>
    void preamble(std::ostream& os, const Title& title)
    {
	os << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\""
	    " \"http://www.w3.org/TR/html4/strict.dtd\">\n";

	os << "<html>\n"
	    "\n"
	    "<head>\n"
	    "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"
	    "<meta http-equiv='Content-Language' content='sv'>\n"
	    "<link rev='made' href='mailto:grahn@snipabacken.se'>\n"
	    "<link rel='stylesheet' href='css' type='text/css'>\n"
	    "\n"
	    "<title>" << title << "</title>\n"
	    "\n"
	    "</head>\n";
    }
}

void allergy::page::Index::put(std::ostream& os) const
{
    preamble(os, "Foton");

    os << "<body>\n"
	"\n"
	"<p>\n"
	"Mina samlade foton (sedan jag skaffade digitalkamera) med beskrivningar.\n"
	"De ligger ute här mest för att jag själv, och släkt och vänner, ska kunna se dem.\n"
	"Dessutom för att det är praktiskt att kunna sortera dem på nyckelord,\n"
	"till exempel se alla foton på <em>svartoxbär</em>, eller katter, eller gräsänder.\n"
	"\n"
	"<table>\n"
	"<caption>Foton per år och månad</caption>\n"
	"<thead>\n"
	"<tr><td> <th>jan <th>feb <th>mar <th>apr <th>maj <th>jun\n"
	"         <th>jul <th>aug <th>sep <th>okt <th>nov <th>dec\n";

    const Years years {ix};

    os << "<tfoot>\n"
	"<tr><td>";

    for (unsigned n : years.msum) {
	os << " <td>";
	hput(os, n);
    }

    os << " <th>";
    hput(os, sum(years.msum)) << '\n';

    os << "<tbody>\n";

    for (const auto& year : years.years) {
	os << "<tr><th>";
	hput(os, year.yyyy);

	auto p = begin(year.msum);
	for (const allergy::Month m : year.yyyy.months()) {
	    os << " <td>";
	    hput(os, m, *p++);
	}
	os << " <th>";
	hput(os, sum(year.msum)) << '\n';
    }

    os << "</table>\n"
	"\n"
	"</body>\n"
	"</html>\n";
}
