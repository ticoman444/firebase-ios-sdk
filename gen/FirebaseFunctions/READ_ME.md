# NOTE

This folder is automatically generated as part of the pre-commit git hook. The purpose of the folder is to have a known defined xcodeproj for XCode Server. To get the XCode Server CI to run, add the following pre-integration script to the XCode Server Bot:

```
# Update the generated project in the directory that XCode Server pulls down the source to, ex:
# /Users/xcodeserver/Library/Caches/XCSBuilder/Bots/d7120e91e980b0ee588a6968510d059f/Source/

cd firebase-ios-sdk
"/usr/local/bin/pod" gen "FirebaseFunctions.podspec" --deterministic-uuids
```