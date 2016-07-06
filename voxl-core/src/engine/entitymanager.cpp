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

#include "voxl.hpp"

#include <iostream>

#include "engine/entitymanager.hpp"

namespace voxl {
	namespace engine {
		Entity* EntityManager::CreateEntity() {
			Entity* entity = new Entity(nextId);
			entities.push_back(entity);
			nextId += 1;
			return entity;
		}

		Entity* EntityManager::GetEntityFromId(u64 id) {
			for (u64 i = 0; i < entities.size(); i++) {
				if (entities[i]->GetId() == id) {
					return entities[i];
				}
			}
			std::cout << "Unable to find requested entity, pointer will be invalid" << std::endl;
			return nullptr;
		}

		void EntityManager::DeleteEntity(Entity* entity) {
			for (u64 i = 0; i < entities.size(); i++) {
				if (entities[i]->GetId() == entity->GetId()) {
					delete entity;
					entities.erase(entities.begin() + i);
					return;
				}
			}
			std::cout << "Unable to delete requested entity, does not exist in entity manager" << std::endl;
		}
	}
}
