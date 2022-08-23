#pragma once

#include "ofxWatchFile.h"
#include <filesystem>
#include <functional>

#define OFX_WATCH_FILE_BEGIN_NAMESPACE namespace ofx { namespace WatchFile {
#define OFX_WATCH_FILE_END_NAMESPACE } }

OFX_WATCH_FILE_BEGIN_NAMESPACE

class Subscribed
{
public:
	Subscribed(const std::filesystem::path &filepath, std::function<void(ofFile&)> func)
	:func_(func) {
		watcher_.addListener(this, &Subscribed::callback);
		watcher_.setTargetPath(filepath);
	}
	bool isEnabled() const { return watcher_.isWatching(); }
	void enable() { watcher_.enableWatching(); }
	void disable() { watcher_.disableWatching(); }
	void setEnabled(bool enabled) { enabled ? enable() : disable(); }
	void callback(ofFile &file) { func_(file); }
private:
	ofxWatchFile watcher_;
	std::function<void(ofFile&)> func_;
};


OFX_WATCH_FILE_END_NAMESPACE

namespace {
std::map<std::filesystem::path, std::shared_ptr<ofx::WatchFile::Subscribed>> subscribed_container;
}

template<typename Func>
std::shared_ptr<ofx::WatchFile::Subscribed> ofxSubscribeFile(const std::filesystem::path &filepath, Func func, bool retain=true) {
	auto ret = std::make_shared<ofx::WatchFile::Subscribed>(filepath, func);
	if(retain) {
		subscribed_container.insert({filepath, ret});
	}
	return ret;
}
template<typename Loader, typename Then>
std::shared_ptr<ofx::WatchFile::Subscribed> ofxSubscribeFile(const std::filesystem::path &filepath, Loader loader, Then func, bool retain=true) {
	return ofxSubscribeFile(filepath, [loader, func](ofFile &file) {
		func(loader(file.path()));
	}, retain);
}

bool ofxUnsubscribeFile(std::shared_ptr<ofx::WatchFile::Subscribed> subscribed) {
	auto found = find_if(begin(subscribed_container), end(subscribed_container), [subscribed](const std::pair<std::filesystem::path, std::shared_ptr<ofx::WatchFile::Subscribed>> s) {
		return subscribed == s.second;
	});
	if(found == end(subscribed_container)) {
		subscribed->disable();
		subscribed.reset();
		return false;
	}
	subscribed_container.erase(found);
	return true;
}
bool ofxUnsubscribeFile(const std::filesystem::path &filepath) {
	auto found = subscribed_container.find(filepath);
	if(found == end(subscribed_container)) {
		return false;
	}
	subscribed_container.erase(found);
	return true;
}
