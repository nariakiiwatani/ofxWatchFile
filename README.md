# Attention
I prefer using [ofxWatcher](https://github.com/nariakiiwatani/ofxWatcher) now because it's more flexible and straightforward.  
Please check it out!

---

# ofxWatchFile

auto reload any file when it is modified
inspired by [ofxAutoReloadedShader](https://github.com/andreasmuller/ofxAutoReloadedShader)

## usage
See example. Shortly, see below.  

There are 2 ways to receive the result,

- event listeners  

```
ofxWatchFile file;
file.addListener(this, &MyClass::onFileChanged);
// MyClass::onFileChanged is called when the target file is changed
```

- derived class  

```
class MyClass : public ofxWatchFile {
	bool reload(ofFile &file) override;
};
// MyClass::reload is called when the target file is changed
```

## License
MIT License.

