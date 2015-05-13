#include "ofxWatchFile.h"
#include "ofAppRunner.h"
#include "ofUtils.h"

OFX_WATCH_FILE_BEGIN_NAMESPACE

File::File()
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

File::~File()
{
	disableWatching();
	loaded_event_.clear();
}

void File::setTargetPath(const string &path, bool load_immediately)
{
	file_path_ = path;
	if(load_immediately) {
		load();
	}
}

bool File::load()
{
	file_.open(file_path_, load_settings_.mode, load_settings_.is_binary);
	if(file_.exists()) {
		last_loaded_timestamp_ = file_.getPocoFile().getLastModified().epochTime();
		reload(file_);
		ofNotifyEvent(loaded_event_, file_, this);
		return true;
	}
	return false;
}

void File::forceLoad()
{
	file_.open(file_path_, load_settings_.mode, load_settings_.is_binary);
	last_loaded_timestamp_ = file_.getPocoFile().getLastModified().epochTime();
	reload(file_);
	ofNotifyEvent(loaded_event_, file_, this);
}

bool File::isChangedFromLastLoaded()
{
	if(file_.exists()) {
		return last_loaded_timestamp_ != file_.getPocoFile().getLastModified().epochTime();
	}
	return false;
}


void File::enableWatching()
{
	if(!isWatching()) {
		ofAddListener(ofEvents().update, this, &File::update);
		is_watching_ = true;
	}
}

void File::disableWatching()
{
	if(isWatching()) {
		ofRemoveListener(ofEvents().update, this, &File::update);
		is_watching_ = false;
	}
}

void File::update(ofEventArgs &args)
{
	time_from_last_checked_ += ofGetLastFrameTime();
	if(time_from_last_checked_ >= check_settings_.interval_timef) {
		if((check_settings_.reckless_mode && last_loaded_timestamp_ != file_.getPocoFile().getLastModified().epochTime()) || isChangedFromLastLoaded()) {
			forceLoad();
		}
		time_from_last_checked_ = 0;
	}
}

OFX_WATCH_FILE_END_NAMESPACE
