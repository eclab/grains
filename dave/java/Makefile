.PHONY: jar install

#JAVAC = javac ${JAVACFLAGS}
JAVAC = javac

all: 
	javac -cp libraries/coremidi4j-1.6.jar:libraries/jSerialComm-2.10.4.jar:. $$(find . -name '*.java') 

indent:
	touch ${HOME}/.emacs
	find . -name "*.java" -print -exec emacs --batch --load ~/.emacs --eval='(progn (find-file "{}") (mark-whole-buffer) (setq indent-tabs-mode nil) (untabify (point-min) (point-max)) (indent-region (point-min) (point-max) nil) (save-buffer))' \;

jar: all
	- mkdir install 
	- rm -rf install/*.jar
	- rm -rf root
	mkdir root
	mkdir root/lib
	mkdir root/main
	- rm -rf install/dave.jar
	cp libraries/coremidi4j-1.6.jar root/lib
	cp libraries/jSerialComm-2.10.4.jar root/lib
	cd root; jar -xvf ../libraries/one-jar-boot-0.97.jar; rm -rf src
	cp boot-manifest.mf root/boot-manifest.mf
	jar -cvf root/main/main.jar Dave*.class Aria*.class Beats*.class Cascade*.class DisclosurePanel*.class WidgetList*.class
	cd root; jar -cvfm ../install/dave.jar boot-manifest.mf .

installold: jar
	rm -rf install/Dave.app install/bundles install/Dave.dmg.html install/Dave.dmg.jnlp
	- javapackager -deploy -native dmg -srcfiles install/dave.jar -appclass com.simontuffs.onejar.Boot -name Dave -outdir install -outfile Dave.dmg -v
	- mv install/bundles/Dave-1.0.dmg install/Dave.dmg
	rm -rf install/bundles install/Dave.dmg.html install/Dave.dmg.jnlp 

install: jar
	- rm install/Dave.dmg
	mkdir app
	rm -rf install/Dave.app install/bundles install/Dave.dmg.html install/Dave.dmg.jnlp
	- jpackage --type dmg --verbose --input install --dest app --name Dave --main-jar dave.jar --main-class com.simontuffs.onejar.Boot
	mv app/Dave-1.0.dmg install/Dave.dmg
	#- mv install/bundles/Dave-1.0.dmg install/Dave.dmg
	rm -rf app install/bundles install/Dave.dmg.html install/Dave.dmg.jnlp


clean:
	find . -name "*.class" -exec rm -f {} \;
	find . -name ".DS_Store" -exec rm -f {} \;
	find . -name "*.java*~" -exec rm -f {} \;
	find . -name ".#*" -exec rm -rf {} \;
	find . -name "#*#" -exec rm -rf {} \;
