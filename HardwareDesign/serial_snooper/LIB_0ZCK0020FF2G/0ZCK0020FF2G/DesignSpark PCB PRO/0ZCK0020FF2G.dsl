SamacSys ECAD Model
822012/1120083/2.50/2/3/Fuse

DESIGNSPARK_INTERMEDIATE_ASCII

(asciiHeader
	(fileUnits MM)
)
(library Library_1
	(padStyleDef "r152_100"
		(holeDiam 0)
		(padShape (layerNumRef 1) (padShapeType Rect)  (shapeWidth 1) (shapeHeight 1.52))
		(padShape (layerNumRef 16) (padShapeType Ellipse)  (shapeWidth 0) (shapeHeight 0))
	)
	(textStyleDef "Default"
		(font
			(fontType Stroke)
			(fontFace "Helvetica")
			(fontHeight 50 mils)
			(strokeWidth 5 mils)
		)
	)
	(patternDef "FUSC2214X100N" (originalName "FUSC2214X100N")
		(multiLayer
			(pad (padNum 1) (padStyleRef r152_100) (pt -1.01, 0) (rotation 0))
			(pad (padNum 2) (padStyleRef r152_100) (pt 1.01, 0) (rotation 0))
		)
		(layerContents (layerNumRef 18)
			(attr "RefDes" "RefDes" (pt 0, 0) (textStyleRef "Default") (isVisible True))
		)
		(layerContents (layerNumRef 30)
			(line (pt -1.76 1.01) (pt 1.76 1.01) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 1.76 1.01) (pt 1.76 -1.01) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt 1.76 -1.01) (pt -1.76 -1.01) (width 0.05))
		)
		(layerContents (layerNumRef 30)
			(line (pt -1.76 -1.01) (pt -1.76 1.01) (width 0.05))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.075 0.675) (pt 1.075 0.675) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.075 0.675) (pt 1.075 -0.675) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt 1.075 -0.675) (pt -1.075 -0.675) (width 0.1))
		)
		(layerContents (layerNumRef 28)
			(line (pt -1.075 -0.675) (pt -1.075 0.675) (width 0.1))
		)
		(layerContents (layerNumRef 18)
			(line (pt 0 0.575) (pt 0 -0.575) (width 0.2))
		)
	)
	(symbolDef "0ZCK0020FF2G" (originalName "0ZCK0020FF2G")

		(pin (pinNum 1) (pt 0 mils 0 mils) (rotation 0) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 0 mils -35 mils) (rotation 0]) (justify "UpperLeft") (textStyleRef "Default"))
		))
		(pin (pinNum 2) (pt 700 mils 0 mils) (rotation 180) (pinLength 200 mils) (pinDisplay (dispPinName false)) (pinName (text (pt 700 mils -35 mils) (rotation 0]) (justify "UpperRight") (textStyleRef "Default"))
		))
		(line (pt 200 mils 50 mils) (pt 500 mils 50 mils) (width 6 mils))
		(line (pt 500 mils 50 mils) (pt 500 mils -50 mils) (width 6 mils))
		(line (pt 500 mils -50 mils) (pt 200 mils -50 mils) (width 6 mils))
		(line (pt 200 mils -50 mils) (pt 200 mils 50 mils) (width 6 mils))
		(line (pt 200 mils 0 mils) (pt 500 mils 0 mils) (width 6 mils))
		(attr "RefDes" "RefDes" (pt 550 mils 250 mils) (justify Left) (isVisible True) (textStyleRef "Default"))

	)
	(compDef "0ZCK0020FF2G" (originalName "0ZCK0020FF2G") (compHeader (numPins 2) (numParts 1) (refDesPrefix F)
		)
		(compPin "1" (pinName "1") (partNum 1) (symPinNum 1) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(compPin "2" (pinName "2") (partNum 1) (symPinNum 2) (gateEq 0) (pinEq 0) (pinType Bidirectional))
		(attachedSymbol (partNum 1) (altType Normal) (symbolName "0ZCK0020FF2G"))
		(attachedPattern (patternNum 1) (patternName "FUSC2214X100N")
			(numPads 2)
			(padPinMap
				(padNum 1) (compPinRef "1")
				(padNum 2) (compPinRef "2")
			)
		)
		(attr "Mouser Part Number" "530-0ZCK0020FF2G")
		(attr "Mouser Price/Stock" "https://www.mouser.co.uk/ProductDetail/Bel-Fuse/0ZCK0020FF2G?qs=SRYZG9HaIQ1YaXTwF3oUbw%3D%3D")
		(attr "Manufacturer_Name" "BelFuse")
		(attr "Manufacturer_Part_Number" "0ZCK0020FF2G")
		(attr "Description" "BEL FUSE - 0ZCK0020FF2G - FUSE, RESETTABLE PTC, 9VDC, 0.2A, SMD")
		(attr "Datasheet Link" "https://datasheet.datasheetarchive.com/originals/dk/DKDS-24/478080.pdf")
		(attr "Height" "1 mm")
	)

)
