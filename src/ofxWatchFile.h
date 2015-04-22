#pragma once

#include "ofFileUtils.h"
#include "ofEvents.h"

class ofxWatchFile
{
public:
	struct LoadSettings {
		ofFile::Mode mode;
		bool is_binary;
		
		LoadSettings()
		:mode(ofFile::ReadOnly)
		,is_binary(false)
		{}
	};
	struct CheckSettings {
		float interval_timef;
		CheckSettings()
		:interval_timef(1)
		{}
	};
	ofxWatchFile();
	~ofxWatchFile();
	void setTargetPath(const string &path, bool load_immediately=true);
	void setCheckIntervalTimef(float timef) { check_settings_.interval_timef = timef; }

	void enableWatching();
	void disableWatching();
	bool isWatching() { return is_watching_; }
	
	ofEvent<ofFile> loadedEvent;

	// if you want to check/load manually, use these functions.
	bool load();
	bool isChangedFromLastLoaded();	

	// advanced settings
	void setLoadSettings(const LoadSettings &settings) { load_settings_ = settings; }
	void setCheckSettings(const CheckSettings &settings) { check_settings_ = settings; }

private:
	string file_path_;
	LoadSettings load_settings_;
	CheckSettings check_settings_;
	
	ofFile file_;
	std::time_t last_loaded_timestamp_;
	float time_from_last_checked_;
	bool is_watching_;

	void update(ofEventArgs &args);
};

