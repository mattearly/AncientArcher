/*
main
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
	copyright notice, this list of conditions and the
	following disclaimer.
* Redistributions in binary form must reproduce the above
	copyright notice, this list of conditions and the
	following disclaimer in the documentation and/or other
	materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
	contributors may be used to endorse or promote products
	derived from this software without specific prior
	written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#include <iostream>
#include <limits>
#include "../dry/coreTests.h"

int main()
{
	char choice = 'a';
	while (choice != 'q') {
		std::cout << "Run:\n";
		std::cout << "b: base loop\n";
		std::cout << "d: demo world\n";
		std::cout << "q: quit\n";
		std::cout << "Choice: ";
		std::cin >> choice;
		char eatbuf;
		//while (std::cin >> eatbuf);
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		switch (choice)
		{
		case 'b':
			if (!testEngineLoopBase())
			{
				std::cout << "failed testEngineLoopBase\n";
			}
			break;
		case 'd':
			if (!testEngineDemoWorld())
			{
				std::cout << "failed testEngineDemoWorld\n";
			}
			break;
		case 'q':
			std::cout << "thanks for playing\n";
			break;
		default:
			std::cout << "invalid choice\n";
		}
		if (choice != 'q') choice = 'a';
	}
	return 0;
}
