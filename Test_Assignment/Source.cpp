#include <iostream>
#include"bitmap.h"
#include <string>
int main() {
	std::string file_name;
	std::cout << "Please enter input file path: \n";
	std::cin >> file_name;

	bitmap bmp = bitmap();
	bmp.load_image(file_name);
	bmp.show_image();

	bmp.bresenham_line_drawing(0, 0, bmp.get_width() - 1, bmp.get_hight() - 1);
	bmp.bresenham_line_drawing(bmp.get_width() - 1, 0, 0, bmp.get_hight() - 1);

	bmp.show_image();

	std::cout << "Please enter export file path: \n";
	std::cin >> file_name;

	bmp.export_image(file_name);
	return 0;
}