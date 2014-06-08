#!/bin/bash
# hdiutil create -volname Lambda -srcfolder Lambda.app -ov -format UDZO Lambda.dmg
mkdir build
cp -r Lambda.app build/Lambda.app
macdeployqt build/Lambda.app -no-plugins -dmg
rm -fr ../dist/Lambda.dmg
mv build/Lambda.dmg ../dist
rm -fr build
