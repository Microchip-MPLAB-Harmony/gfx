import com.microchip.gfx.composer.asset.BINWriter as BINWriter
import com.microchip.gfx.composer.asset.HEXWriter as HEXWriter

class AssetBatch:
	def __init__(self, name, type, fileName):
		self.name = name
		self.type = type
		self.fileName = fileName

		self.assets = []

	def generateFile(self):
		if self.type == "HEX":
			writer = HEXWriter(_OUTDIR + "/" + self.fileName, self.offset)
			writer.write(self.assets)

		elif self.type == "BIN":
			writer = BINWriter(_OUTDIR + "/" + self.fileName)
			writer.write(self.assets)

internalFlashBatch = AssetBatch("Internal Flash", "INTERNAL", None)
internalSRAMBatch = AssetBatch("Internal SRAM", "INTERNAL", None)

batches = {}

def generateAssetBatches():
	locations = AssetManager.getMemoryLocationList()

	for loc in locations:
		if loc.getFormat().toString() == "HEX":
			batchName = loc.getName()
			batchFileName = loc.getPath()
			hexOffset = loc.getOffset()

			batch = AssetBatch(batchName, "HEX", batchFileName)
			batch.offset = hexOffset

			batches[batch.name] = batch


		elif loc.getFormat().toString() == "BIN":
			batchName = loc.getName()
			batchFileName = loc.getPath()

			batch = AssetBatch(batchName, "BIN", batchFileName)

			batches[batch.name] = batch

		elif loc.getFormat().toString() == "NONE":
			batchName = loc.getName()

			batch = AssetBatch(batchName, "NONE", None)

			batches[batch.name] = batch

	# add images to batches
	imageList = ImageManager.getImageList()

	for img in imageList:
		idx = img.getMemoryLocationIndex()

		if idx == 0:
			internalFlashBatch.assets.append(img)

		elif idx == 1:
			internalSRAMBatch.assets.append(img)

		else:
			name = img.getMemoryLocation()

			if name in batches:
				batch = batches[name]

				batch.assets.append(img)

	# add fonts to batches
	fontList = FontManager.getFontList()

	for fnt in fontList:
		idx = fnt.getMemoryLocationIndex()

		if idx == 0:
			internalFlashBatch.assets.append(fnt)

		elif idx == 1:
			internalSRAMBatch.assets.append(fnt)

		else:
			name = fnt.getMemoryLocation()

			if name in batches:
				batch = batches[name]

				batch.assets.append(fnt)

	for batchName in batches:
		batches[batchName].generateFile()
