#include "rss/parser.h"

#include "3rd-party/catch.hpp"
#include "rss/exception.h"
#include "test-helpers.h"

TEST_CASE("Throws exception if file doesn't exist", "[rsspp::Parser]")
{
	using TestHelpers::ExceptionWithMsg;

	rsspp::Parser p;

	REQUIRE_THROWS_MATCHES(p.parse_file("data/non-existent.xml"),
		rsspp::Exception,
		ExceptionWithMsg<rsspp::Exception>("could not parse file"));
}

TEST_CASE("Throws exception if file can't be parsed", "[rsspp::Parser]")
{
	using TestHelpers::ExceptionWithMsg;

	rsspp::Parser p;

	REQUIRE_THROWS_MATCHES(p.parse_file("data/empty.xml"),
		rsspp::Exception,
		ExceptionWithMsg<rsspp::Exception>("could not parse file"));
}

TEST_CASE("Extracts data from RSS 0.91", "[rsspp::Parser]")
{
	rsspp::Parser p;
	rsspp::Feed f;

	REQUIRE_NOTHROW(f = p.parse_file("data/rss091_1.xml"));

	REQUIRE(f.rss_version == rsspp::Feed::RSS_0_91);
	REQUIRE(f.title == "Example Channel");
	REQUIRE(f.description == "an example feed");
	REQUIRE(f.link == "http://example.com/");
	REQUIRE(f.language == "en");

	REQUIRE(f.items.size() == 1u);
	REQUIRE(f.items[0].title == "1 < 2");
	REQUIRE(f.items[0].link == "http://example.com/1_less_than_2.html");
	REQUIRE(f.items[0].description ==
		"1 < 2, 3 < 4.\nIn HTML, <b> starts a bold phrase\nand you "
		"start a link with <a href=\n");
	REQUIRE(f.items[0].author == "");
	REQUIRE(f.items[0].guid == "");
}

TEST_CASE("Extracts data from RSS 0.92", "[rsspp::Parser]")
{
	rsspp::Parser p;
	rsspp::Feed f;

	REQUIRE_NOTHROW(f = p.parse_file("data/rss092_1.xml"));

	REQUIRE(f.rss_version == rsspp::Feed::RSS_0_92);
	REQUIRE(f.title == "Example Channel");
	REQUIRE(f.description == "an example feed");
	REQUIRE(f.link == "http://example.com/");
	REQUIRE(f.language == "en");

	REQUIRE(f.items.size() == 3u);

	REQUIRE(f.items[0].title == "1 < 2");
	REQUIRE(f.items[0].link == "http://example.com/1_less_than_2.html");
	REQUIRE(f.items[0].base == "http://example.com/feed/rss_testing.html");

	REQUIRE(f.items[1].title == "A second item");
	REQUIRE(f.items[1].link == "http://example.com/a_second_item.html");
	REQUIRE(f.items[1].description == "no description");
	REQUIRE(f.items[1].author == "");
	REQUIRE(f.items[1].guid == "");
	REQUIRE(f.items[1].base == "http://example.com/item/rss_testing.html");

	REQUIRE(f.items[2].title == "A third item");
	REQUIRE(f.items[2].link == "http://example.com/a_third_item.html");
	REQUIRE(f.items[2].description == "no description");
	REQUIRE(f.items[2].base == "http://example.com/desc/rss_testing.html");
}

TEST_CASE("Extracts data fro RSS 2.0", "[rsspp::Parser]")
{
	rsspp::Parser p;
	rsspp::Feed f;

	REQUIRE_NOTHROW(f = p.parse_file("data/rss20_1.xml"));

	REQUIRE(f.title == "my weblog");
	REQUIRE(f.link == "http://example.com/blog/");
	REQUIRE(f.description == "my description");

	REQUIRE(f.items.size() == 1u);

	REQUIRE(f.items[0].title == "this is an item");
	REQUIRE(f.items[0].link ==
		"http://example.com/blog/this_is_an_item.html");
	REQUIRE(f.items[0].author == "Andreas Krennmair");
	REQUIRE(f.items[0].author_email == "blog@synflood.at");
	REQUIRE(f.items[0].content_encoded == "oh well, this is the content.");
	REQUIRE(f.items[0].pubDate == "Fri, 12 Dec 2008 02:36:10 +0100");
	REQUIRE(f.items[0].guid ==
		"http://example.com/blog/this_is_an_item.html");
	REQUIRE_FALSE(f.items[0].guid_isPermaLink);
}

TEST_CASE("Extracts data from RSS 1.0", "[rsspp::Parser]")
{
	rsspp::Parser p;
	rsspp::Feed f;

	REQUIRE_NOTHROW(f = p.parse_file("data/rss10_1.xml"));

	REQUIRE(f.rss_version == rsspp::Feed::RSS_1_0);

	REQUIRE(f.title == "Example Dot Org");
	REQUIRE(f.link == "http://www.example.org");
	REQUIRE(f.description == "the Example Organization web site");

	REQUIRE(f.items.size() == 1u);

	REQUIRE(f.items[0].title == "New Status Updates");
	REQUIRE(f.items[0].link == "http://www.example.org/status/foo");
	REQUIRE(f.items[0].guid == "http://www.example.org/status/");
	REQUIRE(f.items[0].description == "News about the Example project");
	REQUIRE(f.items[0].pubDate == "Tue, 30 Dec 2008 07:20:00 +0000");
}

TEST_CASE("Extracts data from Atom 1.0", "[rsspp::Parser]")
{
	rsspp::Parser p;
	rsspp::Feed f;

	REQUIRE_NOTHROW(f = p.parse_file("data/atom10_1.xml"));

	REQUIRE(f.rss_version == rsspp::Feed::ATOM_1_0);

	REQUIRE(f.title == "test atom");
	REQUIRE(f.title_type == "text");
	REQUIRE(f.description == "atom description!");
	REQUIRE(f.pubDate == "Tue, 30 Dec 2008 18:26:15 +0000");
	REQUIRE(f.link == "http://example.com/");

	REQUIRE(f.items.size() == 3u);
	REQUIRE(f.items[0].title == "A gentle introduction to Atom testing");
	REQUIRE(f.items[0].title_type == "html");
	REQUIRE(f.items[0].link == "http://example.com/atom_testing.html");
	REQUIRE(f.items[0].guid == "tag:example.com,2008-12-30:/atom_testing");
	REQUIRE(f.items[0].description == "some content");
	REQUIRE(f.items[0].base == "http://example.com/feed/atom_testing.html");

	REQUIRE(f.items[1].title == "A missing rel attribute");
	REQUIRE(f.items[1].title_type == "html");
	REQUIRE(f.items[1].link == "http://example.com/atom_testing.html");
	REQUIRE(f.items[1].guid == "tag:example.com,2008-12-30:/atom_testing1");
	REQUIRE(f.items[1].description == "some content");
	REQUIRE(f.items[1].base ==
		"http://example.com/entry/atom_testing.html");

	REQUIRE(f.items[2].title == "alternate link isn't first");
	REQUIRE(f.items[2].title_type == "html");
	REQUIRE(f.items[2].link == "http://example.com/atom_testing.html");
	REQUIRE(f.items[2].guid == "tag:example.com,2008-12-30:/atom_testing2");
	REQUIRE(f.items[2].description == "some content");
	REQUIRE(f.items[2].base ==
		"http://example.com/content/atom_testing.html");
}
