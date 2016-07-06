/*
* Voxl Game Engine
* Copyright 2016 Sam H.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#ifndef VOXL_LOG_H_
#define VOXL_LOG_H_

#include <stdlib.h>
#include <iostream>

namespace voxl {
	void LogError(const char* message) {
		std::cout << "Voxl < FATAL ERROR < " << message;
		exit(-1);
	}

	void LogWarning(const char* message) {
		std::cout << "Voxl < Warning < " << message;
	}
}

#endif