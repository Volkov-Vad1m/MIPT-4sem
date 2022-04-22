#include "map.hpp"
// template <typename T>
// class A {
//   T a = 0;
// public:
//   class B {
//     T x = 0;
//   };
// };


int main() 
{
	Hashmap_space::Hashmap<std::string> map1{1000}; // вставка
	for(int i = 0 ; i < 11; i++ )
		map1.insert("string_" + std::to_string(i));
	for(int i = 0 ; i < 11; i++ )
		map1.insert("string_" + std::to_string(i));
	for(int i = 11 ; i < 19; i++ )
		map1.insert("string_" + std::to_string(i));
	std::cout << map1 << std::endl;

	std::cout << "----------------------\n";

	map1.erase("string_9"); // проверка удаления 
	std::cout << map1 << std::endl;
	map1.erase("string_13");
	map1.erase("string_9");
	std::cout << map1 << std::endl;

	std::cout << "----------------------\n";
	std::cout << map1[28]->key_ << std::endl; // проверка []

	std::cout << "----------------------\n";

	std::cout << map1.find("string_14")->key_ << std::endl; 
}