#ifndef MERGESORT_HPP
#define MERGESORT_HPP 1

#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <vector>

namespace MergeSort {

	enum class MergeType { InPlace, ExtraSpace };

	template<class RandomIt, class Compare>
	void InPlaceMerge(RandomIt begin, RandomIt middle, RandomIt end, Compare Comp) {
		while (middle != end) {
			while (begin != middle && !Comp(*middle, *begin)) {
				++begin;
			}
			if (begin == middle) {
				//All good
				return;
			} else {
				//Need to do a rotate
				//	This moves 'middle' to the position of 'begin',
				//	and shifts [begin, middle) to the right one
				std::rotate(begin, middle, middle+1);
				++begin;
				++middle;
			}
		}
	}

	template <class RandomIt>
	RandomIt ReverseIt(std::reverse_iterator<RandomIt> it) {
		return it.base();
	}

	template <class RandomIt>
	std::reverse_iterator<RandomIt> ReverseIt(RandomIt it) {
		return std::reverse_iterator<RandomIt>(it);
	}

	template<class Compare>
	struct ReversedCompare {
		Compare compare;
		template<class Left, class Right>
		bool operator()(const Left& left, const Right& right) {
			return compare(right,left);
		}
	};

	template <class Compare>
	Compare ReverseComp(ReversedCompare<Compare> compare) {
		return compare.compare;
	}

	template <class Compare>
	ReversedCompare<Compare> ReverseComp(Compare compare) {
		return ReversedCompare<Compare>{compare};
	}

	template<class LeftRandomIt, class RightRandomIt, class InsertRandomIt, class Compare>
	void MergeInto(LeftRandomIt leftIt, LeftRandomIt leftEnd, RightRandomIt rightIt, RightRandomIt rightEnd, InsertRandomIt insertIt, Compare Comp) {
		//Begin merging
		while (leftIt != leftEnd && rightIt != rightEnd) {
			//While there are elements in both lists
			auto leftValue = *leftIt;
			auto rightValue = *rightIt;

			if (!Comp(rightValue, leftValue)) {
				//Left list's first element is inserted
				*insertIt = leftValue;
				++leftIt;
			} else {
				//Right list's first element is inserted
				*insertIt = rightValue;
				++rightIt;
			}
			++insertIt;
		}
		//Now at least one list is empty
		if (leftIt != leftEnd) {
			//Insert all of the left list into the list (right is empty)
			std::move(leftIt, leftEnd, insertIt);
		} else if (rightIt != rightEnd) {
			//Insert all of the right list into the list (left is empty)
			std::move(rightIt, rightEnd, insertIt);
		}
	}

	template<class RandomIt, class Compare>
	void Merge(RandomIt begin, RandomIt middle, RandomIt end, Compare Comp) {
		size_t leftLength = std::distance(begin, middle);
		size_t rightLength = std::distance(middle, end);

		if (leftLength > rightLength) {
			//Left list is bigger, swap everything around and recursively call this function
			Merge(ReverseIt(end), ReverseIt(middle), ReverseIt(begin), ReverseComp(Comp));
			return;
		}
		//Now guaranteed that [begin,middle) <= [middle,end)

		//Determine if there are any leftmost positions that are already in place
		auto newLeftBegin = std::upper_bound(begin, middle, *middle, Comp);
		if (newLeftBegin == middle) {
			//Everything in the left list is smaller than the first in the right list, we're done
			return;
		}
		//Determine if there are any righmost positions that are already in place
		auto rightEnd = std::upper_bound(middle, end, *(std::prev(middle)), Comp);
		if (rightEnd == middle) {
			//Everything in the left list is smaller than the first in the right list, we're done
			return;
		}
		//	"These preliminary searches[newLeftBegin&rightEnd] may not pay off, and can be expected *not* to
		//	repay their cost if the data is random.  But they can win huge in all of
		//	time, copying, and memory savings when they do pay"
		//	According to https://svn.python.org/projects/python/trunk/Objects/listsort.txt

		// newLeftBegin saved std::distance(begin, newLeftBegin)*sizeof(RandomIt) bytes of extra space usage
		// rightEnd saved std::distance(rightEnd, end)*sizeof(RandomIt) bytes of extra space usage
		//Save the left list in a temp because we'll be overwriting it as we insert
		using ValueType = typename std::iterator_traits<RandomIt>::value_type;
		std::vector<ValueType> tempList(newLeftBegin, middle);
		auto leftIt = tempList.begin();
		auto leftEnd = tempList.end();

		auto rightIt = middle;

		auto insertIt = newLeftBegin;

		MergeInto(leftIt, leftEnd, rightIt, rightEnd, insertIt, Comp);
	}

	template<class RandomIt, class Compare = std::less<>>
	void Sort(RandomIt begin, RandomIt end, MergeType mergeType = MergeType::ExtraSpace, Compare Comp = Compare()) {
		std::function<void(RandomIt,RandomIt,RandomIt,Compare)> MergeFunction;
		
		if (mergeType == MergeType::ExtraSpace) {
			MergeFunction = Merge<RandomIt,Compare>;
		} else {
			MergeFunction = InPlaceMerge<RandomIt,Compare>;
		}

		size_t length = std::distance(begin,end);

		//Bottom-up merge algorithm
		for (size_t width=1; width<length; width*=2) {
			//We are going to merge lists of length 'width'
			for (size_t listStart=0; listStart<length; listStart+=(width*2)) {
				//These 2 lists are [listStart, listStart+width) and [listStart+width, listStart+(width*2))
				size_t firstPos = listStart;
				size_t middlePos = listStart+width;
				size_t endPos = listStart+(width*2);
				if (middlePos >= length) {
					middlePos = length;
					endPos = length;
				} else if (endPos >= length) {
					endPos = length;
				}
				MergeFunction(begin+firstPos, begin+middlePos, begin+endPos, Comp);
			}
		}
	}
} //namespace MergeSort

#endif //MERGESORT_HPP