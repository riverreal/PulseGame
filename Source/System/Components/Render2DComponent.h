#pragma once

#include "../../Interface/IComponent.h"
#include "../../Helper/TypeHelper.h"
#include <DirectXMath.h>

namespace Elixir
{
	//Renderer does not have a system since it works internally with the engine.
	class Renderer2D : public IComponent
	{
	public:
		virtual void Initialize() override;

	public:
		bool Enabled;
		//Stores the texture ID
		U32 Texture;
		//The opacity of the texture at the range of [0.0f, 1.0f]
		Color Color;
	};


}
