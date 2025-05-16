#pragma once
#include<vector>
#include <string>
struct Pixel {
	float red, green, blue;

	Pixel();
	Pixel(float r, float g, float b);
	~Pixel();
};
class bitmap {
public:
	bitmap(int width, int height);
	~bitmap();
	bitmap();

	int get_width() const;
	int get_hight() const;

	void set_pixel(const Pixel& color, int x, int y);

	void load_image(std::string filename);
	void export_image(std::string filename) const;

	void draw_point(int x, int y);
	void bresenham_line_drawing(int x1, int y1, int x2, int y2);

	void show_image();

private:
	int width;
	int height;
	std::vector<std::vector<Pixel>> pixels;
};