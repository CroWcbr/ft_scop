#pragma once

#include <fstream>

#pragma pack(push,1)
struct TGA_Header {
	char idlength;
	char colormaptype;
	char datatypecode;
	short colormaporigin;
	short colormaplength;
	char colormapdepth;
	short x_origin;
	short y_origin;
	short width;
	short height;
	char  bitsperpixel;
	char  imagedescriptor;
};
#pragma pack(pop)

enum bitsperpixel_format 
{
	GRAYSCALE = 1,
	RGB = 3,
	RGBA = 4
};

class TGAimage
{
private:
	const char*		m_filename;
	unsigned char*	tga_image = nullptr;
	int				m_width;
	int				m_height;
	int				m_bytespp;
	int				m_datatypecode;
	int				m_resultCode;

private:
	int				read_tga_file();
	bool			load_rle_data(std::ifstream& file);
	void			flip_vertically();
	void			flip_horizontally();
	void			flip_color();

	void			print_tga_header(TGA_Header& header);

public:
	TGAimage() = delete;
	TGAimage(const TGAimage& copy) = delete;
	TGAimage& operator=(const TGAimage& copy) = delete;
	TGAimage(TGAimage&& copy) = delete;
	TGAimage& operator=(TGAimage&& copy) = delete;

	TGAimage(const char *filename);
	~TGAimage();

	const unsigned char*	getTGAimage() const { return tga_image; }
	int						getResultCode() const { return m_resultCode; }
	int						getWidth() const { return m_width; }
	int						getHeight() const { return m_height; }
	int						getBytespp() const { return m_bytespp; }
};
