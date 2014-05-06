LAMBDA
======

[Binaries]: Prebuilt packages are provided for OSX >= 10.8  

Building from source
====================

Linux
-----

* prerequisites
	- 'libqt4-core'
 	- 'libqt4-dev'
	- 'libqt4-debug'
	- 'libqt4-debug-dev'
	- a c++ compiler (gcc and g++)

* configuration
	- extract the 'src' directory from this archive to a temporary destination on you file system 
	- change to that directory
	- run 'qmake'

* building and installing
	- run 'make'
	- you should now find the executable 'lambda' in the 'src' directory
	- extract the 'examples', 'scripts' and 'doc' directories from this archive to your file system for your convenience

OSX
---

### Using Homebrew

* prerequisites
	- xcode
	- git

* build & install
	

```
$ brew install xvid
$ brew install qt
$ git clone git@github.com:gesellkammer/lambda.git
$ cd lambda
$ cd dependencies
$ make & make install
$ cd ..
$ cd src
$ qmake
$ make
```

You will see a lambda.app in the same folder. You can move that to /Applications 
