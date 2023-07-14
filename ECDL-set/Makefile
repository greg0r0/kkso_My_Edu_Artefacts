CC = g++-10

compile-tests:
	@mkdir -p ./bin
	@$(CC) test/*.cpp include/EllipricCurve.hpp include/EllipricCurvePoint.hpp include/MathUtil.hpp --std=c++2a -lgtest_main -lgtest -pthread -o ./bin/tests

run-tests:
	@./bin/tests

delete-tests:
	@rm ./bin/tests

tests: compile-tests run-tests delete-tests

compile-example:
	@mkdir -p ./bin
	@$(CC) source/main.cpp include/EllipricCurve.hpp include/EllipricCurvePoint.hpp include/MathUtil.hpp --std=c++2a -o bin/example -DDEBUG -g

run-example:
	@./bin/example

example: compile-example run-example