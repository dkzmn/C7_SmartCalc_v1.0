FLAGS=-Wall -Wextra -Werror# -std=c11 -fsanitize=address -pedantic
LIBS=-lcheck
OS:=$(shell uname -s)
ifeq ($(OS), Darwin)
	FLAGS += -D MACOS
else
	LIBS += -lsubunit -lrt -lm -lpthread
endif

rebuild: clean all

all: install

install:
	mkdir -p build
	cd SmartCalc && cmake -S ./ -B ../build
	cd build && $(MAKE)

uninstall: 
	rm -rf build/
	clean

dvi:
	cd docs && latex dvi.tex

dist: install
	tar -cf smartcalc-1.0.tar build/*

test: test_s21_smartcalc.c ./modules/s21_rpn.c
	gcc $(FLAGS) -c ./modules/s21_rpn.c -o s21_rpn.o
	gcc $(FLAGS) -c test_s21_smartcalc.c -o test_s21_smartcalc.o
	gcc $(FLAGS) test_s21_smartcalc.o s21_rpn.o $(LIBS) -o test
	./test

gcov_report:
	gcc $(FLAGS) -c ./modules/s21_rpn.c --coverage -o s21_rpn.o
	gcc $(FLAGS) -c test_s21_smartcalc.c -o test_s21_smartcalc.o
	gcc $(FLAGS) test_s21_smartcalc.o s21_rpn.o --coverage $(LIBS) -o test_gcov
	./test_gcov
	gcovr -r . --html --html-details -o s21_SmartCalc.html
	rm -rf *.gcda *.gcno

clean:
	@rm -rf *.o *.a *.css *.html *.gcda *.gcno 
	@rm -rf test test_gcov build* ./SmartCalc/*txt.user smartcalc-1.0.tar
	@rm -rf docs/*.aux docs/*.dvi docs/*.log *.log

clf:
	clang-format -n --style=Google *.[ch] modules/*.[ch] SmartCalc/main.cpp SmartCalc/mainwindow.cpp SmartCalc/mainwindow.h

cpk:
	cppcheck *.[ch] modules/*.[ch] SmartCalc/main.cpp SmartCalc/mainwindow.cpp SmartCalc/mainwindow.h

style: clf cpk