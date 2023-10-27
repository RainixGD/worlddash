#pragma once
#include <vector>
#include <string>
#include <functional>
using namespace geode::prelude;

class ReqManager {
public:
	static std::string serverLink;
	static ReqManager* getInstance();
	static void cancelReqById(int id);
	int makeReq(std::string url, json::Value const& fields, std::function<void(std::string const&)> onResponse, std::function<void(std::string const&)> onError);
	ReqManager();
private:
	static std::vector<int> reqIds;
	static ReqManager* instance;
};