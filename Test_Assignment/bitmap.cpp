#include"bitmap.h"
#include<vector> 
#include<iostream>
#include<fstream>
#include <windows.h>
#include <string>
#include <cstdio>

Pixel::Pixel() : red(0), green(0), blue(0) {
};
Pixel::Pixel(float r, float g, float b) : red(r), green(g), blue(b) {

};
Pixel::~Pixel() {

};
bitmap::bitmap() {}
bitmap::bitmap(int _width, int _height) : width(_width),
height(_height) {

};
bitmap::~bitmap() {

};

// поставить пикселю по индексу цвет
void bitmap::set_pixel(const Pixel& pixel, int x, int y) {
	pixels[y][x].red = pixel.red;
	pixels[y][x].green = pixel.green;
	pixels[y][x].blue = pixel.blue;
};

// получить ширину рисунка
int bitmap::get_width() const {
	return this->width;
};
// получить высоту рисунка
int bitmap::get_hight() const {
	return this->height;
};

// загрузить рисунок из файла
void bitmap::load_image(std::string filename) {
	std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);

	if (!file)
	{
		std::cout << "Wrong path\n";
		return;
	}

	BITMAPFILEHEADER bmfh = {};
	BITMAPINFOHEADER bmih = {};
	file.read((char*)(&bmfh), sizeof(BITMAPFILEHEADER));
	file.read((char*)(&bmih), sizeof(BITMAPINFOHEADER));

	width = bmih.biWidth;
	height = bmih.biHeight;

	if (bmih.biBitCount != 24) {
		std::cout << "Unsupported format\n";
		return;
	}

	file.seekg(bmfh.bfOffBits);

	bool flip = true;
	if (height < 0)
	{
		flip = false;
		height = -height;
	}

	for (int row = 0; row < bmih.biHeight; row++)
	{
		std::vector <Pixel> row_data;

		for (int col = 0; col < bmih.biWidth; col++)
		{
			Pixel p = Pixel{};
			
			p.blue = file.get();
			p.green = file.get();
			p.red = file.get();

			row_data.push_back(p);
		}
		file.seekg(width % 4, std::ios::cur);
		if (flip)
		{
			(pixels).insert(pixels.begin(), row_data);
		}
		else
		{
			(pixels).push_back(row_data);
		}
	}
	file.close();
}

// загрузить рисунок в файл
void bitmap::export_image(std::string filename) const {
	std::ofstream file(filename, std::ios_base::binary);
	if (!file)
	{
		std::cout << "Wrong path\n";
		return;
	}

	BITMAPFILEHEADER bmfh;
	bmfh.bfType = 0x4d42;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 

	bmfh.bfSize = bmfh.bfOffBits + (pixels.size() * 3 + pixels[0].size() % 4) * pixels.size();
	file.write((char*)&bmfh, sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER bmih;
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = pixels[0].size();
	bmih.biHeight = pixels.size();
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = 0;
	bmih.biSizeImage = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;
	file.write((char*)&bmih, sizeof(BITMAPINFOHEADER));

	for (int row = pixels.size() - 1; row >= 0; row--)
	{
		const std::vector <Pixel>& row_data = pixels[row];

		for (int col = 0; col < row_data.size(); col++)
		{
			const Pixel& pix = row_data[col];
			
			file.put((unsigned char)(pix.blue));
			file.put((unsigned char)(pix.green));
			file.put((unsigned char)(pix.red));
		}

		for (int i = 0; i < row_data.size() % 4; i++)
		{
			file.put(0);
		}
	}
	file.close();
}

// вывести рисунок в консоль
void bitmap::show_image() {
	std::cout << "Showing image:\n\n";
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			auto& color = pixels[h][w];
			if (color.red == 255 && color.blue == 255 && color.green == 255)
				std::cout << "0";
			else
				std::cout << "1";
		}
		std::cout << '\n';
	}
}

// нарисовать точку
void bitmap::draw_point(int x, int y) {
	Pixel p(0, 0, 0);
	set_pixel(p, x, y);
}

// рисование линии по алгоритму Брезенхэма
void bitmap::bresenham_line_drawing(int x1, int y1, int x2, int y2) {
	bool steep = abs(y2 - y1) > abs(x2 - x1);
	if (steep) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}
	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	int dx = x2 - x1;
	int dy = abs(y2 - y1);
	int error = dx / 2; 
	int ystep = (y1 < y2) ? 1 : -1; 
	int y = y1;
	for (int x = x1; x <= x2; x++)
	{
		(*this).draw_point(steep ? y : x, steep ? x : y);
		error -= dy;
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
};