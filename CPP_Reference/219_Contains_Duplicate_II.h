#pragma once
//https://leetcode.com/problems/contains-duplicate-ii/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdlib.h>  
#include <algorithm>

using namespace std;

namespace contains_duplicate_II {
	class Solution {
	public:
		bool containsNearbyDuplicate(vector<int>& nums, int k) {
			unordered_map<int, vector<int>> mp{};
			for (int i = 0; i < nums.size(); i++) {
				auto ret = mp.insert({ nums[i], vector<int>{i} });//Insert returns std::pair<iterator,bool>. Bool tells if the element was inserted or not and the iterator is the pointer to the map element pair (in our specific case, int, vector<int>)
				if (!ret.second){//If the element already exists
					//Oh man I hate STD interface designers. Look at the following shit.
					//We already know abt ret from above. First refers to the std::pair<int,vector<int>> (element of the map). Then we access the second within the second pair, which is the list of stored indices. Finally, loop over the list and compare it with the current index to check the condition for k.
					if (find_if(ret.first->second.begin(), ret.first->second.end(), [&](int index) {return abs(index - i) <= k; }) != ret.first->second.end()) {
						return true;
					}
					else {
						ret.first->second.emplace_back(i);
					}
				}
			}
			return false;
		}
	};
	void main() {
		//vector<int> nums{ 1, 2, 3, 1 };
		//int k = 3;

		//vector<int> nums{ 1, 0, 1, 1 };
		//int k = 1;

		vector<int> nums{ 1, 2, 3, 1, 1, 1 };
		int k = 2;

		cout << "Contains duplicate with k<=" << k << ", = " << Solution{}.containsNearbyDuplicate(nums, k);
	}
}
