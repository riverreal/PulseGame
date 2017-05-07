#pragma once

#include<Windows.h>
#include <Xinput.h>

#ifndef XUSER_MAX_COUNT
#define XUSER_MAX_COUNT 4
#endif // XUSER_MAX_COUNT

#pragma comment(lib, "xinput.lib")

namespace Elixir
{
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		struct XEntry
		{
			bool connected;
			XINPUT_STATE xState;
		};

		const XEntry* GetEntry(int index) const;
		void Update();

	private:
		

	private:
		XEntry m_entry[XUSER_MAX_COUNT];

	};
}
