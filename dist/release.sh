#!/usr/bin/env bash

mvn clean

export CVSROOT=:pserver:anonymous@jvstwrapper.cvs.sourceforge.net:/cvsroot/jvstwrapper
mkdir target
cd target

cvs login
cvs co jvst_wrapper
cvs co jvst_system
cvs co jvst_examples
cvs co jvst_native

cp ../cvs2cl.pl .
./cvs2cl.pl -f../ChangeLog.txt

cd ..

wget -O READ_ME.html http://jvstwrapper.sourceforge.net/index.html
wget http://jvstwrapper.sourceforge.net/style.css
wget http://jvstwrapper.sourceforge.net/fb.jpg

mvn install
mvn javadoc:jar
mvn package assembly:assembly

echo Mac OSX only --> create custom folder icon
ASSEMBLY_DIR=target/jVSTwRapper-Release-0.9g-osx.dir
rm -f $ASSEMBLY_DIR/jvstwrapper.vst/Icon*
/Developer/Tools/SetFile -a B $ASSEMBLY_DIR/jvstwrapper.vst
sips -i $ASSEMBLY_DIR/jvstwrapper.vst/Contents/Resources/jvstwrapper.icns
ditto $ASSEMBLY_DIR/jvstwrapper.vst/Contents/Resources/jvstwrapper.icns $ASSEMBLY_DIR/jvstwrapper.vst/Icon
/Developer/Tools/SetFile -a V $ASSEMBLY_DIR/jvstwrapper.vst/Icon
echo "file rename $ASSEMBLY_DIR/jvstwrapper.vst/Icon $ASSEMBLY_DIR/jvstwrapper.vst/Icon\r" | tclsh
/Developer/Tools/SetFile -a C $ASSEMBLY_DIR/jvstwrapper.vst/

mvn deploy
#mvn release:prepare
#mvn release:perform