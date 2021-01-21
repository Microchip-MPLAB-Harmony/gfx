# coding: utf-8
##############################################################################
# Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
#
# Subject to your compliance with these terms, you may use Microchip software
# and any derivatives exclusively with Microchip products. It is your
# responsibility to comply with third party license terms applicable to your
# use of third party software (including open source software) that may
# accompany Microchip software.
#
# THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
# EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
# WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
# PARTICULAR PURPOSE.
#
# IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
# INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
# WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
# BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
# FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
# ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
# THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
##############################################################################

bspSupportList = []
bspSupportObjList = None

class bspSupportObj:
	def __init__(self, pinConfig, componentActivateList, componentDeactivateList, componentAutoConnectList, eventCallbackFxn):
		self.pinConfig = pinConfig
		self.componentActivateList = componentActivateList
		self.componentDeactivateList = componentDeactivateList
		self.componentAutoConnectList = componentAutoConnectList
		self.eventCallbackFxn = eventCallbackFxn

	def getPinConfig(self):
		return self.pinConfig

	def getComponentActivateList(self):
		return self.componentActivateList

	def getComponentDeactivateList(self):
		return self.componentDeactivateList
	
	def getComponentAutoConnectList(self):
		return self.componentAutoConnectList
	
	def getEventCallbackFxn(self):
		return self.eventCallbackFxn

def addBSPSupport(bspID, configID, bspSupportObject):
	global bspSupportList
	global bspSupportObjList
	
	keyID = bspID
	if (configID != None):
		keyID += configID
	
	if (bspSupportObjList == None):
		bspSupportObjList = dict([(keyID, bspSupportObject)])
	else:
		bspSupportObjList[keyID] = bspSupportObject
	
	if bspID not in bspSupportList:
		bspSupportList += [bspID]
	
	print("added support for BSP " + bspID + " keyID " + keyID)

def getBSPSupportNode(bspID, configID):
	global bspSupportObjList
	
	keyID = bspID
	if (configID != None):
		keyID += configID

	return bspSupportObjList[keyID]

# Gets the ID of the first supported BSP component in the project
# If none is supported, returns "None"
def getSupportedBSP():
	global bspSupportList
	
	if (bspSupportList == None):
		return None

	activeComponentsList = Database.getActiveComponentIDs()

	print("bsp list " + str(bspSupportList))
	print("component list " + str(activeComponentsList))
	for keyID in bspSupportList:
		if keyID in activeComponentsList:
			return str(keyID)
	
	return None
	
def getSupportedBSPList():
	global bspSupportList
	
	return bspSupportList
