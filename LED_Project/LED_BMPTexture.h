#pragma once

#include <wx/wx.h>
#include <GL/glew.h>
#include "LED_Texture.h"

class CLED_BMPTexture : public CLED_Texture
{
public:
	CLED_BMPTexture(const wxString& filename);
	virtual ~CLED_BMPTexture();
};