#include "MapChipField.h"
#include <map>

using namespace KamataEngine;

namespace {
   std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
   };
}


	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data){
				mapChipDataLine.resize(kNumBlockHorizontal);
