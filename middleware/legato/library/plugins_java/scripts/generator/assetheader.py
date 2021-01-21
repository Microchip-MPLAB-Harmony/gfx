import com.microchip.gfx.composer.asset.AssetManager as AssetManager
import com.microchip.gfx.composer.asset.MemoryLocation as MemoryLocation
import com.microchip.gfx.composer.asset.image.ImageAsset as ImageAsset
import com.microchip.gfx.composer.asset.image.ImageAsset as ImageAsset
import com.microchip.gfx.composer.asset.palette.PaletteAsset as Palette
import com.microchip.gfx.composer.asset.font.FontAsset as FontAsset
import com.microchip.gfx.composer.asset.font.FontManager as FontManager
import com.microchip.gfx.composer.asset.font.FontAsset.FontData as FontData
import com.microchip.gfx.composer.asset.font.FontRange as FontRange
import com.microchip.gfx.composer.asset.stringtable.StringTable as StringTable
import com.microchip.gfx.composer.asset.stringtable.StringLanguage as StringLanguage
import com.microchip.gfx.composer.asset.stringtable.StringTableEntry as StringTableEntry

paletteDict = {}
imagePaletteMap = {}

paletteCount = 0

class PaletteDef:
	def __init__(self, paletteObj):
		global paletteCount
		
		self.name = "leGenPalette%d" % paletteCount
		self.object = paletteObj
		self.checksum = paletteObj.getChecksum()
		self.colorCount = paletteObj.getColorCount()
		self.generated = False
		
		paletteCount += 1
		

def generateLocationIDList(astHdr):
	memoryLocList = AssetManager.getMemoryLocationList()
	
	# loc indices 0 and 1 are system reserved for internal flash and SRAM
	# loc 1 is discarded during generation since it's really just a const flag
	if len(memoryLocList) < 2:
		return
	
	astHdr.write("/*****************************************************************************")
	astHdr.write("* MPLAB Harmony Graphics Asset Location IDs")
	astHdr.write("*****************************************************************************/")
	
	idx = 0
	
	for memLoc in memoryLocList:
		if idx < 2:
			idx += 1
			continue;
	
		astHdr.write("/*********************************")
		astHdr.write(" * Legato Asset Memory Location")
		astHdr.write(" * Name:   %s" % (memLoc.getName()))
		astHdr.write(" * Size:   %d" % (memLoc.getMaxSize()))
		astHdr.write(" * Format: %s" % (memLoc.getFormat()))
		astHdr.write(" * Path:   %s" % (memLoc.getPath()))
		astHdr.write(" * Offset: %d" % (memLoc.getOffset()))
		astHdr.write(" ***********************************/")
		astHdr.write("#define LA_ASSET_LOCATION_ID_%s    %d" % (memLoc.getName(), idx))
		astHdr.writeNewLine()
		
		idx += 1
	
def generateImageIDList(astHdr):
	imageList = ImageManager.getImageList()
	
	if len(imageList) == 0:
		return
	
	astHdr.write("/*****************************************************************************")
	astHdr.write("* MPLAB Harmony Graphics Image Assets")
	astHdr.write("*****************************************************************************/")
	
	for image in imageList:
		if image.shouldGeneratePalette() == True:
			paletteObj = image.generatePalette()
		else:
			paletteObj = None

		palette = None
		
		if paletteObj is not None:
			palette = PaletteDef(paletteObj)
				
			imagePaletteMap[image.getName()] = palette

		sz = image.getOutputSize()
		
		astHdr.write("/*********************************")
		astHdr.write(" * Legato Image Asset")
		astHdr.write(" * Name:   %s" % (image.getName()))
		astHdr.write(" * Size:   %dx%d pixels" % (sz.getWidth(), sz.getHeight()))
		astHdr.write(" * Format: %s" % (image.getOutputFormat()))
		
		if palette != None:
			astHdr.write(" * Mode: %s" % (palette.object.getIndexType()))
		else:
			astHdr.write(" * Mode: %s" % (image.getOutputColorMode()))
		
		astHdr.write(" ***********************************/")
		astHdr.write("extern leImage %s;" % (image.getName()))
		astHdr.writeNewLine()

def generatePaletteIDList(astHdr):
	if len(paletteDict) == 0:
		return
		
	astHdr.write("/*****************************************************************************")
	astHdr.write("* MPLAB Harmony Graphics Palette Assets")
	astHdr.write("*****************************************************************************/")
	
	for checksum in paletteDict:
		palette = paletteDict[checksum]
		
		astHdr.write("/*********************************")
		astHdr.write(" * Legato Palette Asset")
		astHdr.write(" * Mode:   %s" % (palette.object.getColorMode()))
		astHdr.write(" * Size:   %s" % (palette.object.getColorCount()))
		astHdr.write(" ***********************************/")
		astHdr.write("extern lePalette %s;" % (palette.name))
		astHdr.writeNewLine()
			
def generateFontIDList(astHdr):
	fontList = FontManager.getFontList()
	
	if len(fontList) == 0:
		return
	
	astHdr.write("/*****************************************************************************")
	astHdr.write("* MPLAB Harmony Graphics Font Assets")
	astHdr.write("*****************************************************************************/")
	
	for font in fontList:
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
			continue
		
		astHdr.write("/*********************************")
		astHdr.write(" * Legato Font Asset")
		astHdr.write(" * Name:         %s" % (name))
		astHdr.write(" * Height:       %d" % (height))
		astHdr.write(" * Baseline:     %d" % (baseline))
		astHdr.write(" * Style:        %s" % (style))
		astHdr.write(" * Glyph Count:  %d" % (fontData.glyphs.size()))
		astHdr.write(" * Range Count:  %d" % (fontData.ranges.size()))
		astHdr.writeNoNewline(" * Glyph Ranges: ")

		idx = 0
		
		for range in fontData.ranges:
			start = range.getStartOrdinal()
			end = range.getEndOrdinal()
			
			if idx == 0:
				if start != end:
					astHdr.write("0x%02X-0x%02X" % (start, end))
				else:
					astHdr.write("0x%02X" % (start))
			else:
				if start != end:
					astHdr.write("                 0x%02X-0x%02X" % (start, end))
				else:
					astHdr.write("                 0x%02X" % (start))
			
			idx += 1
			
		astHdr.write("***********************************/")
		astHdr.write("extern leRasterFont %s;" % (name))
		astHdr.writeNewLine()
	
def generateStringIDList(astHdr):
	global stringTableMatrix

	if stringTableMatrix.stringIDList.size() == 0:
		return

	languageList = StringTable.getLanguageList()
	stringList = StringTable.getStringList()
	
	astHdr.write("/*****************************************************************************")
	astHdr.write(" * Legato String Table")
	astHdr.write(" * Encoding        %s" % (StringTable.getEncoding()))
	astHdr.write(" * Language Count: %d" % (stringTableMatrix.languageIDList.size()))
	astHdr.write(" * String Count:   %d" % (stringTableMatrix.stringIDList.size()))
	astHdr.write("*****************************************************************************/")
	astHdr.writeNewLine()

	astHdr.write("// language IDs")
	
	langIdx = 0
	
	for name in stringTableMatrix.languageIDList:
		astHdr.write("#define language_%s    %d" % (name, langIdx))
		
		langIdx += 1

	astHdr.writeNewLine()
	
	astHdr.write("// string IDs")
	
	strIdx = 0
	
	for name in stringTableMatrix.stringIDList:
		astHdr.write("#define string_%s    %d" % (name, strIdx))
		
		strIdx += 1
	
	astHdr.writeNewLine()
	
	astHdr.write("extern const leStringTable stringTable;")
	
def generateAssetHeader():
	astHdr = File("generated/le_gen_assets.h")
	
	astHdr.summary = "Header file containing a list of asset specifications for use with the\n    MPLAB Harmony Graphics Stack."
	astHdr.description = astHdr.summary
	
	astHdr.writeDescription()
	astHdr.writeNewLine()
	astHdr.write("// DOM-IGNORE-BEGIN")
	astHdr.write(copyrightText);
	astHdr.write("// DOM-IGNORE-END")
	astHdr.writeNewLine()
	astHdr.write("#ifndef LE_GEN_ASSETS_H")
	astHdr.write("#define LE_GEN_ASSETS_H")
	astHdr.writeNewLine()
	astHdr.write("// DOM-IGNORE-BEGIN")
	astHdr.write("#ifdef __cplusplus  // Provide C++ Compatibility")
	astHdr.write('extern "C" {')
	astHdr.write("#endif")
	astHdr.write("// DOM-IGNORE-END")
	astHdr.writeNewLine()
	astHdr.write('#include "gfx/legato/legato.h"')
	astHdr.writeNewLine()
	
	generateLocationIDList(astHdr)
	
	generateImageIDList(astHdr)
	
	generatePaletteIDList(astHdr)
	
	generateFontIDList(astHdr)
	
	generateStringIDList(astHdr)
	
	astHdr.write("//DOM-IGNORE-BEGIN")
	astHdr.write("#ifdef __cplusplus")
	astHdr.write("}")
	astHdr.write("#endif")
	astHdr.write("//DOM-IGNORE-END")
	astHdr.writeNewLine()
	astHdr.write("#endif /* LE_GEN_ASSETS_H */")
	
	astHdr.close()
