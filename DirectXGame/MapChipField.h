#pragma once
#include "KamataEngine.h"
#include <vector>

/// <summary>
/// MapChipField.h
///	</summary>

enum class MapChipType {
	kBlank,
	kBlock,
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	MapChipData mapChipData_;

	// 1Block Size
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	//Block Quantity

	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	void RestMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

};