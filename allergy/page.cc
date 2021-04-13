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
    void preamble(std::ostream& os, const Title& title,
		  const std::string& prev = "",
		  const std::string& next = "")
    {
	os << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\""
	    " \"http://www.w3.org/TR/html4/strict.dtd\">\n";

	os << "<html>\n"
	    "\n"
	    "<head>\n"
	    "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"
	    "<meta http-equiv='Content-Language' content='sv'>\n"
	    "<link rev='made'       href='mailto:grahn@snipabacken.se'>\n"
	    "<link rel='stylesheet' href='/css'  type='text/css'>\n"
	    "<link rel='icon'       href='/icon' type='image/svg+xml'>\n";
	if (prev.size()) {
	    os << "<link rel='prev' href='" << prev << "'>\n";
	}
	if (next.size()) {
	    os << "<link rel='next' href='" << next << "'>\n";
	}
	os << "\n"
	    "<title>" << title << "</title>\n"
	    "\n"
	    "</head>\n";
    }

    void epilogue(std::ostream& os)
    {
	os << "<div class='badges'>\n"
	      "  <a href='http://validator.w3.org/check/referer'>\n"
	      "  <img src='http://validator.w3.org/images/vh401.png'\n"
	      "       alt='Valid HTML 4.01!' height=31 width=88></a>\n"
	      "  <a href='http://jigsaw.w3.org/css-validator/check/referer'>\n"
	      "  <img src='http://jigsaw.w3.org/css-validator/images/vcss.png'\n"
	      "       alt='Valid CSS!' height=31 width=88></a>\n"
	      "</div>\n"
	      "</body>\n"
	      "</html>\n";
    }
}

void allergy::page::Frontpage::put(std::ostream& os, const Chunk chunk) const
{
    if (chunk.first()) {
	preamble(os, "Foton");

	os << "<body>\n"
	      "\n"
	      "<p>\n"
	      "Mina samlade foton (sedan jag skaffade digitalkamera) med beskrivningar.\n"
	      "De ligger ute här mest för att jag själv, och släkt och vänner, ska kunna se dem.\n"
	      "Dessutom för att det är praktiskt att kunna sortera dem på nyckelord,\n"
	      "till exempel se alla foton på <em>svartoxbär</em>, eller katter, eller gräsänder.\n"
	      "\n"
	      "<h1>Foton per år och månad</h1>\n"
	      "\n"
	      "<table class='cal'>\n"
	      "<thead>\n"
	      "<tr><td> <th>jan <th>feb <th>mar <th>apr <th>maj <th>jun\n"
	      "         <th>jul <th>aug <th>sep <th>okt <th>nov <th>dec\n";
    }

    if (chunk.last()) {
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

	    auto p = std::begin(year.msum);
	    for (const allergy::Month m : year.yyyy.months()) {
		os << " <td>";
		hput(os, m, *p++);
	    }
	    os << " <th>";
	    hput(os, sum(year.msum)) << '\n';
	}

	epilogue(os << "</table>\n");
    }
}

namespace {

    char intensity(unsigned n)
    {
	if (n > 40) return 'f';
	if (n > 20) return 'e';
	if (n > 10) return 'd';
	if (n > 5)  return 'c';
	if (n > 2)  return 'b';
	if (n > 0)  return 'a';
	return {};
    }

    std::ostream& hput(std::ostream& os, allergy::Day day, unsigned n)
    {
	const char ch = intensity(n);
	if (!ch) return os << " <td>" << day.mday();

	return os << "<td class='" << ch
		  << "'><a href='/" << day << "'>" << day.mday() << "</a>";
    }

    std::ostream& hput(std::ostream& os, const allergy::Index& ix,
		       allergy::Year yyyy, unsigned quarter)
    {
	os << "<table class='cal'>\n"
	    "<caption>" << yyyy << 'Q' << quarter << "</caption>\n"
	    "<tbody>\n";

	allergy::Calendar calendar {yyyy.value(), 1 + (quarter-1)*3, 3};
	std::array<allergy::Day, 7> week;

	while (calendar.get(week)) {

	    allergy::Month month;
	    os << "<tr>";
	    for (allergy::Day day : week) {
		if (!day) {
		    os << " <td>";
		}
		else {
		    unsigned n = ix.on(day).size();
		    hput(os, day, n);

		    if (day.first()) month = day.month();
		}
	    }
	    if (month) os << " <th><a href='" << month.url() << "'>"
			  << month.name() << "</a>";
	    os.put('\n');
	}

	return os << "</table>\n\n";
    }
}

void allergy::page::Year::put(std::ostream& os, const Chunk chunk) const
{
    if (chunk.first()) {
	preamble(os, yyyy,
		 prev(yyyy).url(),
		 next(yyyy).url());

	os << "<body>\n"
	      "\n"
	      "<p>\n";
    }

    hput(os, ix, yyyy, chunk.val());

    if (chunk.last()) {
	epilogue(os);
    }
}

namespace {

    std::string url(const allergy::Entry& e) { return e.filename.url(); }
    std::string thumburl(const allergy::Entry& e) { return e.filename.thumburl(); }

    void thumbdiv(std::ostream& os, const allergy::Entry& e)
    {
	os << "<div><a href='" << url(e) << "'>"
	      "<img alt='' src='" << thumburl(e) << "'></a></div>\n";
    }
}

allergy::page::Month::Chunk allergy::page::Month::begin() const
{
    return {20, ee};
}

void allergy::page::Month::put(std::ostream& os, const Chunk chunk) const
{
    if (chunk.first()) {
	preamble(os, month,
		 prev(month).url(),
		 next(month).url());

	os << "<body>\n"
	      "\n"
	      "<div class='gallery'>\n";
    }

    for (const auto& e : chunk.val(ee)) thumbdiv(os, e);

    if (chunk.last()) {
	epilogue(os << "</div>\n");
    }
}

allergy::page::Day::Chunk allergy::page::Day::begin() const
{
    return {20, ee};
}

void allergy::page::Day::put(std::ostream& os, const Chunk chunk) const
{
    if (chunk.first()) {
	preamble(os, day);

	os << "<body>\n"
	      "\n"
	      "<div class='gallery'>\n";
    }

    for (const auto& e : chunk.val(ee)) thumbdiv(os, e);

    if (chunk.last()) {
	epilogue(os << "</div>\n");
    }
}

allergy::page::Keyword::Chunk allergy::page::Keyword::begin() const
{
    return {20, ee};
}

void allergy::page::Keyword::put(std::ostream& os, const Chunk chunk) const
{
    if (chunk.first()) {
	preamble(os, key);

	os << "<body>\n"
	      "\n"
	      "<h1>" << key << "</h1>\n"
	      "<div class='gallery'>\n";
    }

    for (const auto& e : chunk.val(ee)) thumbdiv(os, e);

    if (chunk.last()) {
	epilogue(os << "</div>\n");
    }
}
