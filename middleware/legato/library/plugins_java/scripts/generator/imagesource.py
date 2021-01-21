import com.microchip.gfx.composer.Designer.ColorMode as ColorMode

def getMemoryLocationName(idx):
	if idx == 0 or idx == 1:
		return "LE_STREAM_LOCATION_ID_INTERNAL"
		
	return "LE_ASSET_LOCATION_ID_%s" % AssetManager.getMemoryLocationName(idx - 1)

#def hex2(n):
	#print(n)
	#return hex(n & 0xFF).upper()[:-1]
	
def generateImageSourceFile(image):
	global paletteDict
	global imagePaletteMap
	
	imgSrc = File("generated/image/le_gen_image_" + image.getName() + ".c")
	
	sz = image.getOutputSize()
	
	imgSrc.write('#include "gfx/legato/generated/le_gen_assets.h"')
	imgSrc.writeNewLine()
	imgSrc.write("/*********************************")
	imgSrc.write(" * Legato Image Asset")
	imgSrc.write(" * Name:   %s" % (image.getName()))
	imgSrc.write(" * Size:   %dx%d pixels" % (sz.getWidth(), sz.getHeight()))
	imgSrc.write(" * Mode:   %s" % (image.getOutputColorMode()))
	imgSrc.write(" * Format: %s" % (image.getOutputFormat()))
	imgSrc.write(" ***********************************/")
	imgSrc.writeNewLine()
	
	locIdx = image.getMemoryLocationIndex()

	data = image.toDataArray()
	dataLen = len(data)

	if locIdx < 2:
		itr = 0
		
		if locIdx == 0: # internal flash = const
			imgSrc.writeNoNewline("const ")
			
		imgSrc.write("uint8_t %s_data[%d] =" % (image.getName(), dataLen))
		imgSrc.write("{")
		
		writeBinaryData(imgSrc, data, dataLen)
		
		imgSrc.write("};")
		
		imgSrc.writeNewLine()
		
	# generate palette if necessary
	palette = None
	
	if image.getName() in imagePaletteMap:
		palette = imagePaletteMap[image.getName()]
		
		if palette.generated == False:
			imgSrc.write("/*********************************")
			imgSrc.write(" * Legato Palette Asset")
			imgSrc.write(" * Mode:   %s" % (palette.object.getColorMode()))
			imgSrc.write(" * Size:   %s" % (palette.object.getColorCount()))
			imgSrc.write(" ***********************************/")
			imgSrc.writeNewLine()
			
			paletteData = palette.object.toDataArray()
			paletteDataLength = len(paletteData)

			if locIdx < 2:
				if locIdx == 0:
					imgSrc.writeNoNewline("const ")

				imgSrc.write("uint8_t %s_data[%d] =" % (palette.name, paletteDataLength))
				imgSrc.write("{")
			
				writeBinaryData(imgSrc, paletteData, paletteDataLength)
			
				imgSrc.write("};")
				imgSrc.writeNewLine()
			
			imgSrc.write("lePalette %s =" % (palette.name))
			imgSrc.write("{")
			imgSrc.write("    {")

			if locIdx < 2:
				imgSrc.write("        LE_STREAM_LOCATION_ID_INTERNAL, // data location id")
				imgSrc.write("        (void*)%s_data, // data address pointer" % (palette.name))
			else:
				imgSrc.write("        %d, // data location id" % (locIdx - 1))
				imgSrc.write("        (void*)%d, // external address" % (image.getPaletteAddress()))

			imgSrc.write("        %d, // data size" % (paletteDataLength))
			imgSrc.write("    },")
			imgSrc.write("    %d, // color count" % (palette.object.getColorCount()))
			imgSrc.write("    LE_COLOR_MODE_%s, // color mode" % (palette.object.getColorMode()))
			imgSrc.write("};")
			imgSrc.writeNewLine()
			
			palette.generated = True
	
	flags = ""
		
	if image.getMaskEnabled() == True:
		if len(flags) != 0:
			flags += " | "

		flags += "LE_IMAGE_USE_MASK_COLOR"

	if flags == "":
		flags = "0"
		
	mode = image.getOutputColorMode()
	maskColor = image.getMaskColor().toInteger(mode)

	if palette != None:
		maskColor = str(palette.object.indexOf(maskColor))
	else:
		maskColor = "0x" + image.getMaskColor().toHexString(mode, False)

	outputFormat = str(image.getOutputFormat()).upper()
	
	memLocName = ""
	
	imgSrc.write("leImage %s =" % (image.getName()))
	imgSrc.write("{")
	imgSrc.write("    {")

	if locIdx < 2:
		imgSrc.write("        LE_STREAM_LOCATION_ID_INTERNAL, // data location id")
		imgSrc.write("        (void*)%s_data, // data address pointer" % (image.getName()))
	else:
		imgSrc.write("        %d, // data location id" % (locIdx - 1))
		imgSrc.write("        (void*)%d, // external data address" % (image.getAddress()))

	imgSrc.write("        %d, // data size" % (dataLen))
	imgSrc.write("    },")
	imgSrc.write("    LE_IMAGE_FORMAT_%s," % (outputFormat))
	imgSrc.write("    {")
	
	if palette != None:
		imgSrc.write("        LE_COLOR_MODE_%s," % (palette.object.getIndexType()))
	else:
		imgSrc.write("        LE_COLOR_MODE_%s," % (image.getOutputColorMode()))
	
	imgSrc.write("        {")
	imgSrc.write("            %d," % (sz.getWidth()))
	imgSrc.write("            %d" % (sz.getHeight()))
	imgSrc.write("        },")
	imgSrc.write("        %d," % (sz.getWidth() * sz.getHeight()))
	imgSrc.write("        %d," % (image.getSizeBytes()))

	if locIdx < 2:
		imgSrc.write("        (void*)%s_data" % (image.getName()))
	else:
		imgSrc.write("        (void*)%d, // external data address" % (image.getAddress()))

	imgSrc.write("    },")
	imgSrc.write("    %s, // image flags" % (flags))

	imgSrc.write("    {")

	if maskColor == "-1":
		imgSrc.write("        0, // color mask")
	else:
		imgSrc.write("        %s, // color mask" % (maskColor))

	imgSrc.write("    },")

	imgSrc.write("    NULL, // alpha mask")
	
	if palette is not None:
		imgSrc.write("    &%s, // palette" % (palette.name))
	else:
		imgSrc.write("    NULL, // palette")
		
	imgSrc.write("};")
	
	imgSrc.close()
	
	global fileDict
	fileDict[imgSrc.name] = imgSrc
	