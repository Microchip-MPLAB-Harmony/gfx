def generateStringTableFile():
	global stringTableMatrix

	if stringTableMatrix.stringIDList.size() == 0:
		return

	file = File("generated/le_gen_stringtable.c")
	
	file.write('#include "gfx/legato/generated/le_gen_assets.h"')
	file.writeNewLine()
	
	file.write("/*****************************************************************************")
	file.write(" * Legato String Table")
	file.write(" * Encoding        %s" % (StringTable.getEncoding()))
	file.write(" * Language Count: %d" % (stringTableMatrix.languageIDList.size()))
	file.write(" * String Count:   %d" % (stringTableMatrix.stringIDList.size()))
	file.write("*****************************************************************************/")
	file.writeNewLine()
	
	file.write("/*****************************************************************************")
	file.write(" * string table data")
	file.write(" * ")
	file.write(" * this table contains the raw character data for each string")
	file.write(" * ")
	file.write(" * unsigned short - number of indices in the table")
	file.write(" * unsigned byte - size of each index")
	file.write(" * unsigned byte - number of languages in the table")
	file.write(" * ")
	file.write(" * index array (size = number of indices * number of languages")
	file.write(" * ")
	file.write(" * for each index in the array:")
	file.write(" *   unsigned byte - the font ID for the index")
	file.write(" *   unsigned (index size) - a value of length 'index size' that contains")
	file.write(" *                           the offset of the string codepoint data in")
	file.write(" *                           the table")
	file.write(" * ")
	file.write(" * string data is found by jumping to the index offset from the start")
	file.write(" * of the table")
	file.write(" * ")
	file.write(" * string data entry:")
	file.write(" * unsigned short : length of the string in bytes:")
	file.write(" * 'length' number of codepoints - the string data (encoding dependent")
	file.write(" ****************************************************************************/")
	file.writeNewLine()

	stringTableData = stringTableMatrix.generateStringTableData()
	stringTableDataLength = len(stringTableData)
	
	file.write("const uint8_t stringTable_data[%d] =" % (stringTableDataLength))
	file.write("{")
	
	writeBinaryData(file, stringTableData, stringTableDataLength)
	
	file.write("};")
	file.writeNewLine()

	fontList = FontManager.getFontList()

	if stringTableMatrix.fontList > 0:
		file.write("/* font asset pointer list */")
		file.write("leFont* fontList[%d] =" % (fontList.size()))
		file.write("{")
		
		for font in fontList:
			fontData = font.generateFontData()

			if fontData.glyphs.size() == 0:
				continue

			file.write("    (leFont*)&%s," % (font.getName()))
		
		file.write("};")
		file.writeNewLine()
	
	file.write("const leStringTable stringTable =")
	file.write("{")
	file.write("    {")
	#file.write("        LE_ASSET_TYPE_STRINGTABLE, // asset type")
	file.write("        LE_STREAM_LOCATION_ID_INTERNAL, // data location id")
	file.write("        (void*)stringTable_data, // data address pointer")
	file.write("        %d, // data size" % (stringTableDataLength))
	file.write("    },")
	file.write("    (void*)stringTable_data, // string table data")

	if fontList.size() > 0:
		file.write("    fontList, // font lookup table")
	else:
		file.write("    NULL, // font lookup table")

	file.write("    LE_STRING_ENCODING_%s // encoding standard" % (StringTable.getEncoding()))
	file.write("};")
	file.close()
	
	global fileDict
	fileDict[file.name] = file
