.PHONY: all clean build

# 获取真实路径
REAL_PATH := $(shell readlink -f .)

all: build

build:
	@mkdir -p $(REAL_PATH)/build
	@cd $(REAL_PATH)/build && cmake .. && make

clean:
	@rm -rf $(REAL_PATH)/build