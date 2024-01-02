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
	rm -rf root/main/app.jar
	rm -rf install/dave.jar
	jar -cvfm root/main/app.jar manifest.add `find . -name "Dave*.class"`
	cd root; jar -cvfm ../install/dave.jar boot-manifest.mf .

install: jar
	rm -rf install/Dave.app install/bundles install/Dave.dmg.html install/Dave.dmg.jnlp
	- javapackager -deploy -native dmg -srcfiles install/dave.jar -appclass com.simontuffs.onejar.Boot -name Dave -outdir install -outfile Dave.dmg -v
	- mv install/bundles/Dave-1.0.dmg install/Dave.dmg
	rm -rf install/bundles install/Dave.dmg.html install/Dave.dmg.jnlp 

clean:
	find . -name "*.class" -exec rm -f {} \;
	find . -name ".DS_Store" -exec rm -f {} \;
	find . -name "*.java*~" -exec rm -f {} \;
	find . -name ".#*" -exec rm -rf {} \;
	find . -name "#*#" -exec rm -rf {} \;
