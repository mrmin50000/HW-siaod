#import "uni_doc_template.typ": uni_doc

#let title_page = [
  #set text(font: "Times New Roman", size: 14pt)
  #set align(center)

  #image("mirea_titul_logo.png")
  #text(size: 11pt)[МИНИСТЕРСТВО НАУКИ И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ]\
  #text(size: 13pt)[ФЕДЕРАЛЬНОЕ ГОСУДАРСТВЕННОЕ БЮДЖЕТНОЕ
  ОБРАЗОВАТЕЛЬНОЕ УЧРЕЖДЕНИЕ ВЫСШЕГО ОБРАЗОВАНИЯ]
  == "МИРЭА - Российский технологический университет"
  = РТУ МИРЭА
  
  #image("titul_fancy_line.png", width: 110%)

  \ \

  Отчет по выполнению практического задания №5.1

  \

  *Тема:* РАБОТА С ДАННЫМИ ИЗ ФАЙЛА.\
  Битовые операции. Сортировка числового файла с помощью битового массива

  *Дисциплина*: Структуры и алгоритмы обработки данных
  
  \ \ \ \
  
  #align(right)[
    #grid(
      columns: 2,
      align: (right, center),
      row-gutter: 7pt,
      column-gutter: 15pt,
      [Выполнил студент], [Салтыков Д. М.],
      [группа], [ИКБО-50-24]
    )
    
  ]

  #align(bottom)[*Москва 2025*]
]

#show: uni_doc.with(
  title-page: title_page
)

*Цель работы*: освоить приёмы работы с битовым представлением беззнаковых
целых чисел, реализовать эффективный алгоритм внешней сортировки на основе
битового массива  

*Ход работы:*

= Задание 1
- == а) Реализуйте вышеприведённый пример, проверьте правильность результата в том числе и на других значениях х.
Инициализируем переменную беззнакового символьного типа и проводим битовые операции. В данном случае установка 5-го бита целого числа в 0.
```cpp
#include <iostream>
#include <bitset>

int main() {
	unsigned char x = 255;
	std::cout << "Previous x: " << std::bitset<8> (x);
	unsigned char maska = 1;
	x = x & (~(maska<<4));
	std::cout << "\nCurrent x:  " << std::bitset<8> (x) <<"\nResult: " << (unsigned int) x << '\n';
}
```
Вывод программы для х = 255:
#image("Screenshot_20250907_172146.png")
Вывод программы для х = 222:
#image("Screenshot_20250907_172334.png")
- == б) Реализуйте по аналогии с предыдущим примером установку 7-го бита числа в единицу.
Инициализируем переменную беззнакового символьного типа и проводим битовые операции. В данном случае установка 7-го бита целого числа в 1.
```cpp
#include <iostream>
#include <bitset>

int main() {
	unsigned char x = 128;
	std::cout << "Previous x: " << std::bitset<8> (x);
	unsigned char maska = 1;
	x = x | (maska<<6);
	std::cout << "\nCurrent x:  " << std::bitset<8> (x) <<"\nResult: " << (unsigned int) x << '\n';
}
```
Вывод программы для х = 128:
#image("Screenshot_20250907_172816.png")
- == в) Реализуйте код листинга 1, объясните выводимый программой результат.
У нас есть изначальная маска размером в 32 бита с ведущей единицей. В теле цикла происходит следующее:
+ Сначала вычисляется поразрядная конъюнкция
+ Затем происходит сдвиг вправо на (n - i)
+ Маска также сдвигается вправо на один бит
Образно говоря, мы тащим справа налево число 25 (bin: 11001) по маске, с каждым разом делая поразрядную конъюнкцию, с шагом 1 бит.
```cpp
#include <iostream>
#include <bitset>

int main() {
	unsigned int x = 25;
	const int n = sizeof(int)*8; //=32 - количество разрядов в числе типа int
	unsigned maska = (1 << n - 1); //1 в старщем бите 32-разрядной сетки
	std::cout << "Начальный вид маски: " << std::bitset<n> (maska) << '\n';
	std::cout << "Результат: ";
	for (int i = 1; i <= n; ++i) { //32 раза - по количеству разрядов
		std::cout << ((x & maska) >> (n - i)); 
		maska = maska >> 1; //смещение 1 в маске на разряд вправо
	}
	std::cout << '\n';
	return 0;
}
```
Вывод программы:
#image("Screenshot_20250907_174818.png")
= Задание 2
- == а) Реализуйте вышеописанный пример с вводом произвольного набора до 8-ми чисел (со значениями от 0 до 7) и его сортировкой битовым массивом в виде числа типа unsigned char. Проверьте работу программы.
+ Вводим размер и элементы массива.
+ Сортируем с помощью битовых операций, вставляя единицу в битовую последовательность в индекс arr[i] слева направо.
+ Выводим результат
```cpp
#include <bitset>
#include <iostream>

int main() {
	//Input
	int n;
	std::cout << "Type size of array (0-8): ";
	std::cin >> n;
	if (n > 8) {
		std::cout << "I told u..\n";
		return 0;
	}

	int arr[n];
	for (int i = 0; i < n; ++i) {
		std::cout << "Type array's element (0-7): ";
		std::cin >> arr[i];
		if (arr[i] > 7 || arr[i] < 0) {
			std::cout << "I told u..\n";
			return 0;
		}
	}
	
	//Sort
	unsigned char bitarr = 0;
	unsigned char mask = (1 << 7);
	for (int i = 0; i < n; ++i) {
		bitarr = bitarr | (mask>>arr[i]);
		std:: cout << "Sorting process: " <<std::bitset<8> (bitarr) << '\n';
	}
	
	//Sorted sequence output:
	std::cout << "Sorted array: ";
	for (int i = 0; i < 8; ++i) {
		if (bitarr & (mask >> i))
			std::cout << i << ' ';
	}
	std::cout << '\n';

	return 0;
}
```
\
\
\
\
\
\
\

Вывод программы: 
#image("Screenshot_20250907_191834.png")
- == б) Адаптируйте вышеприведённый пример для набора из 64-х чисел (со значениями от 0 до 63) с битовым массивом в виде числа типа unsigned long long.
Практически идентично пункту а)
```cpp
#include <bitset>
#include <iostream>

int main() {
	//Input
	int n;
	std::cout << "Type size of array (0-64): ";
	std::cin >> n;
	if (n > 64) {
		std::cout << "I told u..\n";
		return 0;
	}

	int arr[n];
	for (int i = 0; i < n; ++i) {
		std::cout << "Type array's element (0-63): ";
		std::cin >> arr[i];
		if (arr[i] > 63 || arr[i] < 0) {
			std::cout << "I told u..\n";
			return 0;
		}
	}
	
	//Sort
	unsigned long long bitarr = 0;
	unsigned long long mask = 0x8000000000000000;
	for (int i = 0; i < n; ++i) {
		bitarr = bitarr | (mask>>arr[i]);
		std:: cout << "Sorting process: " <<std::bitset<64> (bitarr) << '\n';
	}
	
	//Sorted sequence output:
	std::cout << "Sorted array: ";
	for (int i = 0; i < 64; ++i) {
		if (bitarr & (mask >> i))
			std::cout << i << ' ';
	}
	std::cout << '\n';

	return 0;
}
```
\
\
\
\
\
\
\

Вывод программы:
#image("Screenshot_20250907_193501.png")
- == в) Исправьте программу задания 2.б, чтобы для сортировки набора из 64-х чисел использовалось не одно число типа unsigned long long, а линейный массив чисел типа unsigned char.
В этом случае я использовал пару переменных как индексы байтов и битов в этих байтах.

byte_index = i / 8 и bit_index = i % 8
```cpp
#include <bitset>
#include <iostream>
#include <vector>

int main() {
	//Input
	int n;
	std::cout << "Type size of array (0-64): ";
	std::cin >> n;
	if (n > 64) {
		std::cout << "I told u..\n";
		return 0;
	}

	int arr[n];
	for (int i = 0; i < n; ++i) {
		std::cout << "Type array's element (0-63): ";
		std::cin >> arr[i];
		if (arr[i] > 63 || arr[i] < 0) {
			std::cout << "I told u..\n";
			return 0;
		}
	}
	
	//Sort
	std::vector<unsigned char> bitarr = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char mask = 1 << 7;
	for (int i = 0; i < n; ++i) {
		int byte_index = arr[i] / 8;
		int bit_index = arr[i] % 8;
		bitarr[byte_index] = bitarr[byte_index] | (mask >> bit_index);
		std::cout << "Sorting process: ";
		for (int j = 0; j < 8; ++j) {
			std::cout << std::bitset<8> (bitarr[j]);
		}
		std::cout << '\n';
	}
	
	//Sorted sequence output:
	std::cout << "Sorted array: ";
	for (int i = 0; i < 64; ++i) {
		int bit_index = i % 8;
		int byte_index = i / 8;
		if (bitarr[byte_index] & (mask >> bit_index))
			std::cout << i << ' ';
	}
	std::cout << '\n';

	return 0;
}
```
Вывод программы:
#image("Screenshot_20250907_215610.png")

= Задание 3
- == Реализуйте задачу сортировки числового файла с заданными условиями. Добавьте в код возможность определения времени работы программы.
