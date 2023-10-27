#include "ReqManager.h"
#include <Geode/utils/web.hpp>
#include <vector>
#include <algorithm>
#include <random>
#include <functional>
using namespace geode::prelude;


ReqManager* ReqManager::getInstance() {
	if (!instance) {
		instance = new ReqManager();
	}
	return instance;
}

void ReqManager::cancelReqById(int id) {
	reqIds.erase(std::remove(reqIds.begin(), reqIds.end(), id), reqIds.end());

	std::string log = "";
	for (int id : reqIds) {
		log += (std::to_string(id) + ", ");
	}
	log::error(log);
}

int ReqManager::makeReq(std::string url, json::Value const& fields, std::function<void(std::string const&)> onResponse, std::function<void(std::string const&)> onError) {
	int reqId;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(1, 10000);

	do {
		reqId = distribution(gen);
	} while (std::find(reqIds.begin(), reqIds.end(), reqId) != reqIds.end());

	reqIds.push_back(reqId);

	auto req = web::AsyncWebRequest();
	req.postRequest();
	req.postFields(fields);
	req.fetch(url)
		.text()
		.then([onResponse, reqId](std::string const& response) { if (std::find(reqIds.begin(), reqIds.end(), reqId) != reqIds.end()) { reqIds.erase(std::remove(reqIds.begin(), reqIds.end(), reqId), reqIds.end());  onResponse(response); } })
		.expect([onError, reqId](std::string const& error) { if (std::find(reqIds.begin(), reqIds.end(), reqId) != reqIds.end()) { reqIds.erase(std::remove(reqIds.begin(), reqIds.end(), reqId), reqIds.end()); onError(error); } });
	
	
	std::string log = "";
	for (int id : reqIds) {
		log += (std::to_string(id) + ", ");
	}
	log::error(log);

	return reqId;
}

ReqManager::ReqManager() {

}

ReqManager* ReqManager::instance = nullptr;
std::vector<int> ReqManager::reqIds;
std::string ReqManager::serverLink = "http://176.57.220.181:2000/";
//std::string ReqManager::serverLink = "http://localhost:3000/";