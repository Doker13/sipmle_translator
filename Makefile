all: bin/translate

bin/translate: obj/src/translator/main.o obj/src/libtranslator/translator.o obj/src/libtranslator/parser.o
	gcc -Wall -Werror -g -O0 -I src -o bin/translate obj/src/translator/main.o obj/src/libtranslator/translator.o obj/src/libtranslator/parser.o
obj/src/translator/main.o: src/translator/main.c
	gcc -Wall -Werror -g -O0 -I src -c -o obj/src/translator/main.o src/translator/main.c
obj/src/libtranslator/translator.o: src/libtranslator/translator.c
	gcc -Wall -Werror -g -O0 -I src -c -o obj/src/libtranslator/translator.o src/libtranslator/translator.c
obj/src/libtranslator/parser.o: src/libtranslator/parser.c
	gcc -Wall -Werror -g -O0 -I src -c -o obj/src/libtranslator/parser.o src/libtranslator/parser.c
	
.PHONY: run

run: bin/translate
	./bin/translate Text_eng.txt Dictionary.txt Tetx_rus.txt
	
.PHONY: reverse

reverse: bin/translate
	./bin/translate Tetx_rus.txt Dictionary.txt Text_eng.txt
