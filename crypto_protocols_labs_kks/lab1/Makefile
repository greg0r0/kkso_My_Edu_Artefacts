CC = gcc
CPP = g++
OPTIONS  = -Wall -Wextra -O0  -masm=intel 
BUILD = build
SOURCE = source
OUTPUT = magma_cbc_crypto
D_OUTPUT = magma_cbc_crypto_debug

TESTS = test
T_OUTPUT = magma_cbc_crypto_tests
TESTS_TMP = /tmp
TESTS_FLAG = -lgtest_main -lgtest -pthread

info:
	@echo "[?] make info - print this text"

release:
	@mkdir -p $(BUILD)
	$(CC) $(OPTIONS) $(SOURCE)/*.c -s -o $(BUILD)/$(OUTPUT)

debug:
	@mkdir -p $(BUILD)
	$(CC) $(OPTIONS) $(SOURCE)/*.c -DDEBUG -g -o $(BUILD)/$(D_OUTPUT)

tests: 
	@mkdir -p $(TESTS_TMP)/$(TESTS)
	$(CPP) $(OPTIONS) $(TESTS_FLAG) -o $(TESTS_TMP)/$(TESTS)/$(T_OUTPUT) $(TESTS)/*.cpp $(filter-out $(SOURCE)/main.c,$(wildcard $(SOURCE)/*.c))
	@$(TESTS_TMP)/$(TESTS)/$(T_OUTPUT)
	#@rm -rf $(TESTS_TMP)/$(TESTS)

tests_dbg: 
	@mkdir -p $(TESTS_TMP)/$(TESTS)
	$(CPP) $(OPTIONS) $(TESTS_FLAG) -DDEBUG -o $(TESTS_TMP)/$(TESTS)/$(T_OUTPUT) $(TESTS)/*.cpp $(filter-out $(SOURCE)/main.c,$(wildcard $(SOURCE)/*.c))
	@$(TESTS_TMP)/$(TESTS)/$(T_OUTPUT)
	@rm -rf $(TESTS_TMP)/$(TESTS)

clean:
	rm -rf $(BUILD)