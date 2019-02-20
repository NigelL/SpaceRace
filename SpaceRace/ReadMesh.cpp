#include "ReadMesh.h"



//FormatStart must be for instace "ROT(" followed by x ,y ,z)  WORKS ONLY FOR INT
template <class T>
std::vector<T> CheckFormat(std::string originalStr,std::string matchStr, std::string formatStart,int offset) {
	std::string checkStr;

	std::vector<T> allP;

	if (matchStr == formatStart) {
		matchStr = originalStr;
		for (int i = 0; i < originalStr.size() - offset; i++) {
			checkStr += matchStr[offset + i];
		}
		std::stringstream matchStrStream(checkStr);

		T c;
		offset = 0;
		while (matchStrStream >> c) {
		
			allP.push_back(c);
		
			offset++;
			
			if (matchStrStream.peek() == ',') {
				matchStrStream.ignore();
			}
			else if (matchStrStream.peek() == ')') {
				
				break;
			}
		}
	
	}

	return allP;
}


std::vector<MeshInfo> ReadFromMesh(std::string path) {
	std::vector<MeshInfo> retVecInfo;
	MeshInfo retInfo;

	std::fstream curFile(path);
	int loopIndex = 0;
	if (curFile.is_open()) {
		while (!curFile.eof()) {
			std::string curRead,matchStr,checkStr;
			curFile >> curRead;

			int index = 0;
			for (auto cur : curRead) {
				matchStr += cur;
				index++;
				std::string curCompareStr = (CheckFormat<std::string>(curRead, matchStr, "NAME(", index)).size() == 0 ? retInfo.meshName : CheckFormat<std::string>(curRead, matchStr, "NAME(", index)[0];
				retInfo.pos = (CheckFormat<double>(curRead, matchStr, "POS(", index)).size() == 0 ? retInfo.pos : CheckFormat<double>(curRead, matchStr, "POS(", index);
				retInfo.rot = (CheckFormat<double>(curRead, matchStr, "ROT(", index)).size() == 0 ? retInfo.rot : CheckFormat<double>(curRead, matchStr, "ROT(", index);
				retInfo.scale = (CheckFormat<double>(curRead, matchStr, "SCA(", index)).size() == 0 ? retInfo.scale : CheckFormat<double>(curRead, matchStr, "SCA(", index);
				retInfo.amt = (CheckFormat<double>(curRead, matchStr, "AMT(", index)).size() == 0 ? retInfo.amt : CheckFormat<double>(curRead, matchStr, "AMT(", index);
				retInfo.bound = (CheckFormat<double>(curRead, matchStr, "BOUND(", index)).size() == 0 ? retInfo.bound : CheckFormat<double>(curRead, matchStr, "BOUND(", index);
				retInfo.type = (CheckFormat<int>(curRead, matchStr, "TYPE(", index)).size() == 0 ? retInfo.type : CheckFormat<int>(curRead, matchStr, "TYPE(", index);


				retInfo.ambient = (CheckFormat<double>(curRead, matchStr, "AMBIENT(", index)).size() == 0 ? retInfo.ambient : CheckFormat<double>(curRead, matchStr, "AMBIENT(", index);
				retInfo.diffuse = (CheckFormat<double>(curRead, matchStr, "DIFFUSE(", index)).size() == 0 ? retInfo.diffuse : CheckFormat<double>(curRead, matchStr, "DIFFUSE(", index);
				retInfo.specular = (CheckFormat<double>(curRead, matchStr, "SPECULAR(", index)).size() == 0 ? retInfo.specular : CheckFormat<double>(curRead, matchStr, "SPECULAR(", index);
				retInfo.shininess = (CheckFormat<double>(curRead, matchStr, "SHININESS(", index)).size() == 0 ? retInfo.shininess : CheckFormat<double>(curRead, matchStr, "SHININESS(", index);
				std::string actualString;
				for (auto chr : curCompareStr) {
					if (chr != ')') {
						actualString += chr;
					}
				
				}
				retInfo.meshName = actualString;
			}
			loopIndex++;

			if (loopIndex > 10) {
				retVecInfo.push_back(retInfo);
				loopIndex = 0;
			}

		}
	}

	return retVecInfo;
	

}
