#include "ofxWatchFile.h"
#include "ofAppRunner.h"
#include "ofUtils.h"

ofxWatchFile::ofxWatchFile()
:file_path_("")
,load_settings_()
,check_settings_()
,file_()
,last_loaded_timestamp_(0)
,time_from_last_checked_(0)
,is_watching_(false)
{
	enableWatching();
}

ofxWatchFile::~ofxWatchFile()
{
	disableWatching();
}

void ofxWatchFile::setTargetPath(const string &path, bool load_immediately)
{
	file_path_ = path;
	if(load_immediately) {
		load();
	}
}

bool ofxWatchFile::load()
{
	file_.open(file_path_, load_settings_.mode, load_settings_.is_binary);
	if(file_.exists()) {
		last_loaded_timestamp_ = file_.getPocoFile().getLastModified().epochTime();
		ofNotifyEvent(loadedEvent, file_, this);
		return true;
	}
	return false;
}

bool ofxWatchFile::isChangedFromLastLoaded()
{
	if(file_.exists()) {
		return last_loaded_timestamp_ != file_.getPocoFile().getLastModified().epochTime();
	}
	return false;
}


void ofxWatchFile::enableWatching()
{
	if(!isWatching()) {
		ofAddListener(ofEvents().update, this, &ofxWatchFile::update);
		is_watching_ = true;
	}
}

void ofxWatchFile::disableWatching()
{
	if(isWatching()) {
		ofRemoveListener(ofEvents().update, this, &ofxWatchFile::update);
		is_watching_ = false;
	}
}

void ofxWatchFile::update(ofEventArgs &args)
{
	time_from_last_checked_ += ofGetLastFrameTime();
	if(time_from_last_checked_ >= check_settings_.interval_timef) {
		if(isChangedFromLastLoaded()) {
			load();
		}
		time_from_last_checked_ = 0;
	}
}

