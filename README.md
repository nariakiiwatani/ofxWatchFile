inspired by [ofxAutoReloadedShader](https://github.com/andreasmuller/ofxAutoReloadedShader)

# ofxWatchFile

auto reload any file when it is modified

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
	bool reload(ofFile &file);	// override function
};
// MyClass::reload is called when the target file is changed
```

## License
MIT License.

