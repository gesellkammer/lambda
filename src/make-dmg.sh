#!/bin/bash
hdiutil create -volname Lambda -srcfolder Lambda.app -ov -format UDZO Lambda.dmg
rm -fr ../dist/Lambda.dmg
mv Lambda.dmg ../dist
