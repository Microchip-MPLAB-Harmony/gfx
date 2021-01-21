import com.microchip.gfx.composer.asset.image.ImageManager as ImageManager
import com.microchip.gfx.composer.asset.font.FontManager as FontManager

execfile(_SCRIPTPATH + "imagesource.py")
execfile(_SCRIPTPATH + "fontsource.py")
execfile(_SCRIPTPATH + "stringtable.py")


def generateAssetSources():
	imageList = ImageManager.getImageList()

	for image in imageList:

		locName = image.getMemoryLocation()

		generateImageSourceFile(image)
		
	fontList = FontManager.getFontList()
	
	for font in fontList:
		generateFontSourceFile(font)
		
	generateStringTableFile()
	