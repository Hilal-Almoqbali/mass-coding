int maxCombinedTextureUnits, maxVertexTextureUnits, maxTextureUnits;
glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits);
glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureUnits);
glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
std::cout << "Maximum combined texture units: " << maxCombinedTextureUnits << std::endl;
std::cout << "Maximum vertex texture units: " << maxVertexTextureUnits << std::endl;
std::cout << "Maximum texture units: " << maxTextureUnits << std::endl;