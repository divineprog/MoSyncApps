/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include <MAUtil/Moblet.h>
#include <conprint.h>
#include "Lib/Moblet.h"

using namespace MAUtil;

/**
 * A Moblet is a high-level class that defines the
 * behaviour of a MoSync program.
 */
class MyMoblet : public Custom::Moblet
{
public:
	MyMoblet()
	{
		printf(
			"Demo of custom Moblet class where\n"
			"you can enter and exit the event loop.\n"
			"Touch screen twice or press back twice\n"
			"to exit the application.\n");
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			exitEventLoop();
		}
	}

	void pointerReleaseEvent(MAPoint2d point)
	{
		exitEventLoop();
	}
};

extern "C" int MAMain()
{
	Custom::Moblet* moblet = new MyMoblet();
	printf("Entering event loop.\n");
	moblet->enterEventLoop();
	printf("Event loop exited.\n");
	printf("Entering event loop again.\n");
	moblet->enterEventLoop();
	printf("Exiting app.\n");
	return 0;
}
