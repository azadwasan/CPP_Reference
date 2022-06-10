#pragma once
//https://leetcode.com/problems/contains-duplicate/

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

namespace contains_duplicate {
	class Solution {
	public:
		//The following is my solution, which is basically the correct solution. However, there are some fancier optimizations. I am copying those codes below.

		bool containsDuplicate(vector<int>& nums) {
			unordered_set<int> nums_set;
			for (const auto& num : nums) {
				if (nums_set.find(num) == nums_set.end()) {
					nums_set.insert(num);
				}
				else {
					return true;
				}
			}
			return false;
		}

		//bool containsDuplicate(vector<int>& nums) {
		//	unordered_set<int> us;
		//	for (int i = 0; i < nums.size(); i++) {
		//		if (us.insert(nums[i]).second == false) {
		//			return true;
		//		}
		//	}
		//	return false;
		//}

		//bool containsDuplicate(vector<int>& nums) {
		//	return any_of(nums.begin(), nums.end(), [s = unordered_set<int>{}](const auto& a) mutable {
		//		return !s.insert(a).second;
		//	});
		//}

		//bool containsDuplicate(vector<int>& nums) {
		//	return nums.size() > unordered_set<int>(nums.begin(), nums.end()).size();
		//}
	};

	void main() {
		vector<int> nums{ 1, 2, 3, 1 };
		std::cout << "Contains duplicate = " << Solution{}.containsDuplicate(nums) << std::endl;
	}
}
