#include <cmath>
#include <string>

#include "class.hpp"

namespace pool {
std::string task::compute_color(double hue) {
	double val = 1;
	double sat = 1;
	double c = val * sat;
	double x = c * (1 - fabs(fmod(hue / (M_PI / 3), 2) - 1));
	double m = val - c;

	double r, g, b;

	if (hue < 1 * M_PI / 3)
		r = c, g = x, b = 0;
	else if (hue < 2 * M_PI / 3)
		r = x, g = c, b = 0;
	else if (hue < 3 * M_PI / 3)
		r = 0, g = c, b = x;
	else if (hue < 4 * M_PI / 3)
		r = 0, g = x, b = c;
	else if (hue < 5 * M_PI / 3)
		r = x, g = 0, b = c;
	else
		r = c, g = 0, b = x;

	char* str;
	if (asprintf(
			&str,
			"%02x%02x%02x",
			(int)((r + m) * 255),
			(int)((g + m) * 255),
			(int)((b + m) * 255))
		< 0) {
		// TODO: errors
		fprintf(stderr, "compute_color crashed work on errors people");
	};

	return str;
}
}  // namespace pool
