#include "ofxWatchFile.h"
#include "ofAppRunner.h"
#include "ofUtils.h"

OFX_WATCH_FILE_BEGIN_NAMESPACE

bool File::setTargetPath(const std::filesystem::path &path, bool load_immediately)
{
	file_path_ = path;
	if(load_immediately) {
		return load();
	}
	else {
		file_.open(file_path_, load_settings_.mode, load_settings_.is_binary);
		if(file_.exists()) {
			last_loaded_timestamp_ = getLastWriteTime();
		}
	}
	return true;
}

bool File::load()
{
	if(file_path_ != "" && file_.open(file_path_, load_settings_.mode, load_settings_.is_binary)) {
		if(file_.exists()) {
			last_loaded_timestamp_ = getLastWriteTime();
			if(reload(file_)) {
				ofNotifyEvent(loaded_event_, file_, this);
				return true;
			}
		}
	}
	return false;
}

bool File::forceLoad()
{
	file_.open(file_path_, load_settings_.mode, load_settings_.is_binary);
	last_loaded_timestamp_ = getLastWriteTime();
	if(reload(file_)) {
		ofNotifyEvent(loaded_event_, file_, this);
		return true;
	}
	return false;
}

bool File::isChangedFromLastLoaded() const
{
	if(file_.exists()) {
		return last_loaded_timestamp_ != getLastWriteTime();
	}
	return false;
}


void File::enableWatching(bool ignore_current_version)
{
	if(ignore_current_version) {
		last_loaded_timestamp_ = getLastWriteTime();
	}
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
		if((check_settings_.reckless_mode && last_loaded_timestamp_ != getLastWriteTime()) || isChangedFromLastLoaded()) {
			forceLoad();
		}
		time_from_last_checked_ = 0;
	}
}

template <typename TP>
std::time_t to_time_t(TP tp)
{
	using namespace std::chrono;
	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
	return system_clock::to_time_t(sctp);
}

std::time_t File::getLastWriteTime() const
{
	return to_time_t(std::filesystem::last_write_time(ofToDataPath(file_path_)));
}

OFX_WATCH_FILE_END_NAMESPACE
