#pragma once

#include "ofFileUtils.h"
#include "ofEvents.h"

#define OFX_WATCH_FILE_BEGIN_NAMESPACE namespace ofx { namespace WatchFile {
#define OFX_WATCH_FILE_END_NAMESPACE } }

OFX_WATCH_FILE_BEGIN_NAMESPACE

class File
{
public:
	struct LoadSettings {
		ofFile::Mode mode = ofFile::ReadOnly;
		bool is_binary = false;
	};
	struct CheckSettings {
		float interval_timef = 1;
		bool reckless_mode = false;	// set it true if you need speed seriously. but hangs if the file doesn't exist.
	};
	File() { enableWatching(); }
	virtual ~File() { disableWatching(); }
	
	bool setTargetPath(const std::string &path, bool load_immediately=true);
	void setCheckIntervalTimef(float timef) { check_settings_.interval_timef = timef; }

	void enableWatching();
	void disableWatching();
	bool isWatching() { return is_watching_; }
	
	// event listeners
	template<class Listener>
	void addListener(Listener *listener, void (Listener::*method)(ofFile&), int prio=OF_EVENT_ORDER_AFTER_APP) {
		ofAddListener(loaded_event_, listener, method, prio);
	}
	template<class Listener>
	void addListener(Listener *listener, void (Listener::*method)(const void *sender, ofFile&), int prio=OF_EVENT_ORDER_AFTER_APP) {
		ofAddListener(loaded_event_, listener, method, prio);
	}
	template<class Listener>
	void removeListener(Listener *listener, void (Listener::*method)(ofFile&), int prio=OF_EVENT_ORDER_AFTER_APP) {
		ofRemoveListener(loaded_event_, listener, method, prio);
	}
	template<class Listener>
	void removeListener(Listener *listener, void (Listener::*method)(const void *sender, ofFile&), int prio=OF_EVENT_ORDER_AFTER_APP) {
		ofRemoveListener(loaded_event_, listener, method, prio);
	}

	// if you want to check/load manually, use these functions.
	bool load();
	bool forceLoad(); // a bit faster than load() but hangs if the file doesn't exist.
	bool isChangedFromLastLoaded();	

	// advanced settings
	void setLoadSettings(const LoadSettings &settings) { load_settings_ = settings; }
	void setCheckSettings(const CheckSettings &settings) { check_settings_ = settings; }
	LoadSettings& getLoadSettings() { return load_settings_; }
	CheckSettings& getCheckSettings() { return check_settings_; }
	
protected:
	virtual bool reload(ofFile &file) { return true; }
	
private:
	std::string file_path_ = "";
	LoadSettings load_settings_;
	CheckSettings check_settings_;
	
	ofFile file_;
	std::time_t last_loaded_timestamp_ = 0;
	float time_from_last_checked_ = 0;
	bool is_watching_ = false;

	ofEvent<ofFile> loaded_event_;
	void update(ofEventArgs &args);
	std::time_t getLastWriteTime();
};

OFX_WATCH_FILE_END_NAMESPACE

using ofxWatchFile = ofx::WatchFile::File;
