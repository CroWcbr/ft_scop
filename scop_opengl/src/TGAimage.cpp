#include "TGAimage.hpp"

#include <iostream>
#include <fstream>
#include <cstring>

TGAimage::TGAimage(const char* filename)
	: m_filename(filename)
{
	m_resultCode = read_tga_file();

	std::cout << "texture file:\t";
	std::cout << (strrchr(filename, '/') ? strrchr(filename, '/') + 1 : filename) << std::endl;
	std::cout << "loading:\t";
	if (m_resultCode)
	{
		std::cout << "FAILED" << std::endl;
	}
	else
	{
		std::cout << "OK" << std::endl;
		std::cout << "tga info:\t" << m_width << "x" << m_height << "/" << m_bytespp * 8 << "/" << m_datatypecode << std::endl;
	}
	std::cout << std::endl;
}

TGAimage::~TGAimage()
{
	delete [] tga_image;
}

int	TGAimage::read_tga_file()
{
	std::ifstream file(m_filename, std::ios::binary);

	if (!file.is_open())
	{
		std::cerr << "TGAimage Error: can't open file: " << m_filename << std::endl;
		file.close();
		return 1;
	}

	TGA_Header header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	if (!file.good())
	{
		std::cerr << "TGAimage Error: cann't read the header" << std::endl;
		file.close();
		return 2;
	}

	// print_tga_header(header);

	m_width   = header.width;
	m_height  = header.height;
	m_bytespp = header.bitsperpixel >> 3;
	m_datatypecode = header.datatypecode;

	if (m_width <= 0 || m_height <= 0 || \
		(m_bytespp != bitsperpixel_format::GRAYSCALE && \
		m_bytespp != bitsperpixel_format::RGB && \
		m_bytespp != bitsperpixel_format::RGBA)) 
	{
		std::cerr << "TGAimage Error: bad width or heightbpp or bpp value" << std::endl;
		file.close();
		return 3;
	}

	unsigned long nbytes = m_bytespp * m_width * m_height;
	tga_image = new unsigned char[nbytes];

	if (m_datatypecode == 3 || m_datatypecode == 2)
	{
		file.read(reinterpret_cast<char*>(tga_image), nbytes);
		if (!file.good())
		{
			std::cerr << "TGAimage Error: an error occured while reading the datatypecode " << m_datatypecode << std::endl;
			file.close();
			return 4;
		}
	}
	else if (m_datatypecode == 10 || m_datatypecode == 11)
	{
		if (!load_rle_data(file))
		{
			std::cerr << "TGAimage Error: an error occured while reading the datatypecode " << m_datatypecode << std::endl;
			file.close();
			return 5;
		}
	}
	else
	{
		std::cerr << "TGAimage Error: unknown file format: " << static_cast<int>(header.datatypecode) << std::endl;
		file.close();
		return 6;
	}

	flip_color();
	if (header.imagedescriptor & 0x20)
	{
		flip_vertically();
	}

	return 0;
}

bool	TGAimage::load_rle_data(std::ifstream& file)
{
	unsigned long pixelcount = m_width * m_height;
	unsigned long currentpixel = 0;
	unsigned long currentbyte  = 0;

	while (currentpixel < pixelcount)
	{
		unsigned char chunkheader = 0;
		chunkheader = file.get();
		if (!file.good()) {
			std::cerr << "TGAimage Error: an error occured while reading chunkheader" << std::endl;
			return false;
		}
		if (chunkheader < 128)
		{
			chunkheader++;
			currentpixel += chunkheader;
			if (currentpixel > pixelcount) {
				std::cerr << "TGAimage Error: too many pixels read" << std::endl;
				return false;
			}
			file.read(reinterpret_cast<char*>(tga_image + currentbyte), chunkheader * m_bytespp);
			if (!file.good()) {
				std::cerr << "TGAimage Error: an error occured while reading data" << std::endl;
				return false;
			}
			currentbyte += chunkheader * m_bytespp;
		}
		else
		{
			chunkheader -= 127;
			char pixelData[4];
			file.read(pixelData, m_bytespp);
			if (!file.good()) {
				std::cerr << "TGAimage Error: an error occured while reading data" << std::endl;
				return false;
			}
			for (int i = 0; i < chunkheader; i++)
			{
				currentpixel++;
				if (currentpixel > pixelcount)
				{
					std::cerr << "Too many pixels read" << std::endl;
					return false;
				}

				memcpy(tga_image + currentbyte, pixelData, m_bytespp);
				currentbyte += m_bytespp;
			}
		}
	}
	return true;
}

void TGAimage::flip_vertically() 
{
	if (!tga_image)
		return ;
	unsigned long bytes_per_line = m_width * m_bytespp;
	unsigned char *line = new unsigned char[bytes_per_line];
	int half = m_height >> 1;
	for (int j = 0; j<half; j++)
	{
		unsigned long l1 = j * bytes_per_line;
		unsigned long l2 = (m_height - 1 - j) * bytes_per_line;
		memmove((void *)line, (void *)(tga_image + l1), bytes_per_line);
		memmove((void *)(tga_image + l1), (void *)(tga_image + l2), bytes_per_line);
		memmove((void *)(tga_image + l2), (void *)line, bytes_per_line);
	}
	delete [] line;
}

void TGAimage::flip_color() 
{
	if (!tga_image)
		return ;

	unsigned long nbytes = m_bytespp * m_width * m_height;
	if (m_bytespp == 3)
	{
		unsigned char tmp;
		for (int i = 0; i < nbytes; i += 3)
		{
			tmp = tga_image[i];
			tga_image[i] = tga_image[i + 2];
			tga_image[i + 2] = tmp;
		}
	}
	else if (m_bytespp == 4)
	{
		unsigned char tmp;
		for (int i = 0; i < nbytes; i += 4)
		{
			tmp = tga_image[i];
			tga_image[i] = tga_image[i + 3];
			tga_image[i + 3] = tmp;
			tmp = tga_image[i + 1];
			tga_image[i] = tga_image[i + 2];
			tga_image[i + 2] = tmp;
		}
	}
}

void TGAimage::flip_horizontally()
{
	if (!tga_image)
		return ;

	int half = m_width >> 1;
	unsigned char color[m_bytespp];
	for (int i = 0; i < half; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			memcpy(color, \
					tga_image + (j * m_width + i) * m_bytespp, \
					m_bytespp);
			memcpy(tga_image + (j * m_width + i) * m_bytespp, \
					tga_image + (j * m_width + m_width - 1 - i) * m_bytespp, \
					m_bytespp);
			memcpy(tga_image + (j * m_width + m_width - 1 - i) * m_bytespp, \
					color, \
					m_bytespp);
		}
	}
}

void	TGAimage::print_tga_header(TGA_Header& header)
{
	std::cout << "idlength\t" << (int)header.idlength << std::endl;
	std::cout << "colormaptype\t" << (int)header.colormaptype << std::endl;
	std::cout << "datatypecode\t" << (int)header.datatypecode << std::endl;
	std::cout << "colormaporigin\t" << (int)header.colormaporigin << std::endl;
	std::cout << "colormaplength\t" << (int)header.colormaplength << std::endl;
	std::cout << "colormapdepth\t" << (int)header.colormapdepth << std::endl;
	std::cout << "x_origin\t" << (int)header.x_origin << std::endl;
	std::cout << "y_origin\t" << (int)header.y_origin << std::endl;
	std::cout << "width\t" << (int)header.width << std::endl;
	std::cout << "height\t" << (int)header.height << std::endl;
	std::cout << "bitsperpixel\t" << (int)header.bitsperpixel << std::endl;
	std::cout << "imagedescriptor\t" << (int)header.imagedescriptor << std::endl;
}
