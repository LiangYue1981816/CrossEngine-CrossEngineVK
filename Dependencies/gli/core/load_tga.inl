#include "file.hpp"
#include <cstdio>
#include <cassert>

namespace gli {
	namespace detail
	{
		struct tga_header
		{
			char idlength;
			char colourmaptype;
			char imagetype;
			short int colourmaporigin;
			short int colourmaplength;
//			char colourmapdepth;
			short int x_origin;
			short int y_origin;
			short width;
			short height;
			char bitsperpixel;
			char imagedescriptor;
		};

		inline texture load_tga(char const* Data, std::size_t Size)
		{
			detail::tga_header const &Header(*reinterpret_cast<detail::tga_header const*>(Data));

			if (Header.imagetype != 2)
				return texture();

			if (Header.bitsperpixel != 24 && Header.bitsperpixel != 32)
				return texture();

			texture Texture(
				TARGET_2D,
				Header.bitsperpixel == 24 ? FORMAT_BGR8_UNORM_PACK8 : FORMAT_BGRA8_UNORM_PACK8,
//				Header.bitsperpixel == 24 ? FORMAT_RGB8_UNORM_PACK8 : FORMAT_RGBA8_UNORM_PACK8,
				texture::extent_type(std::max<texture::size_type>(Header.width, 1), std::max<texture::size_type>(Header.height, 1), 1),
				1,
				1,
				1);

			std::memcpy(Texture.data(0, 0, 0), Data + sizeof(detail::tga_header), Texture.size(0));

			return Texture;
		}
	}
	//namespace detail

	inline texture load_tga(char const* Data, std::size_t Size)
	{
		return detail::load_tga(Data, Size);
	}

	inline texture load_tga(char const* Filename)
	{
		FILE* File = detail::open_file(Filename, "rb");
		if (!File)
			return texture();

		long Beg = std::ftell(File);
		std::fseek(File, 0, SEEK_END);
		long End = std::ftell(File);
		std::fseek(File, 0, SEEK_SET);

		std::vector<char> Data(static_cast<std::size_t>(End - Beg));

		std::fread(&Data[0], 1, Data.size(), File);
		std::fclose(File);

		return load_tga(&Data[0], Data.size());
	}

	inline texture load_tga(std::string const& Filename)
	{
		return load_tga(Filename.c_str());
	}
}
//namespace gli
