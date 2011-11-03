mkdir ../../bin/Release_Linux/Resources
mkdir ../../bin/Release_Linux/Resources/Fonts
mkdir ../../bin/Release_Linux/Resources/Models
mkdir ../../bin/Release_Linux/Resources/Icons
mkdir ../../bin/Release_Linux/Resources/Icons/32x32

cp -r Icons/32x32 ../../bin/Release_Linux/Resources/Icons

cp -r Fonts/* ../../bin/Release_Linux/Resources/Fonts
cp -r Other/* ../../bin/Release_Linux/Resources/Models

cp Blender/Axis.material ../../bin/Release_Linux/Resources/Models
cp Blender/Axis.mesh ../../bin/Release_Linux/Resources/Models

cp Blender/Bones/BoneMesh.material ../../bin/Release_Linux/Resources/Models

cp Blender/Bones/Bones.png ../../bin/Release_Linux/Resources/Models

cp Blender/Bones/BoneTip.mesh ../../bin/Release_Linux/Resources/Models

cp Blender/Bones/BoneGlobe.mesh ../../bin/Release_Linux/Resources/Models

mkdir ../../bin/Release_Linux/Plugins
cp ../../bin/Release/Readme.txt ../../bin/Release_Linux/
cp ../../bin/Release/ChangeLog.txt ../../bin/Release_Linux/