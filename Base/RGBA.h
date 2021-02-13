#pragma once

namespace Beginner
{
	struct RGBA
	{
		RGBA() :r(255), g(255), b(255), a(255) {}
		RGBA(const unsigned char ch[4]) :RGBA(ch[0], ch[1], ch[2], ch[3]) {}
		RGBA(const unsigned char R, const unsigned char G, const unsigned char B, const unsigned char A)
			:r(R), g(G), b(B), a(A) {}

		~RGBA() {}

		unsigned char r, g, b, a;
	};
}