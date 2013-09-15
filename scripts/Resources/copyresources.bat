mkdir ..\..\bin\Release\Resources\
mkdir ..\..\bin\Release\Resources\Fonts
mkdir ..\..\bin\Release\Resources\Models
mkdir ..\..\bin\Release\Resources\Icons
mkdir ..\..\bin\Release\Resources\Icons\32x32
mkdir ..\..\bin\Release\Resources\RTShaderLib

mkdir ..\..\bin\Debug\Resources\
mkdir ..\..\bin\Debug\Resources\Fonts
mkdir ..\..\bin\Debug\Resources\Models
mkdir ..\..\bin\Debug\Resources\Icons
mkdir ..\..\bin\Debug\Resources\Icons\32x32
mkdir ..\..\bin\Debug\Resources\RTShaderLib

copy Icons\32x32 ..\..\bin\Release\Resources\Icons\32x32
copy Icons\32x32 ..\..\bin\Debug\Resources\Icons\32x32

copy Other ..\..\bin\Debug\Resources\Models
copy Other ..\..\bin\Release\Resources\Models

copy Fonts ..\..\bin\Debug\Resources\Fonts
copy Fonts ..\..\bin\Release\Resources\Fonts

xcopy RTShaderLib ..\..\bin\Debug\Resources\RTShaderLib /E
xcopy RTShaderLib ..\..\bin\Release\Resources\RTShaderLib /E

copy Blender\Axis.material ..\..\bin\Debug\Resources\Models
copy Blender\Axis.material ..\..\bin\Release\Resources\Models
copy Blender\Axis.mesh ..\..\bin\Debug\Resources\Models
copy Blender\Axis.mesh ..\..\bin\Release\Resources\Models

copy Blender\Bones\BoneMesh.material ..\..\bin\Debug\Resources\Models
copy Blender\Bones\BoneMesh.material ..\..\bin\Release\Resources\Models

copy Blender\Bones\Bones.png ..\..\bin\Debug\Resources\Models
copy Blender\Bones\Bones.png ..\..\bin\Release\Resources\Models

copy Blender\Bones\BoneTip.mesh ..\..\bin\Debug\Resources\Models
copy Blender\Bones\BoneTip.mesh ..\..\bin\Release\Resources\Models

copy Blender\Bones\BoneGlobe.mesh ..\..\bin\Debug\Resources\Models
copy Blender\Bones\BoneGlobe.mesh ..\..\bin\Release\Resources\Models