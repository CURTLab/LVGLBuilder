#include "LVGLFont.h"

#include <QDebug>
#include <QFile>
#include <QtMath>
#include "LVGLCore.h"

#include <ft2build.h>
#include FT_FREETYPE_H

LVGLFont::LVGLFont(const QString &name, const QString &codeName, const lv_font_t *font)
	: m_font(const_cast<lv_font_t*>(font))
	, m_name(name)
	, m_codeName(codeName)
	, m_size(m_font->line_height)
	, m_bpp(4)
{
}

LVGLFont *LVGLFont::parse(const QString &fileName, uint8_t size, uint8_t bpp, uint32_t unicodeFirst, uint32_t unicodeLast)
{
	FT_Face face;
	int error = FT_New_Face(lvgl.m_ft, qPrintable(fileName), 0, &face);
	if (error == FT_Err_Unknown_File_Format) {
		qCritical() << "unkown file format";
		return nullptr;
	} else if (error) {
		qCritical() << "error loading font" << FT_Error_String(error);
		return nullptr;
	}

	//QString unicode_start_str = "U+" + QString::number(unicodeFirst, 16).rightJustified(4, '0');
	//QString unicode_last_str  = "U+" + QString::number(unicodeLast, 16).rightJustified(4, '0');

	FT_Set_Char_Size(face, 0, size * 64, 300, 300);
	FT_Set_Pixel_Sizes(face, 0, size);

	const int mask = (~(((1 << bpp) - 1) & 0xff)) & 0xff;

	std::vector<uint8_t> gylph_bitmap;

	const uint32_t glyphs = unicodeLast - unicodeFirst + 1;
	lv_font_fmt_txt_glyph_dsc_t *glyph_dsc = new lv_font_fmt_txt_glyph_dsc_t[glyphs + 1];
	memset(glyph_dsc, 0, sizeof(lv_font_fmt_txt_glyph_dsc_t));

	for(uint32_t unicode_act = unicodeFirst; unicode_act <= unicodeLast; ++unicode_act)
	{
		int error = FT_Load_Char(face, unicode_act, FT_LOAD_RENDER);
		if (error) {
			qCritical() << "Error" << error << FT_Error_String(error);
			continue;
		}

		const size_t curr_buf_size = gylph_bitmap.size();

		uint8_t w = 0, h = 0;
		if (face->glyph->bitmap.buffer != nullptr) {
			FT_Bitmap bitmap = face->glyph->bitmap;

			w = bitmap.width & 0xff;
			h = bitmap.rows & 0xff;

			const uint size_bitmap = bitmap.rows * bitmap.width;
			const size_t buf_size = size_bitmap * bpp / 8;
			gylph_bitmap.resize(curr_buf_size + buf_size);

			uint8_t *buf_in = bitmap.buffer;
			uint8_t *buf_out = gylph_bitmap.data() + curr_buf_size;

			// convert to bpp format
			for (uint j = 0; j < buf_size; ++j) {
				*buf_out = 0;
				for (int i = 0; i < 8; i += bpp) {
					*buf_out |= (((*buf_in) & mask) >> i);
					++buf_in; // avoid pointing to not allocated memory
				}
				++buf_out; // avoid pointing to not allocated memory
			}
		}

		lv_font_fmt_txt_glyph_dsc_t &desc = glyph_dsc[unicode_act - unicodeFirst + 1];
		desc.bitmap_index = curr_buf_size & 0xffff;
		const double advance = face->glyph->metrics.horiAdvance / 64.0;
		desc.adv_w = uint32_t(std::ceil(advance * 16));
		desc.box_w = w;
		desc.box_h = h;
		desc.ofs_x = static_cast<int8_t>(face->glyph->bitmap_left);
		desc.ofs_y = static_cast<int8_t>(face->glyph->bitmap_top) - h;

		//qDebug() << QString::number(unicode_act, 16).rightJustified(4, '0') << QChar(unicode_act);
	}

	uint8_t *gylph_bitmap_buf = new uint8_t[gylph_bitmap.size()];
	memcpy(gylph_bitmap_buf, gylph_bitmap.data(), gylph_bitmap.size());

	// allocate cmap
	lv_font_fmt_txt_cmap_t *cmap = new lv_font_fmt_txt_cmap_t[2];
	memset(cmap, 0, sizeof(lv_font_fmt_txt_cmap_t) * 2);

	cmap[0].range_start = unicodeFirst;
	cmap[0].range_length = static_cast<uint16_t>(unicodeLast - unicodeFirst);
	cmap[0].glyph_id_start = 1;

	cmap[1].unicode_list = nullptr;
	cmap[1].glyph_id_ofs_list = nullptr;
	cmap[1].list_length = 0;
	cmap[1].type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY;

	// allocate txt description
	lv_font_fmt_txt_dsc_t *font_dsc = new lv_font_fmt_txt_dsc_t;
	font_dsc->glyph_bitmap = gylph_bitmap_buf;
	font_dsc->glyph_dsc = glyph_dsc;
	font_dsc->cmaps = cmap;
	font_dsc->kern_dsc = nullptr;
	font_dsc->kern_scale = 0;
	font_dsc->cmap_num = 1;
	font_dsc->bpp = bpp;
	font_dsc->kern_classes = 0;
	font_dsc->bitmap_format = LV_FONT_FMT_TXT_PLAIN;

	// allocate font in with default callbacks
	lv_font_t *font = new lv_font_t;
	font->get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt;
	font->get_glyph_bitmap = lv_font_get_bitmap_fmt_txt;
	font->line_height = face->size->metrics.height / 64;
	font->base_line = -face->size->metrics.descender / 64;
	font->subpx = LV_FONT_SUBPX_NONE;
	font->dsc = font_dsc;

	// generate font name
	QString baseName(face->family_name);
	if (strcmp(face->style_name, "Regular") != 0)
		baseName += QString(" %1").arg(face->style_name);

	QString name = QString("%1 %2").arg(baseName).arg(size);
	return new LVGLFont(name, name.toLower().replace(" ", "_").replace("-", "_"), font);
}

const lv_font_t *LVGLFont::font() const
{
	return m_font;
}

QString LVGLFont::name() const
{
	return m_name;
}

QString LVGLFont::codeName() const
{
	return m_codeName;
}

bool LVGLFont::saveAsCode(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
		return false;

	lv_font_fmt_txt_dsc_t *fdsc = reinterpret_cast<lv_font_fmt_txt_dsc_t *>(m_font->dsc);

	QTextStream stream(&file);
	stream << "#include \"lvgl.h\"\n\n";

	const QString defName = m_codeName.toUpper();
	const QString output_name = "lv_font_helvetica_17";

	stream << "/*******************************************************************************\n";
	stream << " * Size: " << m_size << " px\n";
	stream << " * Bpp: " << fdsc->bpp << "\n";
	stream << " * Opts: \n";
	stream << " ******************************************************************************/\n\n";
	stream << "#ifndef " << defName << "\n";
	stream << "#define " << defName << " 1\n";
	stream << "#endif\n\n";
	stream << "#if " << defName << "\n\n";
	stream << "/*-----------------\n";
	stream << " *    BITMAPS\n";
	stream << " *----------------*/\n\n";
	stream << "/*Store the image of the letters (glyph)*/\n";
	stream << "static const uint8_t " << output_name << "_glyph_bitmap[] =\n{\n";

	stream << "}\n\n";

	stream << "#endif /*#if " << defName << "*/\n";

	file.close();
	return true;
}
