
# 定义编译器
CXX = g++

# 定义编译选项
CXXFLAGS = -Wall -std=c++17

#debug模式
#文件输入DEBUG
#CXXFLAGS += -DFILEINPUTDEBUG
#FF读入后是否输出
#CXXFLAGS += -DFFREADINOUT

# 定义链接选项
LDFLAGS = 

# 定义目标文件
TARGET = solution

# 定义源文件
SOURCES = ./clktree_pack.cpp
SOURCES += ./main.cpp


# 定义对象文件
OBJECTS = $(SOURCES:.cpp=.o)

#定义要解决的problem
PROBLEM = example_problem
#PROBLEM = problem1
#定义def文件
PROBLEM_FILE = ../$(PROBLEM)/problem.def
#定义constrain文件
CONSTRAIN_FILE = ../$(PROBLEM)/constraints.txt

#编译并运行
all: clean compile run

# 默认目标
compile: clean $(TARGET)

# 链接目标文件
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

# 编译源文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理编译生成的文件
clean:
	rm -f $(OBJECTS) $(TARGET)

#运行
run:
	./solution problem=$(PROBLEM_FILE) constrain=$(CONSTRAIN_FILE)

# 防止make命令错误地解释文件名中的'-'
.PHONY: all clean