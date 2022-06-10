#pragma once
#include <vector>
#include <iostream>
#include <string>
//#include <gsl/gsl-lite.hpp>
//#include <gsl/span>

namespace ds_linked_list {
	struct MyType{
		std::string str;
		uint8_t num;
	};

	std::ostream& operator<<(std::ostream& os, const MyType& val) {
		std::cout << val.str << ", " << val.num;
		return os;
	}
	template <typename T>
	class Node {
		T data;
		Node<T>* next;
	public:
		Node(T data_) :data{ data_ }, next{ nullptr }{}
		Node<T>* addNode(T data) {
			return next = new Node<T>{ data };
		}
		Node<T>* getNextNode() {
			return next;
		}
		const T& getValue() const {
			return data;
		}
	};

	template <typename T>
	Node<T>* create_linked_list(const std::vector<T>& values) {
		if (std::size(values) == 0) {
			return nullptr;
		}
		Node<T>* head = new Node<T>{ values[0] };
		Node<T>* node = head;
		for (auto iter = values.begin() + 1; iter != values.end(); iter++) {
			node = node->addNode(*iter);
		}
		return head;
	}

	template <typename T, size_t N>
	Node<T>* create_linked_list(const T(&values)[N]) {
		if (N == 0) {
			return nullptr;
		}
		Node<T>* head = new Node<T>{ values[0] };
		Node<T>* node = head;
		for (int i = 1; i< N; i++) {
			node = node->addNode(values[i]);
		}
		return head;
	}

	template <typename T>
	void display_linked_list(Node<T>* linked_list) {
		Node<T>* node = linked_list;
		while (node) {
			std::cout << node->getValue() << " -> ";
			node = node->getNextNode();
		}
		std::cout << std::endl;
	}
}