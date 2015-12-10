/*
 * indexableSet.h
 *
 *  Created on: 02.12.2015
 *      Author: Galaxus
 */

#ifndef SRC_INDEXABLESET_H_
#define SRC_INDEXABLESET_H_

#include <set>

template<typename T, typename COMPARE = std::less<T>>
class indexableSet: public std::set<T, COMPARE> {
	using base_set = std::set<T, COMPARE>;
public:
	using std::set<T, COMPARE>::set;

	T const & operator[](int index) const {
		return at(index);
	}
	T const & at(int index) const {
		auto oorEx = std::out_of_range { "indexableSet::at(index) is out of range" };
		if (index >= 0) {
			if (index >= this->size())
				throw oorEx;
			return *std::next(this->begin(), index);
		} else {
			if (-index > this->size())
				throw oorEx;
			return *std::prev(this->end(), (-index));
		}
	}
	T const & front() {
		return at(0);
	}
	T const & back() {
		return at(-1);
	}
};

#endif /* SRC_INDEXABLESET_H_ */
