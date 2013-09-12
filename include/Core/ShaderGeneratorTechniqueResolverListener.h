
#ifndef __ShaderGeneratorTechniqueResolverListener__
#define __ShaderGeneratorTechniqueResolverListener__
#include "OgreStringVector.h"
#include "OgreRTShaderSystem.h"
#include "OgreShaderGenerator.h"
#define _RTSS_WRITE_SHADERS_TO_DISK 1
class ShaderGeneratorTechniqueResolverListener : public Ogre::MaterialManager::Listener
{
public:

	ShaderGeneratorTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* pShaderGenerator);
	
	virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, 
		const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, 
		const Ogre::Renderable* rend);
	protected:	
	Ogre::RTShader::ShaderGenerator*	mShaderGenerator;			// The shader generator instance.		
};

#endif
