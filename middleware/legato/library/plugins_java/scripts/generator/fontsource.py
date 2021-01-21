def generateFontSourceFile(font):
	name = font.getName()
	
	antialias = font.getAntialias()
	height = font.getAdjustedHeight()
	baseline = font.getBaseline()
	
	style = ""
	
	if antialias == True:
		style += "Antialias"
		
	if len(style) == 0:
		style = "Plain"
		
	if style.endswith(",") == True:
		style = style[:-1]
		
	fontData = font.generateFontData()

	if fontData.glyphs.size() == 0:
		return

	fntSrc = File("generated/font/le_gen_font_" + name + ".c")
	
	fntSrc.write('#include "gfx/legato/generated/le_gen_assets.h"')
	fntSrc.writeNewLine()
	fntSrc.write("/*********************************")
	fntSrc.write(" * Legato Font Asset")
	fntSrc.write(" * Name:         %s" % (name))
	fntSrc.write(" * Height:       %d" % (height))
	fntSrc.write(" * Baseline:     %d" % (baseline))
	fntSrc.write(" * Style:        %s" % (style))
	fntSrc.write(" * Glyph Count:  %d" % (fontData.glyphs.size()))
	fntSrc.write(" * Range Count:  %d" % (fontData.ranges.size()))
	fntSrc.writeNoNewline(" * Glyph Ranges: ")

	idx = 0
	
	for range in fontData.ranges:
		start = range.getStartOrdinal()
		end = range.getEndOrdinal()
		
		if idx == 0:
			if start != end:
				fntSrc.write("0x%02X-0x%02X" % (start, end))
			else:
				fntSrc.write("0x%02X" % (start))
		else:
			if start != end:
				fntSrc.write("                 0x%02X-0x%02X" % (start, end))
			else:
				fntSrc.write("                 0x%02X" % (start))
		
		idx += 1
		
	fntSrc.write(" *********************************/")
	
	locIdx = font.getMemoryLocationIndex()
	
	kerningData = fontData.getKerningDataArray()
	kerningDataLength = len(kerningData)
	
	fntSrc.write("/*********************************")
	fntSrc.write(" * font glyph kerning table description")
	fntSrc.write(" *")
	fntSrc.write(" * unsigned int - number of glyphs")
	fntSrc.write(" * for each glyph:")
	fntSrc.write(" *     unsigned short - codepoint         * the glyph's codepoint")
	fntSrc.write(" *     short          - width             * the glyph's width in pixels")
	fntSrc.write(" *     short          - height            * the glyph's height in pixels")
	fntSrc.write(" *     short          - advance           * the glyph's advance value in pixels")
	fntSrc.write(" *     short          - bearingX          * the glyph's bearing value in pixels on the X axis")
	fntSrc.write(" *     short          - bearingY          * the glyph's bearing value in pixels on the Y axis")
	fntSrc.write(" *     unsigned short - flags             * status flags for this glyph")
	fntSrc.write(" *     unsigned short - data row width    * the size of a row of glyph data in bytes")
	fntSrc.write(" *     unsigned int   - data table offset * the offset into the corresponding font data table")
	fntSrc.write(" ********************************/")

	fntSrc.write("const uint8_t %s_glyphs[%d] =" % (name, kerningDataLength))
	fntSrc.write("{")

	writeBinaryData(fntSrc, kerningData, kerningDataLength)

	fntSrc.write("};")

	fntSrc.writeNewLine()

	glyphData = fontData.getGlyphDataArray()
	glyphDataLength = len(glyphData)
	
	if locIdx < 2:
		fntSrc.write("/*********************************")
		fntSrc.write(" * raw font glyph data")
		fntSrc.write(" ********************************/")

		if locIdx == 0: # internal flash = const
			fntSrc.writeNoNewline("const ")

		fntSrc.write("uint8_t %s_data[%d] =" % (name, glyphDataLength))
		fntSrc.write("{")
		
		writeBinaryData(fntSrc, glyphData, glyphDataLength)
		
		fntSrc.write("};")
		
		fntSrc.writeNewLine()
	
	antialias = font.getAntialias()
	bpp = 1
	
	if antialias == True:
		bpp = 8
		
	memLocName = ""
	
	if locIdx < 2:
		memLocName = "LE_STREAM_LOCATION_ID_INTERNAL"
	else:
		memLocName = font.getMemoryLocationName()
	
	fntSrc.write("leRasterFont %s =" % (name))
	fntSrc.write("{")
	fntSrc.write("    {")
	fntSrc.write("        {")

	if locIdx < 2:
		fntSrc.write("            %s, // data location id" % (memLocName))
		fntSrc.write("            (void*)%s_data, // data address pointer" % (name))
	else:
		fntSrc.write("            %d, // data location id" % (locIdx - 1))
		fntSrc.write("            (void*)%d, // external data address" % (font.getAddress()))

	fntSrc.write("            %d, // data size" % (glyphDataLength))
	fntSrc.write("        },")
	fntSrc.write("        LE_RASTER_FONT,")
	fntSrc.write("    },")
	fntSrc.write("    %d," % (fontData.getMaxHeight()))
	fntSrc.write("    %d," % (fontData.getMaxBaseline()))
	fntSrc.write("    LE_FONT_BPP_%d, // bits per pixel" % (bpp))
	fntSrc.write("    %s_glyphs, // glyph table" % (name))
	fntSrc.write("};")
	
	fntSrc.close()
	
	global fileDict
	fileDict[fntSrc.name] = fntSrc