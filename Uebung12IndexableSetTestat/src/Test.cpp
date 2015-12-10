#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include "indexableSet.h"

void testEmptyConstructor() {
	indexableSet<int> iset { };
	ASSERT_EQUAL(0, iset.size());
	ASSERT(iset.empty());
}

void testRangeConstructor() {
	std::vector<std::string> stringVector { "a", "c", "b", "g", "e" };
	indexableSet<std::string> iset { stringVector.begin(), stringVector.end() };
	ASSERT(!iset.empty());
	ASSERT_EQUAL(stringVector.size(), iset.size());
}

void testCopyConstructor() {
	const indexableSet<std::string> stringSet { "a", "c", "b", "g", "e" };
	indexableSet<std::string> iset { stringSet };
	ASSERT(!iset.empty());
	ASSERT_EQUAL(stringSet.size(), iset.size());
}

void testMoveConstructor() {
	indexableSet<std::string> stringSet { "a", "c", "b", "g", "e" };
	indexableSet<std::string> iset { std::move(stringSet) };
	ASSERT(!iset.empty());;
	ASSERT(stringSet.empty());
	ASSERT_EQUAL(5, iset.size());
	ASSERT_EQUAL(0, stringSet.size());
}

void testInitializerListConstructor() {
	indexableSet<int> iset { 3, 6, 2, 7, 5, 1 };
	ASSERT_EQUAL(6, iset.size());
	ASSERT(!iset.empty());
}

void testIndexedAccess() {
	indexableSet<int> iset { 3, 6, 2, 7, 5, 1 }; // 1,2,3,5,6,7
	ASSERT_EQUAL(1, iset[0]);
	ASSERT_EQUAL(2, iset[1]);
	ASSERT_EQUAL(7, iset[5]);
	ASSERT_EQUAL(7, iset[-1]);
	ASSERT_EQUAL(6, iset[-2]);
	indexableSet<int> iset2 { 3, 6, 2, 7, 5, 1, 4 }; // 1,2,3,4,5,6,7
	ASSERT_EQUAL(1, iset2[0]);
	ASSERT_EQUAL(2, iset2[1]);
	ASSERT_EQUAL(6, iset2[5]);
	ASSERT_EQUAL(7, iset2[-1]);
	ASSERT_EQUAL(6, iset2[-2]);
}

void testIndexedAccessOutOfRange() {
	indexableSet<int> iset { 3, 6, 2, 7, 5, 1 };
	ASSERT_THROWS(iset[6], std::out_of_range);
	ASSERT_THROWS(iset[-7], std::out_of_range);
	ASSERT_THROWS(iset[-7], std::out_of_range);
}

void testFrontCall() {
	indexableSet<int> iset { 3, 6, 2, 7, 5, 1 }; // 1,2,3,5,6,7
	ASSERT_EQUAL(1, iset.front());
}

void testBackCall() {
	indexableSet<int> iset { 3, 6, 2, 7, 5, 1 }; // 1,2,3,5,6,7
	ASSERT_EQUAL(7, iset.back());
}

void testFrontCallOutOfRange() {
	indexableSet<int> iset { };
	ASSERT_THROWS(iset.front(), std::out_of_range);
}

void testBackCallOutOfRange() {
	indexableSet<int> iset { };
	ASSERT_THROWS(iset.back(), std::out_of_range);
}

void testIndexableSetWithOtherCompare() {
	struct caselessCompare {
		bool operator()(std::string a, std::string b) const {
			return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), [](const auto l, const auto r) {
				return std::tolower(l) < std::tolower(r);
			});
		}
	};
	indexableSet<std::string, caselessCompare> iset { "c", "A", "g", "e", "D", "f", "E", "b" }; // A b c D e f g

	ASSERT_EQUAL(7, iset.size());

	ASSERT_EQUAL("A", iset[0]);
	ASSERT_EQUAL("b", iset[1]);
	ASSERT_EQUAL("c", iset[2]);
	ASSERT_EQUAL("D", iset[3]);
	ASSERT_EQUAL("e", iset[4]);
	ASSERT_EQUAL("f", iset[5]);
	ASSERT_EQUAL("g", iset[6]);
	ASSERT_THROWS(iset[7], std::out_of_range);
}

void runAllTests(int argc, char const *argv[]) {
	cute::suite s;
	s.push_back(CUTE(testEmptyConstructor));
	s.push_back(CUTE(testRangeConstructor));
	s.push_back(CUTE(testCopyConstructor));
	s.push_back(CUTE(testMoveConstructor));
	s.push_back(CUTE(testInitializerListConstructor));

	s.push_back(CUTE(testIndexedAccess));
	s.push_back(CUTE(testIndexedAccessOutOfRange));

	s.push_back(CUTE(testFrontCall));
	s.push_back(CUTE(testBackCall));

	s.push_back(CUTE(testFrontCallOutOfRange));
	s.push_back(CUTE(testBackCallOutOfRange));
	
	s.push_back(CUTE(testIndexableSetWithOtherCompare));

	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<> > lis(xmlfile.out);
	cute::makeRunner(lis, argc, argv)(s, "AllTests");
}

int main(int argc, char const *argv[]) {
	runAllTests(argc, argv);

	return 0;
}

