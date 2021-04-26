#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "BrushManager.h"
#include "ObjectManager.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;
using value_t = nlohmann::detail::value_t;

namespace IOHandler {
	void saveJson_brush(const char* filename, BrushManager& brushManager);
	void loadJson_brush(const char* filename, BrushManager& brushManager);

	void saveJson_terrainVerticesColor(const char* filename, ObjectManager& objectManager);
	void loadJson_terrainVerticesColor(const char* filename, ObjectManager& objectManager, BrushManager& brushManager);

	template<typename T>
	void saveJson_attribute(json& file, const char* filename, const char* attributeName, T attribute) {
		globals::g_hasGlobalsSave = true;

		file[attributeName] = attribute;

		std::ofstream t_o(filename);
		t_o << file << std::endl;
	}
	template<typename T>
	void loadJson_attribute(const char* filename, const char* attributeName, T& attribute) {
		json t_json;
		std::ifstream t_i(filename);
		if (t_i.peek() == std::ifstream::traits_type::eof()) {
			return;
		}

		t_i >> t_json;
		if (t_json.empty() || t_json == "null") {
			return;
		}

		attribute = t_json.find(attributeName).value();
	}
}