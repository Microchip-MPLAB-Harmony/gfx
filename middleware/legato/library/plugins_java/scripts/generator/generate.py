import datetime
import os
import shutil

import com.microchip.gfx.composer.scene.SceneManager as SceneManager
import com.microchip.gfx.composer.state.StateManager as StateManager
import com.microchip.gfx.composer.asset.stringtable.StringTable as StringTable
import com.microchip.gfx.composer.asset.stringtable.StringTableMatrix as StringTableMatrix
import com.microchip.gfx.composer.asset.stringtable.StringTableMatrix.StringData as StringData

execfile(_SCRIPTPATH + "utils.py")

fileDict = {}

f = open(_SCRIPTPATH + "copyright.txt", "r")
copyrightText = f.read()
f.close()

now = datetime.datetime.now()
copyrightText = copyrightText.replace("${CURRENTYEAR}", str(now.year))

f = open(_SCRIPTPATH + "headerdesc.txt", "r")
headerDesc = f.read()
f.close()

class File:
	def __init__(self, name):
		self.name = name
		self.path = _OUTDIR + "/" + name
		self.handle = None
		self.summary = ""
		self.description = ""
		
		try:
			path = os.path.dirname(os.path.abspath(self.path))
			os.makedirs(path)
		except:
			i = 0
			
		self.handle = open(self.path, "w")
		
	def close(self):
		self.handle.close()
		
	def write(self, text):
		self.handle.write(text)
		self.handle.write("\n")
		
	def writeNoNewline(self, text):
		self.handle.write(text)
		
	def writeNewLine(self):
		self.handle.write("\n")
		
	def writeDescription(self):
		global headerDesc
		
		text = headerDesc.replace("${FILENAME}", self.name)
		text = text.replace("${FILESUMMARY}", self.summary)
		text = text.replace("${FILEDESC}", self.description)
		
		self.handle.write(text)
		self.handle.write("\n")

execfile(_SCRIPTPATH + "assetbatch.py")
execfile(_SCRIPTPATH + "assetheader.py")
execfile(_SCRIPTPATH + "assetsource.py")
execfile(_SCRIPTPATH + "screen.py")
execfile(_SCRIPTPATH + "widgetutils.py")
execfile(_SCRIPTPATH + "widget.py")
execfile(_SCRIPTPATH + "scheme.py")
execfile(_SCRIPTPATH + "init.py")
execfile(_SCRIPTPATH + "config.py")
execfile(_SCRIPTPATH + "action.py")
execfile(_SCRIPTPATH + "event.py")

if _HARMONY == True:
	execfile(_SCRIPTPATH + "harmony.py")

# do this here because it is used by both asset header and string table
stringTableMatrix = StringTable.generateData()

generateAssetBatches()
generateAssetHeader()
generateAssetSources()

screenList = SceneManager.getScreenList()

for scr in screenList:
	generateScreen(scr)
	
generateSchemeFile()
generateInitFile()
generateConfigFile()

if _HARMONY == True:
	generateHarmonyFile()
	
	shutil.make_archive(_OUTDIR, 'zip', _OUTDIR)
	
