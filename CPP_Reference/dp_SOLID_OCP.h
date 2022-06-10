#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <type_traits>

namespace dp_SOLID_Filter {
	enum class Color { red, green, blue };
	std::ostream& operator<<(std::ostream& os, Color c) {
		switch (c) {
			case Color::red:	std::cout << "Red"; break;
			case Color::green:	std::cout << "Green"; break;
			case Color::blue:	std::cout << "Blue"; break;
		}
		return os;
	}

	enum class Size { small, medium, large };

	struct Product {
		std::string name;
		Color color;
		Size size;
	};

	class ProductFilter {
	public:
		std::vector<Product*> by_color(std::vector<Product*> items, Color color) {
			std::vector<Product*> products{};
			for (const auto& item : items) {
				if (item->color == color) {
					products.push_back(item);
				}
			}
			return products;
		}
	};

	std::ostream& operator<<(std::ostream& os, const Product& prod) {
		os << "Name = " << prod.name << ", Color = " << prod.color << ", size = " << static_cast<int>(prod.size) << std::endl;
		return os;
	}

	void main() {
		std::vector<Product*> products;
		products.push_back(new Product{ "Product 1", Color::green, Size::small });
		products.push_back(new Product{ "Product 2", Color::red, Size::medium});
		products.push_back(new Product{ "Product 3", Color::green, Size::large });
		products.push_back(new Product{ "Product 4", Color::blue, Size::small });
		
		auto filtered_products = ProductFilter{}.by_color(products, Color::green);

		for (const auto& prod : filtered_products) {
			std::cout << *prod << std::endl;
		}
	}
}

namespace dp_SOLID_Specification {

	enum class Color { red, green, blue };
	std::ostream& operator<<(std::ostream& os, Color c) {
		switch (c) {
		case Color::red:	std::cout << "Red"; break;
		case Color::green:	std::cout << "Green"; break;
		case Color::blue:	std::cout << "Blue"; break;
		}
		return os;
	}

	enum class Size { small, medium, large };
	std::ostream& operator<<(std::ostream& os, Size s) {
		switch (s) {
		case Size::small:	std::cout << "Small"; break;
		case Size::medium:	std::cout << "Medium"; break;
		case Size::large:	std::cout << "Large"; break;
		}
		return os;
	}

	struct Product {
		std::string name;
		Color color;
		Size size;
	};

	std::ostream& operator<<(std::ostream& os, const Product& prod) {
		os << "Name = " << prod.name << ", Color = " << prod.color << ", size = " << prod.size << std::endl;
		return os;
	}
	template <typename>
	class AndSpecification;

	template <typename T>
	class Specification {
	public:
		virtual bool is_satisfied(T* item) = 0;

		AndSpecification<T> operator&&(Specification<T>&& other) {
			return AndSpecification<T>{*this, other};
		}
	};

	template <typename T>
	class SizeSpec : public Specification<T> {
		Size size;
	public:
		SizeSpec(Size s) :size(s) {}
		bool is_satisfied(T* item) override {
			return item->size == size;
		}
	};

	template <typename T>
	class ColorSpec : public Specification<T> {
		Color color;
	public:
		ColorSpec(Color c) :color(c) {}
		bool is_satisfied(T* item) override {
			return item->color == color;
		}
	};

	template <typename T>
	class AndSpecification : public Specification<T> {
		Specification<T>& left;
		Specification<T>& right;

	public:
		AndSpecification(Specification<T>& l, Specification<T>& r) :left{ l }, right{ r }{}
		bool is_satisfied(T* item) override {
			return left.is_satisfied(item) && right.is_satisfied(item);
		}
	};

	template <typename T>
	class Filter {
	public:
		virtual std::vector<T*> filter(std::vector<T*> items, Specification<T>& spec) = 0;
	};

	class BetterFilter : public Filter<Product> {
	public:
		std::vector<Product*> filter(std::vector<Product*> items, Specification<Product>& spec) override{
			std::vector<Product*> filtered_products;
			for (const auto& item : items) {
				if (spec.is_satisfied(item)) {
					filtered_products.push_back(item);
				}
			}
			return filtered_products;
		}
	};

	void main() {
		std::vector<Product*> products;
		products.push_back(new Product{ "Product 1", Color::green, Size::small });
		products.push_back(new Product{ "Product 2", Color::red, Size::medium });
		products.push_back(new Product{ "Product 3", Color::red, Size::large });
		products.push_back(new Product{ "Product 4", Color::blue, Size::small });

		ColorSpec<Product> red_prod{ Color::red };
		SizeSpec<Product> small_prod{ Size::large };
		AndSpecification<Product> red_And_small{ red_prod, small_prod };

		auto red_and_medium = ColorSpec<Product>{ Color::red } && SizeSpec<Product>{Size::medium};

		auto filtered_products = BetterFilter{}.filter(products, red_and_medium);
//		auto filtered_products = bf.filter(bf.filter(products, red_prod), small_prod);
		for (const auto& prod : filtered_products) {
			std::cout << *prod << std::endl;
		}
	}
}