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
	, m_customFont(false)
{
}

LVGLFont::~LVGLFont()
{
	if (m_customFont) {
		lv_font_fmt_txt_dsc_t *fdsc = reinterpret_cast<lv_font_fmt_txt_dsc_t *>(m_font->dsc);
		delete [] fdsc->glyph_bitmap;
		delete [] fdsc->glyph_dsc;
		delete [] fdsc->cmaps;
		delete fdsc;
		delete m_font;
		m_font = nullptr;
	}
}

LVGLFont *LVGLFont::parse(const QString &fileName, uint8_t size, uint8_t bpp, uint32_t unicodeFirst, uint32_t unicodeLast)
{
	FT_Face face;
	int error = FT_New_Face(lvgl.m_ft, qPrintable(fileName), 0, &face);
	if (error == FT_Err_Unknown_File_Format) {
		qCritical() << "LVGLFont: Unkown file format";
		return nullptr;
	} else if (error) {
		qCritical() << "LVGLFont: Error loading font:" << FT_Error_String(error);
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

	uint32_t counter = 0;
	for(uint32_t unicodeAct = unicodeFirst; unicodeAct <= unicodeLast; ++unicodeAct) {
		int error = FT_Load_Char(face, unicodeAct, FT_LOAD_RENDER);
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

		lv_font_fmt_txt_glyph_dsc_t &desc = glyph_dsc[unicodeAct - unicodeFirst + 1];
		desc.bitmap_index = curr_buf_size & 0xffff;
		const double advance = face->glyph->metrics.horiAdvance / 64.0;
		desc.adv_w = uint32_t(std::ceil(advance * 16));
		desc.box_w = w;
		desc.box_h = h;
		desc.ofs_x = static_cast<int8_t>(face->glyph->bitmap_left);
		desc.ofs_y = static_cast<int8_t>(face->glyph->bitmap_top) - h;

		++counter;

		//qDebug() << QString::number(unicodeAct, 16).rightJustified(4, '0') << QChar(unicodeAct);
	}
	Q_ASSERT(counter == glyphs);

	uint8_t *gylph_bitmap_buf = new uint8_t[gylph_bitmap.size()];
	memcpy(gylph_bitmap_buf, gylph_bitmap.data(), gylph_bitmap.size());

	// allocate cmap
	lv_font_fmt_txt_cmap_t *cmap = new lv_font_fmt_txt_cmap_t[1];

	cmap[0].range_start = unicodeFirst;
	cmap[0].range_length = static_cast<uint16_t>(glyphs & 0xffff);
	cmap[0].glyph_id_start = 1;
	cmap[0].unicode_list = nullptr;
	cmap[0].glyph_id_ofs_list = nullptr;
	cmap[0].list_length = 0;
	cmap[0].type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY;

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
	//font->line_height = uint8_t((face->ascender / 64 - face->descender / 64));
	font->line_height = size;
	font->base_line = uint8_t(-face->descender / 64);
	font->subpx = LV_FONT_SUBPX_NONE;
	font->dsc = font_dsc;

	// generate font name
	QString baseName(face->family_name);
	if (strcmp(face->style_name, "Regular") != 0)
		baseName += QString(" %1").arg(face->style_name);

	QString name = QString("%1 %2").arg(baseName).arg(size);
	LVGLFont *ret = new LVGLFont(name, name.toLower().replace(" ", "_").replace("-", "_"), font);
	ret->m_customFont = true;
	ret->m_fileName = fileName;
	return  ret;
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

	if (fdsc->cmap_num != 1) {
		qCritical() << "LVGLFont: Char map count <==> 1!";
		return false;
	}

	const uint32_t unicodeFirst = fdsc->cmaps[0].range_start;
	const uint32_t unicodeSec = unicodeFirst + fdsc->cmaps[0].range_length - 1;

	QTextStream stream(&file);
	stream << "#include \"lvgl.h\"\n\n";

	const QString defName = m_codeName.toUpper();
	const QString output_name = "lv_font_helvetica_17";

	stream << "/*******************************************************************************\n";
	stream << " * Size: " << m_font->line_height << " px\n";
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
	stream << "/* Store the image of the letters (glyph)* /\n";

	stream << "static const uint8_t " << output_name << "_glyph_bitmap[] =\n{\n";
	for (uint32_t unicodeAct = unicodeFirst; unicodeAct <= unicodeSec; ++unicodeAct) {
		stream << "\t/* U+" << QString::number(unicodeAct, 16).rightJustified(4, '0') << " \"" <<  QChar(unicodeAct) << "\" */";
		const lv_font_fmt_txt_glyph_dsc_t &gdsc = fdsc->glyph_dsc[unicodeAct - unicodeFirst + 1];
		const uint32_t size = gdsc.box_w * gdsc.box_h * fdsc->bpp / 8;
		const uint8_t *buf = fdsc->glyph_bitmap + gdsc.bitmap_index;
		for (uint32_t i = 0; i < size; ++i, ++buf) {
			if (i % 8 == 0) stream << "\n\t";
			stream << "0x" << QString::number(*buf, 16).rightJustified(2, '0') << ", ";
		}
		stream << "\n";
		if (unicodeAct != unicodeSec) stream << "\n";
	}
	stream << "}\n\n";

	stream << "/*---------------------\n";
	stream << " *  GLYPH DESCRIPTION\n";
	stream << " *--------------------*/\n\n";

	stream << "static const lv_font_fmt_txt_glyph_dsc_t " << output_name << "_glyph_dsc[] =\n{\n";
	stream << "\t{.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,\n";
	const lv_font_fmt_txt_glyph_dsc_t *gdsc = fdsc->glyph_dsc + 1;
	for (uint32_t unicodeAct = unicodeFirst; unicodeAct <= unicodeSec; ++unicodeAct) {
		stream << "\t{.bitmap_index = " << gdsc->bitmap_index << ", .adv_w = "
				 << gdsc->adv_w << ", .box_w = " << gdsc->box_w << ", .box_h = "
				 << gdsc->box_h << ", .ofs_x = " << gdsc->ofs_x << ", .ofs_y = "
				 << gdsc->ofs_y << "}";
		stream << " /* U+" << QString::number(unicodeAct, 16).rightJustified(4, '0') << " \"" <<  QChar(unicodeAct) << "\" */";
		if (unicodeAct != unicodeSec) stream << ",";
		stream << "\n";
		++gdsc;
	}
	stream << "}\n\n";

	stream << "/*---------------------\n";
	stream << " *  CHARACTER MAPPING\n";
	stream << " *--------------------*/\n\n";
	stream << "/* Collect the unicode lists and glyph_id offsets */\n";
	stream << "static const lv_font_fmt_txt_cmap_t " << output_name << "_cmaps[] =\n{\n";
	stream << "\t{\n";
	stream << "\t\t.range_start = " << fdsc->cmaps[0].range_start
			 << ", .range_length = " << fdsc->cmaps[0].range_length
			 << ", .glyph_id_start = " << fdsc->cmaps[0].glyph_id_start << ",\n";
	stream << "\t\t.unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY\n";
	stream << "\t}\n";
	stream << "};\n\n";

	stream << "/*--------------------\n";
	stream << " *  ALL CUSTOM DATA\n";
	stream << " *--------------------*/\n\n";

	stream << "/* Store all the custom data of the font */\n";
	stream << "static lv_font_fmt_txt_dsc_t " << output_name << "_font_dsc = \n{\n";
	stream << "\t.glyph_bitmap = " << output_name << "_gylph_bitmap,\n";
	stream << "\t.glyph_dsc = " << output_name << "_glyph_dsc,\n";
	stream << "\t.cmaps = " << output_name << "_cmaps,\n";
	stream << "\t.kern_dsc = NULL,\n\t.kern_scale = 0,\n";
	stream << "\t.cmap_num = " << fdsc->cmap_num << ",\n";
	stream << "\t.bpp = " << fdsc->bpp << ",\n";
	stream << "\t.kern_classes = 0,\n";
	stream << "\t.bitmap_format = LV_FONT_FMT_TXT_PLAIN\n};\n\n";

	stream << "/*-----------------\n";
	stream << " *  PUBLIC FONT\n";
	stream << " *----------------*/\n\n";
	stream << "/* Initialize a public general font descriptor */\n";
	stream << "lv_font_t " << output_name << " = \n{\n";
	stream << "\t.get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /* Function pointer to get glyph's data */\n";
	stream << "\t.get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /* Function pointer to get glyph's bitmap */\n";
	stream << "\t.line_height = " << m_font->line_height << ",          /* The maximum line height required by the font */\n";
	stream << "\t.base_line = " << m_font->base_line << ",             /* Baseline measured from the bottom of the line */\n";
	stream << "\t.subpx = LV_FONT_SUBPX_NONE,\n";
	stream << "\t.dsc = &" << output_name << "_font_dsc           /* The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */\n";
	stream << "};\n\n";

	stream << "#endif /*#if " << defName << "*/\n";

	file.close();
	return true;
}

bool LVGLFont::isCustomFont() const
{
	return m_customFont;
}

QString LVGLFont::fileName() const
{
	return m_fileName;
}

QJsonObject LVGLFont::toJson() const
{
	QJsonObject object({{"name", m_name},
							  {"code", m_codeName},
							  {"size", m_font->line_height}
							 });
	if (!m_fileName.isEmpty())
		object.insert("fileName", m_fileName);
	return  object;
}
